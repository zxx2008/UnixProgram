#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <proto.h>

#include "thr_list.h"
#include "server_conf.h"

static pthread_t tid_list; //节目单频道线程号
static int num_list_entry;
static struct mlib_listentry_st *list_entry; //频道列表

static void *thr_list(void *p) {
    int totalsize;
    struct msg_list_st *entrylistptr;   //节目单频道描述
    struct msg_listentry_st *entryptr;  //其他频道描述

    int ret;
    int size;

    // 计算需要传输的字节数
    totalsize = sizeof(chnid_t);
    for(int i = 0; i < num_list_entry; ++i) {
        totalsize += sizeof(struct msg_listentry_st) + strlen(list_entry[i].desc);
    }

    entrylistptr = malloc(totalsize);
    if(entrylistptr == NULL) {
        syslog(LOG_ERR, "malloc(): %s", strerror(errno));
        exit(1);
    }
    entrylistptr->chnid = LISTCHNID;
    entryptr = entrylistptr->entry; // 不再为entryptr重新分配内存，直接使用entrylistptr中entry数组的内存使用
    syslog(LOG_DEBUG, "num_list_entry: %d\n", num_list_entry);
    for(int i = 0; i < num_list_entry; ++i) {
        size = sizeof(struct msg_listentry_st) + strlen(list_entry[i].desc);
        entryptr->chnid = list_entry[i].chnid;
        entryptr->len = htons(size);
        strcpy(entryptr->desc, list_entry[i].desc); //可以直接使用strcpy，因为entrylistptr->entry内存已经在分配完成
        entryptr = (void *)(((char *)entryptr) + size); //指针向后移动
        syslog(LOG_DEBUG, "entry len: %d\n", entryptr->len);
    }

    while(1) {
        //syslog(LOG_INFO, "thr_list sn")
        ret = sendto(server_sd, entrylistptr, totalsize, 0, (void *)&sndaddr, sizeof(sndaddr));
        if(ret < 0) {
            syslog(LOG_WARNING, "sendto(server_sd, ...: %s)", strerror(errno));
        }
        else {
            syslog(LOG_DEBUG, "sendto(server_sd ...): succeed.");
        }
        sleep(1);
    }

}

int thr_list_create(struct mlib_listentry_st *listptr, int num_ent) {
    list_entry = listptr;
    num_list_entry = num_ent;
    syslog(LOG_DEBUG, "list content: chnid:%d, desc:%s\n", listptr->chnid, listptr->desc);
    int err = pthread_create(&tid_list, NULL, thr_list, NULL);
    if(err) {
        syslog(LOG_ERR, "pthread_create():%s", strerror(errno));
        return -1;
    }
    return 0;
}

int thr_list_destory(void) {
    pthread_cancel(tid_list);
    pthread_join(tid_list, NULL);
    return 0;
}