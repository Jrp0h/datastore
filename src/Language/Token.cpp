#include "Language/Token.h"

namespace Language {

Token::Token() 
    : m_type(Type::E_O_F), m_content("") {}

Token::Token(Type type, std::string content) 
    : m_type(type), m_content(content) {  }
}
