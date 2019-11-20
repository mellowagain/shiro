#include "../thirdparty/loguru.hh"
#include "redis.hh"

shiro::redis::redis(std::string address, uint32_t port, std::string password)
    : address(std::move(address))
    , port(port)
    , password(std::move(password)){
    // Initialized in initializer list
}

void shiro::redis::connect() {
    if (is_connected())
        return;

    this->client = std::make_shared<cpp_redis::client>();
    this->client->connect();

    if (!this->password.empty())
        this->client->auth(this->password);

    if (!is_connected())
        ABORT_F("Unable to connect to Redis instance.");

    LOG_F(INFO, "Successfully connected to Redis.");
}

void shiro::redis::disconnect() {
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    this->client->set("shiro.startup", "0");
    this->client->set("shiro.shutdown", std::to_string(seconds.count()));

    this->client->sync_commit();

    if (!is_connected())
        return;

    this->client->disconnect();

    LOG_F(INFO, "Successfully disconnected from Redis.");
}

void shiro::redis::setup() {
    this->client->set("shiro.online_users", "0");

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    this->client->set("shiro.startup", std::to_string(seconds.count()));
    this->client->set("shiro.shutdown", "0");

    this->client->sync_commit();
}

bool shiro::redis::is_connected() {
    return this->client != nullptr;
}

std::shared_ptr<cpp_redis::client> shiro::redis::get() {
    if (!is_connected())
        ABORT_F("Tried to acquire redis connection while not being connected.");

    return this->client;
}
