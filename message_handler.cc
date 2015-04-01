#include <string>
#include"message_handler.h"
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

using namespace std;

MessageHandler::read(const shared_ptr<Connection>& conn){
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
	
}

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

bool MessageHandler::handleEvent(Message msg){
	switch (msg.getMessageType()){
		case Protocol.COM_LIST_NG:
			break;
		case Protocol.COM_CREATE_NG:
			break;
		case Protocol.COM_DELETE_NG:
			break;
		case Protocol.COM_LIST_ART:
			break;
		case Protocol.COM_CREATE_ART:
			break;
		case Protocol.COM_DELETE_ART:
			break;
		case Protocol.COM_GET_ART:
			break;
	}
}