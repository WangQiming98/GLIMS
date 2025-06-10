find_package(SQLiteCpp REQUIRED)
target_link_libraries(GrassrootsLogisticsInformationManagementSystem
    PRIVATE
        SQLiteCpp::SQLiteCpp
        SQLite::SQLite3
)