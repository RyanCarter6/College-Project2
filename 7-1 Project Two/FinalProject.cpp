//============================================================================
// Name        : FinalProject.cpp
// Author      : Ryan Carter
// Version     : 1.0
// Description : Loads and displays courses as a binary search tree
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Defines a structure to hold course info
struct Course {
	string courseID;
	string courseName;
	vector<string> prereqs;
};

// Defines a structure to hold a node to make a tree
struct Node {
	Course course;
	Node* left;
	Node* right;

	// Default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}
	// Initialize with a course
	Node(Course newCourse): Node() {
			course = newCourse;
		}
};

// Creates a binary search tree
class BinarySearchTree {
	private:
		Node* root;
		void addNode(Node* node, Course course);
		void inOrder(Node* node);
	public:
		void insert(Course course);
		void printCourseList();
		void printCourse();
};

// Inserts a course into the tree
void BinarySearchTree::insert(Course course) {
	if(root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}

// Adds a node to the tree
void BinarySearchTree::addNode(Node* node, Course course) {
	if(node->course.courseID.compare(course.courseID) > 0) {
		// Add node if current node's left node is null
		if(node->left == nullptr) {
			node->left = new Node(course);
		}
		// Recursively call addNode otherwise
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		// Add node if current node's right node is null
		if(node->right == nullptr) {
			node->right = new Node(course);
		}
		// Recursively call addNode otherwise
		else {
			this->addNode(node->right, course);
		}
	}
}

// Traverses the tree in alphanumeric order
void BinarySearchTree::inOrder(Node* node) {
	// If node exists
	if(node != nullptr) {
		// Call smaller node, then display info, then call bigger node
		inOrder(node->left);
		cout << node->course.courseID << ", " << node->course.courseName << endl;
		inOrder(node->right);
	}
}

// Prints an ordered list of the courses
void BinarySearchTree::printCourseList() {
	cout << "Here is the list of courses: " << endl;
	this-> inOrder(root);
}

// Prints a specific course
void BinarySearchTree::printCourse() {
	// Get user input
	string courseID;
	cout << "Please enter the course ID: ";
	cin >> courseID;

	// Create reference to root
	Node* current = root;

	// While reference exists
	while(current != nullptr) {
		// Check if current course matches, print course
		if(courseID.compare(current->course.courseID) == 0) {
			cout << current->course.courseID << ", " << current->course.courseName << endl;
			cout << "Prerequisites:";
			// Loop through prerequisites
			for(int i = 0; i < current->course.prereqs.size(); i++) {
				cout << " " << current->course.prereqs[i];
				if(i+1 != current->course.prereqs.size()) {
					cout << ",";
				}
			}
			cout << endl;
			return;
		}
		// else if bid doesn't match and should move to left
		else if(courseID.compare(current->course.courseID) < 0) {
			current = current->left;
		}
		// else if bid doesn't match and should move to right
		else {
			current = current->right;
		}
	}

	cout << "No course found." << endl;;
}

// Loads all courses
void loadCourses(BinarySearchTree* bst, bool* coursesLoaded) {
	// Open file
	ifstream inputFile("ProgramInput");
	// If file opens
	if(inputFile) {
		// Checks for end of row
		string currLine;
		while(getline(inputFile, currLine)) {
			Course newCourse;
			size_t pos = currLine.find(",");
			size_t prevPos = 0;
			int iterateTimes = count(currLine.begin(), currLine.end(), ',');
			// Loop through items in string
			for(int i = 0; i < iterateTimes + 1; i++) {
				switch(i) {
					case 0:
						newCourse.courseID = currLine.substr(prevPos, pos);
						break;
					case 1:
						newCourse.courseName = currLine.substr(prevPos, pos-7);
						break;
					default:
						newCourse.prereqs.push_back(currLine.substr(prevPos,pos-33));
						break;
				}
				// Erase comma and find next
				currLine.erase(pos, 1);
				prevPos = pos;
				pos = currLine.find(',');
				// If no more commas, get last piece of data
				if(pos == string::npos) {
					pos = currLine.size();
				}
			}
			// Add item to node and then tree
			bst->insert(newCourse);
		}
		*coursesLoaded = true;
		cout << "Courses loaded." << endl;
	}
	// If file doesn't open
	else {
		cout << "File not found." << endl;
	}

	// Closes file
	inputFile.close();
}

// Main function where loop is
int main() {

	// Define a binary search tree
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	bool* coursesLoaded = new bool(false);
	// Sets up user input loop
	cout << "Welcome to the course planner." << endl;
	int choice = 0;
	while (choice != 9) {
		// Prints menu
		cout << endl << "\t1. Load Data Structure." << endl <<
		"\t2. Print Course List." << endl <<
		"\t3. Print Course." << endl <<
		"\t9. Exit." << endl << endl;
		cout << "What would you like to do? ";
		cin >> choice;

		// Takes action based on user input
		switch(choice) {
			// Loads data structure
			case 1:
				if(!*coursesLoaded) {
					loadCourses(bst, coursesLoaded);
				}
				else {
					cout << "Courses already loaded." << endl;
				}
				break;
			// Prints course list
			case 2:
				if (!*coursesLoaded) {
					cout << "No courses loaded." << endl;
				}
				else {
					bst->printCourseList();
				}
				break;
			// Prints a course
			case 3:
				if(!*coursesLoaded) {
					cout << "No courses loaded." << endl;
				}
				else {
					bst->printCourse();
				}
				break;
			// Exits the program
			case 9:
				break;
			// Any other input
			default:
				cout << choice << " is not a valid option." << endl;
				break;
		}
	}

	// After loop ends
	cout << "Thank you for using the course planner!" << endl;
	return 0;
}
