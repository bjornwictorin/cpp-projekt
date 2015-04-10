#include "myclient.h"
#include <iostream>
#include "connection.h"
#include <stdexcept>
#include "connectionclosedexception.h"

/*
*The main program for the client
*/


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

	string command;
	int com = 0;
	client.printMenu();
	while(cin >> command){
		try{
		com = stoi(command);
		client.executeCommand(com, conn);
		client.printMenu();
	}catch(exception& e){
		cout<<"Only numbers between 1-8 are allowed"<<endl;
	}
		
	}
	
}