#pragma once

#include "PostgreSQLDB.h"

class MetaDataDB: public PostgreSQLDB {
 protected:
  void createDB() override;
  public:
  	MetaDataDB();
  	~MetaDataDB();
	void InsertFile();
	void InsertChunk();
	void InsertFileChunk();
	void SelectFile();
	void SelectChunk();
	void SelectFileChunk();
	void SelectFilesByUser();
};
