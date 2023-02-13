#include "Utils.h"
#include <iostream>
#include <ctime>

std::string Utils::getUserInput()
{
	std::string input;
	std::getline(std::cin, input);
	std::cout << std::endl;
	return input;
}

void Utils::clearScreen()
{
	// todo: Find a better way to clear the screen that is cross platform.
	for (int i = 0; i < 80; ++i) std::cout << std::endl;
}

std::string Utils::timestampToString(std::chrono::system_clock::time_point timestamp)
{
	std::time_t t = std::chrono::system_clock::to_time_t(timestamp);
	std::string ts = std::ctime(&t);
	ts.resize(ts.size() - 1); // Remove new line character
	return ts;
}
