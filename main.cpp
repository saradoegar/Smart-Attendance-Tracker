/*
 * Smart Attendance and Performance Tracker
 * Author: [Sara Doegar]
 * Description: A simple console-based system to track student attendance
 *              and marks, with file storage so data is not lost on exit.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────
//  Student Class
// ─────────────────────────────────────────
class Student {
private:
    int    rollNumber;
    string name;
    int    totalClasses;    // how many classes have been held so far
    int    attendedClasses; // how many this student attended
    float  marks;
    string grade;

    // helper: calculate grade from marks
    string calculateGrade(float m) {
        if (m >= 85) return "A";
        else if (m >= 70) return "B";
        else if (m >= 50) return "C";
        else return "Fail";
    }

public:
    // Default constructor (needed when loading from file)
    Student() {
        rollNumber      = 0;
        name            = "";
        totalClasses    = 0;
        attendedClasses = 0;
        marks           = 0.0f;
        grade           = "N/A";
    }

    // Parameterized constructor
    Student(int roll, string n) {
        rollNumber      = roll;
        name            = n;
        totalClasses    = 0;
        attendedClasses = 0;
        marks           = 0.0f;
        grade           = "N/A";
    }

    // ── Getters ──
    int    getRoll()      const { return rollNumber; }
    string getName()      const { return name; }
    int    getTotal()     const { return totalClasses; }
    int    getAttended()  const { return attendedClasses; }
    float  getMarks()     const { return marks; }
    string getGrade()     const { return grade; }

    // attendance percentage
    float getAttendancePercent() const {
        if (totalClasses == 0) return 0.0f;
        return (float)attendedClasses / totalClasses * 100.0f;
    }

    // ── Setters / update methods ──
    void addAttendance(bool present) {
        totalClasses++;
        if (present) attendedClasses++;
    }

    void setMarks(float m) {
        marks = m;
        grade = calculateGrade(m);
    }

    // used when loading from file — set all fields directly
    void setAllFields(int roll, string n, int total, int attended, float m) {
        rollNumber      = roll;
        name            = n;
        totalClasses    = total;
        attendedClasses = attended;
        marks           = m;
        grade           = calculateGrade(m);
    }

    // print a single student report row
    void printReport() const {
        cout << left
             << setw(8)  << rollNumber
             << setw(20) << name
             << setw(10) << totalClasses
             << setw(10) << attendedClasses
             << setw(12) << fixed << setprecision(1) << getAttendancePercent()
             << setw(8)  << marks
             << setw(6)  << grade
             << "\n";
    }
};

// ─────────────────────────────────────────
//  Global vector to hold all students
// ─────────────────────────────────────────
vector<Student> students;
const string DATA_FILE = "attendance_data.txt";

// ─────────────────────────────────────────
//  Function Prototypes
// ─────────────────────────────────────────
void addStudent();
void markAttendance();
void enterMarks();
void viewReport();
void saveToFile();
void loadFromFile();
int  findStudentIndex(int roll);  // returns index or -1 if not found

// ─────────────────────────────────────────
//  addStudent: Register a new student
// ─────────────────────────────────────────
void addStudent() {
    int    roll;
    string name;

    cout << "\n--- Add New Student ---\n";
    cout << "Enter Roll Number: ";
    cin >> roll;

    // basic validation: no duplicate rolls
    if (findStudentIndex(roll) != -1) {
        cout << "Error: Student with Roll " << roll << " already exists!\n";
        return;
    }

    cin.ignore(); // clear newline left in buffer
    cout << "Enter Student Name: ";
    getline(cin, name);

    if (name.empty()) {
        cout << "Error: Name cannot be empty.\n";
        return;
    }

    students.push_back(Student(roll, name));
    cout << "Student added successfully!\n";
}

// ─────────────────────────────────────────
//  markAttendance: Mark present/absent for a class
// ─────────────────────────────────────────
void markAttendance() {
    if (students.empty()) {
        cout << "\nNo students found. Please add students first.\n";
        return;
    }

    cout << "\n--- Mark Attendance ---\n";
    cout << "Marking attendance for ALL students for today's class.\n\n";

    for (int i = 0; i < (int)students.size(); i++) {
        char choice;
        cout << "Roll " << students[i].getRoll()
             << " - " << students[i].getName()
             << " | Present? (y/n): ";
        cin >> choice;

        // accept both upper and lower case
        if (choice == 'y' || choice == 'Y') {
            students[i].addAttendance(true);
            cout << "  Marked: Present\n";
        } else {
            students[i].addAttendance(false);
            cout << "  Marked: Absent\n";
        }
    }

    cout << "\nAttendance marked for all students!\n";
}

// ─────────────────────────────────────────
//  enterMarks: Enter exam marks for a student
// ─────────────────────────────────────────
void enterMarks() {
    if (students.empty()) {
        cout << "\nNo students found. Please add students first.\n";
        return;
    }

    int roll;
    cout << "\n--- Enter Marks ---\n";
    cout << "Enter Roll Number: ";
    cin >> roll;

    int idx = findStudentIndex(roll);
    if (idx == -1) {
        cout << "Student not found!\n";
        return;
    }

    float marks;
    cout << "Enter Marks for " << students[idx].getName()
         << " (0 to 100): ";
    cin >> marks;

    // validate marks range
    if (marks < 0 || marks > 100) {
        cout << "Error: Marks must be between 0 and 100.\n";
        return;
    }

    students[idx].setMarks(marks);
    cout << "Marks entered. Grade assigned: " << students[idx].getGrade() << "\n";
}

// ─────────────────────────────────────────
//  viewReport: Show report table for all students
// ─────────────────────────────────────────
void viewReport() {
    if (students.empty()) {
        cout << "\nNo student records to display.\n";
        return;
    }

    cout << "\n========== STUDENT REPORT ==========\n";
    cout << left
         << setw(8)  << "Roll"
         << setw(20) << "Name"
         << setw(10) << "Total"
         << setw(10) << "Present"
         << setw(12) << "Attend%"
         << setw(8)  << "Marks"
         << setw(6)  << "Grade"
         << "\n";
    cout << string(74, '-') << "\n";

    for (const Student& s : students) {
        s.printReport();
    }

    cout << string(74, '=') << "\n";
    cout << "Total Students: " << students.size() << "\n";
}

// ─────────────────────────────────────────
//  saveToFile: Write all student data to file
// ─────────────────────────────────────────
void saveToFile() {
    ofstream file(DATA_FILE);

    if (!file.is_open()) {
        cout << "Error: Could not open file for saving!\n";
        return;
    }

    // each student saved as one line:
    // roll name totalClasses attendedClasses marks
    for (const Student& s : students) {
        file << s.getRoll()     << " "
             << s.getName()     << " "  // note: names with spaces will break this
             << s.getTotal()    << " "
             << s.getAttended() << " "
             << s.getMarks()    << "\n";
    }

    file.close();
    cout << "Data saved to " << DATA_FILE << " successfully!\n";
}

// ─────────────────────────────────────────
//  loadFromFile: Read student data from file at startup
// ─────────────────────────────────────────
void loadFromFile() {
    ifstream file(DATA_FILE);

    if (!file.is_open()) {
        // file doesn't exist yet — that's fine on first run
        cout << "No existing data file found. Starting fresh.\n";
        return;
    }

    students.clear(); // clear any existing data first

    int    roll, total, attended;
    float  marks;
    string name;

    while (file >> roll >> name >> total >> attended >> marks) {
        Student s;
        s.setAllFields(roll, name, total, attended, marks);
        students.push_back(s);
    }

    file.close();
    cout << "Loaded " << students.size() << " student(s) from file.\n";
}

// ─────────────────────────────────────────
//  findStudentIndex: Search by roll number
// ─────────────────────────────────────────
int findStudentIndex(int roll) {
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].getRoll() == roll)
            return i;
    }
    return -1; // not found
}

// ─────────────────────────────────────────
//  main: Menu-driven entry point
// ─────────────────────────────────────────
int main() {
    cout << "======================================\n";
    cout << "  Smart Attendance & Performance Tracker\n";
    cout << "======================================\n";

    // load saved data when program starts
    loadFromFile();

    int choice;

    do {
        cout << "\n====== MAIN MENU ======\n";
        cout << "1. Add Student\n";
        cout << "2. Mark Attendance\n";
        cout << "3. Enter Marks\n";
        cout << "4. View Report\n";
        cout << "5. Save Data\n";
        cout << "0. Exit\n";
        cout << "========================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent();    break;
            case 2: markAttendance(); break;
            case 3: enterMarks();    break;
            case 4: viewReport();    break;
            case 5: saveToFile();    break;
            case 0:
                // auto-save on exit so user doesn't lose data
                cout << "\nSaving data before exit...\n";
                saveToFile();
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please enter 0-5.\n";
        }

    } while (choice != 0);

    return 0;
}
