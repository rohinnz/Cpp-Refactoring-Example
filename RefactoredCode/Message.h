#pragma once
#include <string>
#include <chrono>

/** A simple message struct. Current timestamp gets added on creation.
*/
struct Message {
	std::chrono::system_clock::time_point timestamp;
	std::string from;
	std::string to;
	std::string msg;

	Message(
		std::string from,
		std::string to,
		std::string msg)
		:
		timestamp(std::chrono::system_clock::now()),
		from(from), to(to), msg(msg)
	{}
};
