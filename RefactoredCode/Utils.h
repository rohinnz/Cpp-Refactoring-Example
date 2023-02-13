#pragma once
#include <string>
#include <chrono>

class Utils
{
public:
	static std::string getUserInput();
	static void clearScreen();
	static std::string timestampToString(std::chrono::system_clock::time_point timestamp);
};
