#include "gtest_utils.h"
#include "UsersDB.h"

class MockUserDB: public UsersDB {
 public:
  MOCK_METHOD0(connect, bool());
};


TEST(MockUserDB, connect_Registration_Mock) {
  MockUserDB udb;

  EXPECT_CALL(udb, connect()).Times(1);
  udb.Registration();
}

TEST(MockUserDB, connect_Login_Mock) {
  MockUserDB udb;

  EXPECT_CALL(udb, connect()).Times(1);
  udb.Login();
}

TEST(MockUserDB, connect_CheckLogin_Mock) {
  MockUserDB udb;

  EXPECT_CALL(udb, connect()).Times(1);
  udb.CheckLogin();
}

TEST(MockUserDB, connect_ChangePassword_Mock) {
  MockUserDB udb;

  EXPECT_CALL(udb, connect()).Times(1);
  udb.CheckLogin();
}