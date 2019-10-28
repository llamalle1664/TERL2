#include "StackTrace.hpp"

#include <csignal>
#include <cstdio>
#include <sstream>

#include "backward.hpp"

static void abortHandler(int signum) {
	const char* name = NULL;
	switch (signum) {
		case SIGABRT: name = "SIGABRT"; break;
		case SIGSEGV: name = "SIGSEGV"; break;
		case SIGILL:  name = "SIGILL";  break;
		case SIGFPE:  name = "SIGFPE";  break;
	}

	if (name)
		std::cerr << "Caught signal " << signum << " (" << name << ")\n";
	else
		std::cerr << "Caught signal " << signum << "\n";

	backward::StackTrace st; st.load_here(64);
	backward::Printer p;
	std::stringstream ss;
	p.print(st, ss);

	std::ofstream file("crash.log");
	p.print(st, file);
	file.close();

	for (std::string line; std::getline(ss, line);)
		std::cerr << line << "\n";
	std::cerr.flush();

	std::exit(signum);
}

StackTrace::StackTrace() {
	std::signal(SIGABRT, abortHandler);
	std::signal(SIGSEGV, abortHandler);
	std::signal(SIGILL,  abortHandler);
	std::signal(SIGFPE,  abortHandler);
}
