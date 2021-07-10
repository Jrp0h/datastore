#pragma once

#include "Action.h"
#include "Lexer.h"
#include "Token.h"

namespace Language {

class Parser {
public:
    Parser(std::string content) : m_lexer(content) {
        m_statement = m_lexer.get_all_tokens();
        m_current_token = 0;
    }

    Action parse();

private:
    Action parse_table_from();
    Action parse_table_to();
    Action parse_table_define();
    Action parse_table_destroy();

    void continue_with_update(Action* action);
    void continue_with_create(Action* action);

    void capture_variables(Action* action);
    void capture_where(Action* action);
    void capture_mods(Action* action);
    void capture_columns(Action* action);

    Action parse_database_select();

    Token eat(Token::Type expected);
    Token current();

private:
    Lexer m_lexer;
    std::vector<Token> m_statement;
    int m_current_token;
};

}
