/*
 * @Author: Zu Xixin 2665954635@qq.com
 * @Date: 2024-01-05 15:30:44
 * @LastEditors: Zu Xixin 2665954635@qq.com
 * @LastEditTime: 2024-01-05 15:30:42
 * @FilePath: /src/server/mytbf.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*流量控制，令牌桶实现*/

#ifndef _MYTBF_H_
#define _MYTBF_H_

#define MYTBF_MAX 1024

typedef void mytbf_t;

/// @brief 初始化令牌桶
/// @param caps 令牌桶流速
/// @param burst 令牌数量上限
/// @return 
mytbf_t *mytbf_init(int caps, int burst);

/// @brief 从令牌桶中取令牌
/// @param  令牌桶
/// @param  取出的令牌数量
/// @return 返回状态
int mytbf_fetchtoken(mytbf_t *, int);

/// @brief 返还令牌
/// @param  令牌桶
/// @param  返还数量
/// @return 状态
int mytbf_returntoken(mytbf_t*, int);

/// @brief 销毁令牌桶
/// @param  令牌桶对象
/// @return 状态
int mytbf_destroy(mytbf_t *);

/// @brief 获取当前令牌数
/// @param ptr 
/// @return 当前令牌数
int mytbf_checktoken(mytbf_t *ptr);

#endif