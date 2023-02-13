
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
