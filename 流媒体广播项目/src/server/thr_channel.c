/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2024-01-07 22:12:26
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-08 21:16:30
 * @FilePath: /src/server/thr_channel.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <proto.h>

#include "thr_channel.h"
#include "server_conf.h"
#include "medialib.h"

static int tid_nextpos = 0;
// 频道线程结构体 
struct thr_channel_ent_st
{
    chnid_t chnid;
    pthread_t tid;
};

struct thr_channel_ent_st thr_channel[CHNNR];

// 线程执行函数，发送频道数据包
static void* thr_channel_snder(void* ptr) {
    struct msg_channel_st *sbufp;
    struct mlib_listentry_st *ent = ptr;
    ssize_t len;
    sbufp = malloc(MSG_CHANNEL_MAX);
    if(sbufp == NULL) {
        syslog(LOG_ERR, "malloc():%s\n", strerror(errno));
        exit(1);
    }
    sbufp->chnid = ent->chnid;
    while(1) {
        len = mlib_readchn(ent->chnid, sbufp->data, MAX_DATA);
        if(sendto(server_sd, sbufp, len + sizeof(chnid_t), 0, (void *)&sndaddr, sizeof(sndaddr)) < 0) {
            syslog(LOG_ERR, "thr_channel(%d): sendto():%s",ent->chnid, strerror(errno));
            exit(1);
        }
        sched_yield();
    }
    pthread_exit(NULL);
}

// 创建一个处理频道的线程
int thr_channel_create(struct mlib_listentry_st *ptr) {
    int err = pthread_create(&thr_channel[tid_nextpos].tid, NULL, thr_channel_snder, ptr);
    if(err) {
        syslog(LOG_WARNING, "pthread_create():%s", strerror(err));
        return -err;
    }
    thr_channel[tid_nextpos].chnid = ptr->chnid;
    tid_nextpos++;
    return 0;
}

// 销毁一个处理频道的线程
int thr_channel_destroy(struct mlib_listentry_st *ptr) {
    for(int i = 0; i < CHNNR; ++i) {
        if(thr_channel[i].chnid == ptr->chnid) {
            if(pthread_cancel(thr_channel[i].tid) < 0) {
                syslog(LOG_ERR, "pthread_cancel():thr thread of channel %d", ptr->chnid);
                return -ESRCH;
            }
            pthread_join(thr_channel[i].tid, NULL);
            thr_channel[i].chnid = -1;
            return 0;
        }
        
    }
}

int thr_channel_destroy_all() {
    for(int i = 0; i < CHNNR; ++i) {
        if(thr_channel[i].chnid > 0) {
            if(pthread_cancel(thr_channel[i].tid) < 0) {
                syslog(LOG_ERR,"pthread_cancel():the thread of channel %d", thr_channel[i].chnid); 
                return -ESRCH;
            }
            pthread_join(thr_channel[i].tid, NULL);
            thr_channel[i].chnid = -1;
        }
    }
    return 0;
}