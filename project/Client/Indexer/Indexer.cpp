#include "Indexer.h"
#include "FileMeta.h"
FileMeta Indexer::GetFileMeta(bfs::path file, bool IsDeleted = false) {
  FileMeta new_file_meta{.fileName = file.filename().string(),
      .fileExtension = file.extension().string(),
      .fileSize = boost::filesystem::file_size(file),
      .updateDate = boost::filesystem::last_write_time(file),
      .isDeleted = IsDeleted};
  // TODO: отправить FileMeta для получения FileId
  return new_file_meta;
}

FileInfo Indexer::GetFileInfo(const FileMeta &file, std::vector<Chunk> chunks) {
  //TODO: InternalDB(FileMeta.fileid, &vector<Chunk>) получаем id
  std::vector<ChunkMeta> chunk_meta;
  std::vector<FileChunksMeta> file_chunks_meta;
  int i = 0;
  std::for_each(chunks.begin(), chunks.end(), [&chunk_meta, &file_chunks_meta, &i](const Chunk &chunk) {
    chunk_meta.push_back(ChunkMeta{chunk.chunkId});
    file_chunks_meta.push_back(FileChunksMeta{chunk.chunkId, ++i});
  });
  FileInfo info{
      .chunkMeta = chunk_meta,
      .file = file,
      .userId = 0, //TODO: InternalDB.getUserId()
      .fileChunksMeta = file_chunks_meta
  };
  return info;
}

int Indexer::sentUpdatetoLocalDB(void структура_для_мета_данных) {
  return 0;
}

void Indexer::DeleteFile(boost::filesystem::path file) {

  // TODO: notifier alert for app notification
  // get file_meta_id by file_path
  // sent delete signal to base
}

void Indexer::ModifyFile(boost::filesystem::path file) {

  // TODO: notifier alert for app notification
  // get file_meta_id by file_path
  File modified_file(file.string());
}

int Indexer::sentUpdateToQueue(void структура_для_мета_данных) {
  return 0;
}

void Indexer::updateLocalDB() {
  return;
}

boost::property_tree::ptree Indexer::createNewMetaData() {
  return {};
}