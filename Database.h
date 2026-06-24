#pragma once

#include <string>

class Database
{
public:
    void set(std::string key, std::string value);

    std::string get(std::string key);

    bool del(std::string key);

    bool exists(std::string key);

    void expire(std::string key, int seconds);

    bool isExpired(std::string key);

    long long ttl(std::string key);

    void save();

    void load();
};