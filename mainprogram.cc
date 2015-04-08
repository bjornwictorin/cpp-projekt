#include "myclient.h"
#include <iostream>
#include "connection.h"
#include <stdexcept>
#include "connectionclosedexception.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc !=3){
		cerr<< "Usage: Usage: myclient host-name port-number" << endl;
		exit(1);
	}
	int portnumber = 0;
	try{
		portnumber = stoi(argv[2]);
	}catch(exception& e){
		cerr<< "Wrong port number"<<endl;
	}
	Connection conn(argv[1], portnumber);
	myClient client;
	if(!conn.isConnected()){
  		cerr<<"Connection attempt failed"<<endl;
		exit(1);
 	}

	int command;
	client.printMenu();
	while(cin >> command){
		cout<<"ska execute command"<<endl;
		client.executeCommand(command, conn);
		client.printMenu();
	}
	
}