#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H
#include "databaseinterface.h"
#include "connection.h"

#include <memory>
#include <string>

/*
*Class handling the commands from the server and containing the database
*/


class MessageHandler {
public:
	MessageHandler(DatabaseInterface& idb);
	void handleEvent(const shared_ptr<Connection>& conn);
private:
	int readInt(const std::shared_ptr<Connection>& conn) const ;
	std::string readString(const std::shared_ptr<Connection>& conn) const;
	void writeInt(const std::shared_ptr<Connection>& conn, int n) const;
	void writeString(const std::shared_ptr<Connection>& conn, string s) const;
	void listNG(const std::shared_ptr<Connection>& conn) const;
	void createNG(const std::shared_ptr<Connection>& conn);
	void deleteNG(const std::shared_ptr<Connection>& conn);
	void listArt(const std::shared_ptr<Connection>& conn) const;
	void createArt(const std::shared_ptr<Connection>& conn) const;
	void deleteArt(const std::shared_ptr<Connection>& conn) const;
	void getArt(const std::shared_ptr<Connection>& conn) const;
	DatabaseInterface& db;
};

#endif