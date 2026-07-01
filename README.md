# 🏥 Hospital Management System (C++)

A console-based Hospital Management System developed in **C++** using **Object-Oriented Programming (OOP)** concepts and **File Handling**. The system manages hospital operations such as patient registration, doctor management, appointments, medical reports, and user authentication.

---

## 📌 Project Overview

This project simulates a real-world hospital management environment where different users (Admin, Receptionist, Doctor, and Patient) have separate roles and permissions.

The system stores all data in text files, allowing information to persist even after the program is closed.

---

## ✨ Features

### 👨‍💼 Admin
- Login authentication
- Add Receptionists
- Add Doctors
- Remove Receptionists
- Remove Doctors
- View all hospital staff
- Change password

### 🧑‍💻 Receptionist
- Login authentication
- Register new patients
- Assign doctors to patients
- View all registered patients
- Change password

### 👨‍⚕️ Doctor
- Login authentication
- View assigned patients
- View patient medical reports
- Add medical notes and prescriptions
- View upcoming appointments
- Change password

### 🧑 Patient
- Login authentication
- View medical history
- Book appointments
- View booked appointments
- Change password

---

# 🛠 Technologies Used

- C++
- Object-Oriented Programming (OOP)
- File Handling
- STL Vector
- String Manipulation
- Console-Based User Interface

---

# 📂 Project Structure

```
Hospital Management System
│
├── main.cpp
├── admins.txt
├── receptionists.txt
├── doctors.txt
├── patients.txt
├── appointments.txt
├── reports.txt
└── README.md
```

---

# 💾 Data Storage

The project uses text files to store data permanently.

| File | Purpose |
|------|----------|
| admins.txt | Admin Information |
| receptionists.txt | Receptionist Records |
| doctors.txt | Doctor Records |
| patients.txt | Patient Records |
| appointments.txt | Appointment Details |
| reports.txt | Medical Reports |

---

# 🔑 Default Admin Login

When the project runs for the first time, it automatically creates a default administrator.

**ID**

```
admin
```

**Password**

```
admin123
```

---

# 🧱 Object-Oriented Concepts Used

- Classes and Objects
- Inheritance
- Encapsulation
- Constructors
- Function Overriding
- Polymorphism
- Data Abstraction

---

# 📚 Data Structures Used

- **Vector**
  - Stores Admins
  - Receptionists
  - Doctors
  - Patients
  - Appointments
  - Reports

---

# 📖 File Handling

The system performs:

- Reading from files
- Writing to files
- Updating records
- Persistent storage

using

```cpp
ifstream
ofstream
```

---

# 🔄 System Workflow

```
Start Program
      │
      ▼
Login Screen
      │
      ▼
Select Role
      │
      ├── Admin
      ├── Receptionist
      ├── Doctor
      └── Patient
      │
      ▼
Perform Operations
      │
      ▼
Save Data
      │
      ▼
Logout
```

---

# 🚀 How to Run

1. Open the project in any C++ IDE
   - Code::Blocks
   - Visual Studio
   - Dev-C++
   - VS Code

2. Compile the project

3. Run the executable

4. Login using the default Admin credentials.

---

# 📋 Requirements

- C++11 or later
- Windows/Linux
- GCC Compiler or MSVC

---

# 👥 Team Members

| Name | Contribution |
|------|--------------|
| Afzaal | Utility Functions, Domain Models |
| Saim | System Manager, File Handling, Admin Module |
| Armaan | Receptionist Module, Doctor Module, Patient Module, Authentication System |

---

# 📈 Future Improvements

- Password encryption
- Billing System
- Pharmacy Module
- Laboratory Module
- Search functionality
- Appointment cancellation
- GUI version
- Database (MySQL) integration
- Email/SMS notifications

---

# 📄 License

This project is developed for educational purposes as part of a university Database/Data Structures & OOP project.

---

## ⭐ Acknowledgement

This Hospital Management System was developed as a collaborative academic project to demonstrate the practical implementation of Object-Oriented Programming, File Handling, and Data Structures in C++.
