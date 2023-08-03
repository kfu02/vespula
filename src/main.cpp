#include <stdio.h>
#include <thread>
#include "concurrentqueue.h"
#include "readerwriterqueue.h"

#define PROJECT_NAME "vespula"

int main(int argc, char **argv) {
    // example thread
    std::thread t([]() {
        printf("This is a thread.\n");
    });
    t.join();

    moodycamel::ConcurrentQueue<int> q;
    q.enqueue(25);

    int item;
    bool found = q.try_dequeue(item);
    printf("Found: %d\n", found);
    printf("Item: %d\n", item);

    return 0;
}
