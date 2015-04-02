#include "myclient.h"
#include "protocol.h"
#include "connection.h"
#include <stdexcept>
#include "connectionclosedexception.h"

using namespace std;

myClient::myClient(int argc, char* argv[]){
	if(argc !=3){
		cerr<< "Usage: Usage: myclient host-name port-number" << endl;
		exit(1);
	}
	int a = 12;
	char* s[]={"hello"};
	Connection hej(s[1],a);
	int portnumber = 0;
	try{
		portnumber = stoi(argv[2]);
	}catch(exception& e){
		cerr<< "Wrong port number"<<endl;
	}
	conn(argv[1], portnumber);
	if(!conn.isConnected()){
		cerr<<"Connection attempt failed"<<endl;
		exit(1);
	}
	
}

void myClient::executeCommand(int command){
	try{
		switch(command){
			case 1:
				listNewsGroup();
				break;
			case 2:
				createNewsGroup();
				break;
			case 3:
				deleteNewsGroup();
				break;
			case 4:
				listArticleInNewsGroup();
				break;
			case 5:
				createArticle();
				break;
			case 6:
				deleteArticle();
				break;
			case 7:
				getArticle();
				break;
			case 8:
				exit(1);
				break;
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
}

int myClient::readNumber(){
	if(conn.read()==Protocol::PAR_NUM){
		unsigned char byte1 = conn.read();
		unsigned char byte2 = conn.read();
		unsigned char byte3 = conn.read();
		unsigned char byte4 = conn.read();
		return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
	}	
	cerr<<"wrong answer from server"<<endl;
		exit(1);
}

void myClient::writeNumber(int number){
	conn.write((number >> 24) & 0xFF);
	conn.write((number >> 16) & 0xFF);
	conn.write((number >> 8) & 0xFF);
	conn.write(number & 0xFF);
	
}

string myClient::readString() {
	int length = readNumber();
	string str;
	for (int i = 0; i != length; ++i) {
		str += conn.read();
	}
	return str;
}


void myClient::writeString(string s) {
	conn.write(Protocol::PAR_STRING);
	writeNumber(s.size());
	for(char c : s){
		conn.write(c);
	}
}

void myClient::listNewsGroup(){
	conn.write(Protocol::COM_LIST_NG);
	conn.write(Protocol::COM_END);
	if(conn.read()!=Protocol::ANS_LIST_NG){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	for(int i = 0;i<readNumber();++i){
		cout<< readNumber() << " "<<readString()<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}

	
}
void myClient::createNewsGroup(){
	cout<<"Type in name of newsgroup"<<endl;
	string s;
	cin>>s;
	conn.write(Protocol::COM_CREATE_NG);
	writeString(s);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_CREATE_NG){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		cout<<"Group created"<<endl;
	}
	if(conn.read()!=Protocol::ERR_NG_ALREADY_EXISTS){
		cout<<"Failed to create group, group already exists"<<endl;
	}else{
		cout<<"Unknown error"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	
}

void myClient::deleteNewsGroup(){
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
	writeNumber(id);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_CREATE_NG){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		cout<<"Group deleted"<<endl;
	}else if(conn.read()!=Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Failed to delete group, group does not exists"<<endl;
	}else{
		cout<<"Unknown error"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
}

void myClient::listArticleInNewsGroup(){
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
	writeNumber(id);
	conn.write(Protocol::COM_END);
	if(conn.read()!=Protocol::ANS_LIST_ART){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		for(int i = 0;i<readNumber();++i){
			cout<< readNumber() << " "<<readString()<<endl;
		}
	} else if(conn.read()!=Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Failed to find group, group does not exists"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	
}

void myClient::createArticle(){
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
	cout<<"Type in title"<<endl;
	string title;
	cin>>title;
	cout<<"Type in author"<<endl;
	string author;
	cin>>author;
	cout<<"Type in text"<<endl;
	string text;
	cin>>text;
	conn.write(Protocol::COM_CREATE_ART);
	writeNumber(id);
	writeString(title);
	writeString(author);
	writeString(text);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_CREATE_ART){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
	if(conn.read()==Protocol::ANS_ACK){
		cout<<"Article created"<<endl;
	}else if(conn.read()!=Protocol::ERR_NG_DOES_NOT_EXIST){
		cout<<"Failed to find group, group does not exists"<<endl;
	}
	if(conn.read()!=Protocol::ANS_END){
		cerr<<"wrong answer from server"<<endl;
		exit(1);
	}
}
void myClient::deleteArticle(){
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
	writeNumber(ngid);
	writeNumber(artid);
	conn.write(Protocol::COM_END);
	
	if(conn.read()!=Protocol::ANS_DELETE_ART){
		cerr<<"wrong answer from server"<<endl;
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
void myClient::getArticle(){
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
	writeNumber(ngid);
	writeNumber(artid);
	conn.write(Protocol::COM_END);
	
	if(conn.read()==Protocol::ANS_ACK){
		cout<<readString()<<readString()<<readString()<<endl;
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