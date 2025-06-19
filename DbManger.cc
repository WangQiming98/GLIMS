#include<iostream>
#include "Dbmanager.hpp"

using json = nlohmann::json;

DbManager::DbManager(const std::string& dbPath) : dbPath(dbPath), db(nullptr) {}

//初始化
int DbManager::init() {
    try {
        db = std::make_unique<SQLite::Database>(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db->exec("CREATE TABLE IF NOT EXISTS assets (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, quantity INTEGER, type TEXT NOT NULL CHECK(type IN ('枪械弹药','食品','被服','医疗用品','装备设备','营房设施')))");
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
//获取所有资产
json DbManager::getAllAsset() {
    json result = json::array();
    try {
        SQLite::Statement query(*db, "SELECT id, name, quantity, type FROM assets");
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
//添加资产
int DbManager::addAsset(const std::string& name, int quantity, const std::string& type){
    try{
        SQLite::Statement insert(*db, "INSERT INTO Asset (name, quantity, type) VALUE (? ,? ,?);");
        insert.bind(1, name);
        insert.bind(2, quantity);
        insert.bind(3, type);
        return insert.exec();
    }catch(const SQLite::Exception& e) {
        std::cerr << "Error adding asset: " << e.what() << std::endl;
        return -1; // Error  
    }
}

//删除资产
int DbManager::deleteAsset(int id) {
    try {
        SQLite::Statement del(*db, "DELETE FROM assets WHERE id = ?");
        del.bind(1, id);
        return del.exec() > 0 ? 0 : -1; // Return 0 on success, -1 on failure
    } catch (const SQLite::Exception& e) {
        std::cerr << "Error deleting asset: " << e.what() << std::endl;
        return -1; // Error
    }
}