/**
 * Project Untitled
 */


#ifndef _INDEXER_H
#define _INDEXER_H

#include "Notification.h"
#include "ChunkData.h"
#include "stdlib.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

class Indexer {
public: 
    
/**
 * @param Notification
 */
void GetNotifierEvent(Notification notification);
    
void Operation1();
private: 
    Notification _notification;
    InternalDB _internalDB;
    
/**
 * @param структура_для_мета_данных
 */
int sentUpdatetoLocalDB(void структура_для_мета_данных);
    
/**
 * @param path
 * @param bool
 * @param ChunkData
 */
int invokeChunker(boost::filesystem::path file, bool is_new_file, ChunkData data);
    
/**
 * @param структура_для_мета_данных
 */
int sentUpdateToQueue(void структура_для_мета_данных);
    
void updateLocalDB();
    
boost::property_tree::ptree createNewMetaData();
};

#endif //_INDEXER_H