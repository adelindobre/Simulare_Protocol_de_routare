/******************************************************************************/
/* Tema 2 Protocoale de Comunicatie (Aprilie 2015)                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim.h"
#include "router.h"
#include "routerArray.h"
#include "message.h"
#include "events.h"
#include "api.h"

void init_sim(int argc, char **argv, RouterArray *vec, int dim, int dim2, int dim3) 
{
	FILE *fp;
	char *line = new char[300];
	int row,col,cost;

	fp = fopen(argv[1],"r");

	fgets(line,300,fp);

	// Citirea fisierului topology.in si popularea retelei cu noi rutere
	while (fgets(line,300,fp) != NULL)
	{
		sscanf(line,"%d %d %d", &row, &cost, &col);

		if (vec->contains(row) == 0)
		{
			Router *r = new Router(dim,row);
			r->buildTopology(row,cost,col);
			vec->add(r);
		}
		else
		{
			Router *r = vec->getElemFrom(row);
			r->buildTopology(row,cost,col);
		}

		if (vec->contains(col) == 0)
		{
			Router *r = new Router(dim,col);
			r->buildTopology(col,cost,row);
			vec->add(r);
		}
		else
		{
			Router *r = vec->getElemFrom(col);
			r->buildTopology(col,cost,row);
		}
	}
	fclose(fp);
	// Citirea fisierului message.in si retinerea in vectorul specific din RouterArray
	fp = fopen(argv[2],"r");
	int src, dest, tag, timp;
	char *stringMsg = new char[200];

	fgets(line,300,fp);

	for (int i = 0; i < dim2; i++)
		if (fgets(line,300,fp) != NULL)
		{
			sscanf(line,"%d %d %d %d", &src, &dest, &timp, &tag);
			sprintf(stringMsg, "%d %d %d %d", src, dest, timp, tag);
			strcpy(stringMsg, line + strlen(stringMsg));
			
			Message *mesaj = new Message(src,dest,timp,tag,stringMsg);

			vec->addMessageToArray(*mesaj);
		}	
	fclose(fp);

	// Citirea fisierului events.in si retinerea in vectorul specific din RouterArray
	fp = fopen(argv[3],"r");
	int r1,r2,type;

	fgets(line,300,fp);
	fgets(line,300,fp);

	for (int i = 0; i < dim3; i++)
		if(fgets(line,300,fp) != NULL)
		{
			sscanf(line,"%d %d %d %d %d", &r1, &r2, &type, &cost, &timp);
			Events *ev = new Events(r1,r2,type,cost,timp);
			vec->addEventToArray(*ev);
		}	 
	fclose(fp);	

	// Compunerea primelor mesaje de protocol si trimiterea acestora in retea
	for (int i = 0 ; i < dim; i++)
	{
		Router *r = vec->getElemFrom(i);
		sprintf(line,"%d %d %d", r->getCurrentNode(), r->getNeighbourNumber(), r->getRouterVersion());
		r->writeNeighboursToLine(line);
		r->sendProtocolMessage(line);
	}
}

void clean_sim(RouterArray *vec, int dim) 
{
	for (int i = 0; i < dim; i++)
		delete vec->getElemFrom(i);

	delete  vec->returnMessageVector();
	delete  vec->returnEventsVector(); 
	delete vec;
}

void trigger_events(RouterArray *vec) 
{
	  vec->timeToApplyEvent();
}

void process_messages(RouterArray *vec, int dim) 
{
	for (int i = 0; i < dim; i++)
		vec->getElemFrom(i)->recvProtocolMessage();

	vec->timeToSendMessage();

	for (int i = 0; i < dim; i++)
		vec->getElemFrom(i)->recvRoutableMessage();
}
// Pentru fiecare ruter, se face un update tabelei de rutare
void update_routing_table(RouterArray *vec, int dim)
{
	for (int i = 0; i < dim; i++)
		vec->getElemFrom(i)->updateRoutTable(i);
}
