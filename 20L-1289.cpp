/*
	Mohammad Anas
	20L-1289
	BDS-3A
	Assignment 4 (Mini project)
*/

#include <iostream>
#include <list>
#include <Windows.h>
#include <string>
#include <fstream>
#include <assert.h>
#include <vector>
using namespace std;

// Question 1
// Priority Queue Implemented as Max heaps using STL vectors.


//User information class
class User {
	int id;								// Id to the corresponding user.
	string operation_type;				//Read or write.	
public:
	template<class k, class v>
	friend class User_data;
	template<class k, class v>
	friend class Waiting_List;
	template<class k, class v>
	friend class HashMap;
	User(): id(-1), operation_type("") {}
	User(int i, string type) : id(i), operation_type(type) {}
	~User() {}
	void Print() const {
		cout << "ID: " << id << endl;
		cout << "Operation: " << operation_type << endl;
		cout << endl;
	}
	friend ostream& operator<<(ostream& output, const User& U) {
		cout << "User " << U.id << ", " << U.operation_type;
		return output;
	}
	string get_type()const { return operation_type; }
};

// Class for each node.
template <class k, class v>
class User_Data {
public:
	k key;							//to maintain the priortity of the list.
	v information;					// object to store the user's information.
	~User_Data() {}
	template <class k, class v>
	friend class Waiting_List;
};

//Priority Queue implemented as Max heap using STL vectors as a waiting list for the user.
template <class k, class v>
class Waiting_List { 
	vector<User_Data<k, v>> queue;				//to store the user data.
	User_Data<k, v> minimum;					//to store the minimum element of the heap.
	int curr_size;								//to store the current number of elements in the vector.

	//helper function to BuildQueue().
	void HeapDown(vector<User_Data<k, v>>& array, int n, int index) {
		int maximum = index;					// initially the maximum is in the index variable.
		int leftChild = (index * 2) + 1;		// leftChild position.
		int rightChild = (index * 2) + 2;		// rightChild position.

		if (leftChild < n && array[leftChild].key > array[maximum].key)
			maximum = leftChild;				// If left child is greater than root.

		if (rightChild < n && array[rightChild].key > array[maximum].key)
			maximum = rightChild;				// If right child is greater than maximum so far.

		if (maximum != index){					// If maximum is not root.
			swap(array[index], array[maximum]);	//swap if not equal.
			HeapDown(array, n, maximum);		//continue heap down.
		}
	}
	void HeapUp(int root, int index){
		int parent = (index - 1) / 2;					//index for the parent node.
		if (index > root) {								//node has not reached the root yet.
			if (queue[index].key > queue[parent].key) {
				swap(queue[index], queue[parent]);
				HeapUp(root, parent);
			}
		}
	}
public:
	Waiting_List() { queue.resize(10); curr_size = 0; }		//vector of size 10 and all elements 0.
	Waiting_List(int n) {
		assert(n > 1);
		queue.resize(n);									//vector of size n and all elements 0.
		curr_size = 0;
	}
	~Waiting_List() { queue.clear(); }
	void BuildQueue(vector<User_Data<k, v>>& arr, int n) {
		int i = (n / 2) - 1;					//last non-leaf node's index.
		while (i >= 0) {							//call for every index till we each the root.
			HeapDown(arr, n,  i);
			i--;
		}
	}
	void Insert(k const key, v const user) {
		if (curr_size == 0) {
			queue[0].information = user;		//storing the object into the vector.
			queue[0].key = key;
			minimum.information = user;
			minimum.key = key;
			curr_size++;						//incrementing the size.
			return;
		}

		if (curr_size == queue.capacity())
			queue.resize(curr_size * 2);

		if (curr_size < queue.capacity()) {		//insert if the queue is not full.
			queue[curr_size].key = key;			//storing the object into the vector.
			queue[curr_size].information = user;
			curr_size++;						//incrementing the size.
			if (key < minimum.key) {			//checking for the minimum key.
				minimum.information = user;
				minimum.key = key;
			}
			HeapUp(0, curr_size - 1);			//Heap up to maintain the heap property.
		}
	}
	v ExtractMax() {
		assert(curr_size > 0);
		v max = queue[0].information;			//storing the max element's information.
		int lastIndex = curr_size - 1;			//checking the last index of the heap.
		queue[0] = queue[lastIndex];			//Replacing first element with the last element.
		curr_size--;							//size is decremented by 1.
		HeapDown(queue, curr_size, 0);			//Heap down to maintain the heap property.
		
		return max;
	}
	void printHeap() {
		int i;
		if (get_size() == 0) {
			cout << "none." << endl;
			cout << "Next none." << endl;
			cout << "Waiting none" << endl;
		}
		else{
			for (i = 0; i < curr_size; i++) {
				if (queue[i].information.operation_type != "Write") {
					cout << queue[i].information << endl;
				}
			}
				
				
			cout << "Next " << queue[i].information << endl;
			cout << "Waiting " << get_size() - i << endl;
		}

	}
	User_Data<k, v> FindMax() const {
		assert(curr_size > 0);
		return queue[0];
	}
	User_Data<k, v> FindMin() const {
		assert(curr_size > 0);
		return minimum; 
	}
	bool IsEmpty() const {
		if (!this) return true;						//Returns true if nullptr
		return curr_size == 0;						//Empty if the size is zero.
	}	
	int get_size() const { return curr_size; }		//Returns the current number of users.
};


//Node for each file
template <class k, class v>
class HashItem {
	//Data members
	int file_Id;									//file is stored in every node.
	Waiting_List<k, v>* waiting_line;				//queue for the waiting users.
	list<v> current_users;							//users which are curretnly accessing the file.
public:
	//Constructors
	HashItem(): file_Id(0), waiting_line(nullptr) {}
	HashItem(int id): file_Id(id), waiting_line(nullptr) {}
	~HashItem() {
		current_users.clear();
		waiting_line == nullptr;
	}
	template <class k, class v>
	friend class HashMap;
};

template <class k, class v>
class HashMap {
	vector <list<HashItem<k, v>>> table;
	int currElements;	

	void Read_data(HashMap<k, v>& Table, string path) {
		int fileID;						//variable to store fileID.
		int userID;						//variable to store userID.
		int priority;					//variable to store priority.
		char Access;					//variable to store Access type.
		char comma;						//variable for commas in the file.

		ifstream MyReadFile(path);		//Opening the file at the given path.
		if (!MyReadFile.is_open()) {
			cout << "File not opened" << endl;
			return;
		}
		string curr;
		int count = 0;					//row count
		while (getline(MyReadFile, curr)) count++;
		MyReadFile.close();				//Closing the file.
		MyReadFile.open(path);		//Opening the file at the given path. MyReadFile(path);		//Opening the file at the given path.
		while (count > 0) {				//Get input till the end of file.
			MyReadFile >> fileID >> comma >> userID >> comma >> priority >> comma >> Access;
			Table.insert(fileID);		//Insert file into the hashtable.
			if (Access == 'R')
				Table.RequestFileAccess(User(userID, "Read"), fileID, priority);
			else if (Access == 'W')		//Request File access for the given users.
				Table.RequestFileAccess(User(userID, "Write"), fileID, priority);
			count--;
		}
	}
public:
	HashMap() { table.resize(101); currElements = 0; }		//Default Constructur
	HashMap(int const capacity) {							//overloaded constructor
		assert(capacity > 1);
		table.resize(capacity);								//vector of size capacity and all elements 0.
		currElements = 0;
	}
	~HashMap() {
		typename list<HashItem<k, v>>::iterator it;
		for (int i = 0; i < table.capacity(); i++) {
			for (it = table[i].begin(); it != table[i].end(); it++)
				delete it->waiting_line;
			table[i].clear();
		}
	}
	//Insert function for insertion in the hash table.
	void insert(int const file_Id) {
		assert(file_Id >= 5000 && file_Id <= 99999);
		int index = file_Id % table.capacity();				//Hash Function.
		typename list<HashItem<k, v>>::iterator it;			//To iterate through the list.
		for (it = table[index].begin(); it != table[index].end(); it++)
			if (it->file_Id == file_Id) return; 			//To check if the file already exists in the hashtable. 
		HashItem<k, v> insertObj;							//creating hasitem object to be inserted.
		insertObj.file_Id = file_Id;
		insertObj.waiting_line = nullptr;
		table[index].push_back(insertObj);					//Inserting the object.
		currElements++;										//Incrementing the curr size of the hashtable.
	}
	//Delete file function from the hashtable.
	void delete_file(int file_Id) {
		assert(file_Id >= 5000 && file_Id <= 99999);
		int index = file_Id % table.capacity();				//Hash Function.
		typename list<HashItem<k, v>>::iterator it;			//To iterate through the list.
		for (it = table[index].begin(); it != table[index].end(); it++)
			if (it->file_Id == file_Id) {					//To check if the file already exists in the hashtable.
				if (it->current_users.empty() && it->waiting_line == nullptr) {
					table[index].erase(it);
					cout << "file deleted" << endl;
					return;
				}

			}			 			
				
	}
	void RequestFileAccess(v const user, k const file_Id, int const priority) {
		// -69 for high priority
		// -96 for low priority
		if (priority == -69 || priority == -96) goto start;
		assert(priority >= 0 && priority <= 100);
		start:
		assert(file_Id >= 5000 && file_Id <= 99999);
		assert(user.id >= 0 && user.id <= 10000);
		int index = file_Id % table.capacity();									//Finding the index of the file.
		bool flag = false;														//To check if the access is granted or not.
		typename list<HashItem<k, v>>::iterator it;								//To iterate through the list.
		for (it = table[index].begin(); it != table[index].end(); it++)
			if (it->file_Id == file_Id) {										//File found with give file id.
				if (it->current_users.empty() && it->waiting_line == nullptr) {
					it->current_users.push_back(user);
					flag = true;
					break;
				}
				else if (priority == -69) {										//For high priority.
					if (it->waiting_line == nullptr)
						(it)->waiting_line = new Waiting_List<int, v>();
					User_Data<k, v> obj = it->waiting_line->FindMax();			//Finding the max the of the waiting list.
					int ke = obj.key + 1;										//Incrementing the key to put the user at the top of the waiting list.
					it->waiting_line->Insert(ke, user);							//Inserting the user into the waiting list.
					flag = true;
					break;
				}
				else if (priority == -96) {										//For low priority.
					if (it->waiting_line == nullptr)
						(it)->waiting_line = new Waiting_List<int, v>();
					User_Data<k, v> obj = it->waiting_line->FindMin();			//Finding the min the of the waiting list.
					int ke = obj.key - 1;										//decrementing the key to put the user at the end of the waiting list.
					it->waiting_line->Insert(ke, user);							//Inserting the user into the waiting list.
					flag = true;
					break;
				}
				else {															//For Normal Request.
					if (it->waiting_line == nullptr)
						(it)->waiting_line = new Waiting_List<int, v>();
					it->waiting_line->Insert(priority, user);
					flag = true;
					break;
				}
				
			}
		if (flag) {
			cout << "File ID: " << file_Id << endl;
			cout << "Access granted to " << user << endl;
		}
		else
			cout << "File not found." << endl;
	}
	void ReleaseFile(v user, k file_Id) {
		assert(file_Id >= 5000 && file_Id <= 99999);
		assert(user.id >= 0 && user.id <= 10000);
		int index = file_Id % table.capacity();											//To check if the file is released or not.
		typename list<HashItem<k, v>>::iterator it;										//To iterate through the list.
		for (it = table[index].begin(); it != table[index].end(); it++) {
			if (it->file_Id == file_Id) {												//File found with give file id.
				if (!it->current_users.empty() && it->waiting_line->IsEmpty()) {		//Check 1: current user list is not empty and there is no one waiting.
					typename list<v>::iterator i;
					for (i = it->current_users.begin(); i != it->current_users.end(); i++)
						if (i->id == user.id) {
							it->current_users.erase(i);
							break;
						}
				}
				else if (!it->current_users.empty() && !it->waiting_line->IsEmpty()) {	//Check 2: current user list is not empty and there are users waiting.
					typename list<v>::iterator i;
					for (i = it->current_users.begin(); i != it->current_users.end(); i++)
						if (i->id == user.id) {
							it->current_users.erase(i);
							break;
						}
					v curr = it->waiting_line->ExtractMax();
					it->current_users.push_back(curr);

					while (curr.get_type() != "Write" && !it->waiting_line->IsEmpty()) {
						v curr = it->waiting_line->ExtractMax();
						it->current_users.push_back(curr);
					}
				}
				else if (it->current_users.empty() && !it->waiting_line->IsEmpty()) {	//Check 3: current user list is empty and there are users waiting.
					v curr = it->waiting_line->ExtractMax();
					it->current_users.push_back(curr);

					while (curr.get_type() != "Write" && !it->waiting_line->IsEmpty()) {
						v curr = it->waiting_line->ExtractMax();
						it->current_users.push_back(curr);
					}
				}
			}
		}
	}
	void Print() {
		cout << "////////////FILE ARCHIVE\\\\\\\\\\\\\\\\\\\\\\\\" << endl << endl;
		typename list<HashItem<k, v>>::iterator it;
		for (int i = 0; i < table.size(); i++) {
			if (!table[i].empty()) {																	//List is not empty
				for (it = table[i].begin(); it != table[i].end(); it++) {
					cout << "H" << i + 1 << "-> File " << it->file_Id << ": \n\tAccess granted to ";
					if (it->current_users.empty())
						cout << "None." << endl;
					else {
						typename list<User>::iterator it2;
						for (it2 = it->current_users.begin(); it2 != it->current_users.end(); it2++)
							cout << *(it2) << endl;
						cout << "\tNext user: ";
						if (it->waiting_line->IsEmpty())
							cout << "None. \n\tWaiting: None." << endl;
						else {
							User_Data<k, v> curr_user = it->waiting_line->FindMax();
							v temp_user = curr_user.information;
							cout << temp_user << endl;
							cout << "\tWaiting: " << it->waiting_line->get_size() << endl;
						}
					}

				}

			}
		}
	}
	void Read_data(string path) { Read_data(*this, path); }		//Wrapper function to Read_data function.
};

void menu() {
	HashMap<int, User> table1;
	int choice = 0;
	int fileID;						//variable to store fileID.
	int userID;						//variable to store userID.
	int priority;					//variable to store priority.
	char Access;					//variable to store Access type.
	start:
	cout << "\t\t^^^^^^^^^^ MAXIMUS DATA HOUSE ^^^^^^^^^^" << endl;
	cout << "\t\t\t->Enter 1 to print the Archive. " << endl;
	cout << "\t\t\t->Enter 2 to load data. " << endl;
	cout << "\t\t\t->Enter 3 to insert file. " << endl;
	cout << "\t\t\t->Enter 4 to delete file. " << endl;
	cout << "\t\t\t->Enter 5 to request access. " << endl;
	cout << "\t\t\t->Enter 6 to release file. " << endl;
	cout << "\t\t\t->Enter 7 to exit the system. " << endl;
	cin >> choice;
	switch (choice) {
	case 1:
		table1.Print();
		break;
	case 2:
		table1.Read_data("file_record.txt");
		break;
	case 3:
		cout << "Enter file id to insert. ";
		cin >> fileID;
		table1.insert(fileID);
		break;
	case 4:
		cout << "Enter file id to delete. ";
		cin >> fileID;
		table1.delete_file(fileID);
		break;
	case 5:
		cout << "Enter file id. ";
		cin >> fileID;
		cout << "Enter user id. ";
		cin >> userID;
		cout << "Enter priority. (-69 for high and -96 for low) ";
		cin >> priority;
		cout << "Enter type of operation. (r for read and w for write) ";
		cin >> Access;
		if (Access == 'r')
			table1.RequestFileAccess(User(userID, "Read"), fileID, priority);
		else if (Access == 'w')
			table1.RequestFileAccess(User(userID, "Write"), fileID, priority);
		break;
	case 6:
		cout << "Enter file id. ";
		cin >> fileID;
		cout << "Enter user id. ";
		cin >> userID;
		cout << "Enter type of operation. (r for read and w for write) ";
		cin >> Access;
		if (Access == 'r')
			table1.ReleaseFile(User(userID, "Read"), fileID);
		else if (Access == 'w')
			table1.ReleaseFile(User(userID, "Write"), fileID);

	case 7:
		return;
	default:
		cout << "Please choose a valid option." << endl;
		system("cls");
		goto start;
	}
	//Sleep(20);
	//system("cls");
	goto start;
}

int main() {

	menu();
	//User user1(90, "Read");
	//User user2(91, "Write");
	//User user3(92, "Read");
	//User user4(93, "Write");
	//User user5(94, "Read");

	//Waiting_List<int, User> List1;
	//List1.Insert(1, user1);
	//List1.Insert(2, user2);
	//List1.Insert(3, user3);
	//List1.Insert(4, user4);
	//List1.Insert(5, user5);

	////cout << "MAXIMU"

	/////List1.printHeap();

	//User u1;

	////cout << "Maximum element." << endl;
	//u1 = List1.ExtractMax();
	////cout << u1 << endl;

	////cout << "Minimum element." << endl;
	////u1 = List1.FindMin();
	////cout << u1 << endl;

	////List1.printHeap();
	////cout << "Size of the list: ";
	////cout << List1.get_size() << endl;
	////List1.printHeap();
	//Waiting_List<int, User> List2;
	//vector<User_Data<int, User>> arr;
	//arr.resize(5);
	//User_Data<int, User> temp;
	//temp.key = 0;
	//temp.information = u1;
	//for (int i = 0; i < 5; i++)
	//{
	//	arr[i] = temp;
	//	temp.key++;
	//}
	//List2.BuildQueue(arr, 5);

	//list<User> l1;
	//l1.push_back(user1);
	//l1.push_back(user2);
	//l1.push_back(user3);

	////showTheContent(l1);

	//HashMap<int, User> table1;
	//table1.insert(3);
	////table1.delete_file(3);
	////table1.Print();
	//table1.RequestFileAccess(User(90, "Read"), 3, 6);
	//table1.RequestFileAccess(User(91, "Read"), 3, 5);
	//table1.RequestFileAccess(User(92, "Read"), 3, 4);
	//table1.RequestFileAccess(User(93, "Read"), 3, 3);
	//table1.RequestFileAccess(User(94, "Write"), 3, 2);
	//table1.ReleaseFile(User(90, "Read"), 3);
	//
	////table1.ReleaseFile(User(93, "Read"), 3);

	////cout << endl << endl;
	//table1.Print();
	////system("pause");
	////table1.Read_data("file_record.txt");

	system("pause");
	return 0;
}