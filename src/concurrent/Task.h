#ifndef OGS_TASK_H
#define OGS_TASK_H

#include <functional>
#include <utility>
namespace ogs {

using namespace std;


class Task {
public:
    int status = 0;
    int n = 0;
    std::function<void(void)> caller;
    void execute() const {
        if(caller != nullptr) {
            caller();
        }
    }
    explicit Task(int c): caller(nullptr), n(c) {};
    explicit Task(std::function<void(void)> call): caller(std::move(call)){};
    explicit Task(int c, std::function<void(void)> call): caller(std::move(call)), n(c){};
    void setFunction(std::function<void(void)> call) { caller = std::move(call); };
};

}
#endif //GAME_TASK_H
