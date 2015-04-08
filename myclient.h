#ifndef MYCLIENT_H
#define MYCLIENT_H
#include "protocol.h"
#include "connection.h"
#include<string>
#include<iostream>
#include <memory>


class myClient{
	
public: 
	//myClient();
	void executeCommand(int command, const Connection& conn);
	void printMenu() const;
private:
	int readNumber(const Connection& conn);
	int readPureInt(const Connection& conn);
	void writeNumber(int number, const Connection& conn);
	std::string readString(const Connection& conn);
	void writeString(std::string s, const Connection& conn);
	void listNewsGroup(const Connection& conn);
	void createNewsGroup(const Connection& conn);
	void deleteNewsGroup(const Connection& conn);
	void listArticleInNewsGroup(const Connection& conn);
	void createArticle(const Connection& conn);
	void deleteArticle(const Connection& conn);
	void getArticle(const Connection& conn);
	
};

/*
int main(){
	std::cout<<"hej"<<std::endl;
}
*/
#endif