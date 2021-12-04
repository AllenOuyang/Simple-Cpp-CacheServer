#ifndef _LRU_HPP
#define _LRU_HPP

#include <iostream>
#include <unordered_map>
#include <string>

struct DLinkedNode {
    std::string key, value;
    DLinkedNode* prev;
    DLinkedNode* next;
    DLinkedNode(): key(""), value(""), prev(nullptr), next(nullptr) {}
    DLinkedNode(std::string _key, std::string _value): key(_key), value(_value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    std::unordered_map<std::string, DLinkedNode*> cache;
    DLinkedNode* head;
    DLinkedNode* tail;
    int size;
    int capacity;

public:
    LRUCache();

    LRUCache(int _capacity);

    void set_capacity(int _capacity);
    
    std::string get(std::string key);
    
    void put(std::string key, std::string value);

    void addToHead(DLinkedNode* node);
    
    void removeNode(DLinkedNode* node);

    void moveToHead(DLinkedNode* node);
	
    DLinkedNode* removeTail();

};

#endif