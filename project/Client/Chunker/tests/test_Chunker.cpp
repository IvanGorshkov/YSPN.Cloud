#include "test.h"
#include <thread>

TEST(TestChunker, RunTest) {
    File file("/home/dantedoyl/watch/20new.txt");
    Chunker test_chunk(file);

    std::vector<Chunk> a(test_chunk.ChunkFile());

    File file1("/home/dantedoyl/watch/20newnew.txt");
    Chunker test_chunk1(file1);

    //test_chunk1.MergeFile(a);
    for(int i = 0; i < a.size(); i++){
      a[i].chunkId = i;
    }
    test_chunk1.UpdateChunkFile(a);
}