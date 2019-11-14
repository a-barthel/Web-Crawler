//
//---------------------------------- AUTHOR -----------------------------------------
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
//-----------------------------------------------------------------------------------
//

//
//--------------------------------- HEADERS -----------------------------------------
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

//
//-----------------------------------------------------------------------------------
//



//
//---------------------------- CLASS DECLARATION ------------------------------------
//

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

//
//-----------------------------------------------------------------------------------
//

//
//-------------------------- TEMPLATE INITIALIZATION --------------------------------
//

// Method maps a key to a spot in the Hash Table.
template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

// Method Creates an object instance of a Hash Table Template.
template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  _buckets = (HashTableTemplateEntry<Data>**)malloc(sizeof(HashTableTemplateEntry<Data>*)*TableSize);
  for (int i = 0; i < TableSize; i++) _buckets[i] = NULL;
}

// Method inserts a Hash Table entry.  Returns true if entry is 
// already present && overwrites data, returns false if entry
// is not in Hash Table && creates/inserts entry.
template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  int h = hash(key);
  HashTableTemplateEntry<Data> *ht;
  ht = _buckets[h];
  while (ht != NULL) {
	if (strcmp(ht->_key, key) == 0) {	
		ht->_data = data;
		return true;
	}
	ht = ht->_next;
  }
  ht = (HashTableTemplateEntry<Data>*)malloc(sizeof(HashTableTemplateEntry<Data>));
  ht->_data = data;
  ht->_key = strdup(key);
  ht->_next = _buckets[h];
  _buckets[h] = ht;
  return false;
}

// Method tries to find an entry in the Hash Table using 
// the key passed as an argument.  Returns true if found
// && sets the passed argument of data as a pointer to the
// data in the found entry, returns false if not found.
template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
  //int add = std::addressof(key);
  //if(add == 0x4000) return false;
  int h = hash(key);
  HashTableTemplateEntry<Data> *ht;
  ht = _buckets[h];
  while (ht != NULL) {
	if (strcasecmp(ht->_key, key) == 0) {
		*data = ht->_data;
		return true;
	}
	ht = ht->_next;
  }
  return false;
}

// Method adds Template compatability to the [] operator.
// Similar to found, tries to find entry in Hash Table using
// the passed key, if found returns the data, if not found
// it returns NULL.
template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
  Data d;
  int h = hash(key);
  HashTableTemplateEntry<Data> *ht;
  ht = _buckets[h];
  while (ht != NULL) {
	if (strcmp(ht->_key, key) == 0) d = ht->_data;
	ht = ht->_next;
  }
  return d;
}

// Method removes element that matches with key. If an entry
// is found, it removes the entry, frees any allocated 
// memory, and returns true. If entry not found, returns false.
template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  int h = hash(key);
  HashTableTemplateEntry<Data> *ht1, *ht2;
  ht1 = _buckets[h], ht2 = _buckets[h];
  while (ht1 != NULL) {
	if (strcmp(ht1->_key, key) == 0) {
		if (ht1 == ht2) _buckets[h] = ht1->_next;
		else ht2 = ht1->_next;
		free(ht1->_key);
		return true;
	}
	if (ht1 != ht2) ht2 = ht2->_next;
	ht1 = ht1->_next;
  }
  return false;
}

// Class Definition for an Iterator object.
template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

// Creates instance of Iterator object to iterate the Hash Table passed.
template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  _hashTable = hashTable;
  for (_currentBucket = 0; _currentBucket < _hashTable->TableSize; _currentBucket++) if (_hashTable->_buckets[_currentBucket] != NULL) break;
  if (_currentBucket < _hashTable->TableSize) _currentEntry = _hashTable->_buckets[_currentBucket];
  else _currentEntry = NULL;
}

// Method advances Iterator object to the next entry. If the current
// entry is NULL, it returns false. If current entry is not NULL, it
// advances the current entry to the next non-NULL entry and returns true.
template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
{
  if (_currentEntry == NULL) return false;
  key = _currentEntry->_key;
  data = _currentEntry->_data;
  _currentEntry = _currentEntry->_next;
  if (_currentEntry == NULL) {
	for (_currentBucket++; _currentBucket < _hashTable->TableSize; _currentBucket++)
		if (_hashTable->_buckets[_currentBucket] != NULL) break;
	if (_currentBucket >= _hashTable->TableSize) _currentEntry = NULL;
	else _currentEntry = _hashTable->_buckets[_currentBucket];
  }
  return true;
}

//
//-----------------------------------------------------------------------------------
//


