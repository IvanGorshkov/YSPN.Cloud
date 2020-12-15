#include "Indexer.h"

void Indexer::GetWatcherEvent(Notification notification) {
  switch (notification.event) {
    case 256:CreateNewFile(notification.path);
      break;
    case 512:DeleteFile(notification.path);
      break;
    case 2:ModifyFile(notification.path);
      break;
    case 64:ChangeMetaData(notification.path);
  }
  return;
}

void Indexer::CreateNewFile(boost::filesystem::path file) {

  // TODO: notifier alert for app notification
  File new_file(file.string());
  FileMeta new_file_meta(file.filename().string(),
                         file.extension().string(),
                         boost::filesystem::file_size(file),
                         boost::filesystem::last_write_time(file),
                         boost::filesystem::last_write_time(file));

  // TODO: отправить метаданные в InternalDB
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