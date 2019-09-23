//
// Created by jahongir on 9/24/19.
//

#ifndef HIGHLOAD_TP_SETTINGS_H
#define HIGHLOAD_TP_SETTINGS_H

#include "Config.h"

class Settings {
public:
    unsigned short cpu_limit;
    string document_root;

    Settings();
    void ParseConfigFile(const string&);
};

#endif //HIGHLOAD_TP_SETTINGS_H
