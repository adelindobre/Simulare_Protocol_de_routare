#ifndef MESSAGE_H
#define MESSAGE_H

#include <string.h>

class Message
{

public:
	int src;
	int dest;
	int timeS;
	int tag;
	char *msg;

	Message()
	{}

	Message(int src, int dest, int timeS, int tag, char* mesaj)
	{
		this->src = src;
		this->dest = dest;
		this->timeS = timeS;
		this->tag = tag;
		msg = new char[200];
		strcpy(msg,mesaj);
	}
	
};

#endif
