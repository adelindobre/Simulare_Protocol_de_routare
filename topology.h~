#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "api.h"

/* Clasa corespunzatoare topologiei. Contine o matrice de adiacenta corespunzatoare retelei si
	vectorul de versiuni ale celorlalte rutere. Astfel in vector_version[i] se va retine versiunea 
	ruterului i la momentul curent. Clasa retine si ID-ul ruterului curent.
*/
class Graph
{
	private:
		int current;
		int nr_nods;
		int **g_matrix;
		int *vector_version;
	public:
		Graph()
		{
			nr_nods = 0;
			g_matrix = 0;
			current = 0;
		}
		Graph(int dim, int nod)
		{
			current = nod;
			nr_nods = dim;
			g_matrix = new int*[nr_nods];
			vector_version = new int[nr_nods];

			for (int i = 0; i < nr_nods; i++)
				g_matrix[i] = new int[nr_nods];
		}
		~Graph()
		{
			for (int i = 0; i < nr_nods; i++)
				delete [] g_matrix[i];
			delete [] g_matrix;
		}
		void Erase()
		{
			for (int i = 0; i < nr_nods; i++)
				delete [] g_matrix[i];
			delete [] g_matrix;

			nr_nods = 0;
			g_matrix = 0;
		}
		void initGraph()
		{
			for (int i = 0; i < nr_nods; i++)
				for (int j = 0; j < nr_nods; j++)
					g_matrix[i][j] = 0;
		}
		void initVectorVersion()
		{
			for (int i = 0; i < nr_nods; i++)
					vector_version[i] = 0;
			vector_version[current] = 1;
		}
		// Adaugare conexiune noua
		void writeNeighbour(int row, int cost, int col)
		{
				g_matrix[row][col] = cost;
		}
		// Functie ajutatoare
		void printGraph()
		{
			printf("Graful nodului %d\n\n",current);
			for (int i = 0; i < nr_nods; i++)
			{
				for (int j = 0; j < nr_nods; j++)
					printf("%d ",g_matrix[i][j]);
				printf("\n");
			}
			printf("\n");
		}
		int** getGraph()
		{
			return g_matrix;
		}
		int getRouterVersion()
		{
			return vector_version[current];
		}
		void updateVersion()
		{
			vector_version[current]++;
		}
		// Creare mesaj de protocol dupa aparitia unui eveniment
		void createProtocolMessage()
		{
			char *line = new char[300];
			sprintf(line,"%d %d %d", current, getNeighbourNumber(), vector_version[current]);
			codifyNeighbours(line);
			sendMessage(line);	
		}
		// Returneaza ID-ul ruterului
		int getCurrentNode()
		{
			return current;
		}
		// Returneaza numarul de vecini
		int getNeighbourNumber()
		{
			int cont = 0;
			for (int i = 0; i < nr_nods; i++)
				if (g_matrix[current][i] != 0)
					cont++;
			return cont;
		}
		// Adauga vecinii si costurile pana la ei intr-un mesaj de protocol
		void codifyNeighbours(char *line)
		{
			for (int i = 0; i < nr_nods; i++)
				if (g_matrix[current][i] != 0)
					sprintf(line + strlen(line), " %d %d", i, g_matrix[current][i]);
		}
		// Trimitere mesaj de protocol
		void sendMessage(char *line)
		{
			for (int j = 0 ; j < nr_nods; j++)
				if (g_matrix[current][j] != 0)
					endpoint[current].send_msg(&endpoint[j],line,strlen(line),NULL);
		}
		// Primirea unui mesaj de protocol
		void recvMessage()
		{
			//Decodificare mesajului
			int msg_size;
			char* message = new char[300];
			char* copymsg = new char[300];
			char* token = new char[10];
			int sursa, nrN, version, cont = 0;
			int cost, neighbour;


			while ((msg_size = endpoint[current].recv_protocol_message(message)) != -1)
			{
				strncpy(copymsg,message, strlen(message)+1);			
				token = strtok(copymsg," ");
				sursa = atoi(token);
				token = strtok(NULL," ");
				nrN = atoi(token);
				token = strtok(NULL," ");
				version = atoi(token);

				// Daca versiunea mesajului primit este mai noua decat cea stocata in baza de date pentru ruterul respectiv
				if (vector_version[sursa] < version)
				{
					vector_version[sursa] = version;

					for (int i = 0; i < nr_nods; i++)
						g_matrix[sursa][i] = 0;

					for (cont = 0; cont < 2*nrN; cont = cont+2)
					{
						token = strtok(NULL," ");
						neighbour = atoi(token);
						token = strtok(NULL," ");
						cost = atoi(token);
						g_matrix[sursa][neighbour] = cost;
					}
					sendMessage(message);							
				}
				delete [] message;
				delete [] copymsg;

				message = new char[300];
				copymsg = new char[300];  
			}

		}
};

#endif
