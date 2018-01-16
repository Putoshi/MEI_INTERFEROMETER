#include "Signal.h"

using namespace std;

Signal::Signal() {

}

Signal::~Signal() {

}

void Signal::add(std::vector<unsigned short> targetVector) {

	for (auto itr = targetVector.begin(); itr != targetVector.end(); ++itr) {
		std::cerr << *itr << " ";
	}

	std::cerr << std::endl;
}