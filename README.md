# Refactorign Legacy C++ into Modern C++11 Example

This code is for a simple console app that stores and retrieves messages in memory.

There was also a requirement to add a 5th menu option to the new app to view all user messages.

## Original Code

In the original code, almost all the code was stored in a monolithic class called MessageStore.
```c++
// main.cpp
#include "MessageStore.h"

int main(int, const char* [])
{
	
	MessageStore store;
	
	while (store.ProcessInput() == false){
	
	}
	
	store.terminate();
	
	return 0;
}
```
```c++
// MessageStore.h
#pragma once
#include <string>
#include <vector>

#include <iostream>

using namespace std;

class MessageStore
{
public:
	
	bool ProcessInput(); // returns true when finished
	void terminate();
private:

	bool Exists(std::string u);
	std::vector<std::string> users;
	struct Message {
		std::string from;
		std::string to;
		std::string msg;
	};
	std::vector<Message*> messages;
};
```
```c++
// MessageStore.cpp
#include "MessageStore.h"

bool MessageStore::ProcessInput() {
	bool ret = false;
	// clear screen
	for (int i = 0; i < 80; ++i) cout << endl;
	// show options
	cout << "Please select an option:" << endl;
	cout << "1. Create User" << endl;
	cout << "2. Send Message" << endl;
	cout << "3. Receive All Messages For User" << endl;
	cout << "4. Quit" << endl;
	std::string in;
	std::getline(std::cin, in);
	cout << endl;
	if (in == "1")
	{
		cout << "Please enter name: ";
		std::string str;
		std::getline(std::cin, str);
		cout << endl;
		if (Exists(str))
		{
			cout << "ERROR: User already exists!" << endl;
		} else {
			users.push_back(str);
			cout << "User " << str << " added!" << endl;
		}
	} else if (in == "2"){
		cout << "From: ";
		std::string from;
		std::getline(std::cin, from);
		cout << endl;
		if (Exists(from) == false)
			cout <<"ERROR: User doesn't exist!" << endl;
		else {
			cout << "To: ";
			std::string to;
			std::getline(std::cin, to);
			cout << endl;
			if (Exists(to) == false)
				cout <<"ERROR: User doesn't exist!" << endl;
			else {
				cout << "Message: ";
				std::string msg;
				std::getline(std::cin, msg);
				cout << endl;
				cout << "Message Sent!" << endl;
				Message* m = new Message;
				m->from = from;
				m->to = to;
				m->msg = msg;
				messages.push_back(m);
			}
		}
	} else if (in == "3") {
		cout << "Enter name of user to receive all messages for: " << endl;
		std::string user;
		std::getline(std::cin, user);
		cout << endl;
		if (Exists(user) == true)
		{
			cout << endl << "===== BEGIN MESSAGES =====" << endl;
			int num = 0;
			bool more;
			do {
				more = false;
				for (unsigned int i = 0; i < messages.size(); ++i)
				{
					if (messages[i]->to == user) {
						cout << "Message " << ++num << endl;
						cout << "From: " << messages[i]->from << endl;
						cout << "Content: " << messages[i]->msg << endl << endl;
						delete messages[i];
						messages.erase(messages.begin() + i);
						more = true;
						break;
					}
				}
			} while (more);
			
			cout << endl << "===== END MESSAGES =====" << endl;
		} else
			cout <<"ERROR: User doesn't exist!" << endl;
	} else if (in == "4") {
		cout << "Quitting!" << endl;
		ret=true;
	} else
	{
		cout << "Invalid Option Selected" << endl;
	}
	cout << endl <<"Enter any key and press return to continue.....";
	std::string str;
	std::getline(std::cin, str);
	return ret;
}

void MessageStore::terminate()
{
	for (unsigned int i = 0; i < messages.size(); ++i)
		delete messages[i];
}

bool MessageStore::Exists(std::string u)
{
	for (unsigned int i = 0; i < users.size(); ++i)
		if (users[i] == u)
			return true;
	return false;
}
```

## Refactored Code

The refactored code is broken up into three classes and one struct:
* MessageApp class - app code
* MessageStore class - user and message data storage
* Utils class - static function utils
* Message struct - message data

Also new best practises for memory management are used:
* new and delete calls removed.
* Move schemantics used where possible.

**main function**

```c++
// main.cpp
#include "MessageApp.h"

int main(int, const char* [])
{
	return MessageApp().run();
}
```

**MessageApp class**
```c++
// MessageApp.h
#pragma once
#include "MessageStore.h"
#include "Utils.h"

class MessageApp final
{
public:
	int run();
private:
	MessageStore m_messageStore;

	void addNewUser();
	void sendMessage();
	void receiveMessages();
	void showAllMessages() const;
	void onInvalidOption() const;
	void quit() const;
};
```
```c++
// MessageApp.cpp
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
```

**MessageStore class**
```c++
// MessageStore.h
#pragma once
#include "Message.h"
#include "Utils.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class MessageStore final
{
private:
	typedef std::vector<std::shared_ptr<Message>> MessageVector;
	typedef std::unordered_map<std::string, MessageVector> UnorderedMapMessageVectors;
	typedef std::map<std::string, MessageVector> OrderedMapMessageVectors;

	// Store messages with 'to user' as key for fast retrieval.
	UnorderedMapMessageVectors m_userMessages;
public:
	/** Adds a new user.
	Debug mode: Throws exception if user already exists. */
	void MessageStore::addNewUser(const std::string newUser);

	/** Returns vector of messages for user and removes them from store.
	Debug mode: Throws exception if user does not exist. */
	MessageVector receiveMessagesForUser(const std::string user);

	/** Sends a message.
	Debug mode: Throws exception if from or to users do not exist. */
	void sendMessage(const std::string from, const std::string to, const std::string msg);

	/** Returns messages for all users. Grouped by sender and messages sorted by timestamp.
	Debug mode: Throws exception if from or to users do not exist. */
	OrderedMapMessageVectors getAllUserMessages() const;

	/** Returns true if user exists. */
	inline bool MessageStore::userExists(const std::string user) const;

//#define DEBUG_PRE_POPULATED_MESSAGE_STORE
#ifdef DEBUG_PRE_POPULATED_MESSAGE_STORE
	MessageStore()
	{
		m_userMessages["Luke"];
		m_userMessages["Mike"];
		m_userMessages["Jess"];

		m_userMessages["Luke"].push_back(std::make_shared<Message>(Message{"Mike", "Luke", "Hi"}));
		m_userMessages["Luke"].push_back(std::make_shared<Message>(Message{"Mike", "Luke", "Don't forget about tonight!"}));
		m_userMessages["Mike"].push_back(std::make_shared<Message>(Message{"Luke", "Mike", "I'll be there!"}));
		m_userMessages["Mike"].push_back(std::make_shared<Message>(Message{"Jess", "Mike", "Can I come too?"}));
		m_userMessages["Jess"].push_back(std::make_shared<Message>(Message{"Mike", "Jess", "Absolutely"}));
	}
#endif
};
```
```c++
// MessageStore.cpp
#include "MessageStore.h"
#include "Message.h"
#include <algorithm>

void MessageStore::addNewUser(const std::string newUser)
{
#if _DEBUG
	if (userExists(newUser))
	{
		throw std::runtime_error("User already exists");
	}
#endif

	m_userMessages[newUser];  // Will insert user with empty vector
}

void MessageStore::sendMessage(const std::string from, const std::string to, const std::string msg)
{
#if _DEBUG
	if (!userExists(from))
	{
		throw std::runtime_error("From user does not exist");
	}

	if (!userExists(to))
	{
		throw std::runtime_error("To user does not exist");
	}
#endif

	m_userMessages[to].push_back(std::make_shared<Message>(from, to, msg));
}

MessageStore::MessageVector MessageStore::receiveMessagesForUser(const std::string user)
{
#if _DEBUG
	if (!userExists(user))
	{
		throw std::runtime_error("User does not exist");
	}
#endif

	auto messages = MessageVector(m_userMessages[user]);
	m_userMessages[user].clear();	
	return std::move(messages);
}

MessageStore::OrderedMapMessageVectors MessageStore::getAllUserMessages() const
{
	OrderedMapMessageVectors sortedMessages;

	for (auto element : m_userMessages)
	{
		for (auto& message : element.second)
		{
			sortedMessages[message->from].push_back(message);
		}
	}

	for (auto& element : sortedMessages)
	{
		std::sort(element.second.begin(), element.second.end(),
			[](auto msg1, auto msg2) -> bool
		{
			return msg1->timestamp < msg2->timestamp;
		});
	}

	return std::move(sortedMessages);
}

bool MessageStore::userExists(const std::string user) const
{
	return m_userMessages.find(user) != m_userMessages.end();
}
```

**Utils class**
```c++
// Utils.h
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
```
```c++
// Utils.cpp
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
```

**Message Struct**
```c++
// Message.h
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
```
