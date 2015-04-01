#ifndef ARTICLE_H
#define ARTICLE_H
#include<string>
#include<iostream>
/*
*Klass som håller de olika artiklarna
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

int main(){
	Article a("marc", "hej", "text", 1);
	Article b("marc", "hej", "text", 2);
	Article c("a", "b", "x", 1);
	cout<<a.getTitle()<<endl;
	cout<<b.getId()<<endl;
	cout<<a.getId()<<endl;
	if(a==c){
		cout<<"1"<<endl;
	}
	if(a==b){
		cout<<"2"<<endl;
	}
	if(a==a){
		cout<<"rätt"<<endl;
	}
}
#endif
