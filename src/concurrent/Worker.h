#ifndef OGS_WORKER_H
#define OGS_WORKER_H

#include <thread>
#include <atomic>
#include "Task.h"
#include "Queue.h"


namespace ogs {


class Worker {
public:
    std::atomic<bool> needStop;
    int state = 0;
    short id;
    thread* worked_t = nullptr;
    Queue<Task>* worked_q;
    Worker(Queue<Task>* q): worked_q(q),id(-1), needStop(false) {};
    Worker(Queue<Task>* q, short i): worked_q(q),id(i), needStop(false) {};
    ~Worker() {
        loginfo("Worker", "destroy", "id:" , id);
    }

    void join() {
        if(worked_t != nullptr){
            worked_t->join();
        }
    }

    void stop() {
        needStop = true;
    }

    bool inWork() {
        return state > 0;
    }

    void run() {
        worked_t = new thread( [this]() {
            while(true) {
                if(worked_q->isStop() || needStop) break;
                Task* task = worked_q->pop();
                if(task != nullptr){
                    state = 1;
                    loginfo("Worker", "id:", id, "Worker start task:");
                    task->execute();
                    loginfo("Worker", "id:", id, "Worker end task:");
                    state = 0;
                    delete task;
                }

            }
            loginfo("Worker", "id:", id, "Worker exit from queue loop");
        });
    }
};

}
#endif //WORKER_H
