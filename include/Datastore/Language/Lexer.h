#pragma once

#include "Token.h"

#include <vector>

namespace Language {

class Lexer {
public:
    Lexer(std::string content)
        : m_current_char(content[0])
        , m_content(content)
        , m_index(0) { }

    Lexer() { }

    Token get_next_token();
    std::vector<Token> get_all_tokens();
    std::string& get_content();

private:
    void advance();
    Token advance_with_token(Token token);

    void skip_whitespace();

    Token collect_data();
    Token collect_identifier();
    Token collect_number();

    std::string get_char_as_string();

private:
    char m_current_char;
    std::string m_content;
    unsigned long m_index;
};

}
