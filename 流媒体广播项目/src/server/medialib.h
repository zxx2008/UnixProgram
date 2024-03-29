/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2023-12-29 11:15:33
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-03 21:23:40
 * @FilePath: /src/server/medialib.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _MEDIALIB_H_
#define _MEDIALIB_H_

#include "../include/site_type.h"
// #include <site_type>

// 频道信息的内容
struct mlib_listentry_st
{
    chnid_t chnid;  //频道号
    char *desc; //频道描述
};

// 创建mlib_listentry_st对象指针数组, 表示每个频道
int mlib_getchnlist(struct mlib_listentry_st **, int *);

// 释放对象
int mlib_freechnlist(struct mlib_listentry_st *);

ssize_t mlib_readchn(chnid_t, void *, size_t);
#endif