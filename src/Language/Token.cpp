#include "Language/Token.h"

namespace Language {

Token::Token()
    : m_type(Type::E_O_F)
    , m_content("") { }

Token::Token(Type type, std::string content, int start, int end)
    : m_type(type)
    , m_content(content)
    , m_start(start)
    , m_end(end) { }

}
