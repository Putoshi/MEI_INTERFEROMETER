#include "Signal.h"

using namespace std;

Signal::Signal() {
	//signal0.resize(200000);
	//signal1.resize(200000);
	//signal2.resize(200000);
	//signal3.resize(200000);
	//signal4.resize(200000);
	//signal5.resize(200000);
	//signal6.resize(200000);
	//signal7.resize(200000);
}

Signal::~Signal() {

}

void Signal::add(std::vector<unsigned short> targetVector) {

	//for (auto itr = targetVector.begin(); itr != targetVector.end(); ++itr) {
	//	std::cerr << *itr << " ";
	//}

	signal0.push_back(targetVector[0]);
	signal1.push_back(targetVector[1]);
	signal2.push_back(targetVector[2]);
	signal3.push_back(targetVector[3]);
	signal4.push_back(targetVector[4]);
	signal5.push_back(targetVector[5]);
	signal6.push_back(targetVector[6]);
	signal7.push_back(targetVector[7]);

	//for (auto itr = signal1.begin(); itr != signal1.end(); ++itr) {
	//	std::cerr << "signal1: " << *itr << " " << std::endl;
	//	
	//}
	//std::cerr << std::endl;

	
}