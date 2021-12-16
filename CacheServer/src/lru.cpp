#include "lru.hpp"

LRUCache::LRUCache() : capacity(0), size(0)
{
    // 使用伪头部和伪尾部节点
    head = new DLinkedNode();
    tail = new DLinkedNode();
    head->next = tail;
    tail->prev = head;
}

LRUCache::LRUCache(int _capacity) : size(0), capacity(_capacity)
{
    // 使用伪头部和伪尾部节点
    head = new DLinkedNode();
    tail = new DLinkedNode();
    head->next = tail;
    tail->prev = head;
}

void LRUCache::set_capacity(int _capacity)
{
    capacity = _capacity;
}

int LRUCache::get_capacity()
{
    return capacity;
}

int LRUCache::get_size()
{
    return size;
}

std::string LRUCache::get(std::string key)
{
    std::shared_lock<std::shared_timed_mutex> lock(mutex_);
    if (!cache.count(key))
    {
        return "";
    }
    // 如果 key 存在，先通过哈希表定位，再移到头部
    DLinkedNode *node = cache[key];
    moveToHead(node);
    return node->value;
}

void LRUCache::put(std::string key, std::string value)
{
    std::unique_lock<std::shared_timed_mutex> lock(mutex_);
    if (!cache.count(key))
    {
        // 如果 key 不存在，创建一个新的节点
        DLinkedNode *node = new DLinkedNode(key, value);
        // 添加进哈希表
        cache[key] = node;
        // 添加至双向链表的头部
        addToHead(node);
        ++size;
        if (size > capacity)
        {
            // 如果超出容量，删除双向链表的尾部节点
            DLinkedNode *removed = removeTail();
            // 删除哈希表中对应的项
            cache.erase(removed->key);
            // 防止内存泄漏
            delete removed;
            --size;
        }
    }
    else
    {
        // 如果 key 存在，先通过哈希表定位，再修改 value，并移到头部
        DLinkedNode *node = cache[key];
        node->value = value;
        moveToHead(node);
    }
}

void LRUCache::addToHead(DLinkedNode *node)
{
    node->prev = head;
    node->next = head->next;
    head->next->prev = node;
    head->next = node;
}

void LRUCache::removeNode(DLinkedNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void LRUCache::moveToHead(DLinkedNode *node)
{
    removeNode(node);
    addToHead(node);
}

DLinkedNode* LRUCache::removeTail()
{
    DLinkedNode *node = tail->prev;
    removeNode(node);
    return node;
}
