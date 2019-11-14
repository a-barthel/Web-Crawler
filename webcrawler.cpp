//
//---------------------------------- AUTHOR ----------------------------------------
//
//
// Andrew Walker Barthel
// Summer 2016 - CS251 - Data Structures and Algorithms
// Gustavo Rodriguez-Rivera
// Lab 2 - Web Crawler Project
// abarthe@purdue.edu
// Purdue University - Class of 2018 
//
//
//----------------------------------------------------------------------------------
//

//
//------------------------------ INCLUDE HEADERS -----------------------------------
//

#include "webcrawler.h" // webcrawler.h includes several more headers used here,
			// for more information on libraries used see man pages
			// of the functions used and webcrawler.h.
#include <cstring>
int desFlag = 0;

//
//----------------------------------------------------------------------------------
//

//
// Print how the program expects to be called.  This method will 
// act as a redirect of stderr to the terminal of the exact
// format that the program expects to be called in.  If this 
// method got called, the user did not pass arguments correctly.
//
static void printUsage() {

	// The user called the program incorrectly. State the correct way with stderr.
	fprintf(stderr, "\n USAGE : \n\n\t./webcrawler [-u <maxUrls>] <url-list> \n\n"); 
	fprintf(stderr, " EXAMPLES : \n\n\t./webcrawler -u 100 http://www.purdue.edu\n");
	fprintf(stderr, "\t./webcrawler http://www.purdue.edu thedoors.com www.ledzeppelin.com\n");
	fprintf(stderr, "\t./webcrawler -u 25 www.purdue.edu nba.com/grizzlies www.memphistn.gov/\n\n");
	fprintf(stderr, " NOTE : \n\n\tIf not passed, <maxUrls> is set to 1000 by default.\n");
	fprintf(stderr, "\tThe URL's in <url-list> do NOT have to have \"http://\"\n");
	fprintf(stderr, "\tprepended to the string, or \"www\", for that matter.\n");
	fprintf(stderr, "\tThe format of <url-list>, should also be a list of n\n");
	fprintf(stderr, "\tstrings representing URL's.  This list should have \n");
	fprintf(stderr, "\teach of the n entries separated by a space.\n\t\t");
	fprintf(stderr, "SYNTAX : <url-list[0]> <url-list[1]> .... <url-list[n-1]>\n\t\t");
	fprintf(stderr, "EXAMPLE : http://purdue.edu thedoors.com www.ledzeppelin.com\n\n");
	// Purdue because I fucking love this place, the others..well..You gotta know me ;);

}

//
// Creates the description for websites.  This method is overloaded
// from SimpleHTMLParser.  This method will be rewritten to better
// parse HTML pages and select eligible words to be added to the 
// description of the HTML page. It will be limited to 500 chars. 
// The method will also set this description to the URLRecord struct
// for each instance in the _urlArray, URLRecordList, and also add
// words to the _wordToURLRecordList and insert those items.
//
// EDIT: Description is set in the crawl() method after parse() is
// called, that way we are sure the full description is set to its'
// respective URL.
//
void WebCrawler::onContentFound(char c) {
	if(c=='>') return;	
	// This method is going to have a major renovation.
	// It will keep track of both the 500 chars for 
	// description as well as words. It will then insert
	// each word into _wordToURLRecordList.
	
	// Check description flag && clear if needed.
	if (desF == 0) {
		
		// Description needs to be cleared/initialized.
		if (des == NULL) {
			
			// Description needs to be initialized.
			char* tempDes = new char[1];
			tempDes[0] = 0;
			des = strdup(tempDes);
			delete [] tempDes; // NO MEMORY LEAKS!

		} else {
			
			// Description needs to be cleared.
			free(des); // NO MEMORY LEAKS! Clears des so we can make space
				   // for new one. 
			char* clear = new char[1];
			clear[0] = 0;
			des = strdup(clear);
			delete [] clear; // NO MEMORY LEAKS!

		}

		// Reset Flag
		desF = 1;

	}

	// Check word flag && clear if needed.
	if (wordF == 0) {
		
		// Word needs to be cleared/initialized.
		if (word == NULL) {
			
			// Word needs to be initialized.
			char* tempWord = new char[1];
			tempWord[0] = 0;
			word = strdup(tempWord);
			delete [] tempWord; //NO MEMORY LEAKS!

		} else {
			
			// Word needs to be cleared.
			free(word); // NO MEMORY LEAKS! Clears word so we can make space 
				    // for new one.
			char* clear = new char[1];
			clear[0] = 0;
			word = strdup(clear);
			delete [] clear; // NO MEMORY LEAKS!
			
		}

		// Reset Flag
		wordF = 1;

	}
	
	// Check if description is under 500 chars.
	// The actual length of des will be 501 as 
	// a max for the '\0' char at the end.
	if (strlen(des) < 500 && c != '}') {
		
		// Description is under 500 chars. 
		// Append char c at the end of des.
		if (c==' '&&des[strlen(des)-1]==' ') goto SKIP;
		char* buffer = new char[strlen(des) + 2];
		strcpy(buffer, des);
		buffer[strlen(des)] = c;
		buffer[strlen(des)+1] = 0;
		free(des);
		des = strdup((const char*)buffer);
		delete [] buffer; // NO MEMORY LEAKS!

	} 
	if (strlen(des) >= 500 && !desFlag) {
		char* buffer = new char[strlen(des)+4];
		strcpy(buffer, des);
		buffer[strlen(des)] = '.';
		buffer[strlen(des)+1] = '.';
		buffer[strlen(des)+2] = '.';
		buffer[strlen(des)+3] = 0;
		free(des);
		des = strdup((const char*) buffer);
		delete[] buffer;
		desFlag = 1;
	}
	SKIP:
	// Check if the word is continueing.
	// Only checks for numbers and alpha characters. Possibly add special chars later.
	if (/*(48 <= c && c <= 57) || (65 <= c && c <= 90) || (97 <= c && c <= 122)*/!isspace(c) ) {
		
		// Word still going on. Append char to word.
		char* buffer = new char[strlen(word)+2];
		strcpy(buffer, word);
		buffer[strlen(word)] = c;
		buffer[strlen(word)+1] = 0;
		free(word);
		word = strdup((const char*)buffer);
		delete [] buffer; // NO MEMORY LEAKS!

	} else {

		// Check if the case were two non-word chars in a row.
		if (word == NULL || strlen(word) == 0 || *word == ' ') return;

		// Create URLRecordList to search/catch data in 
		// _wordToURLRecordList if it finds word.
		URLRecordList* catcher;
		URLRecordList* insert = new URLRecordList();
		// Insert word into _wordToURLRecordList
		if (_wordToURLRecordList->find(word, &catcher) == false) {
			
			// Not in _wordToURLRecordList. Insert URLRecordList.
			insert->_urlRecordIndex = _headURL;
			insert->_next = NULL;
			_wordToURLRecordList->insertItem(word, insert);

		} else {
			
			// Found. Insert URLRecordList with caught data.
			insert->_urlRecordIndex = _headURL;
			insert->_next = catcher;
			_wordToURLRecordList->insertItem(word, insert);

		}

		// Can't free insert since in HashTableTemplate.h: insertItem() 
		// sets data to the pointer passed as the argument - Eventually 
		// fix this issue so we have more stable data structure && we 
		// can delete URLRecordList* insert when it is no longer needed.

		// Update Flag.
		wordF = 0;

	}

}

//
// Creates the list of hyperlinks for websites.  This method is
// overloaded from SimpleHTMLParser. This method will be 
// overwritten to better find eligible hyperlinks while it parses 
// an HTML pages for anchor tags. This is also where the URLs 
// get added onto the _urlArray and the _tailURL is incremented.
//
void WebCrawler::onAnchorFound(char* url) {

	// This method is going to have a major renovation.
	// It will do some checking for valid HTTP URL.
	// It will find all hyperlinks in the HTML code and
	// insert them in _urlArray & _urlToUrlRecord iff
	// not already in _urlToUrlRecord & up to _maxUrls.
	
	// Check if we can insert more. If not - return. Even
	// checks if _nurl got really fucked up.
	if (_maxUrls == _nurl || _maxUrls < _nurl) return;

	// Flag to free url at end of method.
	// 0 == Don't free, 1 == free!
	int urlF = 0;

	// The only reason this if statement is here, not
	// after declaration of finalURL and not just
	// setting finalURL to strdup, is : right now, I
	// never have the possibility of setting finalURL
	// twice, therefore, don't have to keep a flag
	// to free it nor even call free until the end.
	// I will take one flag, if statement, and free
	// over one flag, four if statements, and four
	// frees.
	// Postpend "/" to url if not at the end.
	if (url[strlen(url)] != 47) {
			
		// Append "/" at end of url
		char* buffer = new char[strlen(url)+2];
		strcpy(buffer, url);
		buffer[strlen(url)] = 47;
		buffer[strlen(url)+1] = 0;
		url = strdup(buffer);
		delete [] buffer; //NO MEMORY LEAKS!
		
		// Update flag.
		urlF = 1;

	}

	// Get current URL as current path if needed.
	// WHY IS THIS 0 BYTES IN VALGRIND?
	char* currentPath = strdup(_urlArray[_headURL]._url);		

	// This is the URL we will insert to _urlToUrlRecord.
	char* finalURL;

	// Protocol headers to prepend if needed.
	// Eventually will add HTTPS compatability.
	const char* http1 = "http:";
	const char* http2 = "http://";
	if (!strncmp(url, "https://", 8)) return;	
	// Check if URL has a relative-to-current path.
	// ie. "/restOfURL" or "//restOfURL"
	const char* tripleSlash = "///";
	const char* doubleSlash = "//";
	const char* singleSlash = "/";
	if (strncmp(url, doubleSlash, strlen(doubleSlash)) == 0) {
		
		// The URL has a retaltive-to-current path.
		// It is in form of "//restOfURL"
		// We will prepend http1 to url
		char* tempURL = new char[strlen(http1)+strlen(url)+1];
		strcpy(tempURL, http1);
		strcat(tempURL, url);//WHY IS THIS 0 BYTES IN VALGRIND? 
		finalURL = strdup(tempURL);
		delete [] tempURL; //NO MEMORY LEAKS!

	} else if (strncmp(url, singleSlash, strlen(singleSlash)) == 0 && strncmp(url, doubleSlash, strlen(doubleSlash)) != 0) {

		// The URL has a relative-to-current path.
		// It is in the form of "/restOfURL"
		// We will postpend url to root of current path.
		// We must shave the current path to the third "/".
		//LAMESKIP:
		char* thirdSlash; // Hope this works.....
		thirdSlash = strchr(strchr(strchr(currentPath, 47)+1, 47)+1, 47);
		int thirdSlashPos = (currentPath - thirdSlash); //WHY IS THIS -17?
		if(thirdSlashPos<0)thirdSlashPos=-thirdSlashPos;
		//int thirdSlashPos = (int)currentPath - (int)thirdSlash;
		char* tempPath = new char[thirdSlashPos]; // THIS IS THE SOURCE OF THE FUCKING PROBLEM
		strncpy(tempPath, currentPath, thirdSlashPos); // Copy up to third slash.
		tempPath[strlen(tempPath)+1] = 0; // strncpy did not set '\0' && we need it for strcpy.
		char* tempURL = new char[strlen(tempPath)+strlen(url)+1];
		strcpy(tempURL, tempPath);
		strcat(tempURL, url);
		finalURL = strdup(tempURL);

		//NO MEMORY LEAKS!
		delete [] tempPath;
		delete [] tempURL; 

	} else if (!strncmp(url, tripleSlash, strlen(tripleSlash))) {
		//goto LAMESKIP;
	} else {
		
		// Check if protocol is HTTPS.
		const char* https = "https://";

		// URL does not start with "//" or "/"
		// Check if it has the prepended protocol.
		if (strncmp(url, http2, strlen(http2)) != 0) {
			
			// No protocol. Prepend it.
			char* tempURL = new char[strlen(http2)+strlen(url)+1];
			strcpy(tempURL, http2);
			strcat(tempURL, url);
			finalURL = strdup(tempURL);
			delete [] tempURL; //NO MEMORY LEAKS!

		} else if (strncmp(url, https, strlen(https)) == 0) {

			// HTTPS Protocol, force HTTP.
			char* tempURL = new char[strlen(http2)+(strlen(url)-strlen(https))+1];
			url += strlen(https);
			strcpy(tempURL, http2);
			strcat(tempURL, url);
			finalURL = strdup(tempURL);
			delete [] tempURL; //NO MEMORY LEAKS!

		} else {
			
			// If we got here, the URL passed for syntax.
			// You have traveled far, yet your journey is just starting.
			finalURL = strdup(url);

		}

	}

	// Now use openhttp to check if the "final" URL is valid && acessible.
	char* dummy = new char[1024];
	FILE* test = openhttp(finalURL, dummy);
	if (test == NULL) {
		
		// The URL was not valid/acessible. Quit trying to add to record.
		// DO NOT TRY fclose(test); UNDEFINED BEHAVIOUR FOR fclose(NULL);.
		delete [] dummy; //NO MEMORY LEAKS!
		return;

	} else { fclose(test); } delete [] dummy; //NO MEMORY LEAKS!

	// Dummy int to "catch" data/spot in _urlToUrlRecord.
	int catcher;
	int i = 0;
	for(;i<_tailURL;i++) {if(!strcmp(_urlArray[i]._url,finalURL)) return;}

	// We have our "URL" now check if it is in _urlToUrlRecord,
	// if not we insert the URL in.
	if (_urlToUrlRecord->find(finalURL, &catcher) == false) {

		// URL not in _urlToUrlRecord. Create new _urlArray item.
		_urlArray[_tailURL]._url = strdup(finalURL);
		_urlArray[_tailURL]._description = NULL;

		// Insert into _urlToUrlRecord && update nurl.
		_urlToUrlRecord->insertItem(finalURL, _headURL);
		_nurl++, _tailURL++;

	}
	
	// Free - NO MEMORY LEAKS!
	free(currentPath);//strdup not new
	free(finalURL);
	if (urlF == 1) free(url);

}

//
// Writes the output file for URLs.  This method is one of two of
// the actual 'outputs' of the program.  This file will act as a
// redirect of stdout to a file called "url.txt". This file will 
// contain a list of all of the URLs that were gathered by the
// WebCrawler in the _urlArray. It will print one instance of 
// each URL and its' description in the form of:
// 
// <url-index> <url>
// <description>
//
// EXAMPLE:
//
// 1 http://www.purdue.edu
// Purdue Main Page
//
// 2 http://www.purdue.edu/admissions
// Admissions Homepage
//
void WebCrawler::writeURLFile(const char* urlFileName) {
	
	// Set up stream && open file.
	std::ofstream outFile;
	outFile.open(urlFileName);

	// Traverse through _urlArray and print the index, url,
	// and description of each element.
	for (int i = 0; i <= _tailURL && i < _maxUrls; i++) {

		// Print index and URL.
		outFile << i;
		outFile << " ";
		outFile << _urlArray[i]._url;
		outFile << "\n";

		// Print empty string if description is null.
		if (_urlArray[i]._description == NULL) {

			// Description = null, print empty string instead.
			// EDIT: SHOULD NOT HAPPEN. In onAnchorFound, if 
			// the file stream is NULL, I do not add the URL
			// to the _urlArray.
			// I will keep this here for now for debug purposes,
			// I will remove eventually.
			outFile << "";
			outFile << "\n";

		} else {

			// Description is not null. Print normal.
			outFile << _urlArray[i]._description;
			outFile << "\n";

		}

		// New line char inbetween url elements.
		outFile << "\n";

	}

	// Close file at end.
	outFile.close();

}

//
// Writes the output file for words.  This method is the other
// of the two actual 'outputs' of the program.  This file will
// act as a redirect of stdout to a file called "word.txt". 
// This file will contain a list of all of the words that were
// found/parsed by the WebCrawler in _wordToURLRecordList. It
// will print one instance of each word followed by the indicies
// of each URL in which the word apprears.  The index will be 
// the URLs' index in the _urlArray. It will be formatted in 
// the form of: 
//
// <word> <index-list>  
//
// Where index-list with n elements is in the form of:
//
// <first_url_index> <second_url_index> ... <n'th_url_index>
//
// EXAMPLE:
//
// CS 3 4 6 7 89
// Purdue 4 6 3 5 7
//
void WebCrawler::writeWordFile(const char* wordFileName) {
		
	// Set up file stream.
	std::ofstream outFile;
	outFile.open(wordFileName);
	int tableSize = 2039;
	// Not sure why I added an indent for the loop, too lazy to remove now.
	
		// Loop through _wordToURLRecordList && print word.
		for (int i = 0; i < tableSize; i++) {

			// Special case flag for zero index. 
			// 0 == non-printed, 1 == printed.
			int zeroIndexPrint = 0;

			// Check if word is null. No word to print - continue.
			if (_wordToURLRecordList->_buckets[i] == NULL) continue;

			// Word exists - print it. && get RecordList
			outFile << _wordToURLRecordList->_buckets[i]->_key;
			outFile << " ";
			URLRecordList* wordList = _wordToURLRecordList->_buckets[i]->_data;

			// Create flag to signal if index has been printed.
			// Calloc makes all entries 0:
			// 	IF printF[j] == 0 :
			// 		word not previously printed.
			// 	ELSE :
			// 		printF[j] will have an index of a 
			// 		previously printed URL, this way we
			// 		can keep track && match previously
			// 		printed URLs && avoid duplicates.
			int* printF = (int*)calloc(tableSize, sizeof(int));

			// Loop through URLRecordList && print the non-printed indicies.
			while (wordList != NULL) {

				// Create another flag to find the word index && check
				// and or update flag accordingly.  This flag catches URLs
				// with indicies that match current index.  If this flag is 
				// set to 1, then the printF flag == the current index, this
				// means that we have already printed this index for the
				// current word.  Therefore:
				// IF printF[j] == 0
				// 	index not previously printed - keep find == 0
				// IF printF[j] == current index
				// 	index previously printed - set flag == 1
				int findF = 0;

				// After finishing method, I noticed there is one special
				// case.  If the current index == 0, calloc initialized
				// all elements to 0 and I am checking that for not
				// previously printed.  If current index == 0, it may 
				// print an unknown amount of 0's since it will always
				// behave as if it has not printed since it sets printF[j]
				// to zero, and it won't match any printF flag since it will
				// break when printF[j] == 0 before it compares flag to index.
				// Thus I will handle this case here before we 
				// get to the chacking of printF.
				if (wordList->_urlRecordIndex == 0) {

					// Check new zeroIndexPrint flag.
					if (zeroIndexPrint == 0) {

						// The index is non-printed. Print/update.
						outFile << "0 ";
						zeroIndexPrint = 1;
						findF = 1;

					} else {

						// The index is printed. Update flag.
						findF = 1;

					}

				}

				// Traverse through all previous printF flags && check
				// if printF[j] matches the current index. If printF[j]
				// matches the current index - current index has been 
				// previously printed for this word. Set flag and stop 
				// traversing. If printF[j] isn't 0 and doesn't match
				// the current index, continue to next printF[i].  Do
				// this until printF[j] == 0 - meaning we traversed 
				// through all previously printed indicies && no matches
				// meaning break and print index (set printF[j] to index),
				// or we matched an index meaning set find flag to 1 and
				// break resulting in no print, or else we run out of 
				// possible indexes - something that shouldn't happen.
				int j = 0;
				for (; j < tableSize; j++) {

					// Check if index not previously printed.
					if (printF[j] == 0) break;

					// Check if printF[j] == current index.
					if (printF[j] == wordList->_urlRecordIndex) {

						// URL index was previously printed.
						// Update find flag.
						findF = 1;
						break;

					} else continue;

				}

				// We either matched index, ran out of printF[j] that
				// had been printed, or somehow broke the tableSize.
				// Check for the latter.
				if (j >= tableSize) {

					// Massive Error happened. Program broken.
					// YOU DONE FUCKED UP!
					fprintf( stderr, "\n\tError: Broke tableSize.\n\n");
					exit(1);

				}

				// We either matched index or passed all set printF flags.
				// Check for the latter, the find flag should be untouched.
				if (!findF) {

					// The URL index hasn't been printed - print.
					outFile << wordList->_urlRecordIndex;
					outFile << " ";

					// Set the printF[j] flag to this URL index.
					printF[j] = wordList->_urlRecordIndex;

				}

				// Move wordList over one to the next URL index.
				wordList = wordList->_next;

			}

			// Done printing word and all of its' URL indicies. New line char.
			outFile << "\n";

			// Clear printF - NO MEMORY LEAKS!
			free(printF);

		}
		//if(printF) free(printF);	
		// Done printing all words && respective indicies. Close file stream.
		outFile.close();

	// Again, too lazy to delete the weird extra tab.
	
}

//
// Constructor for WebCrawler.  This method will set everything we
// need to create an instance of a WebCrawler object.  There is also
// flexability: the constructor may be called with or without the 
// third argument of maxUrls - if not passed to the constructor, the
// default value of 1000 as specified by the handout will be set as 
// the value for maxUrls.
//
WebCrawler::WebCrawler(int nurlRoots, const char** urlRoots, int maxUrls) {
	
	// Set and instantiate member attributes & data structures.
	desF = 0, wordF = 0, _nurl = 0;
	urlFileName = "url.txt";
	wordFileName = "word.txt";
	_maxUrls = maxUrls, _headURL = 0, _tailURL = 0;
	_urlArray = new URLRecord[_maxUrls];
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>();

	// Initilize urlRoots and add into the _urlArray
	// Must set _description to NULL since we have not parsed yet.
	for (int i = 0; i < nurlRoots && i < _maxUrls; i++) {
		
		//Make copy of URL.
		const char* copy = strdup(urlRoots[i]);
		
		// Check if we need to prepend protocol.
		const char* http = "http://";
		if (strncmp(copy, http, strlen(http)) != 0) {

			// We need to prepend.
			char* buffer = new char[strlen(http)+strlen(copy)+1];
			strcpy(buffer, http);
			strcat(buffer, copy);
			free((void*)copy);
			copy = strdup(buffer);
			delete [] buffer; //NO MEMORY LEAKS!

		}

		// Check if we need to force protocol.
		const char* https = "https://";
		if (strncmp(copy, https, strlen(https)) == 0) {

			// Force protocol.
			char* buffer = new char[strlen(http)+(strlen(copy)-strlen(http))+1];
			strcpy(buffer, http);
			copy += strlen(https);
			strcat(buffer, copy);
			free((void*)copy);
			copy = strdup(buffer);
			delete [] buffer;

		}

		// Check if we need to postpend "/"
		if (copy[strlen(copy)] != 47) {
			
			// Append "/"
			char* buffer = new char[strlen(copy)+2];
			strcpy(buffer, copy);
			buffer[strlen(copy)] = 47;
			buffer[strlen(copy)+1] = 0;
			free((void*)copy);
			copy = strdup(buffer);
			delete [] buffer; //NO MEMORY LEAKS!

		}

		// Check if valid URL/accessible.
		char* dummy = new char[1024];
		FILE* test = openhttp(copy, dummy);
		if (test == NULL) {
		
			// The URL is not valid/acessible.
			delete [] dummy; // NO MEMORY LEAKS!
			continue;

		} else { fclose(test); } delete [] dummy; //NO MEMORY LEAKS!

		// Insert URL into _urlArray as a URLRecord && free.
		_urlArray[i]._url = strdup(copy); //strdup
		_urlArray[i]._description = NULL;
		_tailURL++, _nurl++;
		
		//NO MEMORY LEAKS!
		free((void*)copy);

	}

}

//
// The actual 'web crawling' method and algorithm.  This method will
// be the main, driving algorithm of the WebCrawler.  This method will
// act as a buffer while it parses through, at most, _maxUrls (1000
// if not specified by user through stdin on execution) URLs.  It will
// parse the code of a URL, search for hyperlinks, get a description of
// the page, create a 'database' of URLs and their respective websites'
// descriptions, then advance in the _urlArray to the next URL and start
// the crawling procedure over again with the new URL. It will repeat this
// process until it has 'crawled' through _maxUrls worth of URL's or until
// there are no longer any new hyperlinks that can be parsed.
//
void WebCrawler::crawl() {

	//
	// WEB-CRAWLER ALGORITHM.
	//
	// This algorithm will follow the algorithm provided in 
	// the lab handout. In order to view this handout, visit
	// the URL and hope Gustavo hasn't removed the page.
	// URL: https://www.cs.purdue.edu/homes/cs251/
	// If there, chack the "Lab Handouts" tab and try for
	// "Lab 2: Building a Web Crawler".
	while (_headURL < _tailURL) {

		// Get the next URL.
		char* nextURL = strdup(_urlArray[_headURL]._url);

		// Check if the URL is accessible && valid.
		// If so, get the code of the webpage.
		int bytes;
		char* code = fetchHTML(nextURL, &bytes);
		if (code == NULL) {
		
			// The URL is not valid/accessible.
			// EDIT: Shouldn't need this check, as it is done
			// several other times and shouln't be added to
			// _urlArray if it is NULL.  I will keep this here
			// for now for debugging purposes - will remove 
			// later.
			free(nextURL); // NO MEMORY LEAKS!
			_headURL++;
			continue;

		}

		// Parse to get description, set hyperlinks, set words &&
		// add all needed shit to their respective data structures.
		// This will call onAnchorFound and onContentFound as it comes
		// across hyperlinks && chars, respectively.  Those two
		// functions will handle the nasty things.  By the end of this 
		// function call, we will have the description, hyperlinks
		// checked and added to _urlArray, _urlToUrlRecordList, and
		// _wordToURLRecordList, as well as words added to the 
		// _wordToURLRecordList.
		parse(code, bytes);
		desFlag = 0;
		// Set the "next"/current URL's description.
		_urlArray[_headURL]._description = strdup(des); // strdup(des)
	
		// Update Flags.
		desF = 0;

		// Increment _headURL to advance the traversal.
		_headURL++;
		
		// Free - NO MEMORY LEAKS!
		free(nextURL);
		free(code); // Needed to free since fetchHTML calls malloc&&realloc.
		// Maybe free(bytes)? I couldn't tell from looking at fetchHTML and
		// reading man pages for fread().

	}

	// NO MEMORY LEAKS!
	if (word != NULL) free(word);
	if (des != NULL ) free(des);

}

//
// Main method to run the file.  This method will parse the arguments 
// passed through stdin.  It will check if the calling convention was
// as expected, if not - print a usage page and exit.  If it passes 
// this, the method will then create a new instance of a WebCrawler
// object and begin the 'crawling'.  Once that is done, the method
// will then call the two file print methods and exit as intended.
//
int main(int argc, char** argv) {
	
	// Local variables used for temp storage in parsing.
	int numUrls, firstUrlPos, nFirstUrls;
	const char** firstUrls;

	// Parse the arguments: should have anywhere from 2 to n.
	if (argc == 1) {

		// Program called incorrectly.
		printUsage();
		exit(1);

	} else if (strcmp(argv[1], "-u") == 0) {
		
		// User is trying to specify _maxUrls.
		// Ensure they called correctly for this format.
		if (argc == 2 || argc == 3) {
			
			// Program called incorrectly.
			printUsage();
			exit(1);

		}

		// Correct format for this style.
		// Get user-specified _maxUrls && keep track of first URL
		// index in url-list so we know where to start parsing that
		// and so we can compute nurlRoots.
		numUrls = atoi(argv[2]);
		firstUrlPos = 3;

	} else {
		
		// User did not specify _maxUrls.
		numUrls = -1;
		firstUrlPos = 1;

	}
	if (numUrls == -1 || numUrls > 190) {
		printf("Check url and word\n");
		return 0;
	}
	// Set temporary url-list from user.
	int i = 0;
	nFirstUrls = argc - firstUrlPos;
	firstUrls = (const char**)malloc(sizeof(char*)*nFirstUrls);
	while (firstUrlPos < argc) {
		
		// Skilled memory allocation!
		//firstUrls[i] = (const char*)malloc(sizeof(char)*(strlen(argv[firstUrlPos])+1));
		firstUrls[i] = strdup(argv[firstUrlPos]);
		firstUrlPos++, i++;

	}

	// Create a new instance of a WebCrawler && start crawling. Finally,
	// call the two write functions to print files && finish execution.
	if (numUrls < 0) {
		
		// User did not specify _maxUrls.
		WebCrawler* wc = new WebCrawler(nFirstUrls, firstUrls);
		
		// Skilled Freeing - NO MEMORY LEAKS!
		for (int i = 0; i < nFirstUrls; i++) free((void*)firstUrls[i]);
		free(firstUrls);

		// Start web-craling and create output files when done.
		wc->crawl();
		wc->writeURLFile(wc->urlFileName);
		wc->writeWordFile(wc->wordFileName);

	} else {

		// User used the -u flag.
		WebCrawler* wc = new WebCrawler(nFirstUrls, firstUrls, numUrls);
		
		// Skilled Freeing - NO MEMORY LEAKS!
		for (int i = 0; i < nFirstUrls; i++) free((void*)firstUrls[i]);
		free(firstUrls);

		// Start web-crawling and create output files when done.
		wc->crawl();
		wc->writeURLFile(wc->urlFileName);
		wc->writeWordFile(wc->wordFileName);

	}

	// Sucessfully finished completion! Signify normal exit.
	return 0;

}









