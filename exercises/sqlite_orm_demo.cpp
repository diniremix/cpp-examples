/* ejemplo usando SQLiteCpp
 * vcpkg add port sqlitecpp
 * vcpkg install
 */
#include <SQLiteCpp/SQLiteCpp.h>
#include <fmt/core.h>
#include <string>
#include <vector>

void get_all(SQLite::Database& db)
{
    fmt::println("");
    fmt::println("consultando todos...");
    SQLite::Statement query(db, "SELECT * FROM users");

    while (query.executeStep()) {
        fmt::println("Nombre: {}", query.getColumn(1).getString());
    }
}

void create_db(SQLite::Database& db)
{
    try {
        db.exec("DROP TABLE IF EXISTS users");
        db.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT);");

        std::vector<std::string> user_list = {"Alice", "Bob", "Charlie", "David", "Sam"};

        SQLite::Statement query(db, "INSERT INTO users (name) VALUES (:name);");

        db.exec("BEGIN TRANSACTION;");

        for (const auto& name : user_list) {
            query.bind(":name", name);
            query.exec();
            query.reset();
        }

        db.exec("COMMIT;");
    } catch (const std::exception& e) {
        db.exec("ROLLBACK;");
        fmt::println(stderr, "Error: {}", e.what());
    }
}

int main()
{
    try {
        SQLite::Database db("example_orm.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        create_db(db);

        fmt::println("");
        fmt::println("validando...");
        {
            SQLite::Statement query(db, "SELECT count(*) FROM users");

            if (query.executeStep()) {
                std::string counter = query.getColumn(0);
                fmt::println("conteo: {}", counter);
            }
        }

        fmt::println("");
        fmt::println("consultando un registro...");
        {
            SQLite::Statement query(db, "SELECT name FROM users WHERE id = 1;");
            if (query.executeStep()) {
                std::string name = query.getColumn(0);
                fmt::println("Nombre: {}", name);
            } else {
                fmt::println("user not found!");
            }
        }

        get_all(db);

        fmt::println("");
        fmt::println("actualizar usuario");
        {
            SQLite::Statement query(db, "UPDATE users SET name = :p_name WHERE id = :p_id");
            query.bind(":p_name", "Kyle");
            query.bind(":p_id", 1);

            int rowsChanged = query.exec();
            if (rowsChanged > 0) {
                fmt::println("Update successful: {} row(s) affected", rowsChanged);
            } else {
                fmt::println("The record to be updated was not found");
            }
        }

        fmt::println("");
        fmt::println("eliminar usuario");
        {
            SQLite::Statement query(db, "DELETE FROM users WHERE id = :p_id");
            query.bind(":p_id", 5);

            int rowsChanged = query.exec(); // Retorna número de filas afectadas
            if (rowsChanged > 0) {
                fmt::println("Deletion successful: {} row(s) affected", rowsChanged);
            } else {
                fmt::println("The record to be deleted was not found (ID does not exist)");
            }
        }

        get_all(db);
    }

    catch (const SQLite::Exception& e) {
        fmt::println(stderr, "SQLite Error: {}", e.what());
    }
    return 0;
}
