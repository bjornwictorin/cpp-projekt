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
	List<Newsgroup> listNewsgroup() const;
	bool createNewsgroup(string& title);
	bool deleteNewsgroup(unsigned int id);
	List<Article> listArticlesInNewsgroup(unsigned int id) const;
	bool createArticle(unsigned int id, string title, string author, string text);
	bool deleteArticle(unsigned int groupid, unsigned int articleid);
	Article getArticle(unsigned int groupid, unsigned int articleid) const;

private:
	map<unsigned int, Newsgroup> newsgroups;
};

int main(){
	cout<<"hello"<<endl;
}


#endif