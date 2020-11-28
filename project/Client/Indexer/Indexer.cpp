/**
 * Project Untitled
 */


#include "Indexer.h"


/**
 * Indexer implementation
 */


/**
 * @param Notification
 * @return void
 */
void Indexer::GetNotifierEvent(Notification notification) {
    return;
}

void Indexer::Operation1() {

}

/**
 * @param структура_для_мета_данных
 * @return int
 */
int Indexer::sentUpdatetoLocalDB(void структура_для_мета_данных) {
    return 0;
}

/**
 * @param path
 * @param bool
 * @param ChunkData
 * @return int
 */
int Indexer::invokeChunker(boost::filesystem::path file, bool is_new_file, ChunkData data) {
    return 0;
}

/**
 * @param структура_для_мета_данных
 * @return int
 */
int Indexer::sentUpdateToQueue(void структура_для_мета_данных) {
    return 0;
}

/**
 * @return void
 */
void Indexer::updateLocalDB() {
    return;
}

/**
 * @return json
 */
boost::property_tree::ptree Indexer::createNewMetaData() {
    return {};
}