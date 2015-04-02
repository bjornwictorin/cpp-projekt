#ifndef MYCLIENT_H
#define MYCLIENT_H
#include "protocol.h"
#include "connection.h"
#include<string>
#include<iostream>


class myClient{
	
public: 
	myClient(int argc, const char* argv[]);
	void executeCommand(int command);
	void printMenu() const;
private:
	Connection conn;
	int readNumber();
	void writeNumber(int number);
	std::string readString();
	void writeString(std::string s);
	void listNewsGroup();
	void createNewsGroup();
	void deleteNewsGroup();
	void listArticleInNewsGroup();
	void createArticle();
	void deleteArticle();
	void getArticle();
	
};

int main(){
	std::cout<<"hej"<<std::endl;
}

#endif