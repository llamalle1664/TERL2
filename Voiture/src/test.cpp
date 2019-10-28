#include "Application.hpp"
#include <cstdlib>

#include "StackTrace.hpp"
static StackTrace st;

int main() {
	srand(time(NULL));

	Application app;

	app.run();

	return 0;
}
