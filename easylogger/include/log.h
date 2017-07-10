//
// Created by wangyi on 16-7-15.
//

#ifndef SC_LOG_H
#define SC_LOG_H


#include "elog.h"
#include "elog_cfg.h"
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define log_a(...) elog_a("", __VA_ARGS__)
#define log_e(...) elog_e("", __VA_ARGS__)
#define log_w(...) elog_w("", __VA_ARGS__)
#define log_i(...) elog_i("", __VA_ARGS__)
#define log_d(...) elog_d("", __VA_ARGS__)
#define log_v(...) elog_v("", __VA_ARGS__)

/**
 * \brief Esaylogger 初始化,各等级日志格式配置
 */
void elog_init_config(void);

/**
 * \brief 设置日志的输出等级
 */
void set_elog_filer_lvl(const char *level_config);

#endif //SC_LOG_H
