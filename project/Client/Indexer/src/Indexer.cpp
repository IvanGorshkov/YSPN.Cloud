#include "Indexer.h"

Indexer::Indexer(std::shared_ptr<InternalDB> internalDB)
    : _internalDB(std::move(internalDB)) {
}

FileMeta Indexer::GetFileMeta(bfs::path file, bool IsDeleted = false) {
  FileMeta new_file_meta{.fileName = file.stem().string(),
      .fileExtension = file.extension().string(),
      .fileSize = static_cast<int>(boost::filesystem::file_size(file)),
      .isDeleted = IsDeleted,
      .updateDate = boost::lexical_cast<std::string>(boost::filesystem::last_write_time(file)),
  };
  // TODO: отправить FileMeta для получения FileId
  return new_file_meta;
}

FileInfo Indexer::GetFileInfo(const FileMeta &file, std::vector<Chunk> &chunks) {

  //TODO: InternalDB(FileMeta.fileid, &vector<Chunk>) получаем id
  std::vector<ChunkMeta> chunk_meta;
  std::vector<FileChunksMeta> file_chunks_meta;
  int i = 0;
  std::for_each(chunks.begin(), chunks.end(), [&chunk_meta, &file_chunks_meta, &i](const Chunk &chunk) {
    chunk_meta.push_back(ChunkMeta{chunk.chunkId});
    file_chunks_meta.push_back(FileChunksMeta{chunk.chunkId, ++i});
  });
  FileInfo info{
      .userId = _internalDB->GetUserId(),
      .file = file,
      .chunkMeta = chunk_meta,
      .fileChunksMeta = file_chunks_meta
  };
  return info;
}
