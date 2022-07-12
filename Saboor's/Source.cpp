#include<iostream>
#include"q1.h"
using namespace std;

int main()
{
	user u1(1234, false), u2(3456, true);
	cout << u1 << u2 << u1 << endl;

	heap<int, user> h1,h2;
	h1.insert(2, u1);
	h1.insert(4, u2);
	h1.insert(6, u2);
	h1.insert(8, u1);
	h1.insert(1, u2);
	h1.insert(7, u1);
	u1 = h1.extractMax();
	u1 = h1.extractMax();
	u1 = h1.extractMax();
	u1 = h1.extractMax();
	u1 = h1.extractMax();
	u1 = h1.extractMax();
	h1.insert(2, u1);
	cout << u1;

	u1 = h1.findMax();
	cout << u1;
	u1 = h1.findMin();
	cout << u1;

	vector<heapItem<int, user>> arr;
	arr.resize(8);
	heapItem<int, user> temp;
	temp.key = 0; temp.value = u1;
	for (int i = 0; i < 6; i++)
	{
		arr[i] = temp;
		temp.key++;
	}

	h2.buildQueue(arr,6);
	
	system("pause");
	return 0;
}