/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-25 22:30:49
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2023-12-25 22:41:42
 * @FilePath: /src/client/client.c
 * @Description: 客户端，主进程使用socket接受数据，交给子进程进行解码播放，父子进程通信使用管道进行
 */
#include <stdio.h>
#include <stdlib.h>

/*
* -M --mgroup 指定多播组
* -P --port 指定接收器端口
* -p --player 指定播放器
* -H --help   显示帮助
*/

int main(int argc, char** argv) {
     
    //pipe();

}