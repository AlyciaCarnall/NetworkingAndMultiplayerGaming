#include "game.hpp"
#include <thread>
#include <iostream>
#include <condition_variable>

//struct BoundedBuffer {
//    int* buffer;  // Here, only ints as "messages"
//    int capacity; // Max number of elements
//    int front;    // Read
//    int rear;     // Write
//    int count;
//
//    std::mutex lock;
//
//    std::condition_variable not_full;
//    std::condition_variable not_empty;
//    };

int main()
{
//    std::thread([]{
//        std::cout << "Hi from thread\n";
//    }).join();
    game g;
    g.run();
    return 0;
}
