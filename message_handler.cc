#include <string>
#include <list>
#include"message_handler.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "databaseinterface.h"
#include <memory>


using namespace std;

MessageHandler::MessageHandler(DatabaseInterface idb) : db(idb) {}

#ifdef NOT_USED
	Message MessageHandler::read(const shared_ptr<Connection>& conn){
	Message msg;
	msg.setMessageType(conn->read);
	unsigned char uc;
	while (uc = conn->read() != Protocol.COM_END) {
		if (Protocol.PAR_STRING == uc) {
			msg.addText(readString());
		} else if (Protocol.PAR_NUM == uc) {
			msg.addInt(readNumber());
		}
	}
	return msg;
}
#endif
	

int MessageHandler::readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

string MessageHandler::readString(const shared_ptr<Connection>& conn) {
	int length = readNumber(conn);
	string str;
	for (int i = 0; i != length; ++i) {
		str += conn->read();
	}
	return str;
}

void MessageHandler::writeInt(const shared_ptr<Connection>& conn, int n) const {
	conn->write(Protocol.PAR_NUM);
	conn->write(n >> 24);
	conn->write(n >> 16);
	conn->write(n >> 8);
	conn->write(n);
}

void MessageHandler::writeString(const shared_ptr<Connection>& conn, string s) const {
	conn->write(Protocol.PAR_STRING);
	writeInt(s.size());
	for(char c : s){
		conn->write(c);
	}
}

bool MessageHandler::handleEvent(const shared_ptr<Connection>& conn){
	switch (conn->read()){
		case Protocol.COM_LIST_NG:
			listNG();
			break;
		case Protocol.COM_CREATE_NG:
			createNG();
			break;
		case Protocol.COM_DELETE_NG:
			deleteNG();
			break;
		case Protocol.COM_LIST_ART:
			listArt();
			break;
		case Protocol.COM_CREATE_ART:
			createArt();
			break;
		case Protocol.COM_DELETE_ART:
			deleteArt();
			break;
		case Protocol.COM_GET_ART:
			getArt();
			break;
	}
}

void MessageHandler::listNG(const shared_ptr<Connection>& conn) const{
	conn->read();
	conn->write(Protocol.ANS_LIST_NG);
	list<Newsgroup> newsgroups = db.listNewsgroup();
	writeInt(newsgroups.length());
	for(Newsgroup& tmp : newsgroups) {
		writeInt(tmp.getNumber());
		writeString(tmp.getName());
	}
	conn->write(Protocol.ANS_END);
}

void MessageHandler::createNG(const shared_ptr<Connection>& conn){
	string tmp = readString(conn);
	conn->read();
	conn->write(Protocol.ANS_CREATE_NG);
	if(db.createNewsgroup(tmp)){
		conn->write(Protocol.ANS_ACK);
	}
	else{
		conn->write(Protocol.ANS_NAK);
		conn->write(Protocol.ERR_NG_ALREADY_EXISTS);
	}
	conn->write(Protocol.ANS_END);
}

void MessageHandler::deleteNG(const shared_ptr<Connection>& conn){
	int id = readInt();
	conn->read();
	conn->write(Protocol.ANS_DELETE_NG);
	if(db.deleteNewsgroup(id)){
		conn->write(Protocol.ANS_ACK);
	}
	else{
		conn->write(Protocol.ANS_NAK);
		conn->write(Protocol. ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol.ANS_END);
}

void MessageHandler::listArt(const shared_ptr<Connection>& conn) const {
	int id = readInt();
	conn->read();
	conn->write(Protocol.ANS_LIST_ART);
	if(db.containsNewsgroup()) {
		list<Article> articles = db.listArticlesInNewsgroup();
		writeInt(articles.size());
		for(Article& a : articles) {
			writeInt(a.getId());
			writeString(a.getTitle());
		}
	}
	else{
		conn->write(Protocol.ANS_NAK);
		conn->write(Protocol.ERR_NG_DOES_NOT_EXIST);
	}
	conn->writeProtocol.ANS_END);
}

void MessageHandler::createArt(const shared_ptr<Connection>& conn) const {
	int id = readInt();
	string title = readString();
	string author = readString();
	string text = readString();
	conn->read();
	conn->write(Protocol.ANS_CREATE_ART);
	if(db.createArticle(title, author, text)){
		conn->write(Protocol.ANS_ACK);
	}
	else{
		conn->write(Protocol.ANS_NAK);
		conn->write(Protocol.ERR_NG_DOES_NOT_EXIST);
	}
	conn->writeProtocol.ANS_END);
}

void MessageHandler::deleteArt(const shared_ptr<Connection>& conn) const {
	int ng = readInt();
	int art = readInt();
	conn->read();
	conn->write(Protocol.ANS_DELETE_ART);
	if(db.deleteArticle(nd, art)){
		conn->write(Protocol.ANS_ACK);
	}
	else{
		conn->write(Protocol.ANS_NAK);
		if(db.containsNewsgroup(ng)){
			conn->write(Protocol.ERR_ART_DOES_NOT_EXIST);
		}
		else{
			conn->write(Protocol.ERR_NG_DOES_NOT_EXIST);
		}
	}
	conn->writeProtocol.ANS_END);
}

void MessageHandler::getArt(const shared_ptr<Connection>& conn) const {
	int ng = readInt();
	int art = readInt();
	conn->read();
	conn->write(Protocol.ANS_GET_ART);
	if(db.containsArticle(ng, art)){
		Article a = db.getArticle();
		conn->write(Protocol.ANS_ACK);
		writeString(a.getTitle());
		writeString(a.getAuthor());
		writeString(a.getText());
	}
	else{
		conn->write(Protocol.ANS_NAK);
		if(db.containsNewsgroup(ng)){
			conn->write(Protocol.ERR_ART_DOES_NOT_EXIST);
		}
		else{
			conn->write(Protocol.ERR_NG_DOES_NOT_EXIST);
		}
	}
	conn->writeProtocol.ANS_END);
}