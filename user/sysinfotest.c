#include "types.h"
#include "user.h"
#include "sysinfo.h"  // Bao gồm sysinfo.h đã chứa struct sysinfo

int main(void) {
    struct sysinfo info;

    // Gọi syscall sysinfo để lấy thông tin hệ thống
    if (sysinfo(&info) < 0) {
        printf("sysinfo failed\n");  // In lỗi nếu syscall thất bại
        exit(1);  // Thoát với mã lỗi 1
    }

    // In thông tin hệ thống ra màn hình
    printf("Free memory: %d\n", info.freemem);
    printf("Number of processes: %d\n", info.nproc);
    printf("Sysinfotest: OK \n");

    exit(0);  // Thoát bình thường
}
