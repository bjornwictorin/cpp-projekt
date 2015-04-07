#include "myclient.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
	myClient client(argc, argv);
#if 0
	int command;
	client.printMenu();
	while(cin >> command){
		cout<<"ska execute command"<<endl;
		client.executeCommand(command);
		client.printMenu();
	}
	
#else 
	for(;;)
		;
#endif
}