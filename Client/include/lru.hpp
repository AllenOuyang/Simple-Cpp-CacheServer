#ifndef _LRU_HPP
#define _LRU_HPP

#include <iostream>
#include <unordered_map>
using namespace std;

struct DLinkedNode {
    int key, value;
    DLinkedNode* prev;
    DLinkedNode* next;
    DLinkedNode(): key(0), value(0), prev(nullptr), next(nullptr) {}
    DLinkedNode(int _key, int _value): key(_key), value(_value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    unordered_map<int, DLinkedNode*> cache;
    DLinkedNode* head;
    DLinkedNode* tail;
    int size;
    int capacity;

public:
    LRUCache(int _capacity);
    
    int get(int key);
    
    void put(int key, int value);

    void addToHead(DLinkedNode* node);
    
    void removeNode(DLinkedNode* node);

    void moveToHead(DLinkedNode* node);
	
    DLinkedNode* removeTail();

};

#endif