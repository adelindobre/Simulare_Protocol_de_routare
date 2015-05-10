#ifndef ROUTTABLE_H
#define ROUTTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <deque>
#include "api.h"
#include "message.h"

/*Clasa ce calculeaza cel mai scurt drum plecand de la un ruter catre toate celelalte.
  Nodurile apartinand celor mai scurte cai sunt retinute in vectorul prev.
  prev[target] = anterior, prev[anterior] = ... Inserand nodurile intr-o stiva si parcurgand apoi 
  stiva de la inceput, obtinem nodurile de pe calea cea mai scurta intre sursa si destinatie.
  dest - vectorul ce retine pentru fiecare nod, costul minim de la el la sursa*/

class RoutTable
{
	private:
		int *dist;
		int *prev;
		int dim;

	public:
		RoutTable(int dim)
		{
			dist = new int[dim];
			prev = new int[dim];
			for (int i = 0; i < dim; i++)
				prev[i] = -1;

			this->dim = dim;
		}
		~RoutTable()
		{
			delete [] dist;
			delete [] prev;
		}
		int minDistance(bool *sptSet)
		{
			int min = INT_MAX, min_index;

			for (int v = 0; v < dim; v++)
				if (!sptSet[v])
				{
					if (dist[v] < min)
					{
						min = dist[v];
						min_index = v;
					}
					else 
						if ((dist[v] == min) && (v < min_index))
						{
							min_index = v;
						}
				}
			return min_index;
		}
		void dijkstra(int **graph, int source)
		{
			bool *sptSet = new bool[dim];
	
			for (int i = 0; i < dim; i++)
				prev[i] = -1;

			for (int i = 0; i < dim; i++)
			{
				dist[i] = INT_MAX;
				sptSet[i] = false;                                                   
			}

			dist[source] = 0;

			for (int count = 0; count < dim; count++)
			{
				int u = minDistance(sptSet);
				sptSet[u] = true;

				for (int v = 0; v < dim; v++)
					if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX) 
					{
						if (dist[u] + graph[u][v] < dist[v])
							{
								dist[v] = dist[u] + graph[u][v];
								prev[v] = u;
							}
						else
						{
							if (dist[u] + graph[u][v] == dist[v])
							{
								if (u < v)
									prev[v] = u;								
							}
						}
					}

			}
		}
		int getShortPath(int target)
		{
			int u = target;
			deque<int> deq;
			while (prev[u] != -1)
			{
				deq.push_front(u);
				u = prev[u];
			}
			return deq[0];
		}
		void printPrev()
		{
			for (int i = 0; i < dim; i++)
				printf("Prev[%d] = %d\n",i,prev[i]);
		}
		// Trimiterea pentru prima data a mesajului de rutare de la sursa la next_hope
		void sendInitialMessage(Message mesaj)
		{
			//printf("mesaj: %s --- ",mesaj.msg);
			int next_hop = getShortPath(mesaj.dest);
		
			//printf("Trimite %d -> %d\n", mesaj.src, next_hop);
			endpoint[mesaj.src].route_message(&endpoint[next_hop], mesaj.dest, mesaj.tag, mesaj.msg, NULL);
		
		}
		// Cand un mesaj este primit, receiver-ul isi calculeaza next_hopul pana la sursa, folosindu-se de getShortPath()
		void recvMessage(int router_ID)
		{
			int src, dest, tag;
			char *mesaj = new char[200];
			while (endpoint[router_ID].recv_message(&src, &dest, &tag, mesaj) != false)
			{
				if (router_ID != dest)
				{
 		          //printf("mesaj: %s --- ", mesaj);
		          //printf("Primit %d <- %d\n",router_ID,src);
					int next_hop = getShortPath(dest);

					//printf("mesaj: %s --- ",mesaj);
					//printf("Trimite %d -> %d\n", router_ID, next_hop);
					endpoint[router_ID].route_message(&endpoint[next_hop], dest, tag, mesaj, NULL);
				}
			}			
		} 
};


#endif 
