#include<iostream>
#include "Dbmanager.hpp"

using json = nlohmann::json;

DbManager::DbManager(const std::string& dbPath) : dbPath(dbPath), db(nullptr) {}


int DbManager::init() {
    try {
        db = std::make_unique<SQLite::Database>(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db->exec("CREATE TABLE IF NOT EXISTS assets (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, quantity INTEGER, type TEXT)");
        return 0; // Success
    } catch (const SQLite::Exception& e) {
        std::cerr << "Database initialization error: " << e.what() << std::endl;
        return -1; // Error
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error during database initialization: " << e.what() << std::endl;
        return -1; // Error
    }
}

json DbManager::getAllAsset() {
    json result = json::array();
    try {
        SQLite::Statement query(*db, "SELECT * FROM assets");
        while (query.executeStep()) {
            json asset;
            asset["id"] = query.getColumn(0).getInt();
            asset["name"] = query.getColumn(1).getString();
            asset["quantity"] = query.getColumn(2).getInt();
            asset["type"] = query.getColumn(3).getString();
            result.push_back(asset);
        }
    } catch (const SQLite::Exception& e) {
        std::cerr << "Error fetching assets: " << e.what() << std::endl;
    }
    return result;
}