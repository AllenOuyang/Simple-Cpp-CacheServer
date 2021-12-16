/*
 * @Author: your name
 * @Date: 2021-12-04 00:25:51
 * @LastEditTime: 2021-12-16 16:40:11
 * @LastEditors: Please set LastEditors
 * @Description: 
 * @FilePath: /Project/CacheServer/include/lru.hpp
 */
#ifndef _LRU_HPP
#define _LRU_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <mutex>
#include <shared_mutex>

struct DLinkedNode {
    std::string key, value;
    DLinkedNode* prev;
    DLinkedNode* next;
    DLinkedNode(): key(""), value(""), prev(nullptr), next(nullptr) {}
    DLinkedNode(std::string _key, std::string _value): key(_key), value(_value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
    friend class CacheServer;
    
private:
    std::unordered_map<std::string, DLinkedNode*> cache;
    DLinkedNode* head;
    DLinkedNode* tail;
    int size;
    int capacity;
    mutable std::shared_timed_mutex mutex_;

public:
    LRUCache();

    LRUCache(int _capacity);

    void set_capacity(int _capacity);

    int get_capacity();

    int get_size();
    
    std::string get(std::string key);
    
    void put(std::string key, std::string value);

    void addToHead(DLinkedNode* node);
    
    void removeNode(DLinkedNode* node);

    void moveToHead(DLinkedNode* node);
	
    DLinkedNode* removeTail();

};

#endif