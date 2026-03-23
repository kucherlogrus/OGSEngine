#ifndef OGS_THREADPOOLEXECUTOR_H
#define OGS_THREADPOOLEXECUTOR_H

#include <vector>
#include "Task.h"
#include "Worker.h"

namespace ogs {

class ThreadPoolExecutor {
private:
    std::vector<Worker*> workers;
    int worker_count;
    Queue<Task> taskqueue;
    int worker_index = 0;

    void start() {
        Worker* worker = nullptr;
        while(worker_index < worker_count) {
            worker = new Worker(&taskqueue, worker_index + 1);
            workers.push_back(worker);
            worker_index++;
        }
        for(auto _worker : workers){
            _worker->run();
        }
    }

public:
    explicit ThreadPoolExecutor(int worker_count = thread::hardware_concurrency()): worker_count(worker_count){
        loginfo("ThreadPoolExecutor", "construct");
        start();
    };
    ~ThreadPoolExecutor(){
        loginfo("ThreadPoolExecutor", "destruct");
        clear();
        
    };

    void submit(Task* task) {
        taskqueue.push(task);
    };

    bool hasWork() {

        for(auto worker : workers){
            if( worker->inWork() ) {
                return true;
            }
        }

        return false;
    }

    void clear() {
        for(auto worker : workers){
            worker->stop();
        }
        taskqueue.stop();
    }

    void addWorker() {
        auto worker = new Worker(&taskqueue, worker_index + 1);
        workers.push_back(worker);
        worker_index++;
        worker->run();
    }

    void removeWorker() {
        if (workers.size() == 0) {
            return; 
        }
        auto worker = workers.back();
        worker->stop();
        workers.pop_back();
        delete worker;
    }


};

}


#endif
