#include "diskdatabase.h"
#include <map>
#include <string>
#include <list>
#include <exception>
#include <algorithm>
#include <iostream>
#include "newsgroup.h"
#include "article.h"
#include "databaseinterface.h"


using namespace std;


list<Newsgroup> Diskdatabase::listNewsgroup() const {
	list<Newsgroup> temp;
	for(auto& kv : newsgroups){
		temp.push_back(kv.second);
	}
	return temp;
}

bool Diskdatabase::createNewsgroup(string& title) {
	char *myDir = dirname(title);
	struct stat myStat;
	
	//Checking if directory already exists.
	if ((stat(myDir, &myStat) == 0) && (((myStat.st_mode) & S_IFMT) == S_IFDIR)) {
  		return false;
	}
	mkdir("/data/" + title);
	++groupid;
	return true;
}

bool Diskdatabase::deleteNewsgroup(unsigned int id) {
	unsigned int temp = newsgroups.size();
	newsgroups.erase(id);
	return newsgroups.size()!=temp;
}

list<Article> Diskdatabase::listArticlesInNewsgroup(unsigned int id) const {
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).getAllArticles();
	}
	list<Article> temp;
	return temp;
}

bool Diskdatabase::createArticle(unsigned int id, string title, string author, string text) {
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).createArticle(author, title, text);
	}else{
		return false;
	}
	
}

bool Diskdatabase::deleteArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		return false;
	}
	return newsgroups.at(groupid).deleteArticle(articleid);
}
Article Diskdatabase::getArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		throw out_of_range("could not find article");
	}
	return newsgroups.at(groupid).getArticle(articleid);
}

bool Diskdatabase::containsNewsgroup(unsigned int id) {
	return newsgroups.find(id)!=newsgroups.end();
}

bool Diskdatabase::containsArticle(unsigned int groupid, unsigned int articleid) {
	try{
		getArticle(groupid,articleid);
	}catch(exception& e){
		return false;
	}
	return true;
}
