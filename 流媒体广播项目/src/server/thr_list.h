/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2024-01-05 16:21:35
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-05 16:21:33
 * @FilePath: /src/server/thr_list.h
 * @Description: 节目单频道线程
 */
// 节目单频道线程
#ifndef _THR_LIST_H_
#define _THR_LIST_H_

#include "medialib.h"

int thr_list_create(struct mlib_listentry_st *, int);
int thr_list_destory();


#endif