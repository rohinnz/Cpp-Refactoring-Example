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
