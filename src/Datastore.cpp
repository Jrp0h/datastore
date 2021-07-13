#include "Datastore.h"

#include "Logger.h"

#include <iostream>

Datastore::~Datastore() {
    shutdown();

    if (m_ttl_worker)
        delete m_ttl_worker;
}

void Datastore::boot() {
    LOG_INFO("Datastore::boot", "Booting")

    m_server.start();
    m_ttl_worker = new std::thread([&] { this->t_clean_ttl(); });
}

void Datastore::shutdown() {
    if (m_should_shutdown)
        return;

    LOG_INFO("Datastore::shutdown", "Shutting down")

    m_should_shutdown = true;
    m_server.stop();

    if (m_ttl_worker) {
        LOG_INFO("Datastore::shutdown", "Waiting for worker to shutdown")
        m_ttl_worker->join();
    }

    LOG_INFO("Datastore::shutdown", "Datastore has been shutdown")
}

void Datastore::persist() { }

void Datastore::t_clean_ttl() {
    using namespace std::literals::chrono_literals;

    while (!m_should_shutdown) {
        // LOG_INFO("Datastore::t_clean_ttl", "Cleaning TTL")
        std::this_thread::sleep_for(1s);
    }
}
