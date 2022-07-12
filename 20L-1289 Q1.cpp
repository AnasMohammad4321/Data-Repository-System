/*
	Mohammad Anas
	20L-1289
	BDS-3A
	Assignment 4 (Mini project)
*/

#include <iostream>
#include <string>
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
	User(): id(-1), operation_type("") {}
	User(int i, string type) : id(i), operation_type(type) {}
	void Print() const {
		cout << "ID: " << id << endl;
		cout << "Operation: " << operation_type << endl;
		cout << endl;
	}
	friend ostream& operator<<(ostream& output, const User& U) {
		cout << "ID: " << U.id << endl;
		cout << "Operation: " << U.operation_type << endl;
		return output;
	}
};

// Class for each node.
template <class k, class v>
class User_Data {
public:
	k key;							//to maintain the priortity of the list.
	v information;					// object to store the user's information.

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
	void BuildQueue(vector<User_Data<k, v>>& arr, int n) {
		int initial = (n / 2) - 1;					//last non-leaf node's index.
		int i = initial;
		while (i >= 0) {									//call for every index till we each the root.
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
		assert(curr_size > 1);
		v max = queue[0].information;			//storing the max element's information.
		int lastIndex = curr_size - 1;			//checking the last index of the heap.
		queue[0] = queue[lastIndex];				//Replacing first element with the last element.
		curr_size--;							//size is decremented by 1.
		HeapDown(queue, curr_size, 0);			//Heap down to maintain the heap property.
		
		return max;
	}
	void printHeap() const {
		cout << "Array representation of Heap is:\n";
		int i = 0;
		while(i < curr_size) {
			cout << "Index: " << i << endl;
			cout << "Key: " << queue[i].key << endl;
			cout << queue[i].information << endl;
			i++;
		}
		cout << endl;
		
	}
	v FindMax() const {
		assert(curr_size > 0);
		return queue[0].information;
	}
	v FindMin() const { 
		assert(curr_size > 0);
		return minimum.information; 
	}
	bool IsEmpty() const { return curr_size == 0; }	//Empty if the size is zero.
	int get_size() const { return curr_size; }		//Returns the current number of users.
};

int main() {

	User user1(90, "Read");
	User user2(91, "Write");
	User user3(92, "Read");
	User user4(93, "Write");
	User user5(94, "Read");

	Waiting_List<int, User> List1;
	List1.Insert(1, user1);
	List1.Insert(2, user2);
	List1.Insert(3, user3);
	List1.Insert(4, user4);
	List1.Insert(5, user5);

	List1.printHeap();


	User u1;

	cout << "Maximum element." << endl;
	u1 = List1.ExtractMax();
	cout << u1 << endl;

	cout << "Minimum element." << endl;
	u1 = List1.FindMin();
	cout << u1 << endl;

	List1.printHeap();
	cout << "Size of the list: ";
	cout << List1.get_size() << endl;

	Waiting_List<int, User> List2;
	vector<User_Data<int, User>> arr;
	arr.resize(5);
	User_Data<int, User> temp;
	temp.key = 0;
	temp.information = u1;
	for (int i = 0; i < 5; i++)
	{
		arr[i] = temp;
		temp.key++;
	}
	List2.BuildQueue(arr, 5);

	system("pause");
	return 0;
}