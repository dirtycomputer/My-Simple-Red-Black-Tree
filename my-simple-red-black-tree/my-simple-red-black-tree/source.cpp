#include "my_rbtree.h"

#include <iostream>

using namespace std;

RBtree<int> rbtree;

int main(int argc, char* argv[])
{
	
	/*for (int i = 0; i < 1000000; ++i)
	{
		rbtree.insert(i);
	}

	cout << rbtree.debug_get_black_depth() << endl; // 1e5: 17, 1e6: 20, 1e7: 24
	*/
	/*
	auto iter = rbtree.insert(1000000);
	cout << *iter << endl;
	cout << iter->value << endl; // ok


	auto result = rbtree.find(1);
	cout << result->value << endl;// ok

	auto result2 = rbtree.find(-1);
	cout << (int)(void*)(result2.pointer) << endl; // ok
*/
	
	/*for (int i = 0; i < 10; ++i)
	{
		rbtree.insert(i);
		rbtree.debug_run();
	}*/

	// cout << rbtree.size() << endl;
	

	/*
	for (int i = 0; i < 9; ++i)
	{
		
		cout << *temp << endl;
	}
	*/
	//RBtree<int>::Iterator iter = rbtree.find(4);
	//RBtree<int>::Iterator temp = ++iter;
	// cout << *temp << endl;

	/*for (auto iter = rbtree.find(0);iter.pointer != nullptr; iter++)
	{
		cout << *iter << endl;
	}*/
	
	rbtree.insert(2);
	rbtree.insert(4);
	rbtree.insert(5);
	rbtree.insert(9);
	rbtree.insert(12);
	rbtree.insert(8);
	rbtree.insert(3);
	rbtree.insert(1);
	rbtree.insert(10);
	cout << rbtree.size() << endl;
	rbtree.debug_run();
	rbtree.remove(4);
	cout << rbtree.size() << endl;
	rbtree.debug_run();

	/*for (int i = 0; i < 10000; ++i)
	{
		rbtree.insert(i);
	}
	//rbtree.debug_run();
	cout << rbtree.debug_get_depth() << endl;
	cout << rbtree.debug_get_black_depth() << endl;
	for (int i = 5000; i < 10000; ++i)
	{
		rbtree.remove(i);
	}
	cout << rbtree.debug_get_black_depth() << endl;
	cout << rbtree.debug_get_depth() << endl;
	for (int i = 5000; i < 10000; ++i)
	{
		rbtree.insert(i);
	}
	cout << rbtree.debug_get_black_depth() << endl;
	cout << rbtree.debug_get_depth() << endl;*/
	/*
	rbtree.debug_run();
	for (int i = 10; i < 20; ++i)
	{
		rbtree.remove1(i);
		rbtree.debug_run();
 	}
	cout << rbtree.size() << endl;
	cout << rbtree.debug_get_black_depth() << endl;
	cout << rbtree.debug_get_depth() << endl;*/

	/*

	cout << rbtree.size() << endl;

	for (RBtree<int>::Iterator iter = rbtree.begin(); iter != rbtree.end(); ++iter)
	{
		//cout << *iter << endl;
	}

	cout << rbtree.has(5) << endl;
	*/


//	for (int i = 0; i < 5; ++i)
//	{
//		rbtree.insert(i);
//	}
//	//rbtree.remove(7);
//
//	FILE* newfile;
//
//	newfile = fopen("out.dot", "w");
//	rbtree.debug_dot(newfile);
//
//	fclose(newfile);
//
	int temp3333;
	cin >> temp3333;
	
	return 0;
}
