/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "databaseinterface.h"
//#include "database.h"
#include "diskdatabase.h"
#include "message_handler.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	cout<<"hej";
#if 1
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
#endif
	int port = -1;
	
	//string in;
	//cin >> in;

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
	
	DiskDatabase db;
	MessageHandler handler(db);
	
	while (true) {
		cout<<"before" << endl;
		auto conn = server.waitForActivity();
		cout<<"after" << endl;
		if (conn != nullptr) {
			cout<<"1"<<endl;
			try {
				handler.handleEvent(conn);
				cout<<"3"<<endl;
			} catch (ConnectionClosedException&) {
				cout<<"4"<<endl;
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			cout<<"2"<<endl;
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}

	return EXIT_SUCCESS;
}
