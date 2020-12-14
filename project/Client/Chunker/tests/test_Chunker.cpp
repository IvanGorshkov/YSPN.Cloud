#include "test.h"
#include <thread>

TEST(TestChunker, RunTest) {
    File file("/home/dantedoyl/watch/20.txt");
    Chunker test_chunk(file);

    std::vector<FileChunk> a(test_chunk.ChunkFile());

    File file1("/home/dantedoyl/watch/20new.txt");
    Chunker test_chunk1(file1);

    test_chunk1.MergeFile(a);
}