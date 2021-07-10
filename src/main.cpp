#include <iostream>

#include "Datastore.h"
#include "TTL.h"
#include "Table.h"
#include "Record.h"

#include "Language/Lexer.h"
#include "Language/Token.h"
#include "Language/Parser.h"

int main() {
    // Datastore ds;

    // ds.boot();
    
    // Language::Lexer l("FROM user_auth WHERE token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\" MOD :ONE;");
    // Language::Lexer l("TO user_auth CREATE user_id=\"5\", auth_token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";");
    Language::Parser p("TO user_auth CREATE user_id=\"5\", auth_token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";");
    Language::Action a = p.parse();

    for(auto& var : a.get_table_variables()) {
        std::cout << var.first << ": " << var.second << std::endl;
    }

    // auto tokens = l.get_all_tokens();

    // for(auto& t : tokens) {
        // std::cout << t << std::endl;
    // }

    // std::cin.get();
    // std::cout << "Shutting down..." << std::endl;
    // ds.shutdown();
    // std::cout << "Has been shutdown" << std::endl;
    // std::cin.get();

    // auto db = ds.get_database(5);
    // db->add_table("user_auth", Table({"user_id", "token"}, 300, false));

    // auto t = db->get_table("user_auth");
    // std::cout << "Table ttl: " << t->get_ttl().value() << std::endl;

    // t->add_record(Record(Record::DataType{{"hello", "world"}, {"gucci", "gang"}}));
    // t->add_record(Record(Record::DataType{{"hello", "my friend"}, {"gucci", "no gang"}}));

    // for(auto r : *db->get_table("user_auth")->get_records()) {
        // std::cout << r.get_value("hello").value() << " - " << r.get_value("gucci").value() << std::endl;
    // }

    // TimeToLive ttl(3);

    // std::cout << ttl.life_left() << std::endl;
    // using namespace std::literals::chrono_literals;
    // std::this_thread::sleep_for(5s);
    // std::cout << ttl.life_left() << std::endl;

    // ttl.poke();
}
