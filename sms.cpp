#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

mutex fileMutex;

struct Student {
    string rollNumber;
    string name;
    float marks[5];
    float cgpa;
    string password;
};

struct User {
    string username;
    string password;
    bool isTeacher;
};

string simpleHash(const string& input) 
{
    return input;
}

void addStudent() {
    Student s;
    cout << "\nEnter Roll Number: ";
    cin >> s.rollNumber;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, s.name);
    cout << "Enter marks for 5 courses out of (100)" << endl;
    float total = 0;
    for (int i = 0; i < 5; i++) {
        while (true) {
            cin >> s.marks[i];
            if (cin.fail() || s.marks[i] < 0 || s.marks[i] > 100) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid! Please enter mark " << (i + 1) << " again: ";
                continue;
            }
            break;
        }
        total += s.marks[i];
    }
    cout << "Enter Password: ";
    string password;
    cin >> password;
    s.password = simpleHash(password);
    s.cgpa = total / 125.0;

    lock_guard<mutex> lock(fileMutex);
    ofstream file("students.txt", ios::app);
    file << s.rollNumber << "," << s.name;
    for (int i = 0; i < 5; i++) file << "," << s.marks[i];
    file << "," << s.cgpa << "," << s.password << endl;

    ofstream userFile("users.txt", ios::app);
    userFile << s.rollNumber << "," << s.password << ",0" << endl;

    cout << "Student added successfully!" << endl;
}

Student parseStudent(const string& line) {
    Student s;
    stringstream ss(line);
    string token;
    getline(ss, s.rollNumber, ',');
    getline(ss, s.name, ',');
    for (int i = 0; i < 5; i++) {
        getline(ss, token, ',');
        s.marks[i] = stof(token);
    }
    getline(ss, token, ',');
    s.cgpa = stof(token);
    getline(ss, s.password);
    return s;
}

User parseUser(const string& line) {
    User u;
    string token;
    stringstream ss(line);
    getline(ss, u.username, ',');
    getline(ss, u.password, ',');
    getline(ss, token);
    u.isTeacher = token == "1";
    return u;
}

void viewStudent(const string& rollNumber, bool isTeacher) {
    lock_guard<mutex> lock(fileMutex);
    ifstream file("students.txt");
    string line;
    bool found = false;
    while (getline(file, line)) {
        Student s = parseStudent(line);
        if (s.rollNumber == rollNumber) {
            found = true;
            cout << "Name: " << s.name << endl;
            cout << "Roll Number: " << s.rollNumber << endl;
            cout << "Marks: ";
            for (int i = 0; i < 5; i++) cout << s.marks[i] << " ";
            cout << "\nCGPA: " << s.cgpa << endl;
            break;
        }
    }
    if (!found) cout << "Student not found!" << endl;
}

void sortStudentsByCGPA(bool ascending) {
    vector<Student> students;
    {
        lock_guard<mutex> lock(fileMutex);
        ifstream file("students.txt");
        string line;
        while (getline(file, line)) {
            students.push_back(parseStudent(line));
        }
    }
    auto sortTask = [&students, ascending]() {
        sort(students.begin(), students.end(), [ascending](const Student& a, const Student& b) {
            return ascending ? a.cgpa < b.cgpa : a.cgpa > b.cgpa;
        });
        cout << "Sorted Students by CGPA (" << (ascending ? "Ascending" : "Descending") << "):\n";
        for (const auto& s : students) {
            cout << "Roll: " << s.rollNumber << ", Name: " << s.name << ", CGPA: " << s.cgpa << endl;
        }
    };
    thread sortThread(sortTask);
    sortThread.join();
}

void viewPassedFailed(float threshold) {
    vector<Student> students;
    lock_guard<mutex> lock(fileMutex);
    ifstream file("students.txt");
    string line;
    while (getline(file, line)) {
        students.push_back(parseStudent(line));
    }
    cout << "Passed Students (CGPA >= " << threshold << "):\n";
    for (const auto& s : students)
        if (s.cgpa >= threshold)
            cout << "Roll: " << s.rollNumber << ", Name: " << s.name << ", CGPA: " << s.cgpa << endl;
    cout << "\nFailed Students (CGPA < " << threshold << "):\n";
    for (const auto& s : students)
        if (s.cgpa < threshold)
            cout << "Roll: " << s.rollNumber << ", Name: " << s.name << ", CGPA: " << s.cgpa << endl;
}

bool authenticate(const string& username, const string& password, bool& isTeacher) {
    lock_guard<mutex> lock(fileMutex);
    ifstream file("users.txt");
    string line;
    string hashedPassword = simpleHash(password);
    while (getline(file, line)) {
        User u = parseUser(line);
        if (u.username == username && u.password == hashedPassword) {
            isTeacher = u.isTeacher;
            return true;
        }
    }
    return false;
}

void teacherInterface() {
    string username, password;
    cout << "**Teacher Login**\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    bool isTeacher;
    if (!authenticate(username, password, isTeacher) || !isTeacher) {
        cout << "Authentication failed or not a teacher!" << endl;
        return;
    }
    while (true) {
        int choice;
        while (true) {
            cout << "\n**Teacher Menu**\n1. Add Student\n2. View Student\n3. Sort by CGPA\n4. View Passed/Failed\n5. Exit\nChoice: ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 5) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid! Please enter again.\n";
                continue;
            }
            break;
        }
        if (choice == 5) break;
        switch (choice) {
            case 1: addStudent(); break;
            case 2: {
                string roll;
                cout << "Enter Roll Number: ";
                cin >> roll;
                viewStudent(roll, true);
                break;
            }
            case 3: {
                int order;
                while (true) {
                    cout << "Sort Ascending (1) or Descending (0) :";
                    cin >> order;
                    if (cin.fail() || (order != 0 && order != 1)) {
                        cin.clear(); cin.ignore(1000, '\n');
                        cout << "Invalid! Please enter again.\n";
                        continue;
                    }
                    break;
                }
                sortStudentsByCGPA(order == 1);
                break;
            }
            case 4:
                viewPassedFailed(2.0);
                break;
        }
    }
}

void studentInterface() {
    string rollNumber, password;
    cout << "\n**Student Login**\nRoll Number: ";
    cin >> rollNumber;
    cout << "Password: ";
    cin >> password;
    bool isTeacher;
    if (!authenticate(rollNumber, password, isTeacher) || isTeacher) {
        cout << "Authentication failed or not a student!" << endl;
        return;
    }
    while (true) {
        int choice;
        while (true) {
            cout << "\nStudent Menu:\n1. View My Record\n2. Exit\nChoice: ";
            cin >> choice;
            if (cin.fail() || (choice != 1 && choice != 2)) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid! Please enter again.\n";
                continue;
            }
            break;
        }
        if (choice == 2) break;
        viewStudent(rollNumber, false);
    }
}

int main() {
    {
        lock_guard<mutex> lock(fileMutex);
        ofstream userFile("users.txt", ios::app);
        userFile << "admin," << simpleHash("123") << ",1\n";
    }
    while (true) {
        int choice;
        while (true) {
            cout << "\n***Student Management System***\n\n1. Teacher Login\n2. Student Login\n3. Exit\nChoice: ";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 3) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid! Please enter again.\n";
                continue;
            }
            break;
        }
        if (choice == 3) {
            cout << "Thanks for Visiting!" << endl;
            break;
        }
        switch (choice) {
            case 1: teacherInterface(); break;
            case 2: studentInterface(); break;
        }
    }
    return 0;
}

