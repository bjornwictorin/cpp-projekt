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
#include <unistd.h>



using namespace std;

DiskDatabase::DiskDatabase() : DatabaseInterface(), newsgroups() {
	DIR* dp;
	struct dirent *dirp;
	//create folder if it does not already exist
	
	if((dp = opendir(rootfolder.c_str())) == nullptr){
		mkdir(rootfolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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
		string filenameNoRoot = string(dirp->d_name);
		string filename = rootfolder +"/"+ filenameNoRoot;
		string title = filenameNoRoot.substr(0, filenameNoRoot.find("-"));
		string idnr = filenameNoRoot.substr(title.length()+1);
		unsigned int grid = stoi(idnr);
		if(grid>=groupid){
			groupid=grid+1;
		}
		newsgroups.insert(make_pair(grid, Newsgroup(title, grid)));
		DIR* dp;
		if ((dp = opendir(filename.c_str())) != nullptr) {
		
			while(((dirp = readdir(dp)) != nullptr)) {
				if(strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, "..")){
					ifstream text(filename + "/"+ dirp->d_name);
					if(text.is_open()){
						string fname = string(dirp->d_name);
						cout<<fname<<endl;
						size_t s1 = fname.find("-");
						size_t s2 = fname.rfind("-");
						string artTitle = fname.substr(0, s1);
						string author = fname.substr(s1+1, s2-s1-1);
						string artId = fname.substr(s2+1);
						string artText;
						char c;
						while(text >> c) {
							artText += c;
						}
						unsigned int ID = stoi(artId);
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
	string temp = rootfolder +"/"+ title + "-" + to_string((groupid));
	mkdir(temp.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	++groupid;
	return true;
}

bool DiskDatabase::deleteNewsgroup(unsigned int id) {
	unsigned int temp = newsgroups.size();
	string foldername = rootfolder;
	try{
		Newsgroup ng = newsgroups.at(id);
		foldername += "/" + ng.getName();
		foldername+="-"+to_string(id);
		
	}catch(out_of_range e){
		return false;
	}
	newsgroups.erase(id);
	if(newsgroups.size()!=temp){
		DIR* dp;
		struct dirent* ep;
		dp = opendir(foldername.c_str());
		if(dp!=nullptr){
			while((ep = readdir(dp)) != nullptr){
				if(strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")){
					string fileToRemove = foldername + "/" + string(ep->d_name);
					remove(fileToRemove.c_str());
				}
			}
			closedir(dp);
			cout<<foldername<<endl;
			rmdir(foldername.c_str());
			return true;
		}	
	}
	return false;
	
	
	
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
		string foldername = rootfolder;
		try{
			Newsgroup ng = newsgroups.at(id);
			foldername += "/" + ng.getName();
			foldername+="-"+to_string(id);
			
		}catch(out_of_range e){
			return false;
		}
		DIR* dp;
		dp = opendir(foldername.c_str());
		if(dp!=nullptr){
			string filename = foldername + "/"+ title +"-" + author +"-" + to_string(artid);
			ofstream o;
			o.open(filename);
			o << text;
			o.close();
			closedir(dp);
			cout<<filename<<endl;
			return newsgroups.at(id).createArticle(author, title, text, artid++);
		}
			
		
	}
	return false;
	
}

bool DiskDatabase::deleteArticle(unsigned int groupidparam, unsigned int articleid) {
	if(newsgroups.find(groupidparam)==newsgroups.end()){
		return false;
	}
	
	string foldername = rootfolder;
	string filename;

	try{
		Newsgroup ng = newsgroups.at(groupidparam);
		foldername += "/" + ng.getName();
		foldername+="-"+to_string(groupidparam);
		Article temp = getArticle(groupidparam, articleid);
		filename = temp.getTitle()+"-"+temp.getAuthor()+"-"+to_string(temp.getId());
		
	}catch(out_of_range e){
		return false;
	}
	DIR* dp;
	dp = opendir(foldername.c_str());
	if(dp!=nullptr){
		string fileToRemove = foldername + "/" + filename;
		remove(fileToRemove.c_str());
		closedir(dp);
		cout<<fileToRemove<<endl;
	}
	return newsgroups.at(groupidparam).deleteArticle(articleid);
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
