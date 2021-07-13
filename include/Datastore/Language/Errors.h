#pragma once

#include "Token.h"

#include <exception>
#include <sstream>
#include <string>

#include <fmt/format.h>

namespace Language {

class Error : public std::exception {
public:
    void set_constructed(std::string constructed) {
        m_constructed = constructed;
    }

    const char* what() const throw() {
        return m_constructed.c_str();
    }

protected:
    std::string underline_query(std::string& query, Token& got) {
        std::string line("");

        for (int i = 0; i < (int)query.length(); i++) {
            if (i < got.get_start() || i >= got.get_end())
                line = fmt::format("{} ", line);
            else if (i == got.get_start() || i == got.get_end() - 1)
                line = fmt::format("{}^", line);
            else
                line = fmt::format("{}-", line);
        }

        if (got.get_end() == (int)query.length())
            line = fmt::format("{}^", line);

        return line;
    }

private:
    std::string m_constructed;
};

class UnexpectedToken : public Error {
public:
    UnexpectedToken(std::string query, Token got, Token::Type expected) {
        set_constructed(fmt::format("Expected {} but got {}.\n{}\n{}", Token::get_type_as_string(expected), got.get_type_as_string(), query, underline_query(query, got)));
    }
};

class UnknownIdentifier : public Error {
public:
    UnknownIdentifier(std::string query, Language::Token got) {
        set_constructed(fmt::format("Unknown Identifier {}\n{}\n{}", got.get_type_as_string(), query, underline_query(query, got)));
    }
};

class UnexpectedIdentifier : public Error {
public:
    UnexpectedIdentifier(std::string query, Token got, std::string expected) {
        set_constructed(fmt::format("Expected {} identifier but got {}.\n{}\n{}", expected, got.get_content(), query, underline_query(query, got)));
    }
};

}
