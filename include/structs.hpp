#pragma once

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fmt/core.h>
#include <format>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

struct Users {
    int id;
    std::string name;
    std::string email;
};

template <> struct fmt::formatter<Users> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Users& b, fmt::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "Users{{id={}, name=\"{}\", email=\"{}\"}}", b.id, b.name, b.email);
    }
};

struct Books {
    int id;
    std::string title;
    std::string author;
    bool available;
};

template <> struct fmt::formatter<Books> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Books& b, fmt::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "Books{{id={}, title=\"{}\", author=\"{}\", available={}}}", b.id, b.title,
                              b.author, b.available);
    }
};

struct Borrows {
    int user_id;
    int book_id;
    std::string borrow_date;
};

template <> struct fmt::formatter<Borrows> {
    constexpr auto parse(fmt::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const Borrows& b, fmt::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "Borrows{{user_id={}, book_id=\"{}\", borrow_date=\"{}\"}}", b.user_id,
                              b.book_id, b.borrow_date);
    }
};
