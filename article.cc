#include "article.h"
#include<string>


using namespace std;

Article::Article(string author, string title, string text, unsigned int id) : author(author), title(title), text(text) {}//, id(id) {} 

string Article::getAuthor() const{
	return author;
}

string Article::getTitle() const{
	return title;
}

string Article::getText() const{
	return text;
}

 unsigned int Article::getId() const{
	return id;
}

bool Article::operator==(const Article& a){
	return id==a.getId();
}

bool Article::operator!=(const Article& a){
	return !(this==a);
}