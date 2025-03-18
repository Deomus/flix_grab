// serializing.h
#pragma once

#include <map>

#include <util/str/str_manip.h>
#include <util/str/str_parse.h>

inline std::string serialize(std::map<std::string, std::string> & keyvalue_map) {
    std::string result;

    for (auto it = keyvalue_map.begin(); it != keyvalue_map.end(); ++it) {
        if (!it->second.empty()) {
            result.append(it->first);
            result.append("=");
            result.append(it->second);
            result.append("\n");
        }
    }
    return result;
}

inline int deserialize(const std::string& serialized_array, std::map<std::string, std::string>& keyvalue_map) {
    std::vector<std::string> lines;
    str::tokenize<char>(serialized_array, "\n", lines);

    std::vector<std::string> key_and_value;
    for (size_t i = 0; i < lines.size(); i++) {
        str::tokenize<char>(lines[i], "=", key_and_value);

        if (key_and_value.size() < 2)
            continue;

        keyvalue_map[key_and_value[0]] = key_and_value[1];
    }

    return 0;
}