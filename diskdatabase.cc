#include<map>
#include<string>
#include<list>
#include<exception>
#include <algorithm>
#include <dirent.h>
#include <memory>
#include "newsgroup.h"
#include "article.h"
#include "databaseinterface.h"
#include "diskdatabase.h"
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <cstring>
#include <cstdlib>


using namespace std;

DiskDatabase::DiskDatabase() : DatabaseInterface(), newsgroups() {
	DIR* dp;
	struct dirent *dirp;
	//create folder if it does not already exist
	
	if((dp = opendir(ngs.c_str())) == nullptr){
		mkdir(ngs.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		cout<<"Ny mapp skapades"<<endl;
	}
	else {
		
		while((dirp = readdir(dp)) != nullptr){
			if(strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, "..")){
				createNewsgroupDisk(dirp);
			}
		}
	}
	closedir(dp);
	
}

void DiskDatabase::createNewsgroupDisk(dirent*& dirp){
		string filename = string(dirp->d_name);
		string title = filename.substr(0, filename.find("-"));
		cout<<"  "+filename<<endl;
		string idnr = filename.substr(title.length()+1);
		cout<<title<<endl;
		int grid = stoi(idnr);
		if(grid>=groupid){
			groupid=grid+1;
		}
		newsgroups.insert(make_pair(groupid, Newsgroup(title, grid)));
		DIR* dp;
		if ((dp = opendir(filename.c_str())) != nullptr) {
		
			while(((dirp = readdir(dp)) != nullptr)) {
				if(strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, "..")){
					ifstream text(dirp->d_name);
					if(text.is_open()){
						size_t s1 = filename.find("-");
						size_t s2 = filename.rfind("-");
						string artTitle = filename.substr(0, s1);
						string artId = filename.substr(s1+1, s2);
						string author = filename.substr(s2+1);
						string artText;
						char c;
						while(text >> c) {
							artText += c;
						}
						int ID = stoi(artId);
						if(ID>=artid){
							artid=ID+1;
						}
						text.close();
						newsgroups.at(grid).createArticle(author, artTitle, artText, ID);
					}
				}
			}
			closedir(dp);
		}else{
			cout<<"fel"<<endl;
		}
	}

list<Newsgroup> DiskDatabase::listNewsgroup() const {
	list<Newsgroup> temp;
	for(auto& kv : newsgroups){
		temp.push_back(kv.second);
	}
	return temp;
}

bool DiskDatabase::createNewsgroup(string& title) {
	for(auto& kv : newsgroups){
		if(kv.second.getName() == title){
			return false;
		}
	}
	newsgroups.insert(make_pair(groupid, Newsgroup(title, groupid)));
	string temp = ngs + "/" + title + "-" + to_string((groupid));
	mkdir(temp.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	++groupid;
	return true;
}

bool DiskDatabase::deleteNewsgroup(unsigned int id) {
	unsigned int temp = newsgroups.size();
	newsgroups.erase(id);
	return newsgroups.size()!=temp;
}

list<Article> DiskDatabase::listArticlesInNewsgroup(unsigned int id) const {
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).getAllArticles();
	}
	list<Article> temp;
	return temp;
}

bool DiskDatabase::createArticle(unsigned int id, string title, string author, string text) {
	if(newsgroups.find(id)!=newsgroups.end()){
		return newsgroups.at(id).createArticle(author, title, text, artid);
	}else{
		return false;
	}
	
}

bool DiskDatabase::deleteArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		return false;
	}
	return newsgroups.at(groupid).deleteArticle(articleid);
}
Article DiskDatabase::getArticle(unsigned int groupid, unsigned int articleid) {
	if(newsgroups.find(groupid)==newsgroups.end()){
		throw out_of_range("could not find article");
	}
	return newsgroups.at(groupid).getArticle(articleid);
}

bool DiskDatabase::containsNewsgroup(unsigned int id) {
	return newsgroups.find(id)!=newsgroups.end();
}

bool DiskDatabase::containsArticle(unsigned int groupid, unsigned int articleid) {
	try{
		getArticle(groupid,articleid);
	}catch(exception& e){
		return false;
	}
	return true;
}
