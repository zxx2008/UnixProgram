/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2024-01-02 21:23:42
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-03 21:34:14
 * @FilePath: /src/server/medialib.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: 媒体库
 */
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <proto.h>

#include "medialib.h"
#include "server_conf.h"
#include "mytbf.h"

typedef void mytbf_t;

// 路径大小
#define PATHSIZE 1024
#define LINEBUFSIZE 1024
#define MP3_BITRATE 320 * 1024

// 频道描述
struct channel_context_st
{
    chnid_t chnid;
    char *desc;
    glob_t mp3glob; //文件匹配
    int pos; //当前播放的文件
    int fd; //播放文件的文件描述符
    mytbf_t *tbf;   //流量控制
    off_t offset;
};

//频道数组
struct channel_context_st channel[MAXCHNID+1];

// 将某个目录下的所有文件转为一个频道
static struct channel_context_st* path2entry(const char* path) {
    syslog(LOG_INFO, "current path: %s\n", path);
    char pathstr[PATHSIZE];
    char linebuf[LINEBUFSIZE];
    // 可能会越界，这部分处理不好
    char *context = (char *)malloc(sizeof(char) * LINEBUFSIZE * 5);
    context[0] = '\0';
    FILE *fp;
    struct channel_context_st *me;

    static chnid_t curr_id = MINCHNID;

    // 检测目录合法性
    strcat(pathstr, path);
    strcat(pathstr, "/desc.txt");
    fp = fopen(pathstr, "r"); //打开频道描述文件
    syslog(LOG_INFO, "channel dir: %s\n", pathstr);
    if (fp == NULL) {
        syslog(LOG_INFO, "%s is not a channel, dir(can not find desc.txt)", path);
        fclose(fp);
        return NULL;
    }

    // 读取文件desc.txt内容
    while (fgets(linebuf, LINEBUFSIZE, fp) != NULL) {
        strcat(context, linebuf);
    }
    fclose(fp);
    
    // 初始化频道结构体
    me = malloc(sizeof(*me));
    if(me == NULL) {
        syslog(LOG_ERR, "malloc(): %s", strerror(errno));
        free(me);
        free(context);
        return NULL;
    }

    me->desc = strdup(context);  // 初始化频道描述
    free(context);
    me->tbf = mytbf_init(MP3_BITRATE / 8, MP3_BITRATE / 8 *5) // 初始化流速控制器
    if(me->tbf == NULL) {
        syslog(LOG_ERR, "mytbf_init()");
        free(me);
        return NULL;
    }

    // 搜索path下的.mp3文件
    strncpy(pathstr, path, PATHSIZE);
    strncat(pathstr, "/*.mp3", 6);
    if(glob(pathstr, 0, NULL, &me->mp3glob) != 0) {
        //curr_id++; 
        syslog(LOG_ERR, "%s is not a channel dir(can not find mp3 files)", path);
        free(me);
        return NULL;
    }

    me->pos = 0;
    me->offset = 0;
    me->fd = open(me->mp3glob.gl_pathv[me->pos], O_RDONLY); //打开第一个mp3文件
    if(me->fd < 0) {
        syslog(LOG_WARNING, "%s open failed.", me->mp3glob.gl_pathv[me->pos]);
        free(me);
        return NULL;
    }
    me->chnid = curr_id;
    curr_id++;
    return me;
    
}

int mlib_getchnlist(struct mlib_listentry_st **result, int *resnum) {
    // 存放目录
    char path[PATHSIZE];

    glob_t globres;
    // 目录中匹配项目计数器
    int num = 0;
    struct mlib_listentry_st *ptr;
    struct channel_context_st *res;
    for(int i = 0; i < MAXCHNID + 1; ++i) {
        channel[i].chnid = -1;
    }

    snprintf(path, PATHSIZE, "%s/*", server_conf.media_dir);
    //解析目录中文件
    if(glob(path, 0, NULL, &globres)) {
        return -1;
    }

    ptr = malloc(sizeof(struct mlib_listentry_st) * globres.gl_pathc);
    if(ptr == NULL) {
        syslog(LOG_ERR, "malloc() error.");
        exit(1);
    }

    for(i = 0; i < globres.gl_pathc; ++i) {
        res = path2entry(globres.gl_pathc[i]);
        if(res != NULL) {
            syslog(LOG_INFO, "path2entry() return : %d %s.", res->chnid, res->desc);
            memcpy(channel + res->chnid, res, sizeof(*res));
            (ptr+num)->chnid = res->chnid;
            (ptr+num)->desc = strdup(res->desc);
            num++;
        }
    }

    *result = realloc(ptr, sizeof(struct mlib_listentry_st) * num);
    if (*result == NULL) {
        syslog(LOG_ERR, "realloc() faild: %s", strerror(errno));
        exit(1);
    }
    *resnum = num;

    return 0;
}
