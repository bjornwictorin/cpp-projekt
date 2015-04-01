#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
public:
	Message();
	void setMessageType(int mt);
	void addText(std::string t);
	void addInt(int p1);
	int Message::getMessageType() const;
	std::string Message::getText(int index) const;
	int Message::getInt(int index) const ;
private:
	int messageType;
	std::vector<std::string> textVector;
	std::vector<int> intVector;
};

#endif