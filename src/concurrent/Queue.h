#ifndef OGS_QUEUE_H
#define OGS_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <monitoring/Logsystem.h>

namespace ogs {


template<typename T>
class Queue {

private:
    class Node {
    public:
        Node():t(nullptr), next(nullptr), before(nullptr) {}; 
        T* t;
        Node* next;
        Node* before;
    };

    int count = 0;
    std::mutex mut;
    std::condition_variable condition;
    std::atomic<bool> open;
    Node* head = nullptr;
    Node* tail = nullptr;

    void deleteTasks(){
        while(head != nullptr) {
            Node* tmp = head;
            head = head->next;
            delete tmp->t;
            delete tmp;
        }
        count = 0;
    };

public:

    Queue(): count(0), open(true) {};

    void stop() {
        std::lock_guard<std::mutex> guard(mut);
        deleteTasks();
        open = false;
        condition.notify_all();
    }

    bool isStop() {
        return !open.load();
    }

    // Wake up all blocked pop() calls without stopping the queue.
    // Used when a single Worker needs to exit while others keep running.
    void wakeAll() {
        condition.notify_all();
    }

    void push(T* task) {
        std::lock_guard<std::mutex> guard(mut);
        Node* node = new Node;
        node->t = task;
        if(head == nullptr) {
            head = node;
        } else {
            if(tail == nullptr) {
                tail = head;
                head = node;
                head->next = tail;
                tail->before = head;
            } else {
                Node* tmp = head;
                head = node;
                head->next = tmp;
                tmp->before = head;
            }
        }
        count++;
        condition.notify_all();
    };

    T* popNoWait(){
        return pop(false);
    }

    T* pop(bool wait = true) {
        std::unique_lock<std::mutex> uniq(mut);
        if(wait){
            condition.wait(uniq, [&] {
                return count > 0 || !open.load();
            });
        }
        if(count == 0) {
            return nullptr;
        }
        T* t_task;
        if(count == 1) {
            if(head == tail) {
                tail = nullptr;
            }
            t_task = head->t;
            Node* tmp = head;
            head = nullptr;
            delete tmp;
        } else {
            t_task = tail->t;
            Node* before = tail->before;
            before->next = nullptr;
            delete tail;
            tail = before;
        }
        count--;
        return t_task;
    };

    bool isEmpty(){
        std::lock_guard guard(mut);
        return count == 0;
    };

    int size() {
        std::lock_guard guard(mut);
        return count;
    };
};

}


#endif
