// proj2VS.cpp : Defines the entry point for the console application.
// File:   proj2VS.cpp
// Author : Peter Day
// pday2@uwyo.edu
// Created on April 18, 2018, 6:55 PM
// Written on Win10, Visual Studio 15.6
// Microsoft Visual C++ 2017

#include "stdafx.h"
#include <iostream>
#include <ctime> //for random number generating for test array
#include <vector> //for collecting collisions
#include <string> //for hashing strings
using std::cout;
using namespace std;

int * fillArray(int *theArray, int size);
int * quickSort(int *theArray, int size);
bool isZero(int *theArray, int size);
int binSearch(int *theArray, int low, int high, int value);
int * merge(int *Array, int low, int middle, int high);
int * mergeSort(int *theArray, int low, int high);
int* hashFn(const int *theArray, int size, int hashSize);
void print(const int *theArray, int size);
void testSort(int *theArray, int size);
void testSearch(int *theArray, int size);
void testForCollisions(int *theArray, int *theTable, int size);
int bloomHash(string text, int tableSize);
int * bloomFilter(string *array, int size, int *hashTable, int hashTableSize);
void testBloomFilter(int *hashTable, int tableSize);

int main()
{
	int myArray[47] = { 0 };
	int size = 47;
	fillArray(myArray, size);
	

	//Print original Array
	//print(myArray, size);
	cout << endl << endl;

	quickSort(myArray, size);
	cout << "--- QUICK SORT RESULTS ---" << endl;
	testSort(myArray, size);
	
	cout << "--- BINARY SEARCH RESULTS ---" << endl;
	testSearch(myArray, size);
	
	//Print sorted array
	//cout << "---<<<### SORTED ###>>>---" << endl;
	//print(myArray, size);

	// --- MERGE SORT ----
	int arrayTwo[17] = { 0 };
	int size2 = 17;
	fillArray(arrayTwo, size2);
	mergeSort(arrayTwo, 0, size2);
	cout << "--- MERGE SORT RESULTS ---" << endl;
	//print(arrayTwo, size2);
	testSort(arrayTwo, size2);

	int *hashTable[401] = {};
	int hashTableSize = 401;
	*hashTable = hashFn(myArray, size, hashTableSize);

	//Print Hashed array
	cout << " --- HASHED ARRAY --- " << endl;
	//print(hashTable, size);
	testForCollisions(myArray, *hashTable, size);
	cout << endl;

	//BLOOM FILTER STUFF
	int bloomHashTable[401];
	int bHTSize = 401;
	int textArraySize = 7;
	string someText[7] = { "cat", "swift fox", "horse", "Bison", "antelope",
	"Giant Squid", "one eyed one horned flying purple people eater"};
	int* bloomTable = bloomFilter(someText, textArraySize, bloomHashTable, bHTSize);
	cout << " --- BLOOM FILTER TEST ----" << endl;
	testBloomFilter(bloomTable, hashTableSize);

	return 0;
}
// --- END MAIN

// --- FILL ARRAY ---
int * fillArray(int *theArray, int size)
{
	//Some random ints for testing functions
	std::srand(std::time(0));
	for (int i = 0; i< size; i++)
	{
		theArray[i] = std::rand() % 100000 + 1;
	}
	return theArray;
}

// --- IS ZERO --- 
bool isZero(int *theArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (theArray[i] != 0)
		{
			return false;
		}
	}
	return true;
}// --- END IS ZERO ---

// ---QUICK SORT---
//Stole my code from Lab07
////https://rosettacode.org/wiki/Sorting_algorithms/Quicksort#Python
int * quickSort(int *theArray, int size)
{
	int* less = new int[size];
	int* equal = new int[size];
	int* more = new int[size];
	int lessCounter(0), equalCounter(0), moreCounter(0);
	int pivot = theArray[0];
	if (isZero(theArray, size))
	{
		return theArray;
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			if (theArray[i] < pivot)
			{
				less[lessCounter] = theArray[i];
				lessCounter++;
			}
			else if (theArray[i] > pivot)
			{
				more[moreCounter] = theArray[i];
				moreCounter++;
			}
			else
			{
				equal[equalCounter] = theArray[i];
				equalCounter++;
			}
		}
		quickSort(less, lessCounter);
		quickSort(more, moreCounter);
		for (int i = 0; i < lessCounter; i++)
		{
			theArray[i] = less[i];
		}
		for (int i = lessCounter; i < (lessCounter+equalCounter); i++)
		{
			theArray[i] = equal[i - lessCounter];
		}
		for (int i = (lessCounter + equalCounter); i < (lessCounter + equalCounter + moreCounter); i++)
		{
			theArray[i] = more[i - (lessCounter + equalCounter)];
		}
	}
	return theArray;
} // --- END QUICK SORT ---

// --- BINARY SEARCH ---
//This used to look more like my own code, but the more it was
//wrong, the more I changed it to look just like it was on
//Rosetta Code. My problem? Forgot the "return" at the beginning
//of the binSearch recursive calls, so it was changing the returned
//value when the recursion unfolded.
//https://rosettacode.org/wiki/Binary_search
int binSearch(int *theArray, int low, int high, int value)
{
	int midPoint = (low + high) / 2;
	int midVal = theArray[midPoint];

	
	if (high < low)
	{
		return -1;
	}
	if (value < theArray[midPoint])
	{
		return binSearch(theArray, low, (midPoint - 1), value);
	}
	else if (value > theArray[midPoint])
	{
		return binSearch(theArray, (midPoint + 1), high, value);
	}
	return midPoint;
}// --- END BINARY SEARCH ---

// --- MERGE ---
//https://rosettacode.org/wiki/Sorting_algorithms/Merge_sort
int * merge(int *Array, int low, int mid, int high)
{
	//http://www.softwareandfinance.com/Visual_CPP/Merge_Sort.html
	//http://www.algorithmist.com/index.php/Merge_sort
	//ok it's getting closer, been working with the class handout
	//trying to figure out what's happening
	//My merge quits too early, can't figure out
	//how to keep it going
	while (low <= mid)
	{
		//cout << "Merge LMH: " << low << " " << mid << " " << high << endl;
		if ((Array[low] < Array[(mid + 1)]) || Array[(mid + 1)]<0)
		{
			low++;
		}
		else
		{
			int temp = Array[low];
			Array[low] = Array[(mid + 1)];
			Array[(mid + 1)] = temp;
			low++;
		}
		//print(Array, high);
	}
	return Array;
}// --- END MERGE ---

// --- MERGE SORT ---
//https://rosettacode.org/wiki/Sorting_algorithms/Merge_sort
int * mergeSort(int *theArray, int low, int high)
{
	int mid = (low + high) / 2;
	//cout << "l: " << low << " m: " << mid << " h: " << high << endl;
	if (high <= mid)
	{
		return theArray;
	}
	mergeSort(theArray, low, mid);
	mergeSort(theArray, (mid + 1), high);
	merge(theArray, low, mid, high);
	return theArray;
}// --- END MERGE SORT ---

// --- HASH FUNCTION ---
//Hashes all elements in an array of integers
//Made it up myself, with some bit help
//These things are tougher than I would have thought
int* hashFn(const int *theArray, int size, int hashSize)
{
	int* hashedArray = new int[hashSize];
	for (int i = 0; i < size; i++)
	{
		int modifier(0),c(0);
		int v = theArray[i];
		//https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive
		for (c = 0; v; v>>= 1)
		{
			c += (v & 1)*modifier;
			modifier += 7;
		}
		int tempHash = (c + (i*i) % 7103);
		for (int j = 0; j < (i%17); j++)
		{
			tempHash = ((tempHash << (j%5)) ^ theArray[i]) % hashSize;
		}
		//Some Hash numbers are >> size, so one more modulus
		hashedArray[i] = tempHash% hashSize;
	}
	return hashedArray;
}// --- END HASH FUNCTION ---

// --- PRINT ARRAY ---
//For testing
void print(const int *theArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << theArray[i] << endl;
	}

}// --- END PRINT ARRAY ---

// --- TEST SORT ---
//tests that each element is <= the next
void testSort(int *theArray, int size)
{
	//Print array for testing
	//print(theArray, size);
	bool result = true;
	for (int i = 0; i < (size - 1); i++)
	{
		if (theArray[i] > theArray[i + 1])
		{
			result = false;
		}
	}

	if (result)
	{
		cout << "Array is Sorted!!!" << endl;
	}
	else
	{
		cout << "!!! Array Sort did not work !!!" << endl;
	}
	cout << endl;
}// --- END TEST SORT ---

// --- TEST SEARCH ---
//tests a sorted array of integers
//tests for each i < j, that 
//position[i] < position[j]
void testSearch(int *theArray, int size)
{
	bool result = false;
	int max = 0;
	//Find max value in Array
	for (int i = 0; i < size; i++)
	{
		if (theArray[i] > max)
		{
			max = theArray[i];
		}
	}
	//Test Search for i and j (>i) up to max value
	//position of i should be less than position of j
	for (int i = 0; i < (max-1); i++)
	{
		int positionOne = binSearch(theArray, 0, (size - 1), i);
		//if i isn't in array, don't bother checking the js
		if (positionOne != -1)
		{
			for (int j = i + 1; j < max; j++)
			{
				int positionTwo = binSearch(theArray, 0, (size - 1), j);
				//first, if either aren't found skip this pair
				if (positionOne == -1 || positionTwo == -1)
				{
				}
				else if (positionOne < positionTwo)
				{
					result = true;
					//cout << "TRUE " << positionOne << " " << positionTwo << " " << result << endl;
				}
				else
				{
					//cout << "FALSE " << positionOne << " " << positionTwo << " " << result << endl;
					result = false;
				}
			}
		}
		
		
	}
 
	if (result)
	{
		cout << "Search works!!!" << endl;
	}
	else
	{
		cout << "!!! Search did not work !!!" << endl;
	}
	cout << endl;

} // --- END TEST SEARCH ---

// --- TEST FOR COLLISIONS ---
void testForCollisions(int *theArray, int *theTable, int size)
{
	vector<int> collisions;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			bool collided(false);
			for (int k = 0; k < collisions.size(); k++)
				//check to see if either have alread registered a collision
				if (theArray[i] == collisions.at(k) || theArray[j] == collisions.at(k))
				{
					collided = true;
				}
			if ((theTable[i] == theTable[j]) && (theArray[i] != theArray[j]) &&
				(collided == false))
			{
				//collect numbers that already have collsions
				//to avoid double printing collisions
				collisions.push_back(theArray[i]);
				collisions.push_back(theArray[j]);
				//Print new collisions
				cout << "Both, " << theArray[i] << " and " << theArray[j]
					<< " map to " << theTable[i] << endl;
			}
		}
	}
	return;
}

// --- BLOOM HASH ---
// hash for single string
int bloomHash(string text, int tableSize)
{
	int tempHash(0);
	int q(0);
	int number(0);
	for (char c : text)
	{
		number += c * q;
		q += 7;
	}
	for (int i = 0; i < tableSize; i++)
	{
		tempHash = (number * (i*i) % 7103);
		for (int j = 0; j < (i % 17); j++)
		{
			tempHash = ((tempHash << 5) ^ number) % tableSize;
		}
	}
	return tempHash % tableSize;
}// --- END BLOOM HASH ---

// --- BLOOM FILTER ---
int* bloomFilter(string *array, int size, int *hashTable, int hashTableSize)
{
	int one, two, three, four;
	for (int i = 0; i < hashTableSize; i++)
	{
		hashTable[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		int number(0);
		//convert string to integer
		one = bloomHash(array[i], hashTableSize);
		hashTable[one] = 1;
		two = bloomHash(array[i] + "C", hashTableSize);
		hashTable[two] = 1;
		three = bloomHash(array[i] + "Y", hashTableSize);
		hashTable[three] = 1;
		four = bloomHash(array[i] + "m", hashTableSize);
		hashTable[four] = 1;
	}
	//print(hashTable, hashTableSize);
	return hashTable;
}// --- END BLOOM FILTER ---

// --- TEST BLOOM FILTER ---
void testBloomFilter(int *hashTable, int tableSize)
{
	//print(hashTable, tableSize);
	string testText[16] = { "cat", "swift fox", "horse", "Bison", "antelope",
		"Giant Squid", "one eyed one horned flying purple people eater", 
		"Horned Lark", "Elk", "goat", "Orioles", "42", "PSG", "guitar",
		"pizza", "act"};
	int one, two, three, four;
	for (int i = 0; i < 16; i++)
	{
		int number(0);
		one = bloomHash(testText[i], tableSize);
		two = bloomHash(testText[i] + "C", tableSize);
		three = bloomHash(testText[i] + "Y", tableSize);
		four = bloomHash(testText[i] + "m", tableSize);
		if ((hashTable[one] == 1) && (hashTable[two] == 1) &&
			(hashTable[three] == 1) && (hashTable[four] == 1))
		{
			cout << testText[i] << " is probably in the hash table." << endl;
		}
		else
		{
			cout << testText[i] << " is not in the hash table." << endl;
		}
	}
	
}// --- END TEST BLOOM FILTER ---