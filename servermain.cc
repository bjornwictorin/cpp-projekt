/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "databaseinterface.h"
#include "database.h"
#include "diskdatabase.h"
#include "message_handler.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

/*
*The main program for the server
*/


using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	int port = -1;

	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}
	
	Database db;
	MessageHandler handler(db);
	
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				handler.handleEvent(conn);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}

	return EXIT_SUCCESS;
}
