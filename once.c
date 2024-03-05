/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2024-03-05 13:58:10
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-03-05 13:58:00
 * @FilePath: /UnixProgram/once.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <pthread.h>

pthread_once_t once_control = PTHREAD_ONCE_INIT;

void init_function(void) {
    printf("Initialization function is called\n");
}

void* thread_function(void* arg) {
    pthread_once(&once_control, init_function);
    printf("Thread is continuing...\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
