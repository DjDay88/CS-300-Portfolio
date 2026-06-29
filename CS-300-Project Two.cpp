#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Course object stores the course number, course title, and prerequisites.
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Remove extra spaces from the front and back of a string.
string trim(const string& value) {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }

    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

// Convert course numbers to uppercase so user input is not case sensitive.
string toUpperCase(string value) {
    transform(value.begin(), value.end(), value.begin(), ::toupper);
    return value;
}

// Split a CSV line by commas.
vector<string> splitLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream lineStream(line);

    while (getline(lineStream, field, ',')) {
        fields.push_back(trim(field));
    }

    return fields;
}

// Load course information from the CSV file into a map.
bool loadCourses(const string& fileName, map<string, Course>& courses) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file " << fileName << "." << endl;
        return false;
    }

    courses.clear();
    string line;

    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitLine(line);

        if (fields.size() < 2) {
            cout << "Error: Invalid course data found." << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields.at(0));
        course.title = fields.at(1);

        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields.at(i).empty()) {
                course.prerequisites.push_back(toUpperCase(fields.at(i)));
            }
        }

        courses[course.courseNumber] = course;
    }

    inputFile.close();

    // Check that each prerequisite listed in the file also exists as a course.
    for (const auto& coursePair : courses) {
        for (const string& prerequisite : coursePair.second.prerequisites) {
            if (courses.find(prerequisite) == courses.end()) {
                cout << "Warning: Prerequisite " << prerequisite
                     << " for " << coursePair.first << " was not found in the course list." << endl;
            }
        }
    }

    cout << "Course data loaded successfully." << endl;
    return true;
}

// Print all courses in alphanumeric order.
void printCourseList(const map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "No course data loaded. Please load the data structure first." << endl;
        return;
    }

    cout << "Here is a sample schedule:" << endl;

    for (const auto& coursePair : courses) {
        cout << coursePair.second.courseNumber << ", " << coursePair.second.title << endl;
    }
}

// Print one course and its prerequisites.
void printCourseInformation(const map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "No course data loaded. Please load the data structure first." << endl;
        return;
    }

    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    courseNumber = toUpperCase(trim(courseNumber));

    auto courseIterator = courses.find(courseNumber);

    if (courseIterator == courses.end()) {
        cout << "Course not found." << endl;
        return;
    }

    Course course = courseIterator->second;
    cout << course.courseNumber << ", " << course.title << endl;

    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites.at(i);

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

// Print the main menu.
void printMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
}

int main() {
    map<string, Course> courses;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        printMenu();
        cout << "What would you like to do? ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                string fileName;
                cout << "Enter the course data file name: ";
                cin.ignore(10000, '\n');
                getline(cin, fileName);
                loadCourses(trim(fileName), courses);
                break;
            }
            case 2:
                printCourseList(courses);
                break;
            case 3:
                printCourseInformation(courses);
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << choice << " is not a valid option." << endl;
                break;
        }
    }

    return 0;
}