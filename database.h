#ifndef DATABASE_H
#define DATABASE_H
#include<string>
#include<map>
#include "newsgroup.h"
#include "article.h"



using namespace std;

static unsigned int groupid = 0;
class Database{
	
public:
	Database();
	list<Newsgroup> listNewsgroup() const;
	bool createNewsgroup(string title);
	bool deleteNewsgroup(unsigned int id);
	list<Article> listArticlesInNewsgroup(unsigned int id) const;
	bool createArticle(unsigned int id, string title, string author, string text);
	bool deleteArticle(unsigned int groupid, unsigned int articleid);
	Article getArticle(unsigned int groupid, unsigned int articleid);

private:
	map<unsigned int, Newsgroup> newsgroups;
};

int main(){
	Database d;
	
	d.createNewsgroup("E-sport");
	d.createNewsgroup("Sport");
	cout<<"Test av create NG"<<endl;
	for(auto a : d.listNewsgroup()){
	cout<<a.getName()<<endl;	
	}
	
	cout<<endl;
	cout<<"Test av delete NG"<<endl;
	d.createNewsgroup("nyheter");
	d.deleteNewsgroup(1);
	for(auto a : d.listNewsgroup()){
	cout<<a.getName()<<endl;	
	}
	
	cout<<endl;
	cout<<"Test av create article"<<endl;
	d.createArticle(2, "Hej", "marc", "text");
	d.createArticle(2, "Hej", "tom", "text");
	d.createArticle(0, "Hej", "marc", "text");
	for(auto a : d.listArticlesInNewsgroup(2)){
		a.print();
	}
	
	cout<<endl;
	cout<<"Test av delete article"<<endl;
	d.deleteArticle(2, 1);
	for(auto a : d.listArticlesInNewsgroup(2)){
		a.print();
	}
	cout<<endl;
	cout<<"Test av get article"<<endl;
	d.getArticle(2,1).print();
}


#endif