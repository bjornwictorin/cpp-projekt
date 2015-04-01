

#include <Algorithm>
#include "newsgroup.h"

using namespace std;

NewsGroup::NewsGroup(string name) : name(name){}

bool deleteArticle(int id){
	auto it = articleList.begin();	
	auto end = articleList.end();
	Article a = Article("", "", "", id);
	if(find(it, end, a) != end){
		articleList.remove(articleList.get(it));
		--count;
		return true;
	}else{
		return false;
	}
}

Article get_article(int id){
	auto it = article_list.begin();
	auto end = article_list.end();
	Article a = Article("", "", "", id);
	if(find(it, end, a) != end){
		return article_list.get(it);
	}else{
		return null;
	}
}

bool createArticle(string author, string title, string text){
	articleList.add(Article(author, title, text, uniqueid));
	++uniqueid;
	++count;
	return true;
}

void printAll(){
	for(Article a : articleList){
		a.print();
	}
}
