#include "db.h"


static int times = 0;
void db(std::string message) {
	times++;
	std::cout << times << " : " << message << "\n";
}