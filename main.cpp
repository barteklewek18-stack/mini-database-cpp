#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <vector>
#include <optional>
#include <fstream>
using namespace std;

struct inputBuffer{
    string input;

};

void print_p() {
    cout<<"db > ";
}

void read_input(inputBuffer& input_buffer) {
    getline(cin, input_buffer.input);
}

enum class Meta_result {
    Success,
    Exit,
    Clear,
    Unknown
};

Meta_result do_meta_command(inputBuffer& input_buffer) {
    if (input_buffer.input == ".help") {
        cout<<"help"<<endl;
        return Meta_result::Success;
    }else if (input_buffer.input == ".exit") {
        return Meta_result::Exit;
    }else if (input_buffer.input == ".clear") {
        return Meta_result::Clear;
    }else {
        return Meta_result::Unknown;
    }
}

enum class  Statement_Type{
    Insert,
    Delete,
    Select,
    Update,
    Unknown
};

struct Statement {
    Statement_Type type;
    int id;
    optional<string> username;
    optional<string> email;
};

struct Row {
    int id;
    string username;
    string email;
};

bool prepare_statement(const inputBuffer& input_buffer, Statement& statement) {
    string input=input_buffer.input;

    size_t pos=input.find(' ');
    string keyword;

    if (pos==string::npos) {
        keyword=input;
    }else {
        keyword=input.substr(0,pos);
    }

    transform(keyword.begin(), keyword.end(), keyword.begin(),[](unsigned char c) {return tolower(c);});

    if (keyword=="insert") {

        istringstream iss(input);
        string cmd,username,email;
        int id;

        if (!(iss>>cmd>>id>>username>>email)) {
            return false;
        }

        string excess;
        if (iss>>excess) {
            return false;
        }

        statement.type=Statement_Type::Insert;
        statement.id=id;
        statement.username=username;
        statement.email=email;

        return true;
    }else if (keyword=="delete") {
        istringstream iss(input);
        int id;
        string cmd;
        if (!(iss>>cmd>>id)) {
            return false;
        }

        statement.type=Statement_Type::Delete;
        statement.id=id;

        return true;
    }else if (keyword=="select") {
        statement.type=Statement_Type::Select;
        return true;
    }else if (keyword=="update") {
        istringstream iss(input);
        string cmd,username,email;
        int id;
        if (!(iss>>cmd>>id)){
            return false;
        }
        statement.type=Statement_Type::Update;
        statement.id=id;

        string temp;
        while (iss>>temp) {

            if (temp.find('=')==string::npos) {
                return false;
            }

            int pos=temp.find('=');
            string key=temp.substr(0,pos);
            string value=temp.substr(pos+1);

            if (key=="username") {
                statement.username=value;
            }else if (key=="email") {
                statement.email=value;
            }else
                return false;
        }
        return true;
    }

    return false;
}

struct Database {
    vector<Row> table;

    void execute_statement(const Statement& statement) {
        switch (statement.type) {
            case Statement_Type::Insert: {
                    bool leave=false;
                    for (auto& row : table) {
                        if (statement.id==row.id) {
                            leave=true;
                        }
                    }
                    if (leave) {
                        cout<<"Error: ID already exists"<<endl;
                        break;
                    }
                }

                if (statement.email->find('@')==string::npos) {
                    cout<<"Wrong email"<<endl;
                    break;
                }
                table.push_back({statement.id ,statement.username.value() ,statement.email.value() });
                cout<<"Executed."<<endl;
                break;
            case Statement_Type::Select:
                if (table.size()==0) {
                    cout<<"Table is empty."<<endl;
                }
                for (const Row& row:table) {
                    cout<<row.id<<" "<<row.username<<" "<<row.email<<endl;
                }
                break;
            case Statement_Type::Delete: {
                int s=table.size();
                auto ne=remove_if(table.begin(),table.end(),[statement](const Row& row) {
                    return row.id==statement.id;
                });

                table.erase(ne, table.end());
                int final=s-table.size();

                if (final==0) {
                    cout<<"Nothing deleted"<<endl;
                }else
                    cout<<"Deleted "<<final<<" rows"<<endl;
            }
                break;
            case Statement_Type::Update: {
                int count=0;
                bool found=false;
                bool wrong=false;
                for (Row& row:table) {
                    if (row.id==statement.id) {
                        if (!statement.username.has_value() && !statement.email.has_value()) {
                            found=true;
                            continue;
                        }
                        if (statement.email.has_value()) {
                            if (statement.email->find('@')==string::npos) {
                                wrong=true;
                                break;
                            }
                            row.email=statement.email.value();
                        }
                        if (statement.username.has_value()) {
                            row.username=statement.username.value();
                        }
                        count++;
                    }
                }


                if (wrong) {
                    cout<<"Wrong email"<<endl;
                }else if ((count==0 && found)) {
                    cout<<"Nothing updated"<<endl;
                }else if (count==0) {
                    cout<<"No rows found with id = "<<statement.id<<endl;
                }else
                    cout<<"Updated "<<count<<" rows"<<endl;
            }
                break;
            default:
                break;
        }
    }

    void save_to_file() {
        ofstream out("Database.txt");

        for (const Row& row:table) {
            out<<row.id<<" "<<row.username<<" "<<row.email<<endl;
        }

        out.close();
    }

    void load_from_file() {
        ifstream in("Database.txt");
        int id;
        string username,email;

        while (in>>id>>username>>email) {
            table.push_back({id,username,email});
        }
    }

    void clear_file() {
        ofstream out("Database.txt");
        out.close();
        table.clear();
    }
};



int main(int argc,char * argv[]) {
    inputBuffer input_buff;
    Database db;

    db.load_from_file();

    while (true) {
        print_p();
        read_input(input_buff);
        Statement statement;

        if (!input_buff.input.empty() && input_buff.input[0] == '.') {
            Meta_result result = do_meta_command(input_buff);
            switch (result) {
                case Meta_result::Unknown:
                    cout<<"Unknown meta command"<<endl;
                    break;
                case Meta_result::Success:
                    break;
                case Meta_result::Exit:
                    db.save_to_file();
                    return 0;
                    break;
                case Meta_result::Clear:
                    db.clear_file();
                    cout<<"Database cleard"<<endl;
                    break;
            }
        }else {
            if (!prepare_statement(input_buff, statement)) {
                cout<<"Failed to prepare statement"<<endl;
                continue;
            }
            db.execute_statement(statement);
            }
        }
}

