//
// Created by Yaison Alcantara 3 on 2/24/23.
//

#pragma once

#include <tuple>
#include <ylib/utils/Map.h>
#include <ylib/core/lang.h>
#include <filesystem>
#include <fstream>

namespace ylib::utils {
    namespace fs = std::filesystem;
    using namespace std;

    class Properties {
    private:
        Map<string, string> _data;
    public:
        void put(const string &key, const string &val) {
            _data.put(key, val);
        }

        string get(const string &key) {
            return _data.get(key);
        }

        size_t size() {
            return _data.size();
        }
    };


    class PropertiesParser {
    public:
        Bool isComment(string &line) {
            if (line.length() == 0) {
                return False;
            }

            if (line.at(0) == '#') {
                return True;
            }
            return False;
        }

        optional<pair<string, string>> parseLine(string line) {
            string trimmed = trim(line);
            if (trimmed.empty() || isComment(trimmed) == True) {
                return nullopt;
            }

            size_t pos = trimmed.find_first_of('=');
            if (pos == string::npos) {
                throw Exception(sfput("Invalid line '{}'. No '=' character.", trimmed));
            }
            if (pos == 0) {
                throw Exception(sfput("Invalid line '{}'. Wrong '=' position.", trimmed));
            }
            string key = trimmed.substr(0, pos);
            if (pos + 1 == trimmed.length()) {
                return pair{trim(key), ""};
            }

            string val = trimmed.substr(pos + 1);

            return pair{trim(key), trim(val)};
        }

        Properties load(fs::path path) {
            if (fs::exists(path) == false) {
                throw Exception("Path not exist or might not be a file.");
            }

            ifstream is(path);

            Properties props;
            for (string line; getline(is, line);) {
                auto pairOpt = parseLine(line);
                if (pairOpt.has_value()) {
                    auto [key, val] = pairOpt.value();
                    props.put(key, val);
                }
            }
            is.close();

            return props;
        }
    };

}
