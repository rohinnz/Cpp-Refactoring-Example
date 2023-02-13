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
