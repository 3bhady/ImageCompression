#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include"Sender.h"
#include"Reciever.h"
#include"LZW.h"
#include"CompressionTester.h"
using namespace std;

int main(){
	//Encoder side
	{
	Sender* sender = new Sender();
	
	sender->encodeMessage(
		sender->readMessage("inputMessage.txt")
		);
	sender->printMessage("encodedMessage.txt");
	sender->printTable("encodingTable.txt");
	}

	//Decoder side
	{
	Reciever* reciever = new Reciever();

	reciever->decodeMessage(
		reciever->readMessage("encodedmessage.txt")
		);
	reciever->printMessage("decodedMessage.txt");
	reciever->printTable("decodingTable.txt");
	}

	//testing both(decoded and original message);
	{
		CompressionTester *tester = new CompressionTester();
		(tester->isEqual("decodedMessage.txt", "inputMessage.txt")) ? cout << "Decoded message and original message matches \n" : cout << "Decoded message and original message does not  match \n";
	}

	return 0;
}