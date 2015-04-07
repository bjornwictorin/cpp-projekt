#include <string>
#include <list>
#include"message_handler.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "databaseinterface.h"
#include <iostream>
#include <memory>


using namespace std;

MessageHandler::MessageHandler(DatabaseInterface& idb) : db(idb) {}

#ifdef NOT_USED
	Message MessageHandler::read(const shared_ptr<Connection>& conn){
	Message msg;
	msg.setMessageType(conn->read);
	unsigned char uc;
	while (uc = conn->read() != Protocol.COM_END) {
		if (Protocol.PAR_STRING == uc) {
			msg.addText(readString());
		} else if (Protocol.PAR_NUM == uc) {
			msg.addInt(readInt());
		}
	}
	return msg;
}
#endif
	

int MessageHandler::readInt(const shared_ptr<Connection>& conn) const{
	conn->read();
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

string MessageHandler::readString(const shared_ptr<Connection>& conn) const{
	int length = readInt(conn);
	cout<<"length" << length<<endl;
	string str;
	char c;
	for (int i = 0; i != length; ++i) {
		cout<<i<<endl;
		c = conn->read();
		str += c;
	}
	return str;
}

void MessageHandler::writeInt(const shared_ptr<Connection>& conn, int n) const {
	conn->write(Protocol::PAR_NUM);
	conn->write(n >> 24);
	conn->write(n >> 16);
	conn->write(n >> 8);
	conn->write(n);
}

void MessageHandler::writeString(const shared_ptr<Connection>& conn, string s) const {
	conn->write(Protocol::PAR_STRING);
	int n = s.size();
	conn->write(n >> 24);
	conn->write(n >> 16);
	conn->write(n >> 8);
	conn->write(n);
	for(char c : s){
		conn->write(c);
	}
}

void MessageHandler::handleEvent(const shared_ptr<Connection>& conn){
	cout<<"inne i messagehandler"<<endl;
	if(conn->isConnected()){
		cout<<"ansluten"<<endl;
	}
	conn->write('x');
	cout<<"skriver x"<<endl;
	int printMe;
	printMe = conn->read();
	cout<<"hej"<<endl;
	cout << printMe << endl;
	switch (printMe){
		case Protocol::COM_LIST_NG:
			listNG(conn);
			break;
		case Protocol::COM_CREATE_NG:
			cout<<"6"<<endl;
			createNG(conn);
			break;
		case Protocol::COM_DELETE_NG:
			deleteNG(conn);
			break;
		case Protocol::COM_LIST_ART:
			listArt(conn);
			break;
		case Protocol::COM_CREATE_ART:
			createArt(conn);
			break;
		case Protocol::COM_DELETE_ART:
			deleteArt(conn);
			break;
		case Protocol::COM_GET_ART:
			getArt(conn);
			break;
	}
}

void MessageHandler::listNG(const shared_ptr<Connection>& conn) const{
	conn->read();
	conn->write(Protocol::ANS_LIST_NG);
	list<Newsgroup> newsgroups = db.listNewsgroup();
	writeInt(conn, newsgroups.size());
	for(Newsgroup& tmp : newsgroups) {
		writeInt(conn, tmp.getNumber());
		writeString(conn, tmp.getName());
	}
	conn->write(Protocol::ANS_END);
}

void MessageHandler::createNG(const shared_ptr<Connection>& conn){
	cout<< "inne i createNG"<<endl;
	string tmp = readString(conn);
	cout<<"7"<<endl;

	conn->read();
	conn->write(Protocol::ANS_CREATE_NG);
	if(db.createNewsgroup(tmp)){
		conn->write(Protocol::ANS_ACK);
	}
	else{
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	conn->write(Protocol::ANS_END);
}

void MessageHandler::deleteNG(const shared_ptr<Connection>& conn){
	int id = readInt(conn);
	conn->read();
	conn->write(Protocol::ANS_DELETE_NG);
	if(db.deleteNewsgroup(id)){
		conn->write(Protocol::ANS_ACK);
	}
	else{
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void MessageHandler::listArt(const shared_ptr<Connection>& conn) const {
	int id = readInt(conn);
	conn->read();
	conn->write(Protocol::ANS_LIST_ART);
	if(db.containsNewsgroup(id)) {
		list<Article> articles = db.listArticlesInNewsgroup(id);
		conn->write(Protocol::ANS_ACK);
		writeInt(conn, articles.size());
		for(Article& a : articles) {
			writeInt(conn, a.getId());
			writeString(conn, a.getTitle());
		}
	}
	else{
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void MessageHandler::createArt(const shared_ptr<Connection>& conn) const {
	int id = readInt(conn);
	string title = readString(conn);
	string author = readString(conn);
	string text = readString(conn);
	conn->read();
	conn->write(Protocol::ANS_CREATE_ART);
	if(db.createArticle(id, title, author, text)){
		conn->write(Protocol::ANS_ACK);
	}
	else{
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void MessageHandler::deleteArt(const shared_ptr<Connection>& conn) const {
	int ng = readInt(conn);
	int art = readInt(conn);
	conn->read();
	conn->write(Protocol::ANS_DELETE_ART);
	if(db.deleteArticle(ng, art)){
		conn->write(Protocol::ANS_ACK);
	}
	else{
		conn->write(Protocol::ANS_NAK);
		if(db.containsNewsgroup(ng)){
			conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		else{
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
	}
	conn->write(Protocol::ANS_END);
}

void MessageHandler::getArt(const shared_ptr<Connection>& conn) const {
	int ng = readInt(conn);
	int art = readInt(conn);
	conn->read();
	conn->write(Protocol::ANS_GET_ART);
	if(db.containsArticle(ng, art)){
		Article a = db.getArticle(ng, art);
		conn->write(Protocol::ANS_ACK);
		writeString(conn, a.getTitle());
		writeString(conn, a.getAuthor());
		writeString(conn, a.getText());
	}
	else{
		conn->write(Protocol::ANS_NAK);
		if(db.containsNewsgroup(ng)){
			conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		else{
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
	}
	conn->write(Protocol::ANS_END);
}