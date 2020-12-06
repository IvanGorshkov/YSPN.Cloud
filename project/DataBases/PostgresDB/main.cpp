#include <iostream>

#include <pqxx/pqxx>


int main(int, char**) {
  try {
	pqxx::connection C("dbname = testdb user = postgres");
	if (C.is_open()) {
	  std::cout << "Opened database successfully: " << C.dbname() << std::endl;
	} else {
	  std::cout << "Can't open database" << std::endl;
	  return 1;
	}
	//C.disconnect ();
  } catch (const std::exception &e) {
	std::cerr << e.what() << std::endl;
	return 1;
  }
  return 0;
}