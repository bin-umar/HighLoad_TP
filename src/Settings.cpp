//
// Created by jahongir on 9/24/19.
//

#include "../include/Settings.h"
#include <fstream>

Settings::Settings(): cpu_limit(4), document_root("var/www/html") {};

void Settings::ParseConfigFile(const string& __config_filename) {
    std::ifstream file(__config_filename);
    assert(file.is_open() == 0);

    string line, field, value;
//    std::getline(file, line, '\n');
//    cout << "sdfsdf" << line << endl;
    while (std::getline(file, line, '\n')) {
        std::istringstream stream(line);
        stream >> field >> value;

        cout << field << value;
        if (field == "cpu_limit") {
            cpu_limit = std::atoi(value.c_str());
        } else if (field == "document_root") {
            document_root = value;
        }
    }
}
