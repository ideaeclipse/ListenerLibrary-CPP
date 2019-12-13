//
// Created by myles on 2019-12-12.
//

#include <thread>
#include <future>
#include "ListenerLibrary.h"


template<typename... P>
ListenerLibrary<P...>::ListenerLibrary() {
    this->map = new std::unordered_map<int, std::vector<Function> *>();
}

template<typename... P>
void
ListenerLibrary<P...>::execute_on_seperate_thread(const Function function,
                                                  const P... params) {
    std::async(function, params...);
    this->running_process -= 1;
}


template<typename... P>
void ListenerLibrary<P...>::add_function(const int caller_id, const Function caller_function) {
    if (this->map->count(caller_id) == 1) {
        std::vector<Function> *set = this->map->at(caller_id);
        set->push_back(caller_function);
    } else {
        std::vector<Function> *set = new std::vector<Function>();
        set->push_back(caller_function);
        this->map->insert(std::make_pair(caller_id, set));
    }
}

template<typename... P>
void ListenerLibrary<P...>::notify_listeners(const int caller_id, const P... params) {
    if (this->map->count(caller_id) == 1) {
        std::vector<Function> *set = this->map->at(caller_id);
        for (auto x : *set) {
            std::thread{this->execute_on_seperate_thread, this, x, params...}.detach();
            this->running_process += 1;
        }
    } else
        throw "Caller_Id does not exist";
}

template<typename... P>
void ListenerLibrary<P...>::wait() {
    while (this->running_process > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

template<typename... P>
ListenerLibrary<P...>::~ListenerLibrary() {
    this->wait();

    for (auto x : *this->map) {
        x.second->clear();
        delete x.second;
    }

    this->map->clear();

    delete this->map;
}
