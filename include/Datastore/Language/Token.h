#pragma once

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

    friend std::ostream& operator<<(std::ostream& cout, Token& t) {
        cout << "Token(";

        std::string type = "E_O_F";

        #define TOKEN_CASE(t) { case t: type = "" #t; break; }

        switch(t.m_type) {
            TOKEN_CASE(IDENTIFIER)
            TOKEN_CASE(EQUAL)
            TOKEN_CASE(COMMA)
            TOKEN_CASE(DATA)
            TOKEN_CASE(SEMICOLON)
            TOKEN_CASE(COLON)
            TOKEN_CASE(NUMBER)
            TOKEN_CASE(E_O_F)
        }

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
