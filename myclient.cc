#include "myclient.h"
#include "protocol.h"
#include "connection.h"
#include <stdexcept>
#include "connectionclosedexception.h"
#include <memory>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

void myClient::executeCommand(int command, const Connection& conn){
	try{
		switch(command){
			case 1:
				listNewsGroup(conn);
				break;
			case 2:
				createNewsGroup(conn);
				break;
			case 3:
				deleteNewsGroup(conn);
				break;
			case 4:
				listArticleInNewsGroup(conn);
				break;
			case 5:
				createArticle(conn);
				break;
			case 6:
				deleteArticle(conn);
				break;
			case 7:
				getArticle(conn);
				break;
			case 8:
				exit(1);
				break;
			default:
				cout<<"Only numbers between 1-8 are allowed"<<endl;
		}
		
		
	}catch(ConnectionClosedException&){
		cout<< "Server connection is closed"<<endl;
	}
}

void myClient::printMenu() const{
	cout<<"Press 1 to list newsgroups"<<endl;
	cout<<"Press 2 to create newsgroup"<<endl;
	cout<<"Press 3 to delete newsgroup"<<endl;
	cout<<"Press 4 to list articles in a newsgroup"<<endl;
	cout<<"Press 5 to create an article"<<endl;
	cout<<"Press 6 to delete an article"<<endl;
	cout<<"Press 7 to get an article"<<endl;
	cout<<"Press 8 to get exit"<<endl;
}

int myClient::readNumber(const Connection& conn){
	if(conn.read()==Protocol::PAR_NUM){
		unsigned char byte1 = conn.read();
		unsigned char byte2 = conn.read();
		unsigned char byte3 = conn.read();
		unsigned char byte4 = conn.read();
		return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
	}	
	cerr<<"wrong answer from server in readNumber"<<endl;
		exit(1);
}

int myClient::readPureInt(const Connection& conn){
	unsigned char byte1 = conn.read();
	unsigned char byte2 = conn.read();
	unsigned char byte3 = conn.read();
	unsigned char byte4 = conn.read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void myClient::writeNumber(int number, const Connection& conn){
	conn.write(Protocol::PAR_NUM);
	conn.write((number >> 24) & 0xFF);
	conn.write((number >> 16) & 0xFF);
	conn.write((number >> 8) & 0xFF);
	conn.write(number & 0xFF);
	
}

string myClient::readString(const Connection& conn) {
	if(conn.read() == Protocol::PAR_STRING){
		int length = readPureInt(conn);
		string str;
		for (int i = 0; i != length; ++i) {
			str += conn.read();
		}
		return str;
	}
	else{
		cerr<<"wrong answer from server in readString"<<endl;
		exit(1);
	}
}


void myClient::writeString(string s, const Connection& conn) {
	conn.write(Protocol::PAR_STRING);
	int number = s.size();
	conn.write((number >> 24) & 0xFF);
	conn.write((number >> 16) & 0xFF);
	conn.write((number >> 8) & 0xFF);
	conn.write(number & 0xFF);
	for(char c : s){
		conn.write(c);
	}
}

void myClient::listNewsGroup(const Connection& conn){
	conn.write(Protocol::COM_LIST_NG);
	conn.write(Protocol::COM_END);
	if(conn.read()!=Protocol::ANS_LIST_NG){
		cerr<<"wrong answer from server in listNG"<<endl;
		exit(1);
	}
	int temp = readNumber(conn);
	cout<<temp<<endl;
	for(int i = 0;i != temp; ++i){
		cout<< readNumber(conn) <<" ";
		cout<< readString(conn) <<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server in listNG"<<endl;
		exit(1);
	}

	
}
void myClient::createNewsGroup(const Connection& conn){
	cout<<"Type in name of newsgroup"<<endl;
	string s;
	cin>>s;
	conn.write(Protocol::COM_CREATE_NG);
	writeString(s, conn);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_CREATE_NG){
		cerr<<"wrong answer from server in createNG"<<endl;
		exit(1);
	}
	int temp = conn.read();
	if(temp==Protocol::ANS_ACK){
		cout<<"Group created"<<endl;
	}
	else if(temp==Protocol::ANS_NAK){
		if(conn.read()==Protocol::ERR_NG_ALREADY_EXISTS){
		cout<<"Failed to create group, group already exists"<<endl;
		}else{
		cout<<"Unknown error"<<endl;
		exit(1);
		}
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server in createNG"<<endl;
	}
	
}

void myClient::deleteNewsGroup(const Connection& conn){
	cout<<"Type in id of newsgroup"<<endl;
	string s;
	cin>>s;
	int id = 0;
	try{
		id = stoi(s);
	}catch(exception& e){
		cout<<"Unable to execute command, you have to type a number"<<endl;
		return;
	}
	
	conn.write(Protocol::COM_DELETE_NG);
	writeNumber(id, conn);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_DELETE_NG){
		cerr<<"wrong answer from server in deleteNG1"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		cout<<"Group deleted"<<endl;
	}else if(conn.read()==Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Failed to delete group, group does not exists"<<endl;
	}else{
		cout<<"Unknown error"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server in deleteNG2"<<endl;
		exit(1);
	}
}

void myClient::listArticleInNewsGroup(const Connection& conn){
	cout<<"Type in id of newsgroup"<<endl;
	string s;
	cin>>s;
	int id = 0;
	try{
		id = stoi(s);
	}catch(exception& e){
		cout<<"Unable to execute command, you have to type a number"<<endl;
		return;
	}
	conn.write(Protocol::COM_LIST_ART);
	writeNumber(id, conn);
	conn.write(Protocol::COM_END);
	if(conn.read()!=Protocol::ANS_LIST_ART){
		cerr<<"wrong answer from server in listArt1"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		int temp = readNumber(conn);
		for(int i = 0;i!=temp;++i){
			cout<< readNumber(conn) << " ";
			cout<<readString(conn)<<endl;
		}
	} else if(conn.read()==Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Failed to find group, group does not exists"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server in listArt2"<<endl;
		exit(1);
	}
	
}

void myClient::createArticle(const Connection& conn){
	cout<<"Type in id of newsgroup"<<endl;
	string s;
	cin >> s;
	int id = 0;
	try{
		id = stoi(s);
	}catch(exception& e){
		cout<<"Unable to execute command, you have to type a number"<<endl;
		return;
	}
	cout<<"Type in title"<<endl;
	string title;
	cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
	getline(cin, title);
	//getline används för att texten ska kunna läsas in med mellanslag
	cout<<"Type in author"<<endl;
	string author;
	getline(cin, author);
	cout<<"Type in text"<<endl;
	string text;
	getline(cin, text);
	conn.write(Protocol::COM_CREATE_ART);
	writeNumber(id, conn);
	writeString(title, conn);
	writeString(author, conn);
	writeString(text, conn);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_CREATE_ART){
		cerr<<"wrong answer from server in createArt1"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		cout<<"Article created"<<endl;
	}else if(conn.read()==Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Failed to find group, group does not exists"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server in createArt2"<<endl;
		exit(1);
	}
}
void myClient::deleteArticle(const Connection& conn){
	cout<<"Type in id of newsgroup"<<endl;
	string s;
	cin>>s;
	int ngid = 0;
	try{
		ngid = stoi(s);
	}catch(exception& e){
		cout<<"Unable to execute command, you have to type a number"<<endl;
		return;
	}
	cout<<"Type in id of article"<<endl;
	cin>>s;
	int artid = 0;
	try{
		artid = stoi(s);
	}catch(exception& e){
		cout<<"Unable to execute command, you have to type a number"<<endl;
		return;
	}
	conn.write(Protocol::COM_DELETE_ART);
	writeNumber(ngid, conn);
	writeNumber(artid, conn);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_DELETE_ART){
		cerr<<"wrong answer from server in createArt3"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		cout<<"Article deleted"<<endl;
	}else if(conn.read()==Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Could not find newsgroup"<<endl;
	}else{
		cout<<"Could not find article"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	
}
void myClient::getArticle(const Connection& conn){
	cout<<"Type in id of newsgroup"<<endl;
	string s;
	cin>>s;
	int ngid = 0;
	try{
		ngid = stoi(s);
	}catch(exception& e){
		cout<<"Unable to execute command, you have to type a number"<<endl;
		return;
	}
	cout<<"Type in id of article"<<endl;
	cin>>s;
	int artid = 0;
	try{
		artid = stoi(s);
	}catch(exception& e){
		cout<<"Unable to execute command, you have to type a number"<<endl;
		return;
	}
	conn.write(Protocol::COM_GET_ART);
	writeNumber(ngid, conn);
	writeNumber(artid, conn);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_GET_ART){
		cerr<<"wrong answer from server in getArt1"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		cout<<"Title: "<<readString(conn)<<endl;
		cout<<"Author: "<<readString(conn)<<endl;
		cout<<"Text: "<<readString(conn)<<endl;
	}else if(conn.read()==Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Could not find newsgroup"<<endl;
	}else{
		cout<<"Could not find article"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server in getArt2"<<endl;
		exit(1);
	}
}