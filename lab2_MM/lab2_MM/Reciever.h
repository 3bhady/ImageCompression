#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include"LZW.h"
#include <fstream>
#include<string>
using namespace std;
class Reciever
{
private:
	LZW *lzw;
	string message;
	string DecodedMessage;
public:
	Reciever();
	~Reciever();
	void decodeMessage(vector<string> message);
	vector<string> readMessage(string path);
	void printMessage(string path);
	void printTable(string path);
	void buildInitialTable();
};

