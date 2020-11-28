#include "gtest_utils.h"
#include "MetaDataDB.h"

class MockMetaDB: public MetaDataDB {
 public:
  MOCK_METHOD0(connect, bool());
};


TEST(MockMetaDB, connect_InsertFile_Mock) {
  MockMetaDB mdb;

  EXPECT_CALL(mdb, connect()).Times(1);
  mdb.InsertFile();
}

TEST(MockMetaDB, connect_InsertChunk_Mock) {
  MockMetaDB mdb;

  EXPECT_CALL(mdb, connect()).Times(1);
  mdb.InsertChunk();
}

TEST(MockMetaDB, connect_InsertFileChunk_Mock) {
  MockMetaDB mdb;

  EXPECT_CALL(mdb, connect()).Times(1);
  mdb.InsertFileChunk();
}

TEST(MockMetaDB, connect_SelectFile_Mock) {
  MockMetaDB mdb;

  EXPECT_CALL(mdb, connect()).Times(1);
  mdb.SelectFile();
}

TEST(MockMetaDB, connect_SelectChunk_Mock) {
  MockMetaDB mdb;

  EXPECT_CALL(mdb, connect()).Times(1);
  mdb.SelectChunk();
}

TEST(MockMetaDB, connect_SelectFileChunk_Mock) {
  MockMetaDB mdb;

  EXPECT_CALL(mdb, connect()).Times(1);
  mdb.SelectFileChunk();
}

TEST(MockMetaDB, connect_SelectFilesByUser_Mock) {
  MockMetaDB mdb;

  EXPECT_CALL(mdb, connect()).Times(1);
  mdb.SelectFilesByUser();
}
