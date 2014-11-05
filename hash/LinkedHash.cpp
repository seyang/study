#include <stdio.h>
#include <iostream>

#define TABLE_SIZE 1024

using namespace std;

class LinkedHashEntry {
	private:
		string key;
		int value;
		LinkedHashEntry *next;
	public:
		LinkedHashEntry(string key, int value) {
			this->key = key;
			this->value = value;
			this->next = NULL;
		}

		string getKey() {
			return key;
		}

		int getValue() {
			return value;
		}

		void setValue(int value) {
			this->value = value;
		}

		LinkedHashEntry *getNext() {
			return next;
		}

		void setNext(LinkedHashEntry *next) {
			this->next = next;
		}
};

class HashMap {
	private:
		LinkedHashEntry **table;
	public:
		HashMap() {
			table = new LinkedHashEntry*[TABLE_SIZE];
			for (int i = 0; i < TABLE_SIZE; i++)
				table[i] = NULL;
		}

		unsigned long hashFunc(const char *str) {
			unsigned long hash = 5381;
			int c;
			while ((c = *str++) != '\0') {
				hash = ((hash << 5) + hash) + c;
			}

			return hash;
		}

		int get(string key) {
			int hash = static_cast<int>((hashFunc(key.c_str()) % TABLE_SIZE));
			cout << "Hash: " << hash << endl;
			if (table[hash] == NULL)
				return -1;
			else {
				LinkedHashEntry *entry = table[hash];
				while (entry != NULL && entry->getKey() != key)
					entry = entry->getNext();
				if (entry == NULL)
					return -1;
				else
					return entry->getValue();
			}
		}

		void put(const string key, int value) {
			int hash = static_cast<int>((hashFunc(key.c_str()) % TABLE_SIZE));
			if (table[hash] == NULL)
				table[hash] = new LinkedHashEntry(key, value);
			else {
				LinkedHashEntry *entry = table[hash];
				while (entry->getNext() != NULL)
					entry = entry->getNext();
				if (entry->getKey() == key)
					entry->setValue(value);
				else
					entry->setNext(new LinkedHashEntry(key, value));
			}
		}

		void remove(const string key) {
			int hash = static_cast<int>((hashFunc(key.c_str()) % TABLE_SIZE));
			if (table[hash] != NULL) {
				LinkedHashEntry *prevEntry = NULL;
				LinkedHashEntry *entry = table[hash];
				while (entry->getNext() != NULL && entry->getKey() != key) {
					prevEntry = entry;
					entry = entry->getNext();
				}
				if (entry->getKey() == key) {
					if (prevEntry == NULL) {
						LinkedHashEntry *nextEntry = entry->getNext();
						delete entry;
						table[hash] = nextEntry;
					} else {
						LinkedHashEntry *next = entry->getNext();
						delete entry;
						prevEntry->setNext(next);
					}
				}
			}
		}

		~HashMap() {
			for (int i = 0; i < TABLE_SIZE; i++)
				if (table[i] != NULL) {
					LinkedHashEntry *prevEntry = NULL;
					LinkedHashEntry *entry = table[i];
					while (entry != NULL) {
						prevEntry = entry;
						entry = entry->getNext();
						delete prevEntry;
					}
				}
			delete[] table;
		}
};

int main() {
	HashMap map;

	map.put("A10", 10);
	
	cout << map.get("A10") << endl;

	return 0;
}
