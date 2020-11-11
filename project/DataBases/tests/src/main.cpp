#include "gtest_utils.h"
#include "gmock/gmock.h"
#include "InternalDB.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class MockDB: public InternalDB {
 public:
  explicit MockDB(const std::string& name):InternalDB(name) {}
  MOCK_METHOD0(creatTable, void());
  MOCK_METHOD0(connect, void());
  MOCK_METHOD0(dbExist, bool());
};

TEST(InternalDB, creatTable) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, creatTable()).Times(1);
  idb.CreateDB();
}

TEST(InternalDB, connect) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  idb.GetUserId();
}

TEST(InternalDB, dbExist) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, dbExist()).Times(1);
  idb.GetUserId();
}



int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}