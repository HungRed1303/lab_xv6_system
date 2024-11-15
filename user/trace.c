#include "user.h"
#include "syscall.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        
        return 0;
    }

    //Lay gia tri mask tu tham so dau vao
    int mask = atoi(argv[1]);
    if(trace(mask) < 0) {
        printf("trace failed\n");
        return 1;
    }

    if(argc > 2) {
        char* new_argv[argc-1];
        for(int i=2; i<argc; i++) {
            new_argv[i-2] = argv[i];
        }
        new_argv[argc-2] = 0;
        // Thuc thi lenh voi trace bat
        exec(new_argv[0], new_argv);
        printf("exec failed\n");
        exit(1);
    }
    exit(0);
}