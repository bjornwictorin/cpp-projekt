#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

class DatabaseInterface{
	
public:
	virtual std::list<Newsgroup> listNewsgroup() const = 0;
	virtual bool createNewsgroup(std::string& title) = 0;
	virtual bool deleteNewsgroup(unsigned int id)= 0;
	virtual std::list<Article> listArticlesInNewsgroup(unsigned int id) const = 0;
	virtual bool createArticle(unsigned int id, std::string title, std::string author, std::string text)= 0;
	virtual bool deleteArticle(unsigned int groupid, unsigned int articleid)= 0;
	virtual Article getArticle(unsigned int groupid, unsigned int articleid) const= 0;
	
};


#endif