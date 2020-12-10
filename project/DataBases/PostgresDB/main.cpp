#include <iostream>
#include "PostgreSQLDB.h"
#include "MetaDataDB.h"
#include "FileMeta.h"

int main() {
  MetaDataDB& postgres_sqldb = MetaDataDB::shared("user=ysnp dbname=ysnpcloud");
  postgres_sqldb.Connect();
  FileMeta file_meta;
  file_meta.fileSize = 12;
  file_meta.fileExtension = "btc";
  file_meta.fileName = "asd";
  file_meta.fileId = 23;
  file_meta.isCurrent = true;
  file_meta.version = 1;
  file_meta.chunksCount = 32;
  file_meta.userId = 3;
  file_meta.createDate = "ads";
  file_meta.updateDate = "ads";

  postgres_sqldb.InsertFile(file_meta);
  return 0;
}