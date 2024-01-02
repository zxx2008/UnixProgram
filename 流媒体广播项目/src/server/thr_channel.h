/*普通频道线程*/
#ifndef _THR_CHANNEL_H_
#define _THR_CHANNEL_H_

#include "medialib.h"

//创建一个普通频道线程
int thr_channel_create(struct mlib_listentry_st *);

//销毁一个普通频道线程
int thr_channel_destroy(struct mlib_listentry_st *);

// 销毁所有普通频道线程
int thr_channel_destroy_all();

#endif