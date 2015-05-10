/******************************************************************************/
/* Tema 2 Protocoale de Comunicatie (Aprilie 2015)                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "sim.h"
#include "utils.h"
#include "routerArray.h"
#include "message.h"
#include "events.h"

int main(int argc, char **argv)
{

	FILE *fp;
	fp = fopen(argv[1],"r");
	char *line = new char[100];
	int dim, dim2, dim3;

	fgets(line,100,fp);
	sscanf(line,"%d",&dim);
	fclose(fp);

	fp = fopen(argv[2],"r");

	fgets(line,100,fp);
	sscanf(line,"%d",&dim2);
	fclose(fp);

	fp = fopen(argv[3],"r");
	fgets(line,100,fp);
	fgets(line,100,fp);
	sscanf(line,"%d",&dim3);
	fclose(fp);

	RouterArray *vec = new RouterArray(dim, dim2, dim3);

	init_api(argc, argv);
	init_sim(argc, argv, vec, dim, dim2, dim3);
	
	loop {
		trigger_events(vec);
		process_messages(vec,dim);
		update_routing_table(vec,dim);
		api_update_state();	
		if (api_simulation_ended())
			break;
	}
	clean_sim(vec,dim);
	clean_api();

	return 0;
}