#include "MongoDB.h"

MongoDB::MongoDB() {
  createDB();
  connect();
}

MongoDB::~MongoDB() {
  close();
}

void MongoDB::createDB() { }

void MongoDB::InsertChunk() {
  parseJSON();
}

void MongoDB::GetChunk() { }

void MongoDB::parseJSON() { }

void MongoDB::connect() { }

void MongoDB::close() { }