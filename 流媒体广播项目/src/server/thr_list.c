#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#include <proto.h>

#include "thr_list.h"

static pthread_t tid_list //节目单频道线程号
static int num_list_entry;
static struct mlib_listentry_st *list_entry; //频道列表

static void *thr_list(void *p) {
    int totalsize;
    struct msg_list_st *entrylistptr;   //节目单描述
    struct msg_listentry_st *entryptr;  //其他频道描述

    int ret;
    int size;

    // 计算需要传输的字节数
    totalsize = sizeof(chnid_t);
    for(int i = 0; i < )

}

int thr_list_create(struct mlib_listentry_st *listptr, int num_ent) {
    list_entry = listptr;
    num_list_entry = num_ent;
    syslog(LOG_DEBUG, "list content: chnid:%d, desc:%s\n", listptr->chnid, listptr->desc);
    err = pthread_create(&tid_list, NULL, thr_list, NULL);
    if(err) {
        syslog(LOG_ERR, "pthread_create():%s", strerror(errno));
        return -1;
    }
    return 0;
}