**Student Management System** operating sytems/c++

# 🎓 Student Management System

A simple console-based Student Management System built in C++ with multithreading and file-based storage.

---

## ✨ Features

### 🔐 Login System
- Secure authentication for **teachers** and **students** using usernames and passwords.

### 👨‍🏫 Teacher Functions
- Add new students (Roll Number, Name, Marks, Password).
- View individual or all student details.
- Sort students by CGPA (ascending/descending).
- View passed/failed students based on a CGPA threshold.

### 👨‍🎓 Student Functions
- View personal marks and CGPA.

### 📊 CGPA Calculation
- Automatically computes CGPA based on marks.

---

## ⚙️ Technical Highlights

- 🧵 **Multithreading** for concurrent operations.
- 📁 **File-based storage** using `students.txt` and `users.txt`.

---

## 🛠️ Installation & Usage

### On Ubuntu:

```bash
# Create the file
touch sms.cpp

# Open the file in a text editor and paste the code

# Then compile and run
g++ -o sms sms.cpp -pthread
./sms

📂 Clone the Repository

git clone https://github.com/Danish670/Student-Management-System.git
cd Student-Management-System
