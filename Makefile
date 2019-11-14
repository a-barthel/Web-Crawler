
all: gethttp webcrawler git-commit

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp

webcrawler: webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o webcrawler -g webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp -std=c++11

git-commit:
	git add Makefile *.h *.cpp  >> .local.git.out
	git commit -a -m "Commit web-crawler" >> .local.git.out

clean:
	rm -f *.o *.txt gethttp webcrawler core
