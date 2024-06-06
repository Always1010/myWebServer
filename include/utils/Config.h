#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

class Config {
public:
    Config(const std::string& config_file);
    std::string get(const std::string& key) const;
    std::string getAbsolutePath(const std::string& key) const;

private:
    std::map<std::string, std::string> config_map;
    std::string config_dir;

    void loadConfig(const std::string& config_file);
};

#endif // CONFIG_H
