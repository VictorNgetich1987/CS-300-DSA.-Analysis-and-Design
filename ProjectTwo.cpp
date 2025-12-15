#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

/*
 * Course structure to hold course information
 */
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

/*
 * Split a line by commas (simple CSV parsing without external libraries)
 */
vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/*
 * Load course data from file into data structure
 */
void loadCourses(
    const string& fileName,
    unordered_map<string, Course>& courses,
    vector<string>& courseKeys
) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Unable to open file " << fileName << endl;
        return;
    }

    courses.clear();
    courseKeys.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> tokens = split(line, ',');

        Course course;
        course.courseNumber = tokens[0];
        course.title = tokens[1];

        // Remaining tokens are prerequisites
        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }

        courses[course.courseNumber] = course;
        courseKeys.push_back(course.courseNumber);
    }

    file.close();
    cout << "Course data loaded successfully." << endl;
}

/*
 * Print sorted list of all courses
 */
void printCourseList(
    unordered_map<string, Course>& courses,
    vector<string>& courseKeys
) {
    if (courses.empty()) {
        cout << "Error: No data loaded. Please load a file first." << endl;
        return;
    }

    sort(courseKeys.begin(), courseKeys.end());

    cout << "\nCourse List:\n";
    for (const string& key : courseKeys) {
        cout << key << ", " << courses[key].title << endl;
    }
}

/*
 * Print information for a single course
 */
void printCourseInfo(
    unordered_map<string, Course>& courses
) {
    if (courses.empty()) {
        cout << "Error: No data loaded. Please load a file first." << endl;
        return;
    }

    string courseNumber;
    cout << "Enter course number: ";
    cin >> courseNumber;

    auto it = courses.find(courseNumber);
    if (it == courses.end()) {
        cout << "Error: Course not found." << endl;
        return;
    }

    Course course = it->second;

    cout << "\n" << course.courseNumber << ", " << course.title << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

/*
 * Display menu options
 */
void displayMenu() {
    cout << "\nMenu:\n";
    cout << "  1. Load Data Structure\n";
    cout << "  2. Print Course List\n";
    cout << "  3. Print Course\n";
    cout << "  9. Exit\n";
    cout << "Enter choice: ";
}

/*
 * Main program
 */
int main() {
    unordered_map<string, Course> courses;
    vector<string> courseKeys;

    int choice = 0;
    string fileName;

    cout << "Welcome to the ABCU Advising Assistance Program\n";

    while (choice != 9) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter file name: ";
                cin >> fileName;
                loadCourses(fileName, courses, courseKeys);
                break;

            case 2:
                printCourseList(courses, courseKeys);
                break;

            case 3:
                printCourseInfo(courses);
                break;

            case 9:
                cout << "Thank you for using the program. Goodbye!" << endl;
                break;

            default:
                cout << "Error: Invalid menu option." << endl;
        }
    }

    return 0;
}

