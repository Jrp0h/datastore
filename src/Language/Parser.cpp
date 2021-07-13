#include "Language/Parser.h"

#include "Logger.h"

#include <sstream>

namespace Language {

void Parser::set_query(std::string query) {
    m_lexer = Lexer(query);
    m_statement = m_lexer.get_all_tokens();
    m_current_token = 0;
}

Action Parser::parse() {
    std::string content = eat(Token::IDENTIFIER).get_content();

    if (content == "WHICH") {
        eat(Token::SEMICOLON);
        return Action(Action::DATABASE_WHICH);
    } else if (content == "SELECT")
        return parse_database_select();
    else if (content == "FROM")
        return parse_table_from();
    else if (content == "TO")
        return parse_table_to();
    else if (content == "DEFINE")
        return parse_table_define();
    else if (content == "DESTROY")
        return parse_table_destroy();

    LOG_FATAL("Parser::parse_statement", "Unknown identifier")
}

Action Parser::parse_table_from() {
    Action action(Action::TABLE_READ);

    auto table_name = eat(Token::IDENTIFIER);
    action.m_table_name = table_name.get_content();

    auto what = current();

    if (what.get_content() == "WHERE") {
        eat(Token::IDENTIFIER);
        capture_where(&action);

        auto peek = current();

        if (peek.get_type() == Token::IDENTIFIER) {
            if (peek.get_content() != "MOD") {
                LOG_FATAL("Parser::parse_table_from", "Expected MOD Identifier, got something else")
            }

            eat(Token::IDENTIFIER);
            capture_mods(&action);
        }

    } else if (what.get_content() == "DELETE") {
        action.m_type = Action::TABLE_DELETE;
        eat(Token::IDENTIFIER);
        auto peek = current();

        if (peek.get_type() == Token::IDENTIFIER) {
            if (peek.get_content() != "WHERE") {
                LOG_FATAL("Parser::parse_table_from", "Expected WHERE Identifier, got something else")
            }

            eat(Token::IDENTIFIER);
            capture_where(&action);
        }
    } else if (what.get_content() == "MOD") {
        eat(Token::IDENTIFIER);
        capture_mods(&action);
    }

    eat(Token::SEMICOLON);
    return action;
}

Action Parser::parse_table_to() {
    Action action(Action::TABLE_INSERT);

    auto table_name = eat(Token::IDENTIFIER);
    action.m_table_name = table_name.get_content();

    auto what = eat(Token::IDENTIFIER);

    if (what.get_content() == "SET")
        continue_with_update(&action);
    else if (what.get_content() == "CREATE")
        continue_with_create(&action);

    eat(Token::SEMICOLON);
    return action;
}

void Parser::continue_with_update(Action* action) {
    action->m_type = Action::TABLE_UPDATE;
    capture_variables(action);

    auto peek = current();

    if (peek.get_type() == Token::IDENTIFIER) {
        if (peek.get_content() != "WHERE") {
            LOG_FATAL("Parser::continue_with_update", "Expected WHERE Identifier, got something else")
        }

        eat(Token::IDENTIFIER);
        capture_where(action);
    }
}

void Parser::continue_with_create(Action* action) {
    action->m_type = Action::TABLE_INSERT;
    capture_variables(action);
}

void Parser::capture_variables(Action* action) {
    auto key = eat(Token::IDENTIFIER);
    eat(Token::EQUAL);
    auto value = eat(Token::DATA);

    action->m_table_variables[key.get_content()] = value.get_content();

    if (current() == Token::COMMA) {
        eat(Token::COMMA);
        capture_variables(action);
    }
}

void Parser::capture_where(Action* action) {
    auto key = eat(Token::IDENTIFIER);
    eat(Token::EQUAL);
    auto value = eat(Token::DATA);

    action->m_table_where[key.get_content()] = value.get_content();

    if (current() == Token::COMMA) {
        eat(Token::COMMA);
        capture_where(action);
    }
}

void Parser::capture_mods(Action* action) {
    eat(Token::COLON);
    auto value = eat(Token::IDENTIFIER);

    action->m_table_mods.push_back(value.get_content());

    if (current() == Token::COMMA) {
        eat(Token::COMMA);
        capture_mods(action);
    }
}

void Parser::capture_columns(Action* action) {
    auto value = eat(Token::IDENTIFIER);

    action->m_table_columns.push_back(value.get_content());

    if (current() == Token::COMMA) {
        eat(Token::COMMA);
        capture_columns(action);
    }
}

Action Parser::parse_table_define() {
    Action action(Action::TABLE_DEFINE);
    eat(Token::IDENTIFIER); // "TABLE"

    auto table_name = eat(Token::IDENTIFIER);
    action.m_table_name = table_name.get_content();

    auto peek = current();
    // HAs ttl
    if (peek == Token::COLON) {
        eat(Token::COLON);
        auto value = eat(Token::NUMBER);
        action.m_table_ttl = atoi(value.get_content().c_str());
    }

    eat(Token::IDENTIFIER); // "WITH"
    capture_columns(&action);

    peek = current();

    if (peek.get_type() == Token::IDENTIFIER) {
        if (peek.get_content() != "MOD") {
            LOG_FATAL("Parser::parse_table_define", "Expected MOD Identifier, got something else")
        }

        eat(Token::IDENTIFIER);
        capture_mods(&action);
    }
    eat(Token::SEMICOLON);
    return action;
}

Action Parser::parse_table_destroy() {
    Action action(Action::TABLE_DESTROY);

    auto table_name = eat(Token::IDENTIFIER);
    action.m_table_name = table_name.get_content();

    eat(Token::SEMICOLON);
    return action;
}

Action Parser::parse_database_select() {
    auto index = eat(Token::NUMBER);

    Action action(Action::DATABASE_SELECT);
    action.m_database_index = atoi(index.get_content().c_str());

    eat(Token::SEMICOLON);
    return action;
}

Token Parser::eat(Token::Type expected) {
    Token t = m_statement[m_current_token++];

    if (t != expected) {
        LOG_FATAL("Parser::eat", "Expected {}, Got {} with value {}", Token::get_type_as_string(expected), t.get_type_as_string(), t.get_content())
    }

    return t;
}

Token Parser::current() {
    return m_statement[m_current_token];
}

}
