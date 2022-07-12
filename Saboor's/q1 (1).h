#include<iostream>
#include<assert.h>
#include<vector>
using namespace std;

// definition of user class
class user
{
	int id;
	bool operation; // false is read, true is write

	// default constructor for use in heap
	user() :id(0), operation(false) {}

public:
	// constructor of class
	user(int temp_id, bool temp_op) :id(temp_id), operation(temp_op) {}
	friend ostream& operator<<(ostream& out, user& temp)
	{
		if (temp.operation == false)
			out << "User ID : " << temp.id << ", wants to read from this file" << endl;
		else
			out << "User ID : " << temp.id << ", wants to write on this file" << endl;
		return out;
	}
	
	// friend classes
	template<class k, class v>
	friend class heapItem;
	template<class k, class v>
	friend class heap;
};

// definition of heap class
template<class k, class v>
class heapItem
{
	// public for making an array in main
public:
	k key;
	v value;

	template<class k, class v>
	friend class heap;
};

template<class k, class v>
class heap
{
	vector<heapItem<k, v>> arr;
	int capacity;
	int totalitems;
	heapItem<k,v> minItem; // variable to maintain minimum priority user

public:
	heap() :capacity(1), totalitems(0)
	{
		this->arr.resize(1); // resizing vector to minmum
	}
	~heap()
	{
		arr.clear(); // clearig vector
		this->totalitems = this->capacity = 0;
	}
	bool isEmpty()
	{
		return(this->totalitems == 0); // returns true if no items are present
	}
	int heapsize()
	{
		return this->totalitems;
	}

	void buildQueue(vector<heapItem<k, v>>& temp_arr, int size)
	{
		for (int i = (size / 2) - 1; i >= 0; i--)
			reheapdown(temp_arr, i, size);
	}

	v findMin()
	{
		assert(this->totalitems > 0);
		return this->minItem.value;
	}
	v findMax()
	{
		assert(this->totalitems > 0);
		return this->arr[0].value;
	}

	void insert(k const key, v const value)
	{
		if (this->totalitems == this->capacity) // code to grow heap if capacity reached
		{
			this->capacity *= 2;
			this->arr.resize(this->capacity); // resizing array to accomodate
		}

		// validating and updating minimum variable
		if (this->totalitems == 0)
		{
			this->minItem.key = key;
			this->minItem.value = value;
		}
		else
		{
			if (key < this->minItem.key)
			{
				this->minItem.key = key;
				this->minItem.value = value;
			}
		}

		// adding new user to last of array
		this->arr[this->totalitems].key = key;
		this->arr[this->totalitems++].value = value;
		reheapup(this->arr,0,this->totalitems - 1); // reheaping upwards from inserted item
	}
	v extractMax()
	{
		assert(this->totalitems > 0);
		swap(arr[0], arr[--this->totalitems]);
		reheapdown(this->arr, 0,this->totalitems-1);
		// code to shrink array
		if (this->capacity / 2 > this->totalitems)
		{
			this->arr.resize(this->capacity / 2);
			this->capacity /= 2;
		}
		return arr[totalitems].value;
	}

	void reheapup(vector<heapItem<k,v>> &data,int root, int last)
	{
		int parent;
		if (last > root) { // tree is not empty
			parent = (last - 1) / 2;
			if (data[parent].key < data[last].key) {
				swap(data[parent], data[last]);
				reheapup(data,root, parent);
			}
		}
	}
	void reheapdown(vector<heapItem<k,v>> &data,int cnode, int last) {
		int maxChild, rightChild, leftChild;
		leftChild = 2 * cnode + 1;
		rightChild = 2 * cnode + 2;
		if (leftChild <= last) { // left child is part of the heap
			if (leftChild == last) // only one child
				maxChild = leftChild;
			else {
				if (data[leftChild].key <= data[rightChild].key)
					maxChild = rightChild;
				else
					maxChild = leftChild;
			}
			if (data[cnode].key < data[maxChild].key) {
				swap(data[cnode], data[maxChild]);
				reheapdown(data,maxChild, last);
			}
		}
	}

};