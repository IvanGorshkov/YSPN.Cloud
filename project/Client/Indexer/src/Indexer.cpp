#include "Indexer.h"

Indexer::Indexer(std::shared_ptr<InternalDB> internalDB)
    : _internalDB(std::move(internalDB)) {
}

FileMeta Indexer::GetFileMeta(const bfs::path &file,
                              bool IsDeleted,
                              boost::optional<bfs::path> old_path) {

  std::string filePath = file.parent_path().string();
  size_t pos = filePath.find(_internalDB->GetSyncFolder());
  if (pos != std::string::npos) {
    filePath.erase(pos, _internalDB->GetSyncFolder().length());
  }

  FileMeta new_file_meta{
      .version = -1,
      .fileName = file.stem().string(),
      .fileExtension = file.extension().string(),
      .filePath = filePath,
      .fileSize = (IsDeleted ? 0 : static_cast<int>(boost::filesystem::file_size(file))),
      .isDownload = true,
      .isDeleted = IsDeleted,
      .isCurrent = true,
      .updateDate = (IsDeleted ? boost::lexical_cast<std::string>(std::time(nullptr))
                               : boost::lexical_cast<std::string>(boost::filesystem::last_write_time(file)))
  };

  if (old_path) {
    std::string oldFilePath = old_path->parent_path().string();
    pos = oldFilePath.find(_internalDB->GetSyncFolder());
    if (pos != std::string::npos) {
      oldFilePath.erase(pos, _internalDB->GetSyncFolder().length());
    }

    new_file_meta.fileId = _internalDB->FindIdFile(oldFilePath,
                                                   old_path->stem().string(),
                                                   old_path->extension().string());
  } else
    new_file_meta.fileId =
        _internalDB->FindIdFile(new_file_meta.filePath, new_file_meta.fileName, new_file_meta.fileExtension);

  return new_file_meta;
}

FileInfo Indexer::GetFileInfo(FileMeta &file, std::vector<Chunk> &chunks) {
  std::vector<FileChunksMeta> file_chunks_meta;
  int i = 0;
  file.chunksCount = chunks.size();
  std::for_each(chunks.begin(), chunks.end(), [&file_chunks_meta, &i](const Chunk &chunk) {
    file_chunks_meta.push_back(FileChunksMeta{chunk.chunkId, ++i});
  });
  FileInfo info{
      .userId = _internalDB->GetUserId(),
      .file = file,
      .fileChunksMeta = file_chunks_meta
  };

  if (!file.isDeleted) {
    _internalDB->InsertOrUpdateFileInfo(info);
  }

  for (int j = 0; j < chunks.size(); j++) {
    chunks[j].userId = info.userId;
    chunks[j].chunkId = info.fileChunksMeta[j].chunkId;
  }

  return info;
}
