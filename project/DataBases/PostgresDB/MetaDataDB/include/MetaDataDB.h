#pragma once

#include "PostgreSQLDB.h"

class MetaDataDB: public PostgreSQLDB {
  public:
  	MetaDataDB();
  	~MetaDataDB();
	void CreateDB() override;
	void InsertFile();
	void InsertChunk();
	void InsertFileChunk();
	void SelectFile();
	void SelectChunk();
	void SelectFileChunk();
	void SelectFilesByUser();
};
