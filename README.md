**Student Management System** os/c++

## Features
 **Login System**: Secure authentication for teachers and students using usernames and passwords.
 
 **Teacher Functions**:
Add new students (roll number, name, marks, password).
View individual or all student details.
Sort students by CGPA (ascending/descending).
View passed/failed students based on a CGPA threshold.

**Student Functions**: View personal marks and CGPA.
**CGPA Calculation**: Automatically computes CGPA from marks.
**Technical Highlights**: Uses multithreading for concurrent operations and file-based storage (**students.txt, users.txt**).

## Installation

1. How to execute:
   on ubuntu -> terminal
    ->touch sms.cpp
    ->open the file that created in text editor
    ->paste the code
    -> then on terminal
    ->  g++ -o sms sms.cpp -pthread
   ->./sms
   ->then all is set !

   git clone https://github.com/Danish670/Student-Management-System.git
   cd Student-Management-System
