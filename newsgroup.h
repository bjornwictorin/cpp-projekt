#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <list>
#include "article.h"

static unsigned int uniqueid = 0;
class Newsgroup{
public:
	Newsgroup(string name);
	bool deleteArticle(int id);
	Article getArticle(int id);
	std::list<Article> getAllArticles();
	bool createArticle(std::string author, std::string title, std::string text);
	void printAll();
	std::string getName();
private:
	std::list<Article> articleList;
	int count;
	std::string name;
};

int main(){
	Newsgroup n("Test");
	n.createArticle("Tom", "HeHE", "Folk kodar i källaren");
	n.createArticle("marc", "HeHE", "Folk i källaren");
	n.createArticle("Tom", "HeHE", "Folk kodar i källaren");
	n.deleteArticle(0);
	n.printAll();
	cout<<n.getName()<<endl;
	for(auto a : n.getAllArticles()){
	a.print();
	}
}
#endif
