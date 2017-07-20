#include "Sender.h"


Sender::Sender()
{
	lzw = new LZW();

}


Sender::~Sender()
{
}

void Sender::encodeMessage(string message)
{
	buildInitialTable();
	string p = "";
	string c = "";
	EncodedMessage = "";
	int startIndex = 256;
	for (int i = 0; i < message.size(); i++){
		c = message[i];
		if (lzw->dictionary.count(p + c)){
			p = p + c;
		}
		else {
			lzw->dictionary[p + c] = to_string(startIndex++);
			EncodedMessage += lzw->dictionary[p] + "-";
			p = c;

		}
	}
	EncodedMessage += lzw->dictionary[p];
	
}
string Sender::readMessage(string path){
// read message  to be decoded
	ifstream file;
	file.open(path);
	string readedMessage = "";
	string temp;
	while (getline(file, temp)){
		readedMessage += temp;
	}
	file.close();
	return readedMessage;
}
void Sender::printMessage(string path){
	ofstream file;
	file.open(path);
	file << EncodedMessage;
	file.close();
}
void Sender::printTable(string path){
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
void Sender::buildInitialTable(){
	//construct initial table 
	for (int i = 0; i < 256; i++){
		char tempC = i;
		string tempS = "";
		tempS += tempC;

		lzw->dictionary[tempS] = to_string(i);

	}

}
