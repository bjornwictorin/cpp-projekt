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
	Article editArticle(Article a);
	Article getArticle(int id);	
	bool createArticle(Article a);
	void printAll();
private:
	List<Article> articleList;
	int count;
	std::string name;
};

#endif
