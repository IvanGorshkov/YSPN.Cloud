#pragma once

#include <string>
#include <libpq-fe.h>

class PostgresSQLDB {
 public:
  virtual ~PostgresSQLDB();
  void Connect();
 protected:
  explicit PostgresSQLDB(std::string_view info);
  PGconn *_conn{};
  std::string _connInfo;
 private:
  void close();
};
