#pragma once

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <iterator>
class Signal
{
public:
	Signal();
	~Signal();

	//std::vector<short>& signal0;
	//std::vector<short>& signal1;
	//std::vector<short>& signal2;
	//std::vector<short>& signal3;
	//std::vector<short>& signal4;
	//std::vector<short>& signal5;
	//std::vector<short>& signal6;
	//std::vector<short>& signal7;


	void add(std::vector<unsigned short> targetVector);
};

