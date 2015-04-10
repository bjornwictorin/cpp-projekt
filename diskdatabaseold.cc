#include <map>
#include <string>
#include <list>
#include <exception>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include "newsgroup.h"
#include "article.h"
#include "databaseinterface.h"
#include "diskdatabase.h"


using namespace std;

// Filling newsgroups
/******************************************************************************
 * Function: Diskdatabase()
 * Reason: Constuctor, initializes the database from the saved files.
 *****************************************************************************/
Diskdatabase::Diskdatabase() : DatabaseInterface(){
	DIR *pDIR;
	struct dirent *entry;
	if( pDIR=opendir("/data") ){
		while(entry = readdir(pDIR)){
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
			
				// Finding the location of "-" then creating substrings
				// to and from it and adding to pairs.
				size_t pos = str.find("-");	
				string id = entry->d_name.substr(0, pos);
				string name = entry->d_name.substr(pos+1);
				Newsgroup n(name, id);

				newsgroups.insert(make_pair(id, Newsgroup(name, id)));
				++groupid;
			}
			closedir(pDIR);
		}
	}
	for(auto& kv : newsgroups){
		if(pDIR=opendir("/data/" + kv.first + "-" + kv.second.getName())){
			while(entry = readdir(pDIR)){
				filename = entry->d_name;
				ifstream infile(filename);
				string author;
				string title;
				string text;
				char c;
				
// HÄR BEHÖVER INFILE PLOCKA UT RESPEKTIVE INFO FÖR AUTHOR, TITLE OCH TEXT				
				
				while(infile.get
				n.createArticle(author, title, text);
			}
		}
	}
}



// The same as original database.cc
list<Newsgroup> Diskdatabase::listNewsgroup() const {

	list<Newsgroup> temp;
	for(auto& kv : newsgroups){
		temp.push_back(kv.second);
	}

	return temp;
}


// Checks if newsgroup exists. If not, adding to Map and Directory.
bool Diskdatabase::createNewsgroup(string& title) {
	for(auto& kv : newsgroups){
		if(kv.second.getName() == title){
			return false;
		}
	}
	
	newsgroups.insert(make_pair(groupid, Newsgroup(title, groupid)));
	int added = mkdir("/data/" + groupid + "-" + title);
	++groupid;
	
	return added == 0;
}


// Removes newsgroup from Map and Directory. rmdir returns 0 on success.
bool Diskdatabase::deleteNewsgroup(unsigned int id) {
	unsigned int temp = newsgroups.size();
	
	int removed = rmdir(newsgroups.at(id).first + "-" + newsgroups.at(id).second.getName());
	newsgroups.erase(id);
	
	return newsgroups.size()!=temp && removed == 0;
}


// Same as before.
list<Article> Diskdatabase::listArticlesInNewsgroup(unsigned int id) const {
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).getAllArticles();
	}
	list<Article> temp;
	return temp;
}


bool Diskdatabase::createArticle(unsigned int id, string title, string author, string text) {
	if(newsgroups.find(id)!=newsgroups.end()){
	
// HÄR BEHÖVER VI SKRIVA INFO TILL EN FIL MED ADRESS /data/ngid-ngname/articleid.txt
// LÄGG TILL EN SUCCESS CHECK TILL RETURN.
		return newsgroups.at(id).createArticle(author, title, text);
	}else{
		return false;
	}
	
}


// Removes article from newsgroup. remove returns 0 on success.
bool Diskdatabase::deleteArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		return false;
	}
	
	int removed = remove("/data/" + newsgroups.at(groupid).first + "-" + newsgroups.at(groupid).second.getName() + "/" + articleid + ".txt");
	return newsgroups.at(groupid).deleteArticle(articleid) && removed == 0;
}


// Same as before.
Article Diskdatabase::getArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		throw out_of_range("could not find article");
	}
	return newsgroups.at(groupid).getArticle(articleid);
}


// Same as before.
bool Diskdatabase::containsNewsgroup(unsigned int id) {
	return newsgroups.find(id)!=newsgroups.end();
}


// Same as before.
bool Diskdatabase::containsArticle(unsigned int groupid, unsigned int articleid) {
	try{
		getArticle(groupid,articleid);
	}catch(exception& e){
		return false;
	}
	return true;
}
