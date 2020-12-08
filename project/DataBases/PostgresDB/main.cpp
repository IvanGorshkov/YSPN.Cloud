#include <iostream>
#include "PostgreSQLDB.h"
#include "MetaDataDB.h"

int main() {
  MetaDataDB& postgres_sqldb = MetaDataDB::shared("user=ysnp dbname=ysnpcloud");
  postgres_sqldb.Connect();
  return 0;
}