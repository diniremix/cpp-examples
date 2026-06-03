/* ejemplo usando sqlite3
 * vcpkg add port sqlite3
 * vcpkg install
 */
#include <expected>
#include <fmt/core.h>
#include <optional>
#include <sqlite3.h>
#include <string>
#include <vector>
class Database
{
  private:
    // sqlite3* db_ = {};
    sqlite3* db_ = nullptr;

    std::expected<int, std::string> executeParameterized(const std::string& sql, const std::vector<std::string>& params,
                                                         const std::string& errorMsg)
    {
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            auto msg = fmt::format("{}: {}", errorMsg, sqlite3_errmsg(db_));
            fmt::println(stderr, "prepared Error: {}", msg);
            return std::unexpected(msg);
        }

        // bind params
        for (size_t i = 0; i < params.size(); ++i) {
            sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_TRANSIENT);
        }

        /*if (sqlite3_step(stmt) != SQLITE_DONE) {
            fmt::println(stderr, "step Error: {}, {}", errorMsg, sqlite3_errmsg(db_));
        }*/
        int rc = sqlite3_step(stmt);
        int changes = sqlite3_changes(db_); // filas afectadas

        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) {
            auto msg = fmt::format("{}: {}", errorMsg, sqlite3_errmsg(db_));
            fmt::println(stderr, "exec step Error: {}", msg);
            return std::unexpected(msg);
        }
        return changes;
    }

  public:
    Database(const std::string& dbName)
    {
        if (sqlite3_open(dbName.c_str(), &db_)) {
            fmt::println(stderr, "Cannot open database: {}", sqlite3_errmsg(db_));
            db_ = nullptr;
            throw std::runtime_error("Cannot open database");
        } else {
            fmt::println("database open success!.");
        }
    }

    ~Database()
    {
        if (db_)
            sqlite3_close(db_);
    }

    // evitar double free
    // se prohíbe copiar
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    std::optional<std::string> execute(const std::string& sql)
    {
        char* errMsg = nullptr;
        if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            fmt::println(stderr, "Error executing SQL: {}", errMsg);
            sqlite3_free(errMsg);
        }
        return std::nullopt;
    }

    std::expected<std::vector<std::string>, std::string> select_array(const std::string& sql)
    {
        std::vector<std::string> results;
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            auto msg = fmt::format("{}", sqlite3_errmsg(db_));
            fmt::println(stderr, "select_array Error: {}", msg);
            return std::unexpected(msg);
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Recuperar solo el nombre de la columna 1
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (name) {
                results.emplace_back(name);
            }
        }

        sqlite3_finalize(stmt);
        return results;
    }

    std::optional<std::string> query(const std::string& sql)
    {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            auto msg = fmt::format("{}", sqlite3_errmsg(db_));
            fmt::println(stderr, "query Error: {}", msg);
            return std::nullopt;
        }

        std::string result;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }

        sqlite3_finalize(stmt);
        return result;
    }

    void update(const std::string& sql, const std::vector<std::string>& params)
    {
        auto result = executeParameterized(sql, params, "updating Error");
        if (result && *result > 0) {
            fmt::println("Update successful: {} row(s) affected", *result);
        } else if (result && *result == 0) {
            fmt::println("The record to be updated was not found");
        }
    }

    void remove(const std::string& sql, const std::vector<std::string>& params)
    {
        auto result = executeParameterized(sql, params, "deleting Error");
        if (result && *result > 0) {
            fmt::println("Deletion successful: {} row(s) affected", *result);
        } else if (result && *result == 0) {
            fmt::println("The record to be deleted was not found (ID does not exist)");
        }
    }
};

void get_all(Database& db)
{
    fmt::println("");
    fmt::println("consultando todos...");

    auto result_users = db.select_array("SELECT * FROM users");

    if (!result_users) {
        fmt::println("result Error: {}", result_users.error());
    } else {
        auto users = result_users.value();
        if (!users.empty()) {
            for (const auto& user : users) {
                fmt::println("Nombre: {}", user); // Utiliza fmt para imprimir
            }
        } else {
            fmt::println("No se encontraron usuarios.");
        }
    }
}

void create_db(Database& db)
{
    db.execute("DROP TABLE IF EXISTS users");
    db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT);");
    db.execute("INSERT INTO users (name) VALUES ('Alice');");
    db.execute("INSERT INTO users (name) VALUES ('Sam')");
    db.execute("INSERT INTO users (name) VALUES ('Peter')");
    db.execute("INSERT INTO users (name) VALUES ('Kate')");
    db.execute("INSERT INTO users (name) VALUES ('Adam')");
}

int main()
{
    Database db("example_demo.db");
    create_db(db);

    fmt::println("");
    fmt::println("validando...");

    auto result = db.query("SELECT count(*) FROM users");
    if (result) {
        fmt::println("conteo: {}", *result);
    }

    fmt::println("");
    fmt::println("consultando un registro...");

    result = db.query("SELECT name FROM users WHERE id = 1;");
    if (result) {
        fmt::println("Nombre: {}", *result);
    }

    get_all(db);

    fmt::println("");
    fmt::println("actualizar usuario");

    std::string query = "UPDATE users SET name = ? WHERE id = ?";
    db.update(query, {"Kyle", "1"});

    fmt::println("");
    fmt::println("eliminar usuario");

    query = "DELETE FROM users WHERE id = ?";
    db.remove(query, {"5"});

    get_all(db);

    fmt::println("saliendo...");

    return 0;
}
