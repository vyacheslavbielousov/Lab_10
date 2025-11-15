#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// Constants
const int MAX_STUDENTS = 100;
const int MAX_EXAMS = 5;
const int MAX_NAME_LEN = 50;
const int MAX_FACULTY_LEN = 20;

// Structure definition for Student
struct Student {
    char faculty[MAX_FACULTY_LEN];  // Faculty name
    int course;                      // Course number (1-5)
    int group;                       // Group number
    char surname[MAX_NAME_LEN];      // Student's surname
    int grades[MAX_EXAMS];          // Exam grades (2-5)
    int numExams;                   // Number of exams taken
    bool isExpelled;                // Flag if student is expelled
};

// Global array of students and count
Student students[MAX_STUDENTS];
int studentCount = 0;

// Faculty names for random generation
const char* faculties[] = { "FIT", "FMM", "FEF", "FHT", "FBT" };
const int facultyCount = 5;

// Surnames for random generation
const char* surnames[] = {
    "Smith", "Johnson", "Williams", "Brown", "Jones",
    "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
    "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson",
    "Thomas", "Taylor", "Moore", "Jackson", "Martin",
    "Lee", "Perez", "Thompson", "White", "Harris"
};
const int surnameCount = 25;

// Function prototypes
void displayMenu();
void createStudentArray();
void displayStudentArray();
void query1_FindAndExpelFailingStudents();
void query2_FindFacultyWithMostExcellentStudents();
void query3_FindCourseWithMostExpulsions();
void generateRandomStudent(Student* s, int index);
void inputStudentManually(Student* s);
void printTableHeader();
void printStudent(const Student* s, int index);
int countFailingGrades(const Student* s);
bool hasTwoOrMoreFailingGrades(const Student* s);
bool isExcellentStudent(const Student* s);
void clearInputBuffer();

int main() {
    srand(time(NULL)); // Initialize random seed

    int choice;
    bool running = true;

    cout << "========================================" << endl;
    cout << "   STUDENT RECORDS MANAGEMENT SYSTEM   " << endl;
    cout << "========================================" << endl;

    while (running) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();

        cout << endl;

        switch (choice) {
        case 1:
            createStudentArray();
            break;
        case 2:
            displayStudentArray();
            break;
        case 3:
            query1_FindAndExpelFailingStudents();
            break;
        case 4:
            query2_FindFacultyWithMostExcellentStudents();
            break;
        case 5:
            query3_FindCourseWithMostExpulsions();
            break;
        case 0:
            cout << "Exiting program. Goodbye!" << endl;
            running = false;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }

        if (running) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }

    return 0;
}

// Display main menu
void displayMenu() {
    cout << "\n======================================" << endl;
    cout << "              MAIN MENU               " << endl;
    cout << "======================================" << endl;
    cout << "1. Create student array" << endl;
    cout << "2. Display student array" << endl;
    cout << "3. Query 1: Find and expel FIT students with 2+ failing grades" << endl;
    cout << "4. Query 2: Find faculty with most excellent 1st year students" << endl;
    cout << "5. Query 3: Find course with most expulsions" << endl;
    cout << "0. Exit" << endl;
    cout << "======================================" << endl;
}

// Create student array (manual or random generation)
void createStudentArray() {
    cout << "=== CREATE STUDENT ARRAY ===" << endl;
    cout << "Enter number of students (1-" << MAX_STUDENTS << "): ";
    cin >> studentCount;
    clearInputBuffer();

    if (studentCount < 1 || studentCount > MAX_STUDENTS) {
        cout << "Invalid number! Setting to 10." << endl;
        studentCount = 10;
    }

    cout << "\nChoose input method:" << endl;
    cout << "1. Manual input" << endl;
    cout << "2. Random generation" << endl;
    cout << "Choice: ";

    int method;
    cin >> method;
    clearInputBuffer();

    if (method == 1) {
        // Manual input
        for (int i = 0; i < studentCount; i++) {
            cout << "\n--- Student #" << (i + 1) << " ---" << endl;
            inputStudentManually(&students[i]);
        }
    }
    else {
        // Random generation
        for (int i = 0; i < studentCount; i++) {
            generateRandomStudent(&students[i], i);
        }
        cout << "\n" << studentCount << " students generated successfully!" << endl;
    }
}

// Generate random student data
void generateRandomStudent(Student* s, int index) {
    // Random faculty
    strcpy(s->faculty, faculties[rand() % facultyCount]);

    // Random course (1-5)
    s->course = 1 + rand() % 5;

    // Random group (1-5)
    s->group = 1 + rand() % 5;

    // Random surname
    strcpy(s->surname, surnames[rand() % surnameCount]);

    // Random number of exams (3-5)
    s->numExams = 3 + rand() % 3;

    // Random grades (2-5, with higher probability for 3-5)
    for (int i = 0; i < s->numExams; i++) {
        int randVal = rand() % 100;
        if (randVal < 10) {
            s->grades[i] = 2; // 10% chance of failing grade
        }
        else if (randVal < 40) {
            s->grades[i] = 3; // 30% chance
        }
        else if (randVal < 70) {
            s->grades[i] = 4; // 30% chance
        }
        else {
            s->grades[i] = 5; // 30% chance
        }
    }

    s->isExpelled = false;
}

// Input student data manually
void inputStudentManually(Student* s) {
    cout << "Faculty: ";
    cin.getline(s->faculty, MAX_FACULTY_LEN);

    cout << "Course (1-5): ";
    cin >> s->course;

    cout << "Group: ";
    cin >> s->group;
    clearInputBuffer();

    cout << "Surname: ";
    cin.getline(s->surname, MAX_NAME_LEN);

    cout << "Number of exams (1-" << MAX_EXAMS << "): ";
    cin >> s->numExams;

    if (s->numExams < 1) s->numExams = 1;
    if (s->numExams > MAX_EXAMS) s->numExams = MAX_EXAMS;

    cout << "Enter grades (2-5):" << endl;
    for (int i = 0; i < s->numExams; i++) {
        cout << "  Exam " << (i + 1) << ": ";
        cin >> s->grades[i];
        if (s->grades[i] < 2) s->grades[i] = 2;
        if (s->grades[i] > 5) s->grades[i] = 5;
    }
    clearInputBuffer();

    s->isExpelled = false;
}

// Print table header
void printTableHeader() {
    cout << "-------------------------------------------------------------------------------------" << endl;
    cout << "| No | Faculty |  Course | Group | Surname           | Grades      | Status    |" << endl;
    cout << "-------------------------------------------------------------------------------------" << endl;
}

// Print single student record
void printStudent(const Student* s, int index) {
    cout << "| ";

    // Number
    if (index < 10) cout << " ";
    cout << index << " | ";

    // Faculty
    cout << s->faculty;
    for (int i = strlen(s->faculty); i < 7; i++) cout << " ";
    cout << " | ";

    // Course
    cout << "   " << s->course << "    | ";

    // Group
    cout << "  " << s->group << "   | ";

    // Surname
    cout << s->surname;
    for (int i = strlen(s->surname); i < 17; i++) cout << " ";
    cout << " | ";

    // Grades
    for (int i = 0; i < s->numExams && i < 3; i++) {
        cout << s->grades[i] << " ";
    }
    for (int i = s->numExams; i < 3; i++) {
        cout << "  ";
    }
    cout << "       | ";

    // Status
    if (s->isExpelled) {
        cout << "EXPELLED  |";
    }
    else {
        cout << "Active    |";
    }

    cout << endl;
}

// Display all students in table format
void displayStudentArray() {
    if (studentCount == 0) {
        cout << "No students in the array! Please create student array first." << endl;
        return;
    }

    cout << "\n=== STUDENT RECORDS TABLE ===" << endl;
    cout << "Total students: " << studentCount << endl << endl;

    printTableHeader();

    for (int i = 0; i < studentCount; i++) {
        printStudent(&students[i], i + 1);
    }

    cout << "-------------------------------------------------------------------------------------" << endl;
}

// Count number of failing grades (2) for a student
// This function checks ALL exams for the student
int countFailingGrades(const Student* s) {
    int count = 0;

    // Loop through ALL exams the student has taken
    for (int i = 0; i < s->numExams; i++) {
        if (s->grades[i] == 2) {
            count++;
        }
    }

    return count;
}

// Check if student has 2 or more failing grades
bool hasTwoOrMoreFailingGrades(const Student* s) {
    return countFailingGrades(s) >= 2;
}

// Check if student is excellent (all grades are 5)
bool isExcellentStudent(const Student* s) {
    if (s->isExpelled) return false;

    for (int i = 0; i < s->numExams; i++) {
        if (s->grades[i] != 5) {
            return false;
        }
    }
    return true;
}

// Query 1: Find and expel FIT students with 2 or more failing grades
void query1_FindAndExpelFailingStudents() {
    if (studentCount == 0) {
        cout << "No students in the array!" << endl;
        return;
    }

    cout << "\n=== QUERY 1: FIT Students with 2+ Failing Grades ===" << endl;

    int expelledCount = 0;

    cout << "\nStudents to be expelled:" << endl;
    printTableHeader();

    for (int i = 0; i < studentCount; i++) {
        Student* s = &students[i];

        // Check if student is from FIT faculty and has 2+ failing grades
        if (strcmp(s->faculty, "FIT") == 0 && !s->isExpelled) {
            // Count ALL failing grades across ALL exams
            int failCount = countFailingGrades(s);

            // Debug output to show what we're checking
            // (Remove this in production, just for verification)
            if (failCount > 0) {
                cout << "| DEBUG: " << s->surname << " has " << failCount
                    << " failing grades out of " << s->numExams << " exams";
                cout << " (Grades: ";
                for (int j = 0; j < s->numExams; j++) {
                    cout << s->grades[j];
                    if (j < s->numExams - 1) cout << ", ";
                }
                cout << ")";
                for (int j = 0; j < 30; j++) cout << " ";
                cout << "|" << endl;
            }

            // If student has 2 or more failing grades, expel them
            if (hasTwoOrMoreFailingGrades(s)) {
                printStudent(s, i + 1);
                s->isExpelled = true;
                expelledCount++;
            }
        }
    }

    if (expelledCount == 0) {
        cout << "| No students found with 2+ failing grades on FIT faculty                          |" << endl;
    }

    cout << "-------------------------------------------------------------------------------------" << endl;
    cout << "\nTotal expelled from FIT: " << expelledCount << endl;
}

// Query 2: Find faculty with most excellent students on 1st course
void query2_FindFacultyWithMostExcellentStudents() {
    if (studentCount == 0) {
        cout << "No students in the array!" << endl;
        return;
    }

    cout << "\n=== QUERY 2: Faculty with Most Excellent 1st Year Students ===" << endl;

    // Count excellent students per faculty on 1st course
    int excellentCount[10] = { 0 }; // Max 10 different faculties
    char facultyNames[10][MAX_FACULTY_LEN];
    int facultyIndex = 0;

    for (int i = 0; i < studentCount; i++) {
        Student* s = &students[i];

        if (s->course == 1 && isExcellentStudent(s)) {
            // Find or add faculty
            int fIndex = -1;
            for (int j = 0; j < facultyIndex; j++) {
                if (strcmp(facultyNames[j], s->faculty) == 0) {
                    fIndex = j;
                    break;
                }
            }

            if (fIndex == -1) {
                strcpy(facultyNames[facultyIndex], s->faculty);
                fIndex = facultyIndex;
                facultyIndex++;
            }

            excellentCount[fIndex]++;
        }
    }

    // Find maximum
    int maxCount = 0;
    int maxFacultyIdx = -1;

    for (int i = 0; i < facultyIndex; i++) {
        if (excellentCount[i] > maxCount) {
            maxCount = excellentCount[i];
            maxFacultyIdx = i;
        }
    }

    cout << "\nStatistics by faculty (1st course excellent students):" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "| Faculty | Excellent Students (All 5s)  |" << endl;
    cout << "---------------------------------------------" << endl;

    for (int i = 0; i < facultyIndex; i++) {
        cout << "| " << facultyNames[i];
        for (int j = strlen(facultyNames[i]); j < 7; j++) cout << " ";
        cout << " | " << excellentCount[i];
        if (excellentCount[i] < 10) cout << " ";
        cout << "                           |" << endl;
    }
    cout << "---------------------------------------------" << endl;

    if (maxFacultyIdx != -1) {
        cout << "\nFaculty with MOST excellent 1st year students: "
            << facultyNames[maxFacultyIdx]
            << " (" << maxCount << " students)" << endl;
    }
    else {
        cout << "\nNo excellent students found on 1st course." << endl;
    }
}

// Query 3: Find course with most expulsions
void query3_FindCourseWithMostExpulsions() {
    if (studentCount == 0) {
        cout << "No students in the array!" << endl;
        return;
    }

    cout << "\n=== QUERY 3: Course with Most Expelled Students ===" << endl;

    // Count expelled students per course
    int expelledByCourse[6] = { 0 }; // Courses 1-5 (index 1-5, 0 unused)

    for (int i = 0; i < studentCount; i++) {
        Student* s = &students[i];

        if (s->isExpelled && s->course >= 1 && s->course <= 5) {
            expelledByCourse[s->course]++;
        }
    }

    // Find maximum
    int maxCount = 0;
    int maxCourse = -1;

    for (int i = 1; i <= 5; i++) {
        if (expelledByCourse[i] > maxCount) {
            maxCount = expelledByCourse[i];
            maxCourse = i;
        }
    }

    cout << "\nStatistics by course:" << endl;
    cout << "-------------------------------------" << endl;
    cout << "| Course | Expelled Students      |" << endl;
    cout << "-------------------------------------" << endl;

    for (int i = 1; i <= 5; i++) {
        cout << "|   " << i << "    | " << expelledByCourse[i];
        if (expelledByCourse[i] < 10) cout << " ";
        cout << "                     |" << endl;
    }
    cout << "-------------------------------------" << endl;

    if (maxCourse != -1 && maxCount > 0) {
        cout << "\nCourse with MOST expulsions: Course " << maxCourse
            << " (" << maxCount << " students)" << endl;
    }
    else {
        cout << "\nNo students have been expelled yet." << endl;
    }
}

// Clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}