#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include"Hopcroft-Karp.h"
#include"Huffman.h"
#include<map>
#include<sstream>
#include"md5.h"
#include<string>
#include<ctime>
#include<chrono>
#include<fstream>
#include<set>
#include<bitset>
#include<windows.h>

using namespace cv;
using namespace std;

#define vii vector<pair<int,int> >
#define extraSmallBitsThreshold 31
#define smallBitsThreshold 63
#define mediumBitsThreshold 126
#define LINE pair<pair<int, int>, pair<int, int> >
#define pii pair<int,int>
#define mkp make_pair
#define extraSmallBits 5
#define smallBits 6
#define mediumBits 7
#define largeBits 12

bool galf = false;
bool** vis;
int dx[8] = { 1,0,0,-1,1,-1,-1,1 };
int dy[8] = { 0,1,-1,0,1,-1,1,-1 };
double totalCompressionRatio = 0;
int totalBytes = 0;

void getFilesInDirectory(vector<string>& files) {
	string s = "dir data /b > images.txt";
	system(s.c_str());

	ifstream fin("images.txt");

	while (getline(fin, s)) {
		if (s.length() >= 4 && s.substr(s.length() - 4, 4) == ".jpg") {
			string k = "", y;
			for (int i = s.length() - 5; i >= 0 && s[i] != ' '; --i) {
				k += s[i];
			}
			reverse(k.begin(), k.end());
			files.push_back(k);
		}
	}
	remove("images.txt");
}


bool compareImages(const Mat & originalImage, const Mat & decodedImage)
{
	if (originalImage.size != decodedImage.size)
		return false;

	for (int i = 0; i < originalImage.rows; i++)
	{
		for (int j = 0; j < originalImage.cols; j++)
		{
			if (originalImage.at<uchar>(i, j) != decodedImage.at<uchar>(i, j))
			{
				return false;
			}
		}
	}
	return true;
}

string hashCharacter(const vii& v)
{
	string pixels = "";
	for (int i = 0; i < v.size(); i++)
	{
		stringstream ss;
		string str;
		ss << v[i].first;
		ss >> str;
		pixels += str + ',';
		ss.clear();
		str.clear();
		ss << v[i].second;
		ss >> str;
		pixels += str + '.';
	}
	MD5 md5;
	if (pixels == "") { return ""; }
	char* result = md5.digestString(&(*pixels.begin()));
	string str(result);
	return str;
}

bool isInsideImage(const Mat&image, int row, int col)
{
	//Check for <=
	return (row >= 0 && col >= 0 && row < image.rows && col < image.cols);
}
bool canVisit(const Mat&image, int row, int col)
{
	return isInsideImage(image, row, col)
		&& !vis[row][col]
		&& image.at<uchar>(row, col) == 0;
}
void imageDFS(const Mat& image, vii * vec, int row, int col, int& maxRow, int& maxCol, int& minRow, int& minCol)
{
	vis[row][col] = true;
	vec->push_back(mkp(row, col));
	maxRow = max(maxRow, row);
	maxCol = max(maxCol, col);
	minRow = min(minRow, row);
	minCol = min(minCol, col);

	for (int i = 0; i < 8; ++i)
	{
		if (canVisit(image, row + dx[i], col + dy[i]))
			imageDFS(image, vec, row + dx[i], col + dy[i], maxRow, maxCol, minRow, minCol);
	}
}

//Plus = + in parent to get child   ----   Minus =  - from parent to get child
bool getDist(vii & ChildPlus, vii & ChildMinus, const vii & ParentPlus, const vii & ParentMinus)
{

	int childInitialSize = ChildMinus.size() + ChildPlus.size();

	map <pii, int >PPlus;		//Parent plus map
	map <pii, int >PMinus;		//Parent minus map

	for (int i = 0; i < ParentMinus.size(); ++i)
	{
		if (PMinus.count(ParentMinus[i]) == 0)
			++PMinus[ParentMinus[i]];
	}

	for (int i = 0; i < ChildMinus.size(); ++i)
	{
		if (PMinus.count(ChildMinus[i]) != 0)
		{
			++PMinus[ChildMinus[i]];	//Mark common pairs
			ChildMinus.erase(ChildMinus.begin() + i);	//remove common pairs 
			--i;
		}
	}

	for (int i = 0; i < ParentPlus.size(); ++i)
	{
		if (PPlus.count(ParentPlus[i]) == 0)
			++PPlus[ParentPlus[i]];
	}

	for (int i = 0; i < ChildPlus.size(); ++i)
	{
		if (PPlus.count(ChildPlus[i]) != 0)
		{
			++PPlus[ChildPlus[i]];		//Mark common pairs
			ChildPlus.erase(ChildPlus.begin() + i);		//remove common pairs 
			--i;
		}
	}

	for (auto it = PMinus.begin(); it != PMinus.end(); ++it)
	{
		//Check if exists do not push
		auto i = find(ChildPlus.begin(), ChildPlus.end(), it->first);

		if (it->second == 1 && i == ChildPlus.end())
		{
			ChildPlus.push_back(it->first);		//Add Parent minus to child plus
		}
	}

	for (auto it = PPlus.begin(); it != PPlus.end(); ++it)
	{
		//Check if exists do not push
		auto i = find(ChildMinus.begin(), ChildMinus.end(), it->first);

		if (it->second == 1 && i == ChildMinus.end())
		{
			ChildMinus.push_back(it->first);	//Add Parent plus to child minus
		}
	}

	//return False -> Referencing child to parent not efficient
	return (ChildMinus.size() + ChildPlus.size() < childInitialSize);
}

void parentsDFS(pair<string, pair<vii, vii > >& Node, string Parent, map<string, vector<string> >& parentsGraph, map<string, pair<string, pair< vii, vii > > > &Dictionary)
{
	//Changes Node Plus and Minus vectors 
	getDist(Node.second.first, Node.second.second, Dictionary[Parent].second.first, Dictionary[Parent].second.second);

	//Set current node parent
	Node.first = Parent;

	pair<string, pair<vii, vii > > initialNode = Node;

	for (int i = 0; i < parentsGraph[Parent].size(); ++i)
	{
		pair<string, pair<vii, vii > > resultNode = initialNode;	//DFS result

		parentsDFS(resultNode, parentsGraph[Parent][i], parentsGraph, Dictionary);

		if (resultNode.second.first.size() + resultNode.second.second.size()
			< Node.second.first.size() + Node.second.second.size())
		{
			Node = resultNode;
		}

	}
}

vii getLinePixels(LINE line)
{
	vii v;

	int minx, miny, maxy, maxx;
	minx = min(line.first.first, line.second.first);
	miny = min(line.first.second, line.second.second);
	maxx = max(line.first.first, line.second.first);
	maxy = max(line.first.second, line.second.second);

	if (minx == miny && maxx == maxy) { v.push_back(mkp(minx, miny)); }
	else if (line.first.first == line.second.first)
	{
		for (int i = miny; i <= maxy; i++)
		{
			v.push_back(mkp(line.first.first, i));
		}
	}
	else if (line.first.second == line.second.second)
	{
		for (int i = minx; i <= maxx; i++)
		{
			v.push_back(mkp(i, line.first.second));
		}
	}
	else {
		int i = minx, j = miny;
		while (i <= maxx && j <= maxy)
		{
			v.push_back(mkp(i, j));
			i++; j++;
		}
	}

	return v;
}


void drawline(Mat &image, pii p1, pii p2, int x, int y, int offsetx, int offsety)
{
	image.at<uchar>(p1.first + offsetx, p1.second + offsety) = 0;

	if (p1 == p2)
	{
		return;
	}

	drawline(image, mkp(p1.first + x, p1.second + y), p2, x, y, offsetx, offsety);
}

string convertBitstringToASCII(string str)
{
	char c;
	stringstream ss;
	ss << str;
	string output;

	while (str.length() / 8 != output.length())
	{
		bitset<8> bits;
		ss >> bits;
		char c = char(bits.to_ulong());
		output += c;
	}
	return output;
}

void findParent(vii Node, string hash, map<string, vector<string> > &parentsGraph, map<string, pair<string, pair< vii, vii > > > &Dictionary)
{
	int offsetq = 0;
	int offsetw = 0;


	//childNode => current DFS result node
	//finalNode => best parent match
	pair<string, pair<vii, vii > > childNode;
	pair<string, pair<vii, vii > > finalNode;

	//Make parent as root node
	childNode.first = "-1";
	finalNode.first = "-1";

	//put all pixels in plus vector
	childNode.second.first = Node;
	finalNode.second.first = Node;

	//Search for best parent (Run DFS)
	for (auto it = parentsGraph.begin(); it != parentsGraph.end(); ++it)
	{
		if (it->first == "-1")		//Root node
		{
			//DFS on all parent nodes
			for (int i = 0; i < it->second.size(); ++i)
			{
				parentsDFS(childNode, it->second[i], parentsGraph, Dictionary);

				//if better parent found change final node
				if (childNode.second.first.size() + childNode.second.second.size()
					< finalNode.second.first.size() + finalNode.second.second.size())
				{
					finalNode = childNode;

				}

				childNode.first = "-1";
				childNode.second.first = Node;
				childNode.second.second.clear();

			}
		}
	}

	//Push in parents map
	parentsGraph[finalNode.first].push_back(hash);

	//Push in Dictionary
	Dictionary[hash] = finalNode;
}


int bytes = 0;
void func(Mat& image, map<string, vector<string> > parentsGraph,
	map < string, pair < string, pair < vector<LINE >, vector<LINE >  > > >& Dictionary,
	map<int, int >& Rows, map<int, int >& Cols, vii& startingPixels,
	map<pair<pii, pii >, int> &uniqueHrzntLines, string filename)
{

	int linesSize = 0;
	int symbols = 0;
	int x = 0;
	int m = 0;
	int n = 0;

	string Accumulation = "";

	string linesVertices = "";
	map<pii, vii> Lines2;
	map<pii, vii> Lines3;
	map<LINE, int> uniqueVrtcLines;
	map<int, int> RowsPositions;
	map<int, vector<int>> rowDrivesY;
	map<int, vector<int>> columnDrivesX;
	int bytes = 0;
	int linesbytes = 0;
	vii Ys;
	vii Xs;
	map<string, string> symbolsBinary;
	map<string, vii> symbolsPixels;

	for (int row = 0; row < image.rows; row++)
	{
		for (int col = 0; col < image.cols; col++)
		{
			if (image.at<uchar>(row, col) == 0 && !vis[row][col])
			{
				vii * vec = new  vii;

				int maxRow = INT16_MIN, maxCol = INT16_MIN, minRow = INT16_MAX, minCol = INT16_MAX;

				imageDFS(image, vec, row, col, maxRow, maxCol, minRow, minCol);

				if (rowDrivesY[maxRow].size() == 0)bytes += 2;
				rowDrivesY[maxRow].push_back(minCol);
				bytes += 2;

				RowsPositions[maxRow]++;
				/*for (int i = 0; i < vec->size(); i++)
				{
				image.at<uchar>((*vec)[i].first, (*vec)[i].second) = 127;
				}*/

				/*float avgx = 0;
				float avgy = 0;*/
				for (int i = 0; i < vec->size(); i++)
				{
					(*vec)[i].first -= minRow;
					(*vec)[i].second -= minCol;
					/*avgx += (float)(*vec)[i].first / vec->size();
					avgy += (float)(*vec)[i].second / vec->size();*/

				}
				/*for (int i = 0; i < vec->size(); i++)
				{
				(*vec)[i].first -= (int)avgx;
				(*vec)[i].second -= (int)avgy;

				}*/

				//image.at<uchar>((int)avgx, (int)avgy)=127;

				Rows[maxRow]++;
				Cols[minCol]++;
				startingPixels.push_back(mkp(maxRow, minCol));

				//All symbol pixels
				map<pii, int> pixels;

				//Border pixels
				vii vs;

				//Push symbol pixels in the map
				for (int i = 0; i < vec->size(); i++)
				{
					pixels[mkp((*vec)[i].first, (*vec)[i].second)]++;
				}

				//Get symbol borders
				for (int i = 0; i < vec->size(); i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (pixels.count(mkp((*vec)[i].first + dx[j], (*vec)[i].second + dy[j])) == 0)
						{
							vs.push_back((*vec)[i]);
							break;
						}
					}
				}

				symbols++;

				string hashedString = hashCharacter(*vec);

				symbolsPixels[hashedString].push_back(mkp(minRow, minCol));

				//Check for unique symbol
				if (Dictionary.count(hashedString) == 0)
				{

					vii vec2;

					//(X,Y) => (Y,X) to sort vec2 on Y
					for (int i = 0; i < (*vec).size(); i++)
					{
						vec2.push_back(mkp((*vec)[i].second, (*vec)[i].first));
					}

					//Pair of pixels of all lines of symbol
					vector < LINE > lines;

					map<pii,pii > grid;

					sort((*vec).begin(), (*vec).end());
					sort(vec2.begin(), vec2.end());

					for (int i = 0; i < (*vec).size(); i++)
					{
						pii p = (*vec)[i];	//line start point
						grid[(*vec)[i]].first = lines.size() + 1;
						while (i + 1 < (*vec).size() && (*vec)[i].first == (*vec)[i + 1].first && (*vec)[i].second + 1 == (*vec)[i + 1].second)
						{
							i++;
							grid[(*vec)[i]].first = lines.size() + 1;
						}
						lines.push_back(mkp(p, (*vec)[i]));
					}
					int hrzntLines = lines.size();

					for (int i = 0; i < vec2.size(); i++)
					{
						pii p = vec2[i];		//line start point
						grid[mkp(vec2[i].second,vec2[i].first)].second = lines.size() - hrzntLines + 1;
						while (i + 1 < vec2.size() && vec2[i].first == vec2[i + 1].first && vec2[i].second + 1 == vec2[i + 1].second)
						{
							i++;
							grid[mkp(vec2[i].second, vec2[i].first)].second = lines.size() - hrzntLines + 1;
						}
						lines.push_back(mkp(mkp(p.second, p.first), mkp(vec2[i].second, vec2[i].first)));
					}
					int vrtcLines = lines.size() - hrzntLines;


					//for (auto it = grid.begin(); it != grid.end(); it++)
					//{
					//	image.at<uchar>(it->second.first + offsetx, it->second.second + offsety) = 127;
					//}

					int isLarge = 0;	// Symbol has pixel bigger than 7 bits => largeBits bits
					bool isHorizontal = false; //Symbol is drawn by horizontal or veritcal lines 

					if (hrzntLines < vrtcLines)
					{
						isHorizontal = true;
						lines.erase(lines.begin() + hrzntLines, lines.end());
						for (int i = 0; i < lines.size(); i++)
						{
							if (isLarge < 3
								&& (lines[i].first.first > mediumBitsThreshold
									|| lines[i].first.second > mediumBitsThreshold
									|| lines[i].second.first > mediumBitsThreshold
									|| lines[i].second.second > mediumBitsThreshold)) {
								isLarge = 3;
							}
							else if (isLarge < 2
								&& (lines[i].first.first > smallBitsThreshold
									|| lines[i].first.second > smallBitsThreshold
									|| lines[i].second.first > smallBitsThreshold
									|| lines[i].second.second > smallBitsThreshold)) {
								isLarge = 2;
							}
							else if (isLarge < 1
								&& (lines[i].first.first > extraSmallBitsThreshold
									|| lines[i].first.second > extraSmallBitsThreshold
									|| lines[i].second.first > extraSmallBitsThreshold
									|| lines[i].second.second > extraSmallBitsThreshold)) {
								isLarge = 1;
							}
							Ys.push_back(mkp(lines[i].first.second, lines[i].second.second));
						}
						for (int i = 0; i < lines.size(); i++)
						{
							uniqueHrzntLines[mkp(lines[i].first, lines[i].second)]++;
							Lines2[lines[i].first].push_back(lines[i].second);
							Lines3[lines[i].second].push_back(lines[i].first);

						}
					}
					else {
						lines.erase(lines.begin(), lines.begin() + hrzntLines);
						for (int i = 0; i < lines.size(); i++)
						{
							if (isLarge < 3
								&& (lines[i].first.first > mediumBitsThreshold
									|| lines[i].first.second > mediumBitsThreshold
									|| lines[i].second.first > mediumBitsThreshold
									|| lines[i].second.second > mediumBitsThreshold)) {
								isLarge = 3;
							}
							else if (isLarge < 2
								&& (lines[i].first.first > smallBitsThreshold
									|| lines[i].first.second > smallBitsThreshold
									|| lines[i].second.first > smallBitsThreshold
									|| lines[i].second.second > smallBitsThreshold)) {
								isLarge = 2;
							}
							else if (isLarge < 1
								&& (lines[i].first.first > extraSmallBitsThreshold
									|| lines[i].first.second > extraSmallBitsThreshold
									|| lines[i].second.first > extraSmallBitsThreshold
									|| lines[i].second.second > extraSmallBitsThreshold)) {
								isLarge = 1;
							}
							Xs.push_back(mkp(lines[i].first.first, lines[i].second.first));
						}
						for (int i = 0; i < lines.size(); i++)
						{
							uniqueVrtcLines[mkp(lines[i].first, lines[i].second)]++;
							Lines2[lines[i].first].push_back(lines[i].second);
							Lines3[lines[i].second].push_back(lines[i].first);
						}
					}

					//binary string for representing the symbol with its lines
					//1st bit : 0 => vertical lines, 1 => horizontal lines
					//2nd bit : 0 => 6 bits , 1 => 12 bits
					//3-10 bit => number of lines
					string binary = bitset<11>((int)lines.size()).to_string();


					if (isHorizontal) {
						binary[0] = '1';
					}

					switch (isLarge)
					{
					case 0: {binary[1] = '0'; binary[2] = '0'; break; }
					case 1: {binary[1] = '0'; binary[2] = '1'; break; }
					case 2: {binary[1] = '1'; binary[2] = '0'; break; }
					case 3: {binary[1] = '1'; binary[2] = '1'; break; }
					}

					/*if (isLarge == 0) {
					binary[1] = '1';
					}*/


					//append char c = 0 to string binary if the next line
					//is in the same row,append 1 to increment row
					//append 2 Ys of start and end of line
					if (isHorizontal) {
						for (int i = 0; i < Ys.size(); i++)
						{
							char c = '1';

							//c = 0 (no line increment)
							if (i == 0 || lines[i].first.first == lines[i - 1].first.first) {
								c = '0';
							}
							binary += c;



							//if isLarge
							if (binary[1] =='0' && binary[2] == '0') {
								binary += bitset<extraSmallBits>(Ys[i].first).to_string();
								binary += bitset<extraSmallBits>(Ys[i].second).to_string();
							}
							else if (binary[1] == '0' && binary[2] == '1') {
								binary += bitset<smallBits>(Ys[i].first).to_string();
								binary += bitset<smallBits>(Ys[i].second).to_string();
							}
							else if (binary[1] == '1' && binary[2] == '0') {
								binary += bitset<mediumBits>(Ys[i].first).to_string();
								binary += bitset<mediumBits>(Ys[i].second).to_string();
							}
							else {
								binary += bitset<largeBits>(Ys[i].first).to_string();
								binary += bitset<largeBits>(Ys[i].second).to_string();
							}
						}
					}

					//append char c = 0 to string binary if the next line
					//is in the same column,append 1 to increment column
					//append 2 Xs of start and end of line
					else {
						for (int i = 0; i < Xs.size(); i++)
						{
							char c = '1';

							//c = 0 (no line increment)
							if (i == 0 || lines[i].first.second == lines[i - 1].first.second) {
								c = '0';
							}

							binary += c;

							//if isLarge
							if (binary[1] == '0' && binary[2] == '0') {
								binary += bitset<extraSmallBits>(Xs[i].first).to_string();
								binary += bitset<extraSmallBits>(Xs[i].second).to_string();
							}
							else if (binary[1] == '0' && binary[2] == '1') {
								binary += bitset<smallBits>(Xs[i].first).to_string();
								binary += bitset<smallBits>(Xs[i].second).to_string();
							}
							else if (binary[1] == '1' && binary[2] == '0') {
								binary += bitset<mediumBits>(Xs[i].first).to_string();
								binary += bitset<mediumBits>(Xs[i].second).to_string();
							}
							else {
								binary += bitset<largeBits>(Xs[i].first).to_string();
								binary += bitset<largeBits>(Xs[i].second).to_string();
							}
						}
					}

					symbolsBinary[hashedString] = binary;


					//Accumulation += binary;				

					linesbytes += binary.length();
					Xs.clear();
					Ys.clear();


					linesSize += lines.size();

					//findParent(vv, hashedString, parentsGraph, Dictionary);

					Dictionary[hashedString].first = "-1";
					Dictionary[hashedString].second.first = lines;

				}
			}
		}
	}

	//cout << "Lines Bytes : " << linesbytes / 8 << endl;

	Accumulation += bitset<largeBits>(image.rows).to_string();
	Accumulation += bitset<largeBits>(image.cols).to_string();

	//append pixels position of symbols
	for (auto it = symbolsPixels.begin(); it != symbolsPixels.end(); it++)
	{
		int size = it->second.size();

		//append symbols occurences number
		symbolsBinary[it->first] += bitset<largeBits>(size).to_string();

		//append symbols pixels positions
		for (int i = 0; i < size; i++)
		{
			symbolsBinary[it->first] += bitset<largeBits>(it->second[i].first).to_string();
			symbolsBinary[it->first] += bitset<largeBits>(it->second[i].second).to_string();
		}

		Accumulation += symbolsBinary[it->first];
	}


	ofstream BinFile("CompressedImages/" + filename + ".bin", ios::out | ios::binary | ios::app);

	if (BinFile.is_open())
	{
		int len = Accumulation.length();

		//Make bitstring divisible by 8 to be able to read and write it in bytes
		if (len % 8 != 0)
		{
			for (int i = 0; i < 8 - (len % 8); i++)
			{
				Accumulation += '0';		//append zeroes to the end of the string
			}
		}

		string output = convertBitstringToASCII(Accumulation);

		//Write in file character by character
		for (int i = 0; i < output.length(); i++)
		{
			BinFile.write((char*)&output[i], sizeof(char));
		}

		BinFile.close();
	}
	else cout << "Unable to open file";

	//Symbols pixels are stored here
	vector<vii> readSymbolsPixels;

	//Binaryfile is stored in this string
	string readString = "";

	int linesNum = 0;		//number of lines to be retreaved
	long long index = 0;	//index of bit in string

							//vector of lines read from binary file
	vector<LINE > retreavedLines;

	ifstream file("CompressedImages/" + filename + ".bin", ios::in | ios::binary);

	//read from file till end
	while (!file.eof())
	{
		//read character by character and append bits in readString
		char c;
		file.read((char*)&c, sizeof(char));
		readString += bitset<8>(c).to_string();
	}

	string bits = readString.substr(index, largeBits);
	int imageRows = bitset<largeBits>(bits).to_ulong();
	index += largeBits;

	bits = readString.substr(index, largeBits);
	int imageCols = bitset<largeBits>(bits).to_ulong();
	index += largeBits;

	Mat decodedImage(imageRows, imageCols, IMREAD_GRAYSCALE);

	for (int i = 0; i < imageRows; i++)
	{
		for (int j = 0; j < imageCols; j++)
		{
			decodedImage.at<uchar>(i, j) = 255;
		}
	}

	//loop on symbols
	for (int q = 0; q < Dictionary.size(); q++)
	{

		int isLarge = 3;
		bool isHorizontal = false;

		//1st bit to indicate lines
		if (readString[index++] == '1') {
			isHorizontal = true;
		}

		//2nd bit to indicate lines points size
		if (readString[index] == '0' && readString[index+1] == '0') {
			isLarge = 0;
		}
		else if (readString[index] == '0' && readString[index + 1] == '1') {
			isLarge = 1;
		}
		else if (readString[index] == '1' && readString[index + 1] == '0') {
			isLarge = 2;
		}

		index += 2;

		//get symbol lines number 
		string size = readString.substr(index, 8);
		linesNum = bitset<8>(size).to_ulong();

		index += 8;

		for (int i = 0; i < linesNum; i++)
		{
			//(x1,y1) (x2,y2) two points of line
			int x1, x2, y1, y2;

			if (isHorizontal) {
				if (i == 0)	//first line
					x1 = x2 = 0;
				else x1 = x2 = retreavedLines[retreavedLines.size() - 1].first.first;	//get last line row

																						//read bit of row increment
				if (readString[index++] == '1' && i > 0)
				{
					x1++; x2++;
				}



				// large coordinates => read 12 bits
				// small coordinates => read 6 bits
				switch (isLarge)
				{
				case 0: {size = readString.substr(index, extraSmallBits);
					y1 = bitset<extraSmallBits>(size).to_ulong();
					index += extraSmallBits;

					size = readString.substr(index, extraSmallBits);
					y2 = bitset<extraSmallBits>(size).to_ulong();
					index += extraSmallBits;
					break;
				}
				case 1: {size = readString.substr(index, smallBits);
					y1 = bitset<smallBits>(size).to_ulong();
					index += smallBits;

					size = readString.substr(index, smallBits);
					y2 = bitset<smallBits>(size).to_ulong();
					index += smallBits;
					break;
				}
				case 2: {size = readString.substr(index, mediumBits);
					y1 = bitset<mediumBits>(size).to_ulong();
					index += mediumBits;

					size = readString.substr(index, mediumBits);
					y2 = bitset<mediumBits>(size).to_ulong();
					index += mediumBits;
					break;
				}
				case 3: {size = readString.substr(index, largeBits);
					y1 = bitset<largeBits>(size).to_ulong();
					index += largeBits;

					size = readString.substr(index, largeBits);
					y2 = bitset<largeBits>(size).to_ulong();
					index += largeBits;
					break;
				}
				}

			}
			else {
				if (i == 0)//first line
					y1 = y2 = 0;
				else y1 = y2 = retreavedLines[retreavedLines.size() - 1].first.second;	//get last line column

																						//read bit of column increment
				if (readString[index++] == '1' && i > 0)
				{
					y1++; y2++;
				}

				// large coordinates => read 12 bits
				// small coordinates => read 6 bits
				switch (isLarge)
				{
				case 0: {size = readString.substr(index, extraSmallBits);
					x1 = bitset<extraSmallBits>(size).to_ulong();
					index += extraSmallBits;

					size = readString.substr(index, extraSmallBits);
					x2 = bitset<extraSmallBits>(size).to_ulong();
					index += extraSmallBits;
					break;
				}
				case 1: {size = readString.substr(index, smallBits);
					x1 = bitset<smallBits>(size).to_ulong();
					index += smallBits;

					size = readString.substr(index, smallBits);
					x2 = bitset<smallBits>(size).to_ulong();
					index += smallBits;
					break;
				}
				case 2: {size = readString.substr(index, mediumBits);
					x1 = bitset<mediumBits>(size).to_ulong();
					index += mediumBits;

					size = readString.substr(index, mediumBits);
					x2 = bitset<mediumBits>(size).to_ulong();
					index += mediumBits;
					break;
				}
				case 3: {size = readString.substr(index, largeBits);
					x1 = bitset<largeBits>(size).to_ulong();
					index += largeBits;

					size = readString.substr(index, largeBits);
					x2 = bitset<largeBits>(size).to_ulong();
					index += largeBits;
					break;
				}
				}
			}
			retreavedLines.push_back(mkp(mkp(x1, y1), mkp(x2, y2)));
		}

		int pixelsNum;
		size = readString.substr(index, largeBits);
		pixelsNum = bitset<largeBits>(size).to_ulong();
		index += largeBits;

		vii vc;
		readSymbolsPixels.push_back(vc);

		for (int i = 0; i < pixelsNum; ++i)
		{
			int x, y;

			size = readString.substr(index, largeBits);
			x = bitset<largeBits>(size).to_ulong();
			index += largeBits;

			size = readString.substr(index, largeBits);
			y = bitset<largeBits>(size).to_ulong();
			index += largeBits;

			readSymbolsPixels[q].push_back(mkp(x, y));


			for (int i = 0; i < retreavedLines.size(); ++i)
			{
				int xdiff, ydiff;
				xdiff = retreavedLines[i].first.first - retreavedLines[i].second.first;
				ydiff = retreavedLines[i].first.second - retreavedLines[i].second.second;

				if (xdiff < 0 && ydiff == 0)
				{
					drawline(decodedImage, retreavedLines[i].first, retreavedLines[i].second, 1, 0, x, y);
				}
				if (xdiff >= 0 && ydiff == 0)
				{
					drawline(decodedImage, retreavedLines[i].first, retreavedLines[i].second, -1, 0, x, y);
				}
				if (xdiff == 0 && ydiff <= 0)
				{
					drawline(decodedImage, retreavedLines[i].first, retreavedLines[i].second, 0, 1, x, y);
				}
				if (xdiff == 0 && ydiff > 0)
				{
					drawline(decodedImage, retreavedLines[i].first, retreavedLines[i].second, 0, -1, x, y);
				}
			}
		}
		retreavedLines.clear();
	}

	ofstream statisticsFile("Statistics.txt", ios::out | ios::app);

	if (statisticsFile.is_open())
	{
		if (compareImages(image, decodedImage))
		{
			statisticsFile << "Number of Bytes : " << Accumulation.length() / 8 << endl;
			statisticsFile << "Compression ratio : " << double((image.rows*image.cols * 8)) / (Accumulation.length()) << endl;
			statisticsFile << endl;

			cout << "Image decoded successfully!" << endl;
			cout << "Compression ratio : " << double((image.rows * image.cols * 8)) / (Accumulation.length()) << endl;
			cout << "Number of Bytes : " << Accumulation.length() / 8 << endl;

			totalCompressionRatio += double((image.rows*image.cols * 8)) / (Accumulation.length());
			totalBytes += Accumulation.length() / 8;
		}
		else cout << "Wrong image compression!" << endl;

		statisticsFile.close();
	}
	else cout << "Unable to open file";

	//map<int, int> rowOffset,colOffset;
	//string s1 = "";
	//string s2 = "";
	//for (int i = startingPixels.size() - 1; i > 0; i--)
	//{
	//	//rowOffset[startingPixels[i].first-startingPixels[i-1].first]++;
	//	//colOffset[startingPixels[i].second - startingPixels[i - 1].second]++;
	//	stringstream ss;
	//	string str;
	//	ss << startingPixels[i].first;
	//	ss >> str;
	//	s1 += str + ",";
	//	ss.clear();
	//	str.clear();
	//	ss << startingPixels[i].second;
	//	ss >> str;
	//	s2 += str + ",";
	//}

	//ofstream file3("SymbolsPositionsX.txt", ios::out);
	//if (file3.is_open())
	//{
	//	file3 << s1;
	//	file3.close();
	//}
	//else cout << "Unable to open file";

	//ofstream file4("SymbolsPositionsY.txt", ios::out);
	//if (file4.is_open())
	//{
	//	file4 << s2;
	//	file4.close();
	//}
	//else cout << "Unable to open file";

	//cout << "Unique row offsets: " << rowOffset.size() << endl;
	//cout << "Unique col offsets: " << colOffset.size() << endl;



	//cout << "Bytes to represent symbols " << bytes << endl;
	//cout << "Lines Size : " << linesSize << endl;
	//cout << "Symbols number : " << symbols << endl;

	//cout << "Unique rows : " << Rows.size()<<endl;
	int minrow = 9000, maxrow = -1, rowOccurences = 0;
	for (auto it = Rows.begin(); it != Rows.end(); it++)
	{
		rowOccurences += it->second;
		minrow = min(minrow, it->second);
		maxrow = max(maxrow, it->second);
	}
	//cout << "Minimum row occurences : " << minrow << endl;
	//cout << "Maximum row occurences : " << maxrow << endl;
	//cout << "Total row occurences : " << rowOccurences << endl;

	//cout << "Unique columns : " << Cols.size() << endl;
	int mincol = 9000, maxcol = -1, columnsOccurences = 0;
	for (auto it = Cols.begin(); it != Cols.end(); it++)
	{
		columnsOccurences += it->second;
		mincol = min(mincol, it->second);
		maxcol = max(maxcol, it->second);
	}
	//cout << "Minimum column occurences : " << mincol << endl;
	//cout << "Maximum column occurences : " << maxcol << endl;
	//cout << "Total columns occurences : " << columnsOccurences << endl;
	//cout << "Minimum number of lines : " << x << endl;

	/*string YY = "";
	string XX = "";
	for (int i = 0; i < Ys.size(); i++)
	{
	stringstream ss;
	string str;
	ss << Ys[i].first;
	ss >> str;
	YY += str + ",";
	ss.clear();
	str.clear();
	ss << Ys[i].second;
	ss >> str;
	YY += str + "-";
	}
	for (int i = 0; i < Xs.size(); i++)
	{
	stringstream ss;
	string str;
	ss << Xs[i].first;
	ss >> str;
	XX += str + ",";
	ss.clear();
	str.clear();
	ss << Xs[i].second;
	ss >> str;
	XX += str + "-";
	}
	ofstream file1("XX.txt", ios::out);
	if (file1.is_open())
	{
	file1 << XX;
	file1.close();
	}
	else cout << "Unable to open file";

	ofstream file2("YY.txt", ios::out);
	if (file2.is_open())
	{
	file2 << YY;
	file2.close();
	}
	else cout << "Unable to open file";*/

	//cout << "Max Y : " << m << endl << "Max X : " << n << endl;

};


void Draw(Mat& image, string hash, map<string, pair<string, pair< vii, vii > > >& Dictionary, int& offsetx, int &offsety)
{
	if (hash == "-1")
	{
		offsetx += 50;
		if (offsetx > 1000)
		{
			offsetx = 50;
			offsety += 50;
		}
		return;
	}

	Draw(image, Dictionary[hash].first, Dictionary, offsetx, offsety);

	for (int i = 0; i < Dictionary[hash].second.first.size(); i++)
	{
		image.at<uchar>(Dictionary[hash].second.first[i].first + offsety, Dictionary[hash].second.first[i].second + offsetx) = 127;
	}

	for (int i = 0; i < Dictionary[hash].second.second.size(); i++)
	{
		image.at<uchar>(Dictionary[hash].second.second[i].first + offsety, Dictionary[hash].second.second[i].second + offsetx) = 255;
	}


}


int main(int argc, char** argv)
{

	vector<string > files;

	getFilesInDirectory(files);

	if (argc != 2)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}


	for (int i = 0; i < files.size(); i++)
	{
		Mat image;

		// Read the file
		image = imread("data/" + files[i] + ".jpg");

		cvtColor(image, image, COLOR_BGR2GRAY);
		//std::cout << image.channels();

		//Characters dictionary
		map < string, pair < string, pair < vector<LINE >, vector<LINE >  > > > Dictionary;

		//Parents map
		map<string, vector<string> > parentsGraph;

		//Pair of start and end points of unique lines
		map<LINE, int> uniqueLines;

		map<int, int > Rows;		//Unique Rows 
		map<int, int > Cols;	//Unique Columns

		vii startingPixels;

		if (!image.data)   // Check for invalid input
		{
			cout << "Could not open or find the image" << std::endl;
			return -1;;
		}

		//Convert image to binary
		threshold(image, image, 180, 255, THRESH_BINARY);

		// Create a window for display.
		namedWindow("Display window", WINDOW_AUTOSIZE);

		//initialize visited array for DFS
		vis = new bool*[image.rows];
		for (int i = 0; i < image.rows; i++)
		{
			vis[i] = new bool[image.cols];
		}

		for (int i = 0; i < image.rows; i++)
		{
			for (int j = 0; j < image.cols; j++)
			{
				vis[i][j] = false;
			}
		}

		auto started = std::chrono::high_resolution_clock::now();


		func(image, parentsGraph, Dictionary, Rows, Cols, startingPixels, uniqueLines, files[i]);


		auto done = std::chrono::high_resolution_clock::now();

		std::cout << "Elapsed Time : " << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << endl;


		/*int offsetx = 50;
		int offsety = 50;
		for (auto i = Dictionary.begin(); i != Dictionary.end(); ++i)
		{
		Draw(image, i->first, Dictionary, offsetx, offsety);
		}*/

		/*Size sz;
		sz.height = 700;
		sz.width = 700;
		resize(image, image, sz);*/

		long long dictionarySize = 0;

		for (auto it = Dictionary.begin(); it != Dictionary.end(); it++)
		{
			dictionarySize += it->second.second.first.size() + it->second.second.second.size();
		}

		//cout << "Dictionary Size : " << dictionarySize << endl;
		//cout << "Unique Symbols : " << Dictionary.size() << endl;
		//cout << "bytes to represent  Symbols position : " << bytes << endl;

		for (int i = 0; i < image.rows; i++)
		{
			delete vis[i];
		}

		delete vis;

		vis = NULL;
		//imshow("Display window", image);  // Show our image inside it.
	}

	ofstream statisticsFile("Statistics.txt", ios::out | ios::app);

	if (statisticsFile.is_open())
	{
		statisticsFile << "Total bytes : " << totalBytes << endl;
		statisticsFile << "Total Compression ratio : " << totalCompressionRatio << endl;

		cout << "Total bytes : " << totalBytes << endl;
		cout << "Total Compression ratio : " << totalCompressionRatio << endl;

		statisticsFile.close();
	}
	else cout << "Unable to open file";

	waitKey(0);  // Wait for a keystroke in the window
	return 0;
}