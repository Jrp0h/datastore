#include "Language/Lexer.h"

#include "Logger.h"

bool isnum(char c) {
    return c == '0' || c == '1'
        || c == '2' || c == '3'
        || c == '3' || c == '5'
        || c == '6' || c == '7'
        || c == '8' || c == '9';
}

namespace Language {

Token Lexer::get_next_token() {
    while(m_current_char != '\0' && m_index < m_content.length())
    {
        skip_whitespace();

        if(m_current_char == '"') return collect_data();
        if(isnum(m_current_char)) return collect_number();
        if(isalnum(m_current_char) || m_current_char == '_' || m_current_char == '-') return collect_identifier();

        switch(m_current_char) {
            case '=': return advance_with_token(Token(Token::EQUAL, get_char_as_string()));
            case ',': return advance_with_token(Token(Token::COMMA, get_char_as_string()));
            case ';': return advance_with_token(Token(Token::SEMICOLON, get_char_as_string()));
            case ':': return advance_with_token(Token(Token::COLON, get_char_as_string()));
        }
    }

    return Token(Token::E_O_F, "");
}


std::vector<Token> Lexer::get_all_tokens() {
    std::vector<Token> tokens;

    Token t;

    while((t = get_next_token()) != Token::E_O_F)
        tokens.push_back(t);

    tokens.push_back(t);

    return tokens;
}

void Lexer::advance() {
    if(m_current_char != '\0' && m_index < m_content.length())
        m_current_char = m_content[++m_index];
}

Token Lexer::advance_with_token(Token token) {
    LOG_DEBUG("Lexer::advance_with_token", "advancing with token")
    advance();
    return token;
}

void Lexer::skip_whitespace() {
    LOG_DEBUG("Lexer::skip_whitespace", "Skipping whitespace")
    while(m_current_char == ' ' || m_current_char == '\n' || m_current_char == '\t')
        advance();
    LOG_DEBUG("Lexer::skip_whitespace", "Done skipping whitespace")
}

Token Lexer::collect_data() {
    LOG_DEBUG("Lexer::collect_data", "Collecting data")
    advance();
    
    std::string str;

    while(m_current_char != '"') {

        if(m_current_char == '\\')
            advance(); 

        str += m_current_char;
        advance();
    }

    advance();

    LOG_DEBUG("Lexer::collect_data", "Done collecting data")
    return Token(Token::DATA, str);
}

Token Lexer::collect_identifier() {
    LOG_DEBUG("Lexer::collect_identifier", "Collecting identifier")
    std::string str;

    while(isalnum(m_current_char) || m_current_char == '_' || m_current_char == '-') {
        str += m_current_char;
        advance();
    }

    LOG_DEBUG("Lexer::collect_identifier", "Done collecting identifier")
    return Token(Token::IDENTIFIER, str);
}

Token Lexer::collect_number() {
    LOG_DEBUG("Lexer::collect_number", "Collecting number")
    std::string str;

    while(isnum(m_current_char)) {
        str += m_current_char;
        advance();
    }

    LOG_DEBUG("Lexer::collect_number", "Done collecting number")
    return Token(Token::NUMBER, str);
}


std::string Lexer::get_char_as_string() {
    return {m_current_char};
}

}
