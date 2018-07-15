#ifndef SHIRO_DATABASE_HH
#define SHIRO_DATABASE_HH

#include <memory>
#include <string>
#include <vector>

#include "../thirdparty/mysqlcpp/mysql.hh"
#include "../thirdparty/loguru.hh"

namespace shiro {

    class database {
    private:
        std::shared_ptr<MySql> connection = nullptr;

        std::string address;
        unsigned int port;
        std::string db;

        std::string username;
        std::string password;

    public:
        database(const std::string &address, unsigned int port, const std::string &db, const std::string &username, const std::string &password);

        void connect();
        void setup();

        template <typename... arguments>
        void update(const std::string &query_str, arguments... args) {
            if (!this->is_connected())
                return;

            try {
                this->connection->runCommand(query_str.c_str(), args...);
            } catch (const MySqlException &ex) {
                LOG_S(ERROR) << "Unable to execute update: " << ex.what() << ".";
            }
        }

        template <typename result, typename... arguments>
        std::vector<result> query(const std::string &query_str, arguments... args) {
            if (!this->is_connected())
                return {};

            std::vector<result> result_vector;

            try {
                this->connection->runQuery(&result_vector, query_str.c_str(), args...);
            } catch (const MySqlException &ex) {
                LOG_S(ERROR) << "Unable to execute query: " << ex.what() << ".";
            }

            return result_vector;
        };

        bool is_connected();
        std::shared_ptr<MySql> get_connection();

    };

}

#endif //SHIRO_DATABASE_HH
