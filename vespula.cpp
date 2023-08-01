#include <stdio.h>
#include <thread>

#define PROJECT_NAME "vespula"

int main(int argc, char **argv) {
    if(argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }

    std::thread t([]() {
        printf("This is a thread.\n");
    });
    t.join();

    printf("This is project %s.\n", PROJECT_NAME);
    return 0;
}
