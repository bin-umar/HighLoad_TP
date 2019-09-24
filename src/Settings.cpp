//
// Created by jahongir on 9/24/19.
//

#include "../include/Settings.h"
#include <fstream>

Settings::Settings(): cpu_limit(4), document_root("http-test-suite") {};

void Settings::ParseConfigFile(const string& __config_filename) {
    std::ifstream file;
    file.open(__config_filename);

    string line, field, value;
    while (getline(file, line)) {
        std::istringstream stream(line);
        stream >> field >> value;

        cout << field << " = " << value << endl;
        if (field == "cpu_limit") {
            cpu_limit = std::atoi(value.c_str());
        } else if (field == "document_root") {
            document_root = value;
        }
    }
}
