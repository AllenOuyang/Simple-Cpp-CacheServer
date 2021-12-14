/*
 * @Author: your name
 * @Date: 2021-12-04 00:25:51
 * @LastEditTime: 2021-12-14 21:39:47
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /Project/CacheServer/include/lru.hpp
 */
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
    friend class CacheServer;
    
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