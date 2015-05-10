#ifndef ROUTER_H
#define ROUTER_H

#include <stdio.h>
#include <string>
#include "topology.h"
#include "routTable.h"
#include "message.h"
#include "events.h"


/* Clasa Router defineste un router din retea. Acesta contine topologia retelei 
(variabila rout_top) si tabela de rutare (variabila table) */


class Router
{
	private:
		int dim;
		Graph *rout_top;	
		RoutTable *table;
	public:
		Router()
		{}

		Router(int dim, int current_nod)
		{
			this->dim = dim;
			rout_top = new Graph(dim,current_nod);
			table = new RoutTable(dim);

			rout_top->initGraph();
			rout_top->initVectorVersion();
		}	
		// Adauga in graful topologiei costul dintre doua muchii
		void buildTopology(int row, int cost, int col)
		{
			rout_top->writeNeighbour(row,cost,col);
		}
		// Intoarce ID-ul ruterului
		int getCurrentNode()
		{
			return rout_top->getCurrentNode();
		}
		// Intoarce numarul de vecini
		int getNeighbourNumber()
		{
			return rout_top->getNeighbourNumber();
		}
		// Intoarce versiunea stocata in baza de date a ruterului
		int getRouterVersion()
		{
			return rout_top->getRouterVersion();
		}
		// Scrie vecinii unui ruter in mesajul de protocol
		void writeNeighboursToLine(char *line)
		{
			rout_top->codifyNeighbours(line);
		}
		// Trimite un mesaj de protocol vecinilor ruterului
		void sendProtocolMessage(char *line)
		{
			rout_top->sendMessage(line);
		}
		// Trimite un mesaj de rutare
		void sendRoutableMessage(Message mesaj)
		{
			table->sendInitialMessage(mesaj);
		}
		// Primirea mesajelor de protocol
		void recvProtocolMessage()
		{
			rout_top->recvMessage();
		}
		// Primirea mesajelor rutabile
		void recvRoutableMessage()
		{
			int router = rout_top->getCurrentNode();
			table->recvMessage(router);
		}
		// Schimbarea topologiei cand apare un eveniment nou
		void changeTopology(Events ev)
		{
			// Modificarea grafului topologiei conform modificarii facute			
			if (rout_top->getCurrentNode() == ev.r1)
				if (ev.type == 0)
					rout_top->writeNeighbour(ev.r1, ev.cost, ev.r2);
				else
					rout_top->writeNeighbour(ev.r1, 0, ev.r2);
			else
				if (ev.type == 0)
					rout_top->writeNeighbour(ev.r2, ev.cost, ev.r1);
				else
					rout_top->writeNeighbour(ev.r2, 0, ev.r1);
			
			// Updateaza versiunea ruterului implicat
			rout_top->updateVersion();
		}
		// Crearea unui mesaj de protocol (in momentul unui eveniment)
		void createMessage()
		{
			rout_top->createProtocolMessage();
		}
		// Printare graf-topologie
		void printTopology()
		{
			rout_top->printGraph();
		}
		// Updateaza calea cea mai scurta plecand de la ruterul curent pana la toate celelalte rutere 
		void updateRoutTable(int source)
		{
			table->dijkstra(rout_top->getGraph(), source);
		}
		// Functie ajutatoare 
		void printPrevPath(int source)
		{
			table->printPrev();
		}
};

#endif