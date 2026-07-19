#pragma once
#include <string>
#include <unordered_map>
#include <mutex>

class Database
{
private:
    std::unordered_map<std::string, std::string> db;
    std::unordered_map<std::string, long long> expiryTime;

    std::mutex dbMutex;

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