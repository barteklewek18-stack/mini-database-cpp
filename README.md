\# Mini Database Engine (C++)



A simple command-line database written in C++.



This project implements a small database engine that supports basic CRUD operations and stores data in a file.  

The database loads records from a file on startup and saves them back when the program exits.



---



\## Features



\- Insert records

\- Delete records

\- Update records

\- Select records

\- Unique ID constraint

\- Email validation

\- Persistent storage (data saved to file)

\- Simple command-line interface



---



\## Commands



Example usage:

insert 1 bartek bartek@email.com



select



update 1 username=Bartek email=bartek@new.com



delete 1



.clear

.exit



\### Command description



| Command | Description |

|------|-------------|

| `insert id username email` | Add a new record |

| `select` | Display all records |

| `update id field=value` | Update username or email |

| `delete id` | Remove record |

| `.clear` | Clear the entire database |

| `.exit` | Save database and exit program |



---



\## Storage



The database stores records in:

Database.txt



Behavior:

\- The file is \*\*loaded when the program starts\*\*

\- The file is \*\*saved when `.exit` is used\*\*

\- `.clear` removes all records from memory and the file



---



\## Project Structure



mini-database-cpp

│

├── main.cpp

├── README.md

├── .gitignore

└── Database.txt (generated automatically)



---



\## Technologies



\- C++

\- STL (`vector`, `optional`, `algorithm`)

\- File handling with `fstream`



---



\## Build



Compile using `g++`:

g++ main.cpp -o database





---



\## Example Session

db > insert 1 bartek bartek@email.com



Executed.



db > select

1 bartek bartek@email.com



db > update 1 username=Bartek

Updated 1 rows



db > delete 1

Deleted 1 rows



