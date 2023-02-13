#include "MessageApp.h"
#include "Utils.h"
#include "Message.h"
#include <iostream>
#include <chrono>

int MessageApp::run()
{
	while (true)
	{
		Utils::clearScreen();
		std::cout << "Please select an option:" << std::endl;
		std::cout << "1. Create User" << std::endl;
		std::cout << "2. Send Message" << std::endl;
		std::cout << "3. Receive All Messages For User" << std::endl;
		std::cout << "4. View All User Messages" << std::endl;
		std::cout << "5. Quit" << std::endl;

		auto input = Utils::getUserInput();
		switch (input[0])
		{
			case '1': addNewUser();				break;
			case '2': sendMessage();			break;
			case '3': receiveMessages();	break;
			case '4': showAllMessages();	break;
			case '5': quit();							return EXIT_SUCCESS;
			default:  onInvalidOption();	break;
		}

		std::cout << std::endl << "Enter any key and press return to continue.....";
		Utils::getUserInput();
	}
}

void MessageApp::addNewUser()
{
	std::cout << "Please enter name: ";
	std::string user = Utils::getUserInput();
	if (m_messageStore.userExists(user))
	{
		std::cout << "ERROR: User already exists!" << std::endl;
		return;
	}

	m_messageStore.addNewUser(user);
	std::cout << "User " << user << " added!" << std::endl;
}

void MessageApp::sendMessage()
{
	std::cout << "From: ";
	std::string from = Utils::getUserInput();
	if (!m_messageStore.userExists(from))
	{
		std::cout << "ERROR: User doesn't exist!" << std::endl;
		return;
	}

	std::cout << "To: ";
	std::string to = Utils::getUserInput();
	if (!m_messageStore.userExists(to))
	{
		std::cout << "ERROR: User doesn't exist!" << std::endl;
		return;
	}

	std::cout << "Message: ";
	std::string msg = Utils::getUserInput();
	std::cout << "Message Sent!" << std::endl;

	m_messageStore.sendMessage(from, to, msg);
}

void MessageApp::receiveMessages()
{
	std::cout << "Enter name of user to receive all messages for: " << std::endl;
	std::string user = Utils::getUserInput();
	if (!m_messageStore.userExists(user))
	{
		std::cout << "ERROR: User doesn't exist!" << std::endl;
		return;
	}

	std::cout << std::endl << "===== BEGIN MESSAGES =====" << std::endl;

	auto messages = m_messageStore.receiveMessagesForUser(user);
	int num = 0;
	for (auto& message : messages)
	{
		std::cout << "Message " << ++num << std::endl;
		std::cout << "From: " << message->from << std::endl;
		std::cout << "Content: " << message->msg << std::endl << std::endl;
	}

	std::cout << std::endl << "===== END MESSAGES =====" << std::endl;
}

void MessageApp::showAllMessages() const
{
	std::cout << std::endl << "===== BEGIN MESSAGES =====" << std::endl;

	auto messages = m_messageStore.getAllUserMessages();

	for (auto& element : messages)
	{
		std::cout << element.first << " (" << element.second.size() << " message"
			<< (element.second.size() != 1 ? "s)" : ")") << std::endl;

		for (auto& message : element.second)
		{
			std::cout << '\t' << Utils::timestampToString(message->timestamp) << ", " << message->to << std::endl;
		}
	}

	std::cout << std::endl << "===== END MESSAGES =====" << std::endl;
}

void MessageApp::quit() const
{
	std::cout << "Quitting!" << std::endl;
}

void MessageApp::onInvalidOption() const
{
	std::cout << "Invalid Option Selected" << std::endl;
}
