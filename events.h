#ifndef EVENTS_H
#define EVENTS_H

class Events
{

public:
	int r1;
	int r2;
	int type;
	int cost;
	int timp;

	Events()
	{}

	Events(int r1, int r2, int type, int cost, int timp)
	{
		this->r1 = r1;
		this->r2 = r2;
		this->type = type;
		this->cost = cost;
		this->timp = timp;
	}
		
};

#endif