#include "database.h"
#include<map>
#include<string>
#include<list>
#include "newsgroup.h"
#include "article.h"

using namspace std;

Database::Database(){}

list<Newsgroup> Database::listNewsgroup() const{
	list<Newsgroup> temp;
	for(auto& kv : newsgroups){
		temp.add(kv.second);
	}
	return temp;
}

bool Database::createNewsgroup(string& title){
	for(auto& kv : newsgroups){
		if(kv.second.getName() == *title){
			return false;
		}
	}
	newsgroups.insert(make_pair(groupid, Newsgroup(title)));
	++groupid;
	return true;
}

bool Database::deleteNewsgroup(unsigned int id){
	int temp = newsgroups.size();
	newsgroup.erase(id);
	return newsgroups.size()!=temp;
}

list<Article> Database::listArticlesInNewsgroup(unsigned int id) const{
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).getAllArticels();
	}	
	return nullptr;
}

bool Database::createArticle(unsigned int id, string title, string author, string text){
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).createArticle(author, title, text);
	}else{
		return false;
	}
	
}

bool Database::deleteArticle(unsigned int groupid, unsigned int articleid){
	if(newsgroups.find(groupid)==newsgroups.end()){
		return false;
	}
	return newsgroups.at(groupid).deleteArticle(articleid);
}

Article Database::getArticle(unsigned int groupid, unsigned int articleid) const{
	if(newsgroups.find(groupid)==newsgroups.end()){
		return nullptr;
	}
	return newsgroups.at(groupid).getArticle(articleid);
}
