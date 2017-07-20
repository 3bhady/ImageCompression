/*int leastDistance = INT16_MAX;
string p = "-1";
for (auto it = Dictionary.begin(); it != Dictionary.end(); it++)
{
vii * v = new  vii;
//getPixels(it->first, *v, Dictionary);
int distance = 0;
getDistance(*vec, *v, distance);
if (leastDistance > distance)
{
leastDistance = distance;
p = it->first;
}
}
if (p != "-1")
{
int distance = 0;
vii v1;
//getPixels(p, v1, Dictionary);
vii v = getDistance(*vec, v1, distance);
string str;
stringstream ss;
ss << p;
ss >> str;
Dictionary[hashedString] = make_pair(str, v);
}
else Dictionary[hashedString] = make_pair("-1", *vec);
*/




/*vector < pair<pii,pii > > lines;
map<pair<pii,pii >, vii > linePixels;
vii vp;

for (int i = 0; i < vs.size(); i++)
{
for (int j = 0; j < 8; j++)
{
pair<int, int > p = make_pair(vs[i].first + dx[j], vs[i].second + dy[j]);
while (pixels.count(p) != 0)
{
vp.push_back(p);
p.first += dx[j];
p.second += dy[j];
}
p.first -= dx[j];
p.second -= dy[j];
if (linePixels.count(make_pair(p, vs[i])) == 0)
{
lines.push_back(make_pair(vs[i], p));
linePixels[make_pair(vs[i], p)] = vp;
vp.clear();
}
}
}

map<pii, int > pixelsCovered;
string indexes="";
int linesNum = findMinLines(pixelsCovered, (*vec).size(), lines, 0, indexes);

for (int i = 0; i < lines.size(); ++i)
{

if (indexes[i] == '1')
{
int x, y;
x = lines[i].first.first - lines[i].second.first;
y = lines[i].first.second - lines[i].second.second;
if (x < 0 && y < 0)
{
drawline(image, lines[i].first, lines[i].second, 1, 1, offsetx, offsety);
}
if (x > 0 && y > 0)
{
drawline(image, lines[i].first, lines[i].second, -1, -1, offsetx, offsety);
}
if (x < 0 && y == 0)
{
drawline(image, lines[i].first, lines[i].second, 1, 0, offsetx, offsety);
}
if (x > 0 && y == 0)
{
drawline(image, lines[i].first, lines[i].second, -1, 0, offsetx, offsety);
}
if (x == 0 && y < 0)
{
drawline(image, lines[i].first, lines[i].second, 0, 1, offsetx, offsety);
}
if (x == 0 && y > 0)
{
drawline(image, lines[i].first, lines[i].second, 0, -1, offsetx, offsety);
}
if (x < 0 && y > 0)
{
drawline(image, lines[i].first, lines[i].second, 1, -1, offsetx, offsety);
}
if (x > 0 && y < 0)
{
drawline(image, lines[i].first, lines[i].second, -1, 1, offsetx, offsety);
}

}
}*/



/*for (int i = 0; i < lines.size(); i++)
{
if (lines[i].first.second < 200 && lines[i].second.second < 200) {
m = max(max(m, lines[i].first.first), lines[i].second.first);
n = max(max(n, lines[i].first.second), lines[i].second.second);
stringstream ss;
string str;
ss << lines[i].first.first;
ss >> str;
linesVertices += str + ",";
ss.clear();
str.clear();
ss << lines[i].first.second;
ss >> str;
linesVertices += str + ",";
ss.clear();
str.clear();
ss << lines[i].second.first;
ss >> str;
linesVertices += str + ",";
ss.clear();
str.clear();
ss << lines[i].second.second;
ss >> str;
linesVertices += str + ",";
}
}*/





//BipGraph graph(hrzntLines, vrtcLines);

//for (auto it = grid.begin(); it != grid.end(); it++)
//	graph.addEdge(it->second.first, it->second.second);

//vector<int> rowsIndexes;
//vector<int>	colsIndexes;
//graph.hopcroftKarp(rowsIndexes,colsIndexes);

//x += (rowsIndexes.size() + colsIndexes.size());
//for (int i = 0; i < rowsIndexes.size(); i++)
//{
//	int y;
//	y = lines[rowsIndexes[i] - 1].first.second - lines[rowsIndexes[i] - 1].second.second;
//	if ( y <= 0)
//	{
//		drawline(image, lines[rowsIndexes[i] - 1].first, lines[rowsIndexes[i] - 1].second, 0, 1, offsetx, offsety);
//	}
//	else
//	{
//		drawline(image, lines[rowsIndexes[i] - 1].first, lines[rowsIndexes[i] - 1].second, 0, -1, offsetx, offsety);
//	}
//}
//for (int i = 0; i < colsIndexes.size(); i++)
//{
//	int x;
//	x = lines[colsIndexes[i] + hrzntLines - 1].first.first - lines[colsIndexes[i] + hrzntLines - 1].second.first;
//	if (x <= 0)
//	{
//		drawline(image, lines[colsIndexes[i] + hrzntLines - 1].first, lines[colsIndexes[i] + hrzntLines - 1].second, 1, 0, offsetx, offsety);
//	}
//	else 
//	{
//		drawline(image, lines[colsIndexes[i] + hrzntLines - 1].first, lines[colsIndexes[i] + hrzntLines - 1].second, -1, 0, offsetx, offsety);
//	}
//}



/*vii vs2;
for (int i = 0; i < vs.size(); i++)
{
vs2.push_back(make_pair(vs[i].second, vs[i].first));
}

vector < pair<pii,pii > > lines;

sort(vs.begin(), vs.end());
sort(vs2.begin(), vs2.end());

for (int i = 0; i < vs.size(); i++)
{
pii p = vs[i];
while (i + 1 < vs.size() && vs[i].first == vs[i + 1].first && vs[i].second + 1 == vs[i + 1].second)
{
i++;
}
lines.push_back(make_pair(p, vs[i]));
}

for (int i = 0; i < vs2.size(); i++)
{
pii p = vs2[i];
while (i + 1 < vs2.size() && vs2[i].first == vs2[i + 1].first && vs2[i].second + 1 == vs2[i + 1].second)
{
i++;
}
lines.push_back(make_pair(make_pair(p.second, p.first), make_pair(vs2[i].second, vs2[i].first)));
}*/



/*map<pii, int > pixelsCovered;
string indexes = "";
int linesNum = findMinLines(pixelsCovered, (*vec).size(), lines, 0, indexes);*/

//for (int i = 0; i < lines.size(); ++i)
//{

//	//if (indexes[i] == '1')
//	//{
//	int x, y;
//	x = lines[i].first.first - lines[i].second.first;
//	y = lines[i].first.second - lines[i].second.second;
//	if (x < 0 && y == 0)
//	{
//		drawline(image, lines[i].first, lines[i].second, 1, 0, offsetx, offsety);
//	}
//	if (x >= 0 && y == 0)
//	{
//		drawline(image, lines[i].first, lines[i].second, -1, 0, offsetx, offsety);
//	}
//	if (x == 0 && y <= 0)
//	{
//		drawline(image, lines[i].first, lines[i].second, 0, 1, offsetx, offsety);
//	}
//	if (x == 0 && y > 0)
//	{
//		drawline(image, lines[i].first, lines[i].second, 0, -1, offsetx, offsety);
//	}

//	//}
//}



//map<string, int> LZW;
//int cnt = 0;
//for (int i = -24; i < 23; i++)
//{
//	for (int j = -25; j < 32; j++)
//	{
//		string pixel = "";
//		string str;
//		stringstream ss;
//		ss << i;
//		ss >> str;
//		pixel += str + ',';
//		ss.clear();
//		str.clear();
//		ss << j;
//		ss >> str;
//		pixel += str + ',';
//		LZW[pixel] = cnt++;
//	}
//}
//string S = "";
//string EF = "";
////int xmax = -1, xmin = 1, ymax = -1, ymin = 1;
//for (auto it = Dictionary.begin(); it != Dictionary.end(); it++)
//{
//	for (int i = 0; i < it->second.second.first.size(); i++)
//	{
//		
//		if (it->second.second.first.size() > 1000)continue;
//		/*xmax = max(xmax, it->second.second.first[i].first);
//		xmin = min(xmin, it->second.second.first[i].first);
//		ymax = max(ymax, it->second.second.first[i].second);
//		ymin = min(ymin, it->second.second.first[i].second);*/
//		string pixel = "";
//		string str;
//		stringstream ss;
//		ss << it->second.second.first[i].first;
//		ss >> str;
//		pixel += str + ',';
//		ss.clear();
//		str.clear();
//		ss << it->second.second.first[i].second;
//		ss >> str;
//		pixel += str + ',';
//		S += pixel;
//		if (LZW.count(S) == 0)
//		{
//			LZW[S] = cnt++;
//			bool flag = false;
//			int m;
//			for (m = S.length() - 2; m >= 0; m--)
//			{
//				if (flag && S[m] == ',')break;
//				if (S[m] == ',')flag = true;
//				
//			}
//			string z = S.substr(0, m+1);
//			stringstream ss;
//			string k;
//			ss << LZW[S.substr(0, m+1)];
//			ss >> k;
//			EF += k + ".";
//			S = pixel;
//		}
//	}	
//}
//cout << endl << ymax << endl << xmax << endl << ymin << endl << xmin << endl;




//vector<string>data;
//vector<int>freq;
//map<string, int> maa;


//for (auto it = Dictionary.begin(); it != Dictionary.end(); it++)
//{
//	for (int i = 0; i < it->second.second.first.size(); i++)
//	{
//		string pixel = "";
//		stringstream ss;
//		string str;
//		ss << it->second.second.first[i].first;
//		ss >> str;
//		pixel += str + ",";
//		//maa[pixel]++;
//		ss.clear();
//		str.clear();
//		ss << it->second.second.first[i].second;
//		ss >> str;
//		pixel += str + ",";
//		maa[pixel]++;
//	}
//	for (int i = 0; i < it->second.second.second.size(); i++)
//	{
//		string pixel = "";
//		stringstream ss;
//		string str;
//		ss << it->second.second.second[i].first;
//		ss >> str;
//		pixel += str + ",";
//		//maa[pixel]++;
//		ss.clear();
//		str.clear();
//		ss << it->second.second.second[i].second;
//		ss >> str;
//		pixel += str + ",";
//		maa[pixel]++;
//	}
//}

//for (auto it = maa.begin(); it != maa.end(); ++it)
//{
//	data.push_back(it->first);
//	freq.push_back(it->second);

//}
//Huffman H;
//H.HuffmanCodes(data, freq);



/*ofstream file("example.bin", ios::out | ios::binary );

if (file.is_open())
{
for (auto it = Dictionary.begin(); it != Dictionary.end(); it++)
{
file.write((char*)&it->first, sizeof(char));
file.write((char*)&it->second.first, sizeof(char));
file.write((char*)&it->second.second.first, sizeof(pair<char, char>)*it->second.second.first.size());
file.write((char*)&it->second.second.second, sizeof(pair<char, char>)*it->second.second.second.size());

}
file.close();
}
else cout << "Unable to open file";*/



//vii getDistance(vii & v1, vii & v2,int& x)
//{
//	vii v;
//	for (int a = 0; a < v1.size(); a++)
//	{
//		bool found = false;
//		for (int b = 0; b < v2.size(); b++)
//		{
//			if (v1[a].first == v2[b].first && v1[a].second == v2[b].second)
//			{
//				found = true;
//				break;
//			}
//		}
//		if (!found)
//		{
//			x++;
//			v.push_back(make_pair(v1[a].first, v1[a].second));
//		}
//	}
//	return v;
//}


/*void getPixels(string index, vii & v, map<string, pair<string, pair< vii, vii > > >& Dictionary)
{
if (index == "-1") {
return;
}
if (Pixels.count(index) != 0)
{
v = Pixels[index];
return;
}
getPixels(Dictionary[index].first, v, Dictionary);
for (int i = 0; i < Dictionary[index].second.size(); i++)
v.push_back(make_pair(Dictionary[index].second[i].first, Dictionary[index].second[i].second));
Pixels[index] = v;
}*/


/*map<pii, int > mp;
for (int i = 0; i < Parent.size(); ++i)
++mp[Parent[i]];

// Find pairs in child only
// Mark pairs in child and in parent (mp[common pair] = 2)
for (int i = 0; i < Child.size(); ++i)
{
if (mp.count(Child[i]) == 0)
{
Plus.push_back(Child[i]);
}
else ++mp[Child[i]];	//mark common pairs
}

//Find pairs in parent only
for (auto it = mp.begin(); it != mp.end(); ++it)
{
if (it->second == 1)
{
Minus.push_back(it->first);
}
}*/


//int findMinLines(map<pii, int >& pixelsCovered, int pixelsNum,
	//	vector < LINE > lines,int index,string& indexes)
	//{
	//	if (pixelsCovered.size() == pixelsNum || index >= lines.size())
	//		return 0;
	//
	//	if (dp.count(mkp(pixelsCovered, index)) != 0)
	//		return dp[mkp(pixelsCovered, index)];
	//
	//	int ret = findMinLines(pixelsCovered, pixelsNum, lines, index + 1, indexes + "0");
	//
	//	stringstream ss;
	//	string str;
	//	ss << index;
	//	ss >> str;
	//
	//	vii v= getLinePixels(lines[index]);
	//
	//	for (int i = 0; i < v.size(); i++)
	//	{
	//		pixelsCovered[v[i]]++;
	//	}
	//	ret = min(ret, 1 + findMinLines(pixelsCovered, pixelsNum, lines, index + 1, indexes + "1"));
	//	
	//	return dp[mkp(pixelsCovered, index)] = ret;
	//}



