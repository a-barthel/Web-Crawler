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

#include "HashTableTemplate.h"
#include "SimpleHTMLParser.h"
#include "openhttp.h"
#include <stdio.h>
#include <fstream>
#include <iostream>

//
//----------------------------------------------------------------------------------
//

//
//-------------------------------- STRUCTURES ---------------------------------------
//

// Stores a URL and the first 100 words
struct URLRecord {
  char * _url;          // URL of the document.
  char * _description;  // First 100 words of the document without tags
};

// Used to implement a list of URL indexes of the url array.
// There is one list of URLs for each word 
struct URLRecordList {
  int _urlRecordIndex;     // Index into the URL Array
  URLRecordList * _next;   // Pointer to the next URL Record in the list
};

//
//-----------------------------------------------------------------------------------
//

// Create WebCrawler as a subclass of SimpleHTMLParser
class WebCrawler : public SimpleHTMLParser {
  // The url array stores all the URLs that have been scanned
  int _maxUrls;            // Max size of the URL array
  URLRecord * _urlArray;   // Array of URLs 
  int _headURL;            // Points to the next URL to scan by the web crawler
  int _tailURL;            // Next position unused in the urlArray

  HashTableTemplate<int> * _urlToUrlRecord;         //  maps a URL to its index in the URLArray 
  
  HashTableTemplate<URLRecordList *> *_wordToURLRecordList; // maps a word to a list of URLs

 public:
  // Create a web crawler object with the initial urls
  // To comply with the handout, there is a defaul value
  // of 1000 for maxUrls if not passed as argument.
  WebCrawler(int nurlRoots, const char ** urlRoots, int maxUrls = 1000);
  
  // Method to crawl the web using the algorithm described in the lab.
  void crawl();

  // Method to write array of URLs and descriptions to file.
  void writeURLFile(const char * urlFileName);

  // Method to write list of words with their urls to file.
  void writeWordFile(const char* wordFileName);

  // Overloaded method from SimpleHTMLParser updated to better handle
  // getting descriptions of HTML pages.
  void onContentFound(char c);

  // Overloaded method from SimpleHTMLParser updated to better handle
  // getting hyperlinks of HTML pages.
  void onAnchorFound(char* url);

  // Method to print the calling convention of the program.
  static void printUsage();
  
  //
  // Helper Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //
  
  // Flag to indicate we need to clear description. : 0 = clear, 1 = don't clear
  int desF;

  // Flag to indicate we need to clear word. : 0 = clear, 1 = don't clear
  int wordF;

  // To store description across 500+ calls from parse.
  char* des;

  // To store words across multiple calls from parse.
  char* word;

  // To keep track of numbers in the _urlToURLRecordList.
  int _nurl;

  // To hold file name for url write method.
  const char* urlFileName;

  // To hold file name for word write method.
  const char* wordFileName;

  //
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //

};


