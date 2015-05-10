/******************************************************************************/
/* Tema 2 Protocoale de Comunicatie (Aprilie 2015)                            */
/******************************************************************************/

#ifndef _SIM_H_
#define _SIM_H_

#include "routerArray.h"

void init_sim(int argc, char **argv, RouterArray *vec, int dim, int dim2, int dim3);
void clean_sim(RouterArray *vec, int dim);

void trigger_events(RouterArray *vec);
void process_messages(RouterArray *vec, int dim);
void update_routing_table(RouterArray *vec, int dim);

#endif /* _SIM_H_ */
