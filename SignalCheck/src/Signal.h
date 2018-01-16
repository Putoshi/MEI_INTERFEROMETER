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

	std::vector<unsigned short> signal0;
	std::vector<unsigned short> signal1;
	std::vector<unsigned short> signal2;
	std::vector<unsigned short> signal3;
	std::vector<unsigned short> signal4;
	std::vector<unsigned short> signal5;
	std::vector<unsigned short> signal6;
	std::vector<unsigned short> signal7;


	void add(std::vector<unsigned short> targetVector);
};

