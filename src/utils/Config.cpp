#include "utils/Config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

Config::Config(const std::string& config_file) {
    config_dir = std::filesystem::path(config_file).parent_path().string();
    loadConfig(config_file);
}

void Config::loadConfig(const std::string& config_file) {
    std::ifstream file(config_file);
    if (!file) {
        throw std::runtime_error("Could not open config file: " + config_file);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::string key;
        if (std::getline(line_stream, key, '=')) {
            std::string value;
            if (std::getline(line_stream, value)) {
                config_map[key] = value;
            }
        }
    }
}

std::string Config::get(const std::string& key) const {
    auto it = config_map.find(key);
    if (it != config_map.end()) {
        return it->second;
    }
    return "";
}

std::string Config::getAbsolutePath(const std::string& key) const {
    auto relative_path = get(key);
    if (!relative_path.empty() && !std::filesystem::path(relative_path).is_absolute()) {
        return std::filesystem::absolute(config_dir + "/" + relative_path).string();
    }
    return relative_path;
}
