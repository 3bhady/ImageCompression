#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <map>
#include <vector>
#include <fstream>

using namespace std;
struct MinHeapNode
{
	string data;			 // One of the input characters
	unsigned freq;			 // Frequency of the character
	MinHeapNode *left , *right; // Left and right child

	MinHeapNode( string data , unsigned freq )
	{
		left = right = NULL;
		this->data = data;
		this->freq = freq;
	}
};

// For comparison of two heap nodes (needed in min heap)
struct compare
{
	bool operator()( MinHeapNode* l , MinHeapNode* r )
	{
		return (l->freq > r->freq);
	}
};
class Huffman
{
public:
	ofstream out;
	long long tot = 0;
	string result="";
	Huffman();
	~Huffman( );
	void HuffmanCodes( vector<string> data , vector<int> freq );
void printCodes( struct MinHeapNode* , string, map<string , int>& mp,string& maxCWLength);





};

