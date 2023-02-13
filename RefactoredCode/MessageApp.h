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
