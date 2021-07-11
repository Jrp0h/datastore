#include <iostream>
#include <thread>
#include <chrono>

#include "Datastore.h"
#include "TTL.h"
#include "Table.h"
#include "Record.h"

#include "Language/Lexer.h"
#include "Language/Token.h"
#include "Language/Parser.h"

#include "Network/Server.h"

#include <fmt/core.h>

int main() {

    Network::Server server;
    server.start();

    Datastore ds;

    fmt::print("{}\n", sizeof(char));

    ds.boot();

    std::cin.get();
    
    // Language::Lexer l("TO user_auth CREATE user_id=\"5\", auth_token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";");
    // Language::Lexer l("DEFINE TABLE dead_drop:450 WITH message MOD :ONE_TOUCH;");
    
    // Fails (Syntax):
    // Language::Parser p("TO user_auth CREATE user_id=\"5\", auth_token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\" WHERE test=\"yoooo\";"); // MISSPLACED WHERE CLAUSE

    // DONE
    // Language::Parser p("TO user_auth CREATE user_id=\"5\", auth_token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";");
    // Language::Parser p("TO user_auth SET user_id = \"5\", auth_token = \"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\" WHERE yoo=\"Hello\";");
    // Language::Parser p("TO user_auth SET user_id = \"5\", auth_token = \"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";");
    // Language::Parser p("SELECT 7;");
    // Language::Parser p("WHICH;");
    // Language::Parser p("DESTROY user_auth;");
    // Language::Parser p("FROM user_auth DELETE WHERE toke=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\";");
    // Language::Parser p("FROM posts DELETE;");
    // Language::Parser p("FROM user_auth WHERE token=\"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9\" MOD :ONE;");
    // Language::Parser p("FROM dead_drop MOD :WITH_TTL, :ONE;");
    // Language::Parser p("DEFINE TABLE user_auth:300 WITH user_id, token MOD :POKE;");
    // Language::Parser p("DEFINE TABLE posts WITH post_id, description, title;");
    // Language::Parser p("DEFINE TABLE dead_drop:450 WITH message MOD :ONE_TOUCH;");
    // Language::Parser p("DEFINE TABLE key_value_pair WITH key, value;");

    // Language::Action a = p.parse();

    // std::cout << "Type: " << a.get_type_as_string() << std::endl;
    // std::cout << "DB Index: " << a.get_database_index() << std::endl;

    // std::cout << "Table name: " << a.get_table_name() << std::endl;

    // if(a.get_table_ttl()) std::cout << "TTL: " << *a.get_table_ttl() << std::endl;
    // else std::cout << "TTL: No TTL" << std::endl;

    // std::cout << "Variables:" << std::endl;
    // for(auto& var : a.get_table_variables()) {
        // std::cout << "\t" << var.first << ": " << var.second << std::endl;
    // }

    // std::cout << "WHERE:" << std::endl;
    // for(auto& var : a.get_table_where()) {
        // std::cout << "\t" << var.first << ": " << var.second << std::endl;
    // }

    // std::cout << "MODS:" << std::endl;
    // for(auto& var : a.get_table_mods()) {
        // std::cout << "\t" << var << std::endl;
    // }

    // std::cout << "Columns:" << std::endl;
    // for(auto& var : a.get_table_columns()) {
        // std::cout << "\t" << var << std::endl;
    // }

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
