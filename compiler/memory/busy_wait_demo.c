#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int flag = 0;  // 不使用volatile
int counter = 0;  // 防止编译器过度优化的计数器

void signal_handler(int sig) {
    flag = 1;
    printf("\n信号处理器: 设置flag为1\n");
}

int main() {
    signal(SIGINT, signal_handler);  // Ctrl+C触发
    
    printf("按 Ctrl+C 来触发信号...\n");
    printf("程序正在等待flag变为1...\n");
    
    // 添加一些"无用"操作来欺骗编译器
    while (flag == 0) {
        counter++;
        if (counter % 100000000 == 0) {  // 每1亿次循环打印一次
            printf("仍在等待... (计数: %d)\n", counter / 100000000);
        }
    }
    
    printf("flag变为1了，总共循环了 %d 次\n", counter);
    return 0;
}