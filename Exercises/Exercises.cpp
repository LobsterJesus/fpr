#include "stdafx.h"
#include <string>
#include <iostream>
#include <functional>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
vector<T> map(function<T(T)> func, vector<T> list)
{
	vector<T> newList(list.size());

	for (auto i = 0; i < list.size(); i++) 
	{
		newList[i] = func(list[i]);
	}

	return newList;
}

template <typename T>
T fold(function<T(T, T)> func, T initValue, vector<T> list)
{
	T result = initValue;

	for (auto i = 0; i < list.size(); i++)
	{
		result = func(result, list[i]);
	}

	return result;
}

template <typename T>
vector<T> scan(function<T(T, T)> func, T initValue, vector<T> list)
{
	vector<T> newList(list.size());
	T result = initValue;

	for (auto i = 0; i < list.size(); i++)
	{
		result = func(result, list[i]);
		newList[i] = result;
	}

	return newList;
}

template <typename T>
vector<T> divideAndConquer(
	function<bool(vector<T>)> trivial,
	function<vector<T>(vector<T>)> solve,
	function<vector<vector<int>>(vector<T>)> divide,
	function<vector<T>(vector<vector<int>>)> combine,
	vector<T> problemSet)
{
	if (trivial(problemSet)) 
	{
		return solve(problemSet);
	}
	else 
	{
		vector<vector<int>> parts = divide(problemSet);

		for (auto i = 0; i < parts.size(); i++)
		{
			parts[i] = divideAndConquer(trivial, solve, divide, combine, parts[i]);
		}

		return combine(parts);
	}	
}



// Support functions (writing to console)
template <typename T>
void print(string title, std::vector<T> list) 
{
	cout << title << endl << "\t";
	for (auto iter = list.begin(); iter != list.end(); iter++) 
	{
		if (iter != list.begin()) cout << ", ";
		cout << *iter;
	}
	cout << endl;
}

template <typename T>
void print(string title, T value)
{
	cout << title << endl << "\t" << value << endl;
}

/*
* Exercise 1: Pure Polymorphic Map
*/
void exercise_1() 
{
	cout << "1) Pure Polymorphic Map" << endl;

	function<int(int)> increment = [](int i) { return i + 1; };
	function<string(string)> addDot = [](string s) { return s + '.'; };
	function<float(float)> half = [](float f) { return f / 2; };

	print(
		"a) Example 1 (increase integer)",
		map(increment, { 1, 2, 3, 4 })
	);

	print(
		"b) Example 2 (add '.' to string)",
		map(addDot, { "H", "a", "l", "l", "o" })
	);

	print(
		"c) Example 3 (half floating point number)",
		map(half, { 1.0f, 2.0f, 3.0f, 4.0f })
	);
}

/*
* Exercise 2: Pure Polymorphic Fold
*/
void exercise_2_and_3()
{
	cout << endl << "2) Pure Polymorphic Fold" << endl;

	function<int(int, int)> sum = [](int a, int b) { return a + b; };
	function<string(string, string)> concat = [](string a, string b) { return a + b; };
	function<float(float, float)> divideNumbers = [](float a, float b) { return a / b; };

	print(
		"a) Example 1 (integer sum; 1 + 2 + 3 + 4)",
		fold(sum, 0, { 1, 2, 3, 4 })
	);

	print(
		"b) Example 2 (concat strings)",
		fold(concat, (string)"", { "H", "a", "l", "l", "o" })
	);

	print(
		"c) Example 3 (divide; 100 / 5 / 2)",
		fold(divideNumbers, 100.0f, { 5.0f, 2.0f })
	);

	/*
	* Exercise 3: Pure Polymorphic Scan (using the same functions as in 2)
	*/
	cout << endl << "3) Pure Polymorphic Scan" << endl;

	print(
		"a) Example 1 (integer sum for [1, 2, 3, 4])",
		scan(sum, 0, { 1, 2, 3, 4 })
	);

	print(
		"b) Example 2 (concating strings ['H', 'a', 'l', 'l', 'o'])",
		scan(concat, (string)"", { "H", "a", "l", "l", "o" })
	);

	print(
		"c) Example 3 (divide; [100, 5, 2]",
		scan(divideNumbers, 100.0f, { 5.0f, 2.0f })
	);
}

/*
* Exercise 4: Higher-Order Divide and Conquer
*/
vector<int> quickSort(vector<int> list)
{
	function<bool(vector<int>)> trivial = [](vector<int> l) { return l.size() <= 1; };
	function<vector<int>(vector<int>)> solution = [](vector<int> l) { return l; };
	function<vector<vector<int>>(vector<int>)> divide = [](vector<int> l)
	{
		int pivotI = (int)(l.size() / 2.0f);
		int pivot = l[pivotI];

		vector<int> part1 = vector<int>();
		vector<int> part2 = vector<int>();

		for (auto i = 0; i < l.size(); i++)
		{
			if (i == pivotI) continue;

			if (l[i] <= pivot)
			{
				part1.push_back(l[i]);
			}
			else {
				part2.push_back(l[i]);
			}
		}

		vector<vector<int>> vv = { part1,{ pivot }, part2 };

		return vv;
	};
	function<vector<int>(vector<vector<int>>)> combine = [](vector<vector<int>> parts)
	{
		vector<int> combined;

		for (auto i = 0; i < parts.size(); i++)
		{
			for (auto j = 0; j < parts[i].size(); j++)
			{
				combined.push_back(parts[i][j]);
			}
		}

		return combined;
	};

	return divideAndConquer(trivial, solution, divide, combine, list);
}

bool hasElement(int value, vector<int> list)
{
	// Prepare list for binary search (sort)
	vector<int> sortedList = quickSort(list);
	bool hasValue = false;

	function<bool(vector<int>)> trivial = [](vector<int> l) { return l.size() <= 1; };
	function<vector<int>(vector<int>)> solution = [value, &hasValue](vector<int> l) {
		if (l[0] == value) hasValue = true;
		return l; 
	};
	function<vector<vector<int>>(vector<int>)> divide = [value, &hasValue](vector<int> l)
	{
		int pivotI = (int)(l.size() / 2.0f);
		int pivot = l[pivotI];

		if (value == pivot)
		{
			hasValue = true;
			return vector<vector<int>>();
		}
		else if (value <= pivot)
		{
			return vector<vector<int>> { vector<int>(l.begin(), l.begin() + pivotI) };
		}
		else
		{
			return vector<vector<int>> { vector<int>(l.begin() + pivotI + 1, l.end()) };
		}
	};
	function<vector<int>(vector<vector<int>>)> combine = [](vector<vector<int>> parts)
	{
		vector<int> combined;

		for (auto i = 0; i < parts.size(); i++)
		{
			for (auto j = 0; j < parts[i].size(); j++)
			{
				combined.push_back(parts[i][j]);
			}
		}

		return combined;
	};

	divideAndConquer(trivial, solution, divide, combine, sortedList);

	return hasValue;
}

void exercise_4()
{
	vector<int> list = { 5, 9, 21, 2, 0, -6, -5, 5, 3, 13, 9, 5 };
	vector<int> results = quickSort(list);
	
	cout << endl << "4) Higher-Order Divide and Conquer" << endl;

	print(
		"a) Example 1 (quicksort on [5, 9, 21, 2, 0, -6, -5, 5, 3, 13, 9, 5])",
		results
	);

	print(
		"b) Example 2 (binary search for value '9')",
		hasElement(9, list) ? "contains value? yes" : "contains value? no"
	);

}

int main()
{
	exercise_1();
	exercise_2_and_3();
	exercise_4();

    return 0;
}

