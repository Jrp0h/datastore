#include "Language/Parser.h"

#include "Language/Errors.h"
#include "Logger.h"

#include <sstream>

namespace Language {

void Parser::set_query(std::string query) {
    m_lexer = Lexer(query);
    m_statement = m_lexer.get_all_tokens();
    m_current_token = 0;
}

Action Parser::parse() {
    auto got = eat(Token::IDENTIFIER);
    std::string content = got.get_content();

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

    throw UnknownIdentifier(m_lexer.get_content(), got);
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
                throw UnexpectedIdentifier(m_lexer.get_content(), peek, "MOD");
            }

            eat(Token::IDENTIFIER);
            capture_mods(&action, false);
        }

    } else if (what.get_content() == "DELETE") {
        action.m_type = Action::TABLE_DELETE;
        eat(Token::IDENTIFIER);
        auto peek = current();

        if (peek.get_type() == Token::IDENTIFIER) {
            if (peek.get_content() != "WHERE") {
                throw UnexpectedIdentifier(m_lexer.get_content(), peek, "WHERE");
            }

            eat(Token::IDENTIFIER);
            capture_where(&action);
        }
    } else if (what.get_content() == "MOD") {
        eat(Token::IDENTIFIER);
        capture_mods(&action, false);
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
            throw UnexpectedIdentifier(m_lexer.get_content(), peek, "WHERE");
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

void Parser::capture_mods(Action* action, bool is_table_definition) {
    eat(Token::COLON);
    auto value = eat(Token::IDENTIFIER);

    if (is_table_definition) {
        if (value != "POKE" && value != "ONE_TOUCH")
            throw UnknownMod(m_lexer.get_content(), value);
    } else {
        if (value != "ONE" && value != "WITH_TTL")
            throw UnknownMod(m_lexer.get_content(), value);
    }

    action->m_table_mods.push_back(value.get_content());

    if (current() == Token::COMMA) {
        eat(Token::COMMA);
        capture_mods(action, is_table_definition);
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
    auto id = eat(Token::IDENTIFIER); // "TABLE"

    if (id != "TABLE")
        throw UnexpectedIdentifier(m_lexer.get_content(), id, "TABLE");

    auto table_name = eat(Token::IDENTIFIER);
    action.m_table_name = table_name.get_content();

    auto peek = current();
    // HAs ttl
    if (peek == Token::COLON) {
        eat(Token::COLON);
        auto value = eat(Token::NUMBER);
        action.m_table_ttl = atoi(value.get_content().c_str());
    }

    id = eat(Token::IDENTIFIER); // "WITH"

    if (id != "WITH")
        throw UnexpectedIdentifier(m_lexer.get_content(), id, "WITH");

    capture_columns(&action);

    peek = current();

    if (peek.get_type() == Token::IDENTIFIER) {
        if (peek.get_content() != "MOD") {
            throw UnexpectedIdentifier(m_lexer.get_content(), peek, "MOD");
        }

        eat(Token::IDENTIFIER);
        capture_mods(&action, true);
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
        throw UnexpectedToken(m_lexer.get_content(), t, expected);
    }

    return t;
}

Token Parser::current() {
    return m_statement[m_current_token];
}

}
