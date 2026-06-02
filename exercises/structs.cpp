#include "structs.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fmt/core.h>
#include <format>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

// =============Library=============
struct Library {
    std::vector<Books> books;
    std::vector<Users> users;
    std::vector<Borrows> borrows;

    void add_book(std::string title, std::string author)
    {
        int id = books.size() + 1;
        Books new_book = {id, title, author, true};
        // objeto existente se agrega una copia a un contenedor.
        //  books.push_back(Books{new_id, title, author, true});
        books.push_back(new_book);
        //
        //  crear un nuevo objeto directamente en el contenedor,
        //  books.emplace_back(id + 1, "Dune2", "Frank Herbert2");
    }
    void add_user(std::string name, std::string email)
    {
        int id = users.size() + 1;
        Users new_user = {id, name, email};
        users.push_back(new_user);
    }

    bool borrow_book(int user_id, int book_id, std::string borrow_date)
    {
        if ((book_id <= books.size()) && (user_id <= users.size())) {
            if (books[book_id - 1].available) {
                books[book_id - 1].available = false;
                Borrows new_borrow = {user_id, book_id, borrow_date};
                borrows.push_back(new_borrow);
                return true;
            }
        }
        return false;
    }

    bool return_book(int book_id)
    {
        if (book_id > 0 && book_id <= books.size()) {
            // Buscar el libro prestado
            /*for (const auto& borrow : borrows) {
                if (borrow.book_id == book_id) {
                    // Eliminarlo de la lista de prestamos
                    borrows.erase(borrows.begin() + 1);
                    books[book_id - 1].available = true;
                    return true;
                }
            }*/
            /*std::erase_if(borrows, [book_id](const auto& b) {
                return b.book_id == book_id;
            });*/
            for (auto it = borrows.begin(); it != borrows.end(); ++it) {
                if (it->book_id == book_id) {
                    // Eliminarlo de la lista de prestamos
                    borrows.erase(it);
                    books[book_id - 1].available = true;
                    return true;
                }
            }
        }
        return false;
    }

    void list_books() const
    {
        fmt::println("");
        fmt::println("======books======");
        for (const auto& book : books) {
            fmt::println("book info: {}", book);
        }
    }

    void list_users() const
    {
        fmt::println("");
        fmt::println("======users======");
        for (const auto& user : users) {
            fmt::println("user info: {}", user);
        }
    }

    void list_borrows() const
    {
        for (const auto& borrow : borrows) {
            fmt::println("borrow info: {}", borrow);
        }
    }
};

int main()
{
    Library lib{};
    lib.add_book("1984", "George Orwell"); // book_id 1
    lib.add_book("Dune", "Frank Herbert"); // book_id 2

    lib.add_user("bob", "bob@example.com");     // user_id 1
    lib.add_user("alice", "alice@example.com"); // user_id 2

    lib.list_books();
    lib.list_users();
    fmt::println("");

    fmt::println("======borrows before======");
    lib.list_borrows();

    lib.borrow_book(1, 1, "2026-01-11");
    lib.borrow_book(1, 2, "2026-01-12");

    fmt::println("");
    fmt::println("======borrows after======");
    lib.list_borrows();

    lib.return_book(1);
    lib.return_book(0);
    lib.return_book(-1);
    lib.return_book(5);
    fmt::println("======borrows after returns======");
    lib.list_borrows();
}
