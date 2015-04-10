#ifndef ARTICLE_H
#define ARTICLE_H
#include<string>
#include<iostream>
/*
*Class describing an article 
*/


using namespace std;
class Article {
	
public:
	Article(string author, string title, string text, unsigned int id);
	string getAuthor() const;
	string getTitle() const;
	string getText() const;
	unsigned int getId() const;
	bool operator!=(const Article& a) ;
	bool operator==(const Article& a) ;
	void print();
	
private: 
	string author;
	string title;
	string text;
	unsigned int id;
	
};


#endif
