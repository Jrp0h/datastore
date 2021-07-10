#pragma once


#include "Logger.h"
#include "Macros.h"

#include <iostream>
#include <string>

namespace Language {

class Token {
public:
    enum Type {
        IDENTIFIER,
        EQUAL,
        COMMA,
        DATA,
        SEMICOLON,
        COLON,
        NUMBER,
        E_O_F
    };

    Token();
    Token(Type type, std::string content);

    Type get_type() const { return m_type; }
    std::string get_content() const { return m_content; }

    static std::string get_type_as_string(Type type) {
        switch(type) {
            TYPE_TO_STRING(IDENTIFIER)
            TYPE_TO_STRING(EQUAL)
            TYPE_TO_STRING(COMMA)
            TYPE_TO_STRING(DATA)
            TYPE_TO_STRING(SEMICOLON)
            TYPE_TO_STRING(COLON)
            TYPE_TO_STRING(NUMBER)
            TYPE_TO_STRING(E_O_F)
        }

        LOG_FATAL("Token::get_type_as_string", "UNKNOW TOKEN")
    }

    std::string get_type_as_string() const { return get_type_as_string(m_type); }

    friend std::ostream& operator<<(std::ostream& cout, Token& t) {
        cout << "Token(";

        std::string type = t.get_type_as_string();

        if(t.m_type == E_O_F)
           cout << type << ")";
        else if(t.m_type == DATA)
            cout << type << ", \"" << t.m_content << "\")";
        else
            cout << type << ", " << t.m_content << ")";

        return cout;
    }

    bool operator==(Type t) {
        return m_type == t;
    }

    bool operator==(std::string c) {
        return m_content == c;
    }

    bool operator==(Token& other) {
        return *this == other.m_type 
            && *this == other.m_content;
    }

    bool operator!=(Type t) {
        return !operator==(t);
    }
    bool operator!=(std::string c) {
        return !operator==(c);
    }

    bool operator!=(Token& other) {
        return !operator==(other);
    }

private:
    Type m_type;
    std::string m_content;
};

}
