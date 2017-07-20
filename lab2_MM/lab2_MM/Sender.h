#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include"LZW.h"
#include <fstream>
#include <string>
#include <math.h>   
using namespace std;
class Sender
{
private:
	LZW *lzw;
	string message;
	string EncodedMessage;
public:
	Sender();
	~Sender();
	void encodeMessage(string message);
	string readMessage(string path);
	void printMessage(string path);
	void printTable(string path);
	void buildInitialTable();
};

