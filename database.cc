#include "database.h"
#include<map>
#include<string>
#include<list>
#include<exception>
#include <algorithm>
#include "newsgroup.h"
#include "article.h"
#include "databaseinterface.h"

using namespace std;



list<Newsgroup> Database::listNewsgroup() const {
	list<Newsgroup> temp;
	for(auto& kv : newsgroups){
		temp.push_back(kv.second);
	}
	return temp;
}

bool Database::createNewsgroup(string& title) {
	for(auto& kv : newsgroups){
		if(kv.second.getName() == title){
			return false;
		}
	}
	newsgroups.insert(make_pair(groupid, Newsgroup(title, groupid)));
	++groupid;
	return true;
}

bool Database::deleteNewsgroup(unsigned int id) {
	unsigned int temp = newsgroups.size();
	newsgroups.erase(id);
	return newsgroups.size()!=temp;
}

list<Article> Database::listArticlesInNewsgroup(unsigned int id) const {
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).getAllArticles();
	}
	list<Article> temp;
	return temp;
}

bool Database::createArticle(unsigned int id, string title, string author, string text) {
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).createArticle(author, title, text, artid++);
	}else{
		return false;
	}
	
}

bool Database::deleteArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		return false;
	}
	return newsgroups.at(groupid).deleteArticle(articleid);
}

Article Database::getArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		throw out_of_range("could not find article");
	}
	return newsgroups.at(groupid).getArticle(articleid);
}

bool Database::containsNewsgroup(unsigned int id) {
	return newsgroups.find(id)!=newsgroups.end();
}

bool Database::containsArticle(unsigned int groupid, unsigned int articleid) {
	try{
		getArticle(groupid,articleid);
	}catch(exception& e){
		return false;
	}
	return true;
}
