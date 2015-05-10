#ifndef ROUTERARRAY_H
#define ROUTERARRAY_H

#include <stdio.h>
#include <string.h>
#include "router.h"
#include "api.h"
#include "message.h"
#include "events.h"

/* RouterArray - clasa container 
   Variabile membre: Vector de rutere; Vector de mesage; Vector de evenimente
   Pentru fiecare vector este retinuta si dimensiunea.
*/
			

class RouterArray
{
private:
	int r_length;
	int index;
	int msg_number;
	int index2;
	int events_number;
	int index3;

	Router **r_data;
	Message *vector_msg;
	Events *vector_events;

public:
	RouterArray(int length, int nr_msg, int nr_events)
	{
		index = 0;
		r_length = length;
		r_data = new Router*[r_length];

		index2 = 0;
		msg_number = nr_msg;
		vector_msg = new Message[msg_number];
	
		index3 = 0;
		events_number = nr_events;
		vector_events = new Events[events_number];
	}
	/* Adaugarea unui no ruter. Adaugarea in vector se face la 
	   pozitia indicata de id-ul ruterului */ 
	void add(Router *r)
	{
		r_data[r->getCurrentNode()] = r;
	}
	// Adaugarea unui nou mesaj in vector
	void addMessageToArray(Message mesaj)
	{
		vector_msg[index2++] = mesaj;
	}
	// Adaugare eveniment
	void addEventToArray(Events ev)
	{
		vector_events[index3++] = ev;
	}
	// Functie ajutatoare
    void printMessageArray()
	{
		for (int i = 0; i < index2; i++)
				printf("%s\n",vector_msg[i].msg);
	}
	// Determina ce mesaj trebuie pornit la momentul actual de timp
	void timeToSendMessage()
	{
		for (int i = 0; i < msg_number; i++)
			if (vector_msg[i].timeS == get_time())
				//ruterul specificat ca sursa in mesaj este instiintat
				r_data[vector_msg[i].src]->sendRoutableMessage(vector_msg[i]);
	}
	// Determina ce eveniment trebuie pornit la momentul actual de timp
	void timeToApplyEvent()
	{
		/*Ambele rutere implicate in eveniment ca trebuie sa-si updateze 
		  topologia*/ 
		for (int i = 0; i < events_number; i++)
			if (vector_events[i].timp == get_time())
			{
				r_data[vector_events[i].r1]->changeTopology(vector_events[i]);
				r_data[vector_events[i].r2]->changeTopology(vector_events[i]);
			}
		// Apoi, ruterele trebuie sa trimita in retea mesaje de protocol	
		for (int i = 0; i < events_number; i++)
			if (vector_events[i].timp == get_time())
			{
				r_data[vector_events[i].r1]->createMessage();
				r_data[vector_events[i].r2]->createMessage();
			}
	}
	// Verifica daca un ruter a fost sau nu adaugat in retea
	int contains(int nr_nod)
	{
		for (int i = 0 ; i < r_length; i++)
			if (r_data[i])
				if (r_data[i]->getCurrentNode() == nr_nod)
				return 1;
		return 0;
	}
	// Returneaza ruterul cu ID-ul specificat
	Router* getElemFrom(int nr_nod)
	{
		if (r_data[nr_nod])
			return r_data[nr_nod];
		return NULL;
	}
	// Returneaza vectorul de mesaje
	Message* returnMessageVector()
	{
		return vector_msg;
	}
	Events* returnEventsVector()
	{
		return vector_events;
	}
	// Functie ajutatoare
	void printRouter()
	{
		for (int i = 0; i < r_length; i++)
			printf("%d\n",r_data[i]->getCurrentNode());
	}
};

#endif
