#pragma once

#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class DbManager {
public:
    explicit DbManager(const std::string& dbPath);

    int init();
    nlohmann::json getAllAsset();
    int addAsset(const std::string& name, int quantity, const std::string& tyep);
    int deleteAsset(int id);
    int updateAsset(int id, const nlohmann::json& fields);

private:
    std::string dbPath;
    std::unique_ptr<SQLite::Database> db;
};
