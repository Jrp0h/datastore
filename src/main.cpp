#include <chrono>
#include <iostream>
#include <thread>

#include "Datastore.h"
#include "Record.h"
#include "TTL.h"
#include "Table.h"

#include "Language/Lexer.h"
#include "Language/Parser.h"
#include "Language/Token.h"

#include "Network/Server.h"

#include <fmt/core.h>

int main() {
    try {
        Datastore ds;
        Network::Server server(&ds);

        ds.on_boot([&server] {
            server.start();
        });

        ds.on_shutdown([&server] {
            server.start();
        });

        ds.boot();
        using namespace std::literals::chrono_literals;

        while (true) {
            std::this_thread::sleep_for(1s);
        }

    } catch (std::exception& ex) {
        fmt::print("Exception: {}", ex.what());
    }
}
