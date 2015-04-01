#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

class MessageHandler {
public:
	Message read(const std::shared_ptr<Connection>& conn);
	bool handleEvent(Message m);
private:
	int MessageHandler::readNumber(const std::shared_ptr<Connection>& conn);
	std::string MessageHandler::readString(const std::shared_ptr<Connection>& conn);
};

#endif