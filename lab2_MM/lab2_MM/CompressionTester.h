#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include"LZW.h"
#include <fstream>
#include<string>
using namespace std;
class CompressionTester
{
public:
	bool isEqual(string path1, string path2);
	CompressionTester();
	~CompressionTester();
};

