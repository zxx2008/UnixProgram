#include <stdio.h>
#include <stdlib.h>

#include "medialib.h"

typedef void mytbf_t;


// 频道描述
struct channel_context_st
{
    chnid_t chnid;
    char *desc;
    glob_t mp3glob; //文件匹配
    int pos; //当前播放的文件
    int fd; //播放文件的文件描述符
    mytbf_t *tbf;   //流量控制
};
