# Smart-Attendance-Tracker

A console-based C++ project I built as part of my CPP learning.
The idea was simple — I wanted to make something actually useful instead of just a textbook exercise.

---

## What It Does

- Add students with roll number and name
- Mark daily attendance (present/absent) for all students at once
- Enter exam marks and auto-assign grades
- View a full report table with attendance percentage and grade
- Save all data to a file so it persists between runs

---

## Grade Logic

| Marks | Grade |
|-------|-------|
| 85 and above | A |
| 70 – 84 | B |
| 50 – 69 | C |
| Below 50 | Fail |

---

## Concepts Used

- OOP (class, constructor, encapsulation)
- `vector<Student>` for dynamic storage
- `fstream` for reading and writing data to a `.txt` file
- Modular functions for each feature
- Basic input validation

---


## Folder Structure

```
SmartAttendanceTracker/
├── main.cpp
├── attendance_data.txt  
└── README.md
```

---

## Known Limitations

- Student names with spaces might not load correctly from file (single-word names work fine)
- No login or authentication — it's a simple local system
- Data is stored in plain text, not a database

I plan to fix the named issue in a future version .

---

## Sara Doegar 
