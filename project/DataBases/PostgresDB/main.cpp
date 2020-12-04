#include <iostream>

#include <pqxx/pqxx>

#include <libpq-fe.h>

/*void do_exit(PGconn *conn) {

  PQfinish(conn);
  exit(1);
}*/

int main(int argc, const char** argv) {
  try {
	pqxx::connection C("dbname = testdb user = postgres");
	if (C.is_open()) {
	  std::cout << "Opened database successfully: " << C.dbname() << std::endl;
	} else {
	  std::cout << "Can't open database" << std::endl;
	  return 1;
	}
	C.disconnect ();
  } catch (const std::exception &e) {
	std::cerr << e.what() << std::endl;
	return 1;
  }

  /*PGconn *conn = PQconnectdb("user=postgres dbname=ivangorshkov");

  if (PQstatus(conn) == CONNECTION_BAD) {

	fprintf(stderr, "Connection to database failed: %s\n",
			PQerrorMessage(conn));
	do_exit(conn);
  }

  int ver = PQserverVersion(conn);
  char *user = PQuser(conn);
  char *db_name = PQdb(conn);
  char *pswd = PQpass(conn);

  printf("User: %s\n", user);
  printf("Database name: %s\n", db_name);
  printf("Password: %s\n", pswd);
  printf("Server version: %d\n", ver);

  PQfinish(conn);*/
  return 0;
}