#include <string>
#include "message.h"

using namespace std;


void Message::setMessageType(int mt){
	messageType = mt;
}

void Message::addText(string t){
	textVector.push_back(t);
}

void Message::addInt(int p1){
	intVector.push_back(p1);
}

int Message::getMessageType() const {
	return messageType;
}

string Message::getText(int index) const {
	return textVector[index];
}

int Message::getInt(int index) const {
	return intVector[index];
}