#include "gtest_utils.h"
#include "MongoDB.h"

class MockMongoDB: public MongoDB {
 public:
  MOCK_METHOD0(parseJSON, void());
};

TEST(MongoDB, parseJSON_Mock) {
  MockMongoDB mdb;

  EXPECT_CALL(mdb, parseJSON()).Times(1);
  mdb.InsertChunk();
}
