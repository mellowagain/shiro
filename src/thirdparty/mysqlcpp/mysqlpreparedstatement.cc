
#include <cassert>
#include <cstring>

#if __has_include(<mysql.h>)
	#include <mysql.h>
#else
	#include <mysql/mysql.h>
#endif

#include <string>

#include "mysqlexception.hh"
#include "mysqlpreparedstatement.hh"

using std::string;
using std::strlen;

MySqlPreparedStatement::MySqlPreparedStatement(
        const char* query,
        MYSQL* const connection
)
        : statementHandle_(mysql_stmt_init(connection))
        , parameterCount_()
        , fieldCount_()
{
    assert(nullptr != connection);
    if (nullptr == statementHandle_) {
        throw MySqlException("MySQL out of memory");
    }

    const size_t length = strlen(query);
    if (0 != mysql_stmt_prepare(statementHandle_, query, length)) {
        string errorMessage(
                MySqlException::getServerErrorMessage(statementHandle_));
        if (0 != mysql_stmt_free_result(statementHandle_)) {
            errorMessage += "; There was an error freeing this statement";
        }
        if (0 != mysql_stmt_close(statementHandle_)) {
            errorMessage += "; There was an error closing this statement";
        }
        throw MySqlException(errorMessage);
    }

    parameterCount_ = mysql_stmt_param_count(statementHandle_);
    fieldCount_ = mysql_stmt_field_count(statementHandle_);
}


MySqlPreparedStatement::~MySqlPreparedStatement() {
    if (0 != mysql_stmt_free_result(statementHandle_)) {
        // TODO Log an error
    }
    if (0 != mysql_stmt_close(statementHandle_)) {
        // TODO Log an error
    }
}
