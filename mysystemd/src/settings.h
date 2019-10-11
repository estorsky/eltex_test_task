#ifndef SETTINGS_H
#define SETTINGS_H

extern char const PATH_DEAMONS[];
extern char const CONFIG_FILE[];
extern char const ZLOG_CONFIG_FILE[];

enum Settings
{
    LIMIT_NON_RESPONSE = 2,
    DELAY_ROUND_SEC = 10,
};

#endif

