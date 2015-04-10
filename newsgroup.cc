

#include <algorithm>
#include <exception>
#include "newsgroup.h"

using namespace std;

Newsgroup::Newsgroup(string name, unsigned int id) : name(name), id(id){}

bool Newsgroup::deleteArticle(int id){
	auto it = articleList.begin();	
	auto end = articleList.end();
	Article a = Article("", "", "", id);
	if(find(it, end, a) != end){
		articleList.remove(*it);
		--count;
		return true;
	}else{
		return false;
	}
}

unsigned int Newsgroup::getNumber()const{
	return id;
}

Article Newsgroup::getArticle(int id) {
	auto it = articleList.begin();
	auto end = articleList.end();
	Article a = Article("", "", "", id);
	auto pos = find(it, end, a);
	if(pos != end) {
		return *pos;
	}else{
		throw out_of_range("could not find article");
	}
}

list<Article> Newsgroup::getAllArticles() const{
	list<Article> temp;
	for(Article a : articleList){
		temp.push_back(a);
	}
	return temp;
}

bool Newsgroup::createArticle(string author, string title, string text){
	articleList.push_back(Article(author, title, text, uniqueid));
	++uniqueid;
	++count;
	return true;
}

void Newsgroup::printAll(){
	for(Article a : articleList){
		a.print();
	}
}

string Newsgroup::getName(){
	return name;
}
