#include "Indexer.h"

Indexer::Indexer(std::shared_ptr<InternalDB> internalDB)
    : _internalDB(std::move(internalDB)) {
}

FileMeta Indexer::GetFileMeta(const bfs::path &file,
                              bool IsDeleted = false,
                              boost::optional<bfs::path> old_path) {
  FileMeta new_file_meta{
      .version = -1,
      .fileName = file.stem().string(),
      .fileExtension = file.extension().string(),
      .filePath = file.parent_path().string(),
      .fileSize = static_cast<int>(boost::filesystem::file_size(file)),
      .isDeleted = IsDeleted,
      .isCurrent = true,
      .updateDate = boost::lexical_cast<std::string>(boost::filesystem::last_write_time(file))
  };
  if (old_path)
    new_file_meta.fileId = _internalDB->FindIdFile(old_path->parent_path().string(),
                                                   old_path->stem().string(),
                                                   old_path->extension().string());
  else
    new_file_meta.fileId =
        _internalDB->FindIdFile(new_file_meta.filePath, new_file_meta.fileName, new_file_meta.fileExtension);
  return new_file_meta;
}

FileInfo Indexer::GetFileInfo(const FileMeta &file, std::vector<Chunk> &chunks) {
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
  _internalDB->InsertOrUpdateFileInfo(info);

  // TODO Maxim for each
  for (int j = 0; j < chunks.size(); j++) {
    chunks[j].userId = info.userId;
    chunks[j].chunkId = info.fileChunksMeta[j].chunkId;
  }

  return info;
}
