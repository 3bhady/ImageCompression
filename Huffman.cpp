#include "Huffman.h"
#include <opencv2/core/core.hpp>


Huffman::Huffman( )
{
	out.open( "huffman.txt" );
}


Huffman::~Huffman( )
{
}



// C++ program for Huffman Coding

// A Huffman tree node


// Prints huffman codes from the root of Huffman Tree.
void Huffman::printCodes( struct MinHeapNode* root , string str,map<string,int>& mp )
{
	if ( !root )
		return;

	if ( root->data != "$" )
		tot += str.size( )*mp[root->data];
	//	cout << root->data << ": " << str << "\n";

	printCodes( root->left , str + "0",mp );
	printCodes( root->right , str + "1",mp );
}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
void Huffman::HuffmanCodes( vector<string> data , vector<int> freq )
{
	struct MinHeapNode *left , *right , *top;
	int size = data.size( );
	// Create a min heap & inserts all characters of data[]
	priority_queue<MinHeapNode* , vector<MinHeapNode*> , compare> minHeap;
	for ( int i = 0; i < size; ++i )
		minHeap.push( new MinHeapNode( data[i] , freq[i] ) );

	// Iterate while size of heap doesn't become 1
	while ( minHeap.size( ) != 1 )
	{
		// Extract the two minimum freq items from min heap
		left = minHeap.top( );
		minHeap.pop( );

		right = minHeap.top( );
		minHeap.pop( );

		// Create a new internal node with frequency equal to the
		// sum of the two nodes frequencies. Make the two extracted
		// node as left and right children of this new node. Add
		// this node to the min heap
		// '$' is a special value for internal nodes, not used
		top = new MinHeapNode( "$" , left->freq + right->freq );
		top->left = left;
		top->right = right;
		minHeap.push( top );
	}
	map<string , int> mp;
	for ( int i = 0; i < size; i++ )
		mp[data[i]] = freq[i];
	// Print Huffman codes using the Huffman tree built above
	printCodes( minHeap.top( ) , "",mp );
	result += to_string( tot );
	out << result;
	out.close( );
}



/*
// Driver program to test above functions
int main( )
{
	char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int freq[] = { 5, 9, 12, 13, 16, 45 };
	int size = sizeof( arr ) / sizeof( arr[0] );

	HuffmanCodes( arr , freq , size );

	return 0;
}
// This code is contributed by Aditya Goel
  */