#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
using namespace std;
class LZW
{
private:

public:
	map<string, string> dictionaryInv;
	map<string, string> dictionary;

	LZW();
	~LZW();
};

