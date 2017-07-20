#include "Reciever.h"


Reciever::Reciever()
{
	lzw = new LZW();
}


Reciever::~Reciever()
{
}
void Reciever::decodeMessage(vector<string> message)
{
	buildInitialTable();
	string decodedMessage = "";
	string cW = "";
	string pW = "";
	string string_cW = "";
	string string_pW = "";
	string p = "";
	string c = "";

	cW = message[0];
	string_cW = lzw->dictionaryInv[cW];
	decodedMessage += string_cW;
	int startIndex = 256;
	for (int i = 1; i < message.size(); i++){
		pW = cW;
		cW = message[i];
		string_cW = lzw->dictionaryInv[cW];
		string_pW = lzw->dictionaryInv[pW];
		if (lzw->dictionary.count(string_cW)){
			decodedMessage += string_cW;
			p = string_pW;
			c = string_cW[0];
			lzw->dictionary[p + c] = to_string(startIndex);
			lzw->dictionaryInv[to_string(startIndex)] = p+c;
			startIndex++;
		}
		else {
			p = string_pW;
			c = p[0];
			decodedMessage += (p + c);
			lzw->dictionary[p + c] = to_string(startIndex);
			lzw->dictionaryInv[to_string(startIndex)] =p+c;
			startIndex++;
		}

	}
	DecodedMessage = decodedMessage;
}
vector<string> Reciever::readMessage(string path){
	// read message  to be decoded
	ifstream file;
	file.open(path);
	vector<string> readedMessage ;
	string temp;
	while (getline(file, temp,'-')){
		readedMessage.push_back(temp);
	}
	file.close();
	return readedMessage;

}
void Reciever::printMessage(string path){
	ofstream file;
	file.open(path);
	file << DecodedMessage;
	file.close();
}
void Reciever::printTable(string path){
	map<string, string>::iterator it;
	string temp = "";
	string table = "";
	ofstream file;

	for (it = lzw->dictionary.begin(); it != lzw->dictionary.end(); it++)
	{
		temp = it->first + "=" + lzw->dictionary[it->first] + "\n";
		table += temp;
	}
	file.open(path);
	file << table;
	file.close();

}
void Reciever::buildInitialTable(){
	//construct initial table 
	for (int i = 0; i < 256; i++){
		char tempC = i;
		string tempS = "";
		tempS += tempC;
		lzw->dictionaryInv[to_string(i)] = tempS;
		lzw->dictionary[tempS] = to_string(i);
		

	}

}
