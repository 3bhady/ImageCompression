#include "CompressionTester.h"


CompressionTester::CompressionTester()
{
}


CompressionTester::~CompressionTester()
{
}
bool CompressionTester::isEqual(string path1, string path2){
	//read file1
	ifstream file1;
	file1.open(path1);
	string readedMessage1 = "";
	string temp;
	while (getline(file1, temp)){
		readedMessage1 += temp;
	}
	file1.close();
	//read file 2
	ifstream file2;
	file2.open(path2);
	string readedMessage2 = "";
	while (getline(file2, temp)){
		readedMessage2 += temp;
	}
	file2.close();
return 	readedMessage1 == readedMessage2 ? true : false;

}