// =========================================================================
// ======================= AFZAAL'S CODE STARTS HERE =======================
// =========================================================================
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

using namespace std;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================
int getChoice()
{
    int choice;
    while (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

string getStringInput()
{
    string input;
    getline(cin, input);
    return input;
}

vector<string> split(const string &s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

string trim(const string &s)
{
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start]))
        ++start;
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1]))
        --end;
    return s.substr(start, end - start);
}

void pauseAndClear()
{
    cout << "\nPress Enter to continue...";
    string temp;
    getline(cin, temp);
}

void displayHeader(const string &panelName)
{
    cout << "\033[2J\033[1;1H"; // Clear screen
    cout << "\n=======================================================\n";
    cout << "  _   _                 _ _        _                   \n";
    cout << " | | | | ___  ___ _ __ (_) |_ __ _| |                  \n";
    cout << " | |_| |/ _ \\/ __| '_ \\| | __/ _` | |                \n";
    cout << " |  _  | (_) \\__ \\ |_) | | || (_| | |                \n";
    cout << " |_| |_|\\___/|___/ .__/|_|\\__\\__,_|_|                \n";
    cout << "                 |_|                                   \n";
    cout << "=======================================================\n";
    cout << "              HOSPITAL MANAGEMENT SYSTEM               \n";
    cout << "=======================================================\n";
    if (!panelName.empty())
    {
        int padding = (55 - panelName.length()) / 2;
        for (int i = 0; i < padding; ++i)
            cout << " ";
        cout << panelName << "\n=======================================================\n";
    }
}

// ==========================================
// DOMAIN MODELS (OOP)
// ==========================================
class Person
{
protected:
    string id;
    string password;
    string name;

public:
    Person(string i, string p, string n) : id(i), password(p), name(n) {}
    string getId() const { return id; }
    string getPassword() const { return password; }
    string getName() const { return name; }
    void setPassword(const string &newPass) { password = newPass; }
    virtual string toFileString() const { return id + "|" + password + "|" + name; }
    virtual ~Person() {}
};

class Admin : public Person
{
public:
    Admin(string i, string p, string n) : Person(i, p, n) {}
};

class Receptionist : public Person
{
public:
    Receptionist(string i, string p, string n) : Person(i, p, n) {}
};

class Doctor : public Person
{
private:
    string specialization;

public:
    Doctor(string i, string p, string n, string spec) : Person(i, p, n), specialization(spec) {}
    string getSpecialization() const { return specialization; }
    string toFileString() const override
    {
        return id + "|" + password + "|" + name + "|" + specialization;
    }
};

class Patient : public Person
{
private:
    string referredDoctorId;

public:
    Patient(string i, string p, string n, string refDocId) : Person(i, p, n), referredDoctorId(refDocId) {}
    string getReferredDoctorId() const { return referredDoctorId; }
    string toFileString() const override
    {
        return id + "|" + password + "|" + name + "|" + referredDoctorId;
    }
};

struct Appointment
{
    string patientId;
    string doctorId;
    string dateTime;
    string toFileString() const { return patientId + "|" + doctorId + "|" + dateTime; }
};

struct Report
{
    string patientId;
    string doctorId;
    string notes;
    string toFileString() const { return patientId + "|" + doctorId + "|" + notes; }
};
// =========================================================================
// ======================== AFZAAL'S CODE ENDS HERE ========================
// =========================================================================

// =========================================================================
// ======================== SAIM'S CODE STARTS HERE ========================
// =========================================================================

// ==========================================
// SYSTEM MANAGER (Core Logic & File I/O)
// ==========================================
class SystemManager
{
private:
    vector<Admin> admins;
    vector<Receptionist> receptionists;
    vector<Doctor> doctors;
    vector<Patient> patients;
    vector<Appointment> appointments;
    vector<Report> reports;

    const string f_admins = "admins.txt";
    const string f_receptionists = "receptionists.txt";
    const string f_doctors = "doctors.txt";
    const string f_patients = "patients.txt";
    const string f_appointments = "appointments.txt";
    const string f_reports = "reports.txt";

    void ensureFileExists(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            ofstream out(filename);
            out.close();
        }
    }

    void loadData()
    {
        ensureFileExists(f_admins);
        ifstream in_admins(f_admins);
        string line;
        while (getline(in_admins, line))
        {
            vector<string> data = split(line, '|');
            for (auto &t : data)
                t = trim(t);
            if (data.size() >= 3)
                admins.push_back(Admin(data[0], data[1], data[2]));
        }
        in_admins.close();
        if (admins.empty())
        {
            admins.push_back(Admin("admin", "admin123", "System Admin"));
            saveAdmins();
        }

        ensureFileExists(f_receptionists);
        ifstream in_recep(f_receptionists);
        while (getline(in_recep, line))
        {
            vector<string> data = split(line, '|');
            for (auto &t : data)
                t = trim(t);
            if (data.size() >= 3)
                receptionists.push_back(Receptionist(data[0], data[1], data[2]));
        }
        in_recep.close();

        ensureFileExists(f_doctors);
        ifstream in_docs(f_doctors);
        while (getline(in_docs, line))
        {
            vector<string> data = split(line, '|');
            for (auto &t : data)
                t = trim(t);
            if (data.size() >= 3)
            {
                string spec = (data.size() >= 4) ? data[3] : string();
                doctors.push_back(Doctor(data[0], data[1], data[2], spec));
            }
        }
        in_docs.close();

        ensureFileExists(f_patients);
        ifstream in_pat(f_patients);
        while (getline(in_pat, line))
        {
            vector<string> data = split(line, '|');
            for (auto &t : data)
                t = trim(t);
            if (data.size() >= 3)
            {
                string refDoc = (data.size() >= 4) ? data[3] : string();
                patients.push_back(Patient(data[0], data[1], data[2], refDoc));
            }
        }
        in_pat.close();

        ensureFileExists(f_appointments);
        ifstream in_app(f_appointments);
        while (getline(in_app, line))
        {
            vector<string> data = split(line, '|');
            for (auto &t : data)
                t = trim(t);
            if (data.size() >= 3)
                appointments.push_back({data[0], data[1], data[2]});
        }
        in_app.close();

        ensureFileExists(f_reports);
        ifstream in_rep(f_reports);
        while (getline(in_rep, line))
        {
            vector<string> data = split(line, '|');
            for (auto &t : data)
                t = trim(t);
            if (data.size() >= 3)
                reports.push_back({data[0], data[1], data[2]});
        }
        in_rep.close();
    }

    void saveAdmins()
    {
        ofstream out(f_admins);
        for (const auto &a : admins)
            out << a.toFileString() << "\n";
    }
    void saveReceptionists()
    {
        ofstream out(f_receptionists);
        for (const auto &r : receptionists)
            out << r.toFileString() << "\n";
    }
    void saveDoctors()
    {
        ofstream out(f_doctors);
        for (const auto &d : doctors)
            out << d.toFileString() << "\n";
    }
    void savePatients()
    {
        ofstream out(f_patients);
        for (const auto &p : patients)
            out << p.toFileString() << "\n";
    }
    void saveAppointments()
    {
        ofstream out(f_appointments);
        for (const auto &a : appointments)
            out << a.toFileString() << "\n";
    }
    void saveReports()
    {
        ofstream out(f_reports);
        for (const auto &r : reports)
            out << r.toFileString() << "\n";
    }

    Doctor *findDoctor(const string &id)
    {
        for (auto &d : doctors)
            if (d.getId() == id)
                return &d;
        return nullptr;
    }
    Patient *findPatient(const string &id)
    {
        for (auto &p : patients)
            if (p.getId() == id)
                return &p;
        return nullptr;
    }

    // --- Menus ---
    void adminMenu(Admin &admin)
    {
        while (true)
        {
            displayHeader("ADMINISTRATOR PANEL");
            cout << " User: " << admin.getName() << "\n";
            cout << "-------------------------------------------------------\n";
            cout << " [1] Add Receptionist\n [2] Add Doctor\n [3] Remove Receptionist\n";
            cout << " [4] Remove Doctor\n [5] View All Staff\n [6] Change Password\n [0] Logout\n";
            cout << "-------------------------------------------------------\n";
            cout << " Enter choice: ";
            int choice = getChoice();

            if (choice == 0)
                break;

            if (choice == 1)
            {
                cout << "Enter Receptionist ID: ";
                string id = getStringInput();
                cout << "Enter Password: ";
                string pass = getStringInput();
                cout << "Enter Name: ";
                string name = getStringInput();
                receptionists.push_back(Receptionist(id, pass, name));
                saveReceptionists();
                cout << "Receptionist added successfully.\n";
            }
            else if (choice == 2)
            {
                cout << "Enter Doctor ID: ";
                string id = getStringInput();
                cout << "Enter Password: ";
                string pass = getStringInput();
                cout << "Enter Name: ";
                string name = getStringInput();
                cout << "Enter Specialization: ";
                string spec = getStringInput();
                doctors.push_back(Doctor(id, pass, name, spec));
                saveDoctors();
                cout << "Doctor added successfully.\n";
            }
            else if (choice == 3)
            {
                cout << "Enter Receptionist ID to remove: ";
                string id = getStringInput();
                bool found = false;
                for (auto it = receptionists.begin(); it != receptionists.end(); ++it)
                {
                    if (it->getId() == id)
                    {
                        receptionists.erase(it);
                        saveReceptionists();
                        cout << "Receptionist removed.\n";
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "Receptionist not found.\n";
            }
            else if (choice == 4)
            {
                cout << "Enter Doctor ID to remove: ";
                string id = getStringInput();
                bool found = false;
                for (auto it = doctors.begin(); it != doctors.end(); ++it)
                {
                    if (it->getId() == id)
                    {
                        doctors.erase(it);
                        saveDoctors();
                        cout << "Doctor removed.\n";
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "Doctor not found.\n";
            }
            else if (choice == 5)
            {
                cout << "\n--- Doctors ---\n";
                for (const auto &d : doctors)
                    cout << "[" << d.getId() << "] " << d.getName() << " (" << d.getSpecialization() << ")\n";
                cout << "\n--- Receptionists ---\n";
                for (const auto &r : receptionists)
                    cout << "[" << r.getId() << "] " << r.getName() << "\n";
            }
            else if (choice == 6)
            {
                cout << "Enter new password: ";
                string newPass = getStringInput();
                admin.setPassword(newPass);
                saveAdmins();
                cout << "Password updated successfully.\n";
            }
            pauseAndClear();
        }
    }
    // =========================================================================
    // ========================= SAIM'S CODE ENDS HERE =========================
    // =========================================================================

    // =========================================================================
    // ======================= ARMAAN'S CODE STARTS HERE =======================
    // =========================================================================
    void receptionistMenu(Receptionist &rec)
    {
        while (true)
        {
            displayHeader("RECEPTIONIST PANEL");
            cout << " User: " << rec.getName() << "\n";
            cout << "-------------------------------------------------------\n";
            cout << " [1] Register New Patient\n [2] View All Patients\n";
            cout << " [3] Change Password\n [0] Logout\n";
            cout << "-------------------------------------------------------\n";
            cout << " Enter choice: ";
            int choice = getChoice();

            if (choice == 0)
                break;
            if (choice == 1)
            {
                cout << "Enter Patient ID: ";
                string id = getStringInput();
                cout << "Enter Password: ";
                string pass = getStringInput();
                cout << "Enter Patient Name: ";
                string name = getStringInput();
                cout << "\nAvailable Doctors:\n";
                for (const auto &d : doctors)
                {
                    cout << "[" << d.getId() << "] " << d.getName() << " (" << d.getSpecialization() << ")\n";
                }
                cout << "Enter Referred Doctor ID: ";
                string docId = getStringInput();
                if (findDoctor(docId) != nullptr)
                {
                    patients.push_back(Patient(id, pass, name, docId));
                    savePatients();
                    cout << "Patient registered successfully.\n";
                }
                else
                {
                    cout << "Error: Doctor ID not found. Registration aborted.\n";
                }
            }
            else if (choice == 2)
            {
                cout << "\n--- Registered Patients ---\n";
                for (const auto &p : patients)
                {
                    cout << "[" << p.getId() << "] " << p.getName() << " (Doc: " << p.getReferredDoctorId() << ")\n";
                }
            }
            else if (choice == 3)
            {
                cout << "Enter new password: ";
                string newPass = getStringInput();
                rec.setPassword(newPass);
                saveReceptionists();
                cout << "Password updated successfully.\n";
            }
            pauseAndClear();
        }
    }

    void doctorMenu(Doctor &doc)
    {
        while (true)
        {
            displayHeader("DOCTOR PANEL");
            cout << " Dr. " << doc.getName() << " | Spec: " << doc.getSpecialization() << "\n";
            cout << "-------------------------------------------------------\n";
            cout << " [1] View Assigned Patients\n [2] View Patient Medical Reports\n";
            cout << " [3] Add Medical Note/Prescription\n [4] View Upcoming Appointments\n";
            cout << " [5] Change Password\n [0] Logout\n";
            cout << "-------------------------------------------------------\n";
            cout << " Enter choice: ";
            int choice = getChoice();

            if (choice == 0)
                break;
            if (choice == 1)
            {
                cout << "\n--- Assigned Patients ---\n";
                bool found = false;
                for (const auto &p : patients)
                {
                    if (p.getReferredDoctorId() == doc.getId())
                    {
                        cout << "[" << p.getId() << "] " << p.getName() << "\n";
                        found = true;
                    }
                }
                if (!found)
                    cout << "No patients currently assigned to you.\n";
            }
            else if (choice == 2)
            {
                cout << "Enter Patient ID: ";
                string pId = getStringInput();
                cout << "\n--- Reports for Patient " << pId << " ---\n";
                bool found = false;
                for (const auto &r : reports)
                {
                    if (r.patientId == pId)
                    {
                        cout << "By Doc " << r.doctorId << ": " << r.notes << "\n";
                        found = true;
                    }
                }
                if (!found)
                    cout << "No records found for this patient.\n";
            }
            else if (choice == 3)
            {
                cout << "Enter Patient ID: ";
                string pId = getStringInput();
                if (findPatient(pId) != nullptr)
                {
                    cout << "Enter Medical Note/Prescription: ";
                    string note = getStringInput();
                    reports.push_back({pId, doc.getId(), note});
                    saveReports();
                    cout << "Report added.\n";
                }
                else
                {
                    cout << "Patient not found.\n";
                }
            }
            else if (choice == 4)
            {
                cout << "\n--- Upcoming Appointments ---\n";
                bool found = false;
                for (const auto &a : appointments)
                {
                    if (a.doctorId == doc.getId())
                    {
                        Patient *p = findPatient(a.patientId);
                        string pName = p ? p->getName() : "Unknown";
                        cout << "Date/Time: " << a.dateTime << " | Patient: " << pName << " (" << a.patientId << ")\n";
                        found = true;
                    }
                }
                if (!found)
                    cout << "No appointments booked.\n";
            }
            else if (choice == 5)
            {
                cout << "Enter new password: ";
                string newPass = getStringInput();
                doc.setPassword(newPass);
                saveDoctors();
                cout << "Password updated successfully.\n";
            }
            pauseAndClear();
        }
    }

    void patientMenu(Patient &pat)
    {
        while (true)
        {
            displayHeader("PATIENT PANEL");
            cout << " Patient: " << pat.getName() << "\n";
            cout << "-------------------------------------------------------\n";
            cout << " [1] View My Medical Reports\n [2] Book an Appointment\n";
            cout << " [3] View My Appointments\n [4] Change Password\n [0] Logout\n";
            cout << "-------------------------------------------------------\n";
            cout << " Enter choice: ";
            int choice = getChoice();

            if (choice == 0)
                break;
            if (choice == 1)
            {
                cout << "\n--- My Medical History ---\n";
                bool found = false;
                for (const auto &r : reports)
                {
                    if (r.patientId == pat.getId())
                    {
                        Doctor *d = findDoctor(r.doctorId);
                        string dName = d ? d->getName() : "Unknown";
                        cout << "From Dr. " << dName << ": " << r.notes << "\n";
                        found = true;
                    }
                }
                if (!found)
                    cout << "No medical history found.\n";
            }
            else if (choice == 2)
            {
                cout << "\n--- Available Doctors ---\n";
                for (const auto &d : doctors)
                    cout << "[" << d.getId() << "] " << d.getName() << " (" << d.getSpecialization() << ")\n";
                cout << "Enter Doctor ID to book with: ";
                string dId = getStringInput();
                if (findDoctor(dId) != nullptr)
                {
                    cout << "Enter Date/Time: ";
                    string dt = getStringInput();
                    appointments.push_back({pat.getId(), dId, dt});
                    saveAppointments();
                    cout << "Booked.\n";
                }
                else
                {
                    cout << "Invalid Doctor ID.\n";
                }
            }
            else if (choice == 3)
            {
                cout << "\n--- My Appointments ---\n";
                bool found = false;
                for (const auto &a : appointments)
                {
                    if (a.patientId == pat.getId())
                    {
                        Doctor *d = findDoctor(a.doctorId);
                        string dName = d ? d->getName() : "Unknown";
                        cout << "Date: " << a.dateTime << " | Dr. " << dName << "\n";
                        found = true;
                    }
                }
                if (!found)
                    cout << "No appointments scheduled.\n";
            }
            else if (choice == 4)
            {
                cout << "Enter new password: ";
                string newPass = getStringInput();
                pat.setPassword(newPass);
                savePatients();
                cout << "Password updated.\n";
            }
            pauseAndClear();
        }
    }

public:
    SystemManager() { loadData(); }

    void start()
    {
        while (true)
        {
            displayHeader("");
            cout << "  [1] Admin Login\n  [2] Receptionist Login\n";
            cout << "  [3] Doctor Login\n  [4] Patient Login\n  [5] Exit System\n";
            cout << "-------------------------------------------------------\n";
            cout << " Enter your role: ";

            int roleChoice = getChoice();
            if (roleChoice == 5)
            {
                cout << "Exiting System. Goodbye!\n";
                break;
            }
            if (roleChoice < 1 || roleChoice > 5)
            {
                cout << "Invalid selection.\n";
                pauseAndClear();
                continue;
            }

            cout << "Enter ID: ";
            string id = getStringInput();
            cout << "Enter Password: ";
            string pass = getStringInput();
            bool loggedIn = false;

            if (roleChoice == 1)
            {
                for (auto &a : admins)
                    if (a.getId() == id && a.getPassword() == pass)
                    {
                        adminMenu(a);
                        loggedIn = true;
                        break;
                    }
            }
            else if (roleChoice == 2)
            {
                for (auto &r : receptionists)
                    if (r.getId() == id && r.getPassword() == pass)
                    {
                        receptionistMenu(r);
                        loggedIn = true;
                        break;
                    }
            }
            else if (roleChoice == 3)
            {
                for (auto &d : doctors)
                    if (d.getId() == id && d.getPassword() == pass)
                    {
                        doctorMenu(d);
                        loggedIn = true;
                        break;
                    }
            }
            else if (roleChoice == 4)
            {
                for (auto &p : patients)
                    if (p.getId() == id && p.getPassword() == pass)
                    {
                        patientMenu(p);
                        loggedIn = true;
                        break;
                    }
            }

            if (!loggedIn)
            {
                cout << "Error: Invalid ID/Password. Access Denied.\n";
                pauseAndClear();
            }
        }
    }
};

// ==========================================
// MAIN ENTRY POINT
// ==========================================
int main()
{
    SystemManager app;
    app.start();
    return 0;
}
// =========================================================================
// ======================== ARMAAN'S CODE ENDS HERE ========================
// =========================================================================