//
// Created by myles on 2019-12-12.
//

#ifndef LISTENERLIBRARY_CPP_LISTENERLIBRARY_H
#define LISTENERLIBRARY_CPP_LISTENERLIBRARY_H

#include <unordered_map>
#include <functional>

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
    int running_process = 0;

    /**
     * Executes each element of some vector on a separate detached thread to allow for the main thread to continue to run.
     *
     * @param function function to execute
     * @param params params to pass to the listener
     */
    void execute_on_seperate_thread(const Function function, const P... params);

public:

    /**
     * Initializes map.
     */
    ListenerLibrary();

    /**
 * Adds function to the map with that specific caller_id
 *
 * @param caller_id caller_id to recognize this function
 * @param caller_function caller function associated with caller_id
 */
    void add_function(const int caller_id, const Function caller_function);

    /**
     * Call functions with this designated caller id.
     *
     * If the caller_id does not exist an error will be thrown. Make sure to catch the exception.
     *
     * @param caller_id caller_id to call
     * @param params for functions
     */
    void notify_listeners(const int caller_id, const P... params);

    /**
     * Caller functions are notified on a separate thread. You can call this function to halt the main thread before
     * they finish, otherwise don't call it if you don't want that behaviour
     */
    void wait();

    /**
     * Garbage collection and calls wait to ensure that all child threads are completed before program termination
     */
    ~ListenerLibrary();

};


#endif //LISTENERLIBRARY_CPP_LISTENERLIBRARY_H
