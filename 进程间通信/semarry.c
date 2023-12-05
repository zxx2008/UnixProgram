/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-05 22:03:50
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-05 22:03:43
 * @FilePath: /进程间通信/semarry.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdio.h>

#define PROCNUM 20 //子进程数量
#define FNAME "/tmp/out"
#define BUFSIZE 1024

// 多个函数都要使用这个信号量ID，所以定义为全局变量
static int semid;

//PV操作中的P，占用一个资源


//PV操作中的V，释放一个资源


//子进程函数


int main() {
    //获得信号量id
}