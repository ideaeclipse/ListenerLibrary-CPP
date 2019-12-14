#include <iostream>
#include "ListenerLibrary.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *listenerLibrary = new ListenerLibrary<int>();

    listenerLibrary->add_function(0, [](int a) {
        std::cout << "Function 1 with id: " << a << std::endl;
    });
    listenerLibrary->add_function(0, [](int a) {
        std::cout << "Function 2 with id: " << a << std::endl;
    });
    listenerLibrary->add_function(0, [](int a) {
        std::cout << "Function 3 with id: " << a << std::endl;
    });

    listenerLibrary->add_function(1, [](int a) {
        std::cout << "Function 1 with id: " << a << std::endl;
    });
    listenerLibrary->add_function(1, [](int a) {
        std::cout << "Function 2 with id: " << a << std::endl;
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