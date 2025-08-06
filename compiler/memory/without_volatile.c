#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// 不使用volatile的例子
int flag = 0;

void* thread_function(void* arg) {
    sleep(2);  // 等待2秒
    flag = 1;
    printf("Thread: 设置flag为1\n");
    return NULL;
}

int main() {
    pthread_t thread;
    
    printf("=== 不使用 volatile ===\n");
    pthread_create(&thread, NULL, thread_function, NULL);
    
    printf("主线程等待flag变为1...\n");
    while (flag == 0) {
        // 编译器可能优化这个循环为死循环
        // 特别是在-O2优化级别下
    }
    
    printf("flag变为了1!\n");
    pthread_join(thread, NULL);
    return 0;
}