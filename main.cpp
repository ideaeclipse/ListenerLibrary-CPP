#include <iostream>
#include "ListenerLibrary.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *listenerLibrary = new ListenerLibrary<int>();

    listenerLibrary->add_function(0, [](int a) {
        printf("Function 1 with id: %d\n", a);
    });
    listenerLibrary->add_function(0, [](int a) {
        printf("Function 2 with id: %d\n", a);
    });
    listenerLibrary->add_function(0, [](int a) {
        printf("Function 3 with id: %d\n", a);
    });

    listenerLibrary->add_function(1, [](int a) {
        printf("Function 1 with id: %d\n", a);
    });
    listenerLibrary->add_function(1, [](int a) {
        printf("Function 2 with id: %d\n", a);
    });


    printf("Waiting for nodes with caller_id 0\n");

    listenerLibrary->notify_listeners(0, 0);

    listenerLibrary->wait();

    printf("Finished waiting on nodes with caller_id 0\n");

    printf("Waiting for nodes with caller_id 1\n");

    listenerLibrary->notify_listeners(1, 1);

    listenerLibrary->wait();

    printf("Finished waiting on nodes with caller_id 1\n");

    delete listenerLibrary;

    printf("Program complete\n");

    return 0;
}
