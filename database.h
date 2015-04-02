#ifndef DATABASE_H
#define DATABASE_H
#include<string>
#include<map>
#include "newsgroup.h"
#include "article.h"
#include "databaseinterface.h"



using namespace std;

static unsigned int groupid = 0;
class Database : public DatabaseInterface {
	
public:
	Database() : DatabaseInterface(){}
	virtual list<Newsgroup> listNewsgroup() const override;
	virtual bool createNewsgroup(std::string& title) override;
	virtual bool deleteNewsgroup(unsigned int id) override;
	virtual list<Article> listArticlesInNewsgroup(unsigned int id) const override;
	virtual bool createArticle(unsigned int id, std::string title, std::string author, std::string text) override;
	virtual bool deleteArticle(unsigned int groupid, unsigned int articleid) override;
	virtual Article getArticle(unsigned int groupid, unsigned int articleid) override;
	virtual bool containsNewsgroup(unsigned int id) override;
	virtual bool containsArticle(unsigned int groupid, unsigned int articleid) override;

private:
	map<unsigned int, Newsgroup> newsgroups;
};

/*
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
	d.deleteArticle(2, 10);
	for(auto a : d.listArticlesInNewsgroup(2)){
		a.print();
	}
	cout<<endl;
	cout<<"Test av get article"<<endl;
	d.getArticle(2,10).print();
}
*/


#endif