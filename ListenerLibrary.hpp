//
// Created by myles on 2019-12-12.
//
#ifndef LISTENERLIBRARY_CPP_LISTENERLIBRARY_H
#define LISTENERLIBRARY_CPP_LISTENERLIBRARY_H


#include <unordered_map>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>
#include <vector>

template<typename... P>
class ListenerLibrary {
private:

  /**
   * Function definition for readability
   */
  using Function = std::function<void(P...)>;

  /**
   * Map of list of functions to notify.
   */
  std::unordered_map<int, std::vector<Function> *> *map;

  /**
   * Counts the number of sub running processes.
   */
  std::atomic<int> running_process{0};

  /**
   * Mutex for waiting for threads to complete
   */
  std::mutex mutex;

  /**
   * Conditional variable for waiting for threads to complete
   */
  std::condition_variable condition_variable;

  /**
   * Executes each element of some vector on a separate detached thread to allow for the main thread to continue to run.
   *
   * @param function function to execute
   * @param params params to pass to the listener
   */
  void execute_on_seperate_thread(const Function function, const P... params) {
    function(params...);
    this->running_process -= 1;
    if (this->running_process == 0) {
      std::unique_lock<std::mutex> lk(this->mutex);
      lk.unlock();
      this->condition_variable.notify_one();
    }
  }

public:

  /**
   * Initializes map.
   */
  ListenerLibrary() {
    this->map = new std::unordered_map<int, std::vector<Function> *>();
  }

  /**
  * Adds function to the map with that specific caller_id
  *
  * @param caller_id caller_id to recognize this function
  * @param caller_function caller function associated with caller_id
  */
  void add_function(const int caller_id, const Function caller_function) {
    if (this->map->count(caller_id) == 1) {
      std::vector<Function> *set = this->map->at(caller_id);
      set->push_back(caller_function);
    } else {
      std::vector<Function> *set = new std::vector<Function>();
      set->push_back(caller_function);
      this->map->insert(std::make_pair(caller_id, set));
    }
  }

  /**
   * Call functions with this designated caller id.
   *
   * If the caller_id does not exist an error will be thrown. Make sure to catch the exception.
   *
   * @param caller_id caller_id to call
   * @param params for functions
   */
  void notify_listeners(const int caller_id, const P... params) {
    if (this->map->count(caller_id) == 1) {
      std::vector<Function> *set = this->map->at(caller_id);
      for (auto x : *set) {
        std::thread{&ListenerLibrary<P...>::execute_on_seperate_thread, this, x, params...}.detach();
        this->running_process += 1;
      }
    } else
      throw "Caller_Id does not exist";
  }

  /**
   * Caller functions are notified on a separate thread. You can call this function to halt the main thread before
   * they finish, otherwise don't call it if you don't want that behaviour
   */
  void wait() {
    if (this->running_process > 0) {
      std::unique_lock<std::mutex> lk(this->mutex);
      this->condition_variable.wait(lk);
    }
  }

  /**
   * Garbage collection and calls wait to ensure that all child threads are completed before program termination
   */
  ~ListenerLibrary() {
    this->wait();

    for (auto x : *this->map) {
      x.second->clear();
      delete x.second;
    }

    this->map->clear();

    delete this->map;
  }

};


#endif //LISTENERLIBRARY_CPP_LISTENERLIBRARY_H
