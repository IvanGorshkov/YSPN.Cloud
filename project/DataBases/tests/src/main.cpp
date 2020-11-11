#include "gtest_utils.h"
#include <gmock/gmock.h>
#include "InternalDB.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class MockDB: public InternalDB {
 public:
  explicit MockDB(const std::string& name):InternalDB(name) {}
  MOCK_METHOD0(close, void());
  MOCK_METHOD0(connect, bool());
};

TEST(InternalDB, creatTable) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, close());
  idb.GetUserId();
}

TEST(InternalDB, connect) {
  MockDB idb("db.sqlite");

  EXPECT_CALL(idb, connect()).Times(1);
  idb.GetUserId();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}