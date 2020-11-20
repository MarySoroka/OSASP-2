#include "PhoneBookLib.h"



template <typename T>
typename Node<T>* RBTree<T>::searchTreeHelper(Node<T>* node, int key) {

	if (node == TNULL || key == node->key) {
		return node;
	}

	if (key < node->key) {
		return searchTreeHelper(node->left, key);
	}
	return searchTreeHelper(node->right, key);
}

template <typename T>
void RBTree<T>::fixDelete(Node<T>* x) {
	Node<T>* s;
	while (x != root && x->color == 0) {
		if (x == x->parent->left) {
			s = x->parent->right;
			if (s->color == 1) {
				s->color = 0;
				x->parent->color = 1;
				leftRotate(x->parent);
				s = x->parent->right;
			}

			if (s->left->color == 0 && s->right->color == 0) {
				s->color = 1;
				x = x->parent;
			}
			else {
				if (s->right->color == 0) {
					s->left->color = 0;
					s->color = 1;
					rightRotate(s);
					s = x->parent->right;
				}

				s->color = x->parent->color;
				x->parent->color = 0;
				s->right->color = 0;
				leftRotate(x->parent);
				x = root;
			}
		}
		else {
			s = x->parent->left;
			if (s->color == 1) {
				s->color = 0;
				x->parent->color = 1;
				rightRotate(x->parent);
				s = x->parent->left;
			}

			if (s->right->color == 0) {
				s->color = 1;
				x = x->parent;
			}
			else {
				if (s->left->color == 0) {
					s->right->color = 0;
					s->color = 1;
					leftRotate(s);
					s = x->parent->left;
				}

				s->color = x->parent->color;
				x->parent->color = 0;
				s->left->color = 0;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = 0;
}

template <typename T>
Node<T>* RBTree<T>::minimum(Node<T>* node) {
	while (node->left != TNULL) {
		node = node->left;
	}
	return node;
}

template <typename T>
Node<T>* RBTree<T>::maximum(Node<T>* node) {
	while (node->right != TNULL) {
		node = node->right;
	}
	return node;
}

template <typename T>
void RBTree<T>::rbTransplant(Node<T>* u, Node<T>* v) {
	if (u->parent == nullptr) {
		root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	v->parent = u->parent;
}

template <typename T>
void RBTree<T>::deleteNodeHelper(Node<T>* node, int key) {
	Node<T>* z = TNULL;
	Node<T>* x, *y;
	while (node != TNULL) {
		if (node->key == key) {
			z = node;
		}

		if (node->key <= key) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}

	if (z == TNULL) {
		return;
	}

	y = z;
	int y_original_color = y->color;
	if (z->left == TNULL) {
		x = z->right;
		rbTransplant(z, z->right);
	}
	else if (z->right == TNULL) {
		x = z->left;
		rbTransplant(z, z->left);
	}
	else {
		y = minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z) {
			x->parent = y;
		}
		else {
			rbTransplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}

		rbTransplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	delete z;
	if (y_original_color == 0) {
		fixDelete(x);
	}
}

template <typename T>
void RBTree<T>::fixInsert(Node<T>* k) {
	Node<T>* u;
	while (k->parent->color == 1) {
		if (k->parent == k->parent->parent->right) {
			u = k->parent->parent->left;
			if (u->color == 1) {
				u->color = 0;
				k->parent->color = 0;
				k->parent->parent->color = 1;
				k = k->parent->parent;
			}
			else {
				if (k == k->parent->left) {
					k = k->parent;
					rightRotate(k);
				}
				k->parent->color = 0;
				k->parent->parent->color = 1;
				leftRotate(k->parent->parent);
			}
		}
		else {
			u = k->parent->parent->right;

			if (u->color == 1) {
				u->color = 0;
				k->parent->color = 0;
				k->parent->parent->color = 1;
				k = k->parent->parent;
			}
			else {
				if (k == k->parent->right) {
					k = k->parent;
					leftRotate(k);
				}
				k->parent->color = 0;
				k->parent->parent->color = 1;
				rightRotate(k->parent->parent);
			}
		}
		if (k == root) {
			break;
		}
	}
	root->color = 0;
}

template <typename T>
Node<T>* RBTree<T>::searchTree(int k) {
	return searchTreeHelper(this->root, k);
}

template <typename T>
RBTree<T>::RBTree() {
	TNULL = new Node<T>;
	TNULL->color = 0;
	TNULL->left = TNULL;
	TNULL->right = TNULL;
	root = TNULL;
}

template <typename T>
void RBTree<T>::leftRotate(Node<T>* x) {
	Node<T>* y = x->right;
	x->right = y->left;
	if (y->left != TNULL) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == TNULL) {
		this->root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

template <typename T>
void RBTree<T>::rightRotate(Node<T>* x) {
	Node<T>* y = x->left;
	x->left = y->right;
	if (y->right != TNULL) {
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == TNULL) {
		this->root = y;
	}
	else if (x == x->parent->right) {
		x->parent->right = y;
	}
	else {
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
}

template <typename T>
void RBTree<T>::tryInsert(int key, T data) {
	Node<T>* node = searchTree(key);
	if (node != TNULL) {
		node->data.push_back(data);
		return;
	}
	node = new Node<T>;
	node->parent = TNULL;
	node->key = key;
	node->data.push_back(data);
	node->left = TNULL;
	node->right = TNULL;
	node->color = 1;

	typename Node<T>::NodePtr y = TNULL;
	typename Node<T>::NodePtr x = this->root;

	while (x != TNULL) {
		y = x;
		if (node->key < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	node->parent = y;
	if (y == TNULL) {
		root = node;
	}
	else if (node->key < y->key) {
		y->left = node;
	}
	else {
		y->right = node;
	}

	if (node->parent == TNULL) {
		node->color = 0;
		return;
	}

	if (node->parent->parent == TNULL) {
		return;
	}

	fixInsert(node);
}
template <typename T>
void RBTree<T>::deleteNode(int key) {
	deleteNodeHelper(this->root, key);
}

template <typename T>
void RBTree<T>::clear() {
	while (this->root->right != TNULL)
		deleteNode(maximum(this->root)->key);
	while (this->root->left != TNULL)
		deleteNode(minimum(this->root)->key);
	delete this->root;
	this->root = TNULL;
		
}


template<typename T> 
T& Indexer<T>::operator[](int i) {
	if (i > (int)this->collection.size()) {
		throw std::out_of_range("Index out of bounds");
	}
	return *std::next(this->collection.begin(), i);
}

template<typename T>
int Indexer<T>::operator[](T value)
{
	for (auto it = collection.begin(); it != collection.end(); it++) {
		auto i = std::distance(collection.begin(), it);
		if (*it == value)
			return i;
	}
	return -1;
}

template <typename T> int Indexer<T>::tryAdd(T value) {
	int pos = this->operator[](value);
	if (pos == -1) {
		this->collection.push_back(value);
		return collection.size() - 1;
	}
	return pos;
}

template<typename T>
void Indexer<T>::clear()
{
	this->collection.clear();
}

Record* PhoneBook::operator[](int i)
{
	return &(*std::next(this->records.begin(), i));
}

void PhoneBook::addRecord(std::wstring* telephone, std::wstring* fname, std::wstring* lname, std::wstring* mname, std::wstring* street, int house, int housing, int flat) {

	int telId = this->telephoneIndexer.tryAdd(*telephone);
	int fnameId = this->fnameIndexer.tryAdd(*fname);
	int mnameId = this->mnameIndexer.tryAdd(*mname);
	int lnameId = this->lnameIndexer.tryAdd(*lname);
	int streetId = this->streetIndexer.tryAdd(*street);

	Record r(&this->telephoneIndexer[telId], &this->fnameIndexer[fnameId], &this->lnameIndexer[lnameId], &this->mnameIndexer[mnameId], &this->streetIndexer[streetId], house, housing, flat);
	this->records.push_back(r);
	Record* rPtr = &records.back();

	this->telephoneSearchTree.tryInsert(telId, rPtr);
	this->fnameSearchTree.tryInsert(fnameId, rPtr);
	this->mnameSearchTree.tryInsert(mnameId, rPtr);
	this->lnameSearchTree.tryInsert(lnameId, rPtr);
	this->streetSearchTree.tryInsert(streetId, rPtr);
	this->houseSearchTree.tryInsert(house, rPtr);
	this->housingSearchTree.tryInsert(housing, rPtr);
	this->flatSearchTree.tryInsert(flat, rPtr);
}

std::wstring* PhoneBook::getWstr(wchar_t* src, int srcOffset, int len)
{
	wchar_t* tmp = new wchar_t[len + 1];
	for (int i = srcOffset; i < srcOffset + len; i++) {
		tmp[i - srcOffset] = src[i];
	}
	tmp[len] = L'\0';
	std::wstring* result = new std::wstring(tmp);
	delete[] tmp;
	return result;

}

void PhoneBook::loadFromFile(std::wstring filename) {	
	std::wifstream infile(filename.c_str(), _SH_DENYWR);
	std::wstring line;
	while (std::getline(infile, line))
	{
		std::wistringstream iss(line);
		std::wstring telephone, lname, fname, mname, street;
		int house, housing, flat;

		if (!(iss >> telephone >> fname >> lname >> mname >> street >> house >> housing >> flat)) { break; } // error
		addRecord(&telephone, &fname, &lname, &mname, &street, house, housing, flat);
	}
}

std::list<Record*> PhoneBook::search(SearchField s, int key) {
	switch (s)
	{
	case PhoneBook::SearchField::telephone:
		return telephoneSearchTree.searchTree(key)->data;
	case PhoneBook::SearchField::firstName:
		return fnameSearchTree.searchTree(key)->data;
	case PhoneBook::SearchField::middleName:
		return mnameSearchTree.searchTree(key)->data;
	case PhoneBook::SearchField::lastName:
		return lnameSearchTree.searchTree(key)->data;
	case PhoneBook::SearchField::street:
		return streetSearchTree.searchTree(key)->data;
	case PhoneBook::SearchField::house:
		return houseSearchTree.searchTree(key)->data;
	case PhoneBook::SearchField::housing:
		return housingSearchTree.searchTree(key)->data;
	case PhoneBook::SearchField::flat:
		return flatSearchTree.searchTree(key)->data;
	default:
		return std::list<Record*>();
	}
}


std::list<Record*> PhoneBook::search(Record* r) {
	std::list<Record*> result;
	if (r->telephone->empty() && r->fname->empty() && r->mname->empty() && r->lname->empty() && r->street->empty() && r->house == 0 && r->housing == 0 && r->flat == 0) {
		for (auto it = this->records.begin(); it != this->records.end(); it++) {
			result.push_back(&(*it));
		}
		return result;
	}

	if (!(r->telephone->empty()))
		result = search(PhoneBook::SearchField::telephone, this->telephoneIndexer[*r->telephone]);
	else if (!(r->fname->empty()))
		result = search(PhoneBook::SearchField::firstName, this->fnameIndexer[*r->fname]);
	else if (!r->mname->empty())
		result = search(PhoneBook::SearchField::middleName, this->mnameIndexer[*r->mname]);
	else if (!r->lname->empty())
		result = search(PhoneBook::SearchField::lastName, this->lnameIndexer[*r->lname]);
	else if (!r->street->empty())
		result = search(PhoneBook::SearchField::street, this->streetIndexer[*r->street]);
	else if (r->house != 0)
		result = search(PhoneBook::SearchField::house, r->house);
	else if (r->housing != 0)
		result = search(PhoneBook::SearchField::housing, r->housing);
	else if (r->flat != 0)
		result = search(PhoneBook::SearchField::flat, r->flat);

	for (auto it = result.begin(); it != result.end();) {
		bool erased = false;
		if ((!r->telephone->empty() && *(*it)->telephone != *r->telephone) || (!r->fname->empty() && *(*it)->fname != *r->fname)
			|| (!r->lname->empty() && *(*it)->lname != *r->lname) || (!r->mname->empty() && *(*it)->mname != *r->mname)
			|| (!r->street->empty() && *(*it)->street != *r->street) || (r->house != 0 && (*it)->house != r->house)
			|| (r->housing != 0 && (*it)->housing != r->housing) || (r->flat != 0 && (*it)->flat != r->flat)) 
		{
			auto itErase = it++;
			result.erase(itErase);
			erased = true;
		}
		if (!erased)
			it++;
	}
	return result;
}

std::list<Record*> PhoneBook::searchLong(std::wstring filename, Record* r, int maxFrames)
{
	this->telephoneIndexer.clear();
	this->fnameIndexer.clear();
	this->lnameIndexer.clear();
	this->mnameIndexer.clear();
	this->streetIndexer.clear();

	std::list<Record*> result;
	int fileMappingOffset = 0;
	int fileMappingFrameSize = frameSize * maxFrames;

	

	HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hFile == NULL)
		return result;
	
	int fileSize = GetFileSize(hFile, NULL);

	HANDLE hMem = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fileSize,NULL);
	int tmp = GetLastError();
	if (hMem == NULL)
		return result;

	
	int invalidRegionSize = 0;
	wchar_t invalidText[recordSize] = { 0 };

	while (fileMappingOffset != fileSize)
	{
		int validRegionSize = fileMappingFrameSize - 1;
		if (fileMappingOffset + fileMappingFrameSize > fileSize) {
			fileMappingFrameSize = fileSize - fileMappingOffset;
			validRegionSize = fileMappingFrameSize;
		}
		char* mappedTextPtr = (char*)MapViewOfFile(hMem, FILE_MAP_READ, 0, fileMappingOffset, fileMappingFrameSize);

		 
		int tmp = GetLastError();
		if (mappedTextPtr == NULL)
			return result;

		

		while (mappedTextPtr[validRegionSize-1] != '\n' && validRegionSize > 0)
			validRegionSize--;

		size_t convertedChars = 0;
		wchar_t* validText = new wchar_t[fileMappingFrameSize + invalidRegionSize + 1];
		mbstowcs_s(&convertedChars, validText + invalidRegionSize, fileMappingFrameSize +1, mappedTextPtr, fileMappingFrameSize);
		for (int i = 0; i < invalidRegionSize; i++)
			validText[i] = invalidText[i];

		int newInvalidRegionSize = fileMappingFrameSize - validRegionSize;
		validRegionSize += invalidRegionSize;
		
		for (int i = validRegionSize; i < fileMappingFrameSize+ invalidRegionSize; i++)
			invalidText[i - validRegionSize] = validText[i];

		invalidRegionSize = newInvalidRegionSize;



		int regionOffset = 0;
		while (regionOffset != validRegionSize)
		{
			
			std::wstring* telephone = nullptr;
			std::wstring* lname = nullptr;
			std::wstring* fname = nullptr;
			std::wstring* mname = nullptr;
			std::wstring* street = nullptr;
			std::wstring* houseStr = nullptr;
			std::wstring* housingStr = nullptr;
			std::wstring* flatStr = nullptr;
			int house = 0, housing = 0, flat = 0;
			
			wchar_t* currentPtr = validText + regionOffset;
			int i, strStartIndex, field;
			for (i = 0, strStartIndex = 0, field = 0; currentPtr[i] != L'\n'; i++) {
				if (currentPtr[i] == L' ' || currentPtr[i] == L'\r') {
					switch (field) {
					case 0:
						telephone = getWstr(currentPtr, strStartIndex, i - strStartIndex);
						break;
					case 1:
						fname = getWstr(currentPtr, strStartIndex, i - strStartIndex);
						break;
					case 2:
						lname = getWstr(currentPtr, strStartIndex, i - strStartIndex);
						break;
					case 3:
						mname = getWstr(currentPtr, strStartIndex, i - strStartIndex);
						break;
					case 4:
						street = getWstr(currentPtr, strStartIndex, i - strStartIndex);
						break;
					case 5:
						houseStr = getWstr(currentPtr, strStartIndex, i - strStartIndex);
						house = _wtoi(houseStr->c_str());
						break;
					case 6:
						housingStr = getWstr(currentPtr, strStartIndex, i - strStartIndex);
						housing = _wtoi(housingStr->c_str());
						break;
					case 7:
						flatStr =  getWstr(currentPtr, strStartIndex, i - strStartIndex);
						flat = _wtoi(flatStr->c_str());
						break;
					}
					field++;
					strStartIndex += i - strStartIndex + 1;
				}
			}
			regionOffset += i + 1;

			if ((r->telephone->empty() || *telephone == *r->telephone) && (r->fname->empty() || *fname == *r->fname)
				&& (r->lname->empty() || *lname == *r->lname) && (r->mname->empty() || *mname == *r->mname)
				&& (r->street->empty() || *street == *r->street) && (r->house == 0 || house == r->house)
				&& (r->housing == 0 || housing == r->housing) && (r->flat == 0 || flat == r->flat))
			{
				int telId = this->telephoneIndexer.tryAdd(*telephone);
				int fnameId = this->fnameIndexer.tryAdd(*fname);
				int lnameId = this->lnameIndexer.tryAdd(*lname);
				int mnameId = this->mnameIndexer.tryAdd(*mname);
				int streetId = this->streetIndexer.tryAdd(*street);
				result.push_back(new Record(&this->telephoneIndexer[telId],&this->fnameIndexer[fnameId],&this->lnameIndexer[lnameId],&this->mnameIndexer[mnameId],&this->streetIndexer[streetId],house, housing, flat));
			}
			delete telephone;
			delete lname;
			delete fname;
			delete mname;
			delete street;
			delete houseStr;
			delete housingStr;
			delete flatStr;
		}
		fileMappingOffset += fileMappingFrameSize;

		delete[] validText;
		UnmapViewOfFile(mappedTextPtr);
		
	}
	
	CloseHandle(hMem);
	CloseHandle(hFile);
	return result;
}

void PhoneBook::clear() {
	this->telephoneIndexer.clear();
	this->fnameIndexer.clear();
	this->mnameIndexer.clear();
	this->lnameIndexer.clear();
	this->streetIndexer.clear();
	this->telephoneSearchTree.clear();
	this->fnameSearchTree.clear();
	this->lnameSearchTree.clear();
	this->mnameSearchTree.clear();
	this->streetSearchTree.clear();
	this->houseSearchTree.clear();
	this->housingSearchTree.clear();
	this->flatSearchTree.clear();
	this->records.clear();
}

Record::Record(std::wstring* telephone, std::wstring* fname, std::wstring* lname, std::wstring* mname, std::wstring* street, int house, int housing, int flat) {
	this->telephone = telephone;
	this->fname = fname;
	this->lname = lname;
	this->mname = mname;
	this->street = street;
	this->house = house;
	this->housing = housing;
	this->flat = flat;
}