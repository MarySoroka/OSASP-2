#pragma once

#ifdef PHONEBOOKLIB_EXPORTS
#define PBLIB_API __declspec(dllexport)
#else
#define PBLIB_API __declspec(dllimport)
#endif

#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>

#pragma warning(disable : 4251)

constexpr int stringMaxLen = 255;
constexpr int intMaxLen = 10;
constexpr int unusedCharsLen = 9;
constexpr int recordSize = stringMaxLen * 5 + intMaxLen * 3 + unusedCharsLen;
constexpr int frameSize = 65536;

template <typename T>
struct Node {
	int key;
	std::list<T> data;
	Node* parent;
	Node* left;
	Node* right;
	int color;
	typedef Node<T>* NodePtr;
};

template <typename T>
class RBTree {
private:
	Node<T>* root;
	Node<T>* TNULL;
	Node<T>* searchTreeHelper(Node<T>* node, int key);
	void deleteNodeHelper(Node<T>* node, int key);
	void fixDelete(Node<T>* x);
	Node<T>* minimum(Node<T>* node);
	Node<T>* maximum(Node<T>* node);
	void rbTransplant(Node<T>* u, Node<T>* v);

	void fixInsert(Node<T>* k);
	void leftRotate(Node<T>* x);
	void rightRotate(Node<T>* x);

public:
	RBTree();
	Node<T>* searchTree(int key);
	void tryInsert(int key, T data);
	void deleteNode(int key);
	void clear();
};


template <typename T>
class Indexer {
private:
	std::list<T> collection;
public:
	T& operator[](int i);
	int operator[](T value);
	int tryAdd(T value);
	void clear();
};

struct PBLIB_API Record {
	std::wstring* telephone;
	std::wstring* fname;
	std::wstring* lname;
	std::wstring* mname;
	std::wstring* street;
	int house;
	int housing;
	int flat;
	Record(std::wstring* telephone, std::wstring* fname, std::wstring* lname, std::wstring* mname, std::wstring* street, int house, int housing, int flat);
};

class PBLIB_API PhoneBook {
private:
	Indexer<std::wstring> telephoneIndexer;
	Indexer<std::wstring> fnameIndexer;
	Indexer<std::wstring> lnameIndexer;
	Indexer<std::wstring> mnameIndexer;
	Indexer<std::wstring> streetIndexer;

	RBTree<Record*> telephoneSearchTree;
	RBTree<Record*> fnameSearchTree;
	RBTree<Record*> lnameSearchTree;
	RBTree<Record*> mnameSearchTree;
	RBTree<Record*> streetSearchTree;
	RBTree<Record*> houseSearchTree;
	RBTree<Record*> housingSearchTree;
	RBTree<Record*> flatSearchTree;

	std::wstring* getWstr(wchar_t* src, int srcOffset, int len);
	
public:
	enum class SearchField {
		telephone,
		firstName,
		middleName,
		lastName,
		street,
		house,
		housing,
		flat
	};
	std::list<Record> records;
	Record* operator[](int i);
	void addRecord(std::wstring* telephone, std::wstring* fname, std::wstring* lname, std::wstring* mname, std::wstring* street, int house, int housing, int flat);
	void loadFromFile(std::wstring filename);	
	std::list<Record*> search(SearchField s, int key);
	std::list<Record*> search(Record* r);
	std::list<Record*> searchLong(std::wstring filename, Record* r, int maxFrames);
	void clear();
};

