#include "PostgreSQLDB.h"
#include "PostgresExceptions.h"
void PostgresSQLDB::close() {
  PQfinish(_conn);
}

void PostgresSQLDB::Connect() {
  _conn = PQconnectdb(_connInfo.c_str());
  if (PQstatus(_conn) == CONNECTION_BAD) {
    throw PostgresExceptions("invalid to connect to DB");
  }
}

PostgresSQLDB::~PostgresSQLDB() {
  close();
}

PostgresSQLDB::PostgresSQLDB(std::string_view info): _connInfo(info) { }
