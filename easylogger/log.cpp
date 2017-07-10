//
// Created by wangyi on 16-7-15.
//
#include "log.h"

/*
 * Esaylogger 初始化,各等级日志格式配置
 */
void elog_init_config(void)
{
    setbuf(stdout, NULL);
    elog_init();
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL |  ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TIME |ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TIME | ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_TAG & ~ELOG_FMT_DIR);/*TODO: 去掉DIR*/
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_TAG & ~ELOG_FMT_DIR);
    elog_start();
}

/*TODO:配置文件控制 ,ok*/
/*
 * 设置日志的输出等级
 */
void set_elog_filer_lvl(const char *level_config)
{
    uint8_t filer_level = ELOG_LVL_DEBUG;
    if(!strcmp(level_config, "ELOG_LVL_VERBOSE")) {
        filer_level = ELOG_LVL_VERBOSE;
    } else if(!strcmp(level_config, "ELOG_LVL_ASSERT")) {
        filer_level = ELOG_LVL_ASSERT;
    } else if(!strcmp(level_config, "ELOG_LVL_ERROR")) {
        filer_level = ELOG_LVL_ERROR;
    }else if(!strcmp(level_config, "ELOG_LVL_WARN")) {
        filer_level = ELOG_LVL_WARN;
    }else if(!strcmp(level_config, "ELOG_LVL_INFO")) {
        filer_level = ELOG_LVL_INFO;
    }else if(!strcmp(level_config, "ELOG_LVL_DEBUG")) {
        filer_level = ELOG_LVL_DEBUG;
    }else {
        log_e("incorrect mini elog filter level config[%s], use default level[ELOG_LVL_VERBOSE]", level_config);
    }
    log_i("elog filter level[%d]", filer_level);
    elog_set_filter_lvl(filer_level); //Esaylogger 日志等级屏蔽设置
}