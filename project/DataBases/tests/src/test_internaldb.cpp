#include "gtest_utils.h"
#include "InternalDB.h"

class MockDB: public InternalDB {
 public:
  explicit MockDB(const std::string& name):InternalDB(name) {}
  MOCK_METHOD0(close, void());
  MOCK_METHOD0(connect, bool());
};


TEST(InternalDB, GetUserId_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.GetUserId();
}

TEST(InternalDB, GetDeviceId_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.GetDeviceId();
}

TEST(InternalDB, GetSyncFolder_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.GetSyncFolder();
}

TEST(InternalDB, InsertUser_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.InsertUser();
}

TEST(InternalDB, DeleteUser_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.DeleteUser();
}

TEST(InternalDB, ExistUser_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.ExistUser();
}

TEST(InternalDB, UpdateSyncFolder_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.UpdateSyncFolder();
}

TEST(InternalDB, UpdatePassword_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.UpdatePassword();
}

TEST(InternalDB, SelectUserPassword_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.SelectUserPassword();
}


TEST(InternalDB, InsertFile_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.InsertFile();
}

TEST(InternalDB, SelectFile_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.SelectFile();
}

TEST(InternalDB, UpdateFile_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.UpdateFile();
}

TEST(InternalDB, InsertChunk_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.InsertChunk();
}

TEST(InternalDB, SelectChunk_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.SelectChunk();
}

TEST(InternalDB, UpdateChunk_Mock) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  EXPECT_CALL(idb, close()).Times(1);
  idb.UpdateChunk();
}
