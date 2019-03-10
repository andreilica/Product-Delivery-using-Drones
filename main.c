#include <stdio.h> 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFF_MAX 100 // Dimensiunea string-ului de input

/* Structura de graf */

typedef struct graf {
int n;
int **mat;
} graf;

/* Initializarea grafului */

graf *init(int n)
{
	int i;
	graf *aux = (graf*)malloc(sizeof(graf));
	aux -> n = n;
	aux->mat = (int **)malloc(n * sizeof(int*));
	for( i = 0; i < n; i++) 
		aux->mat[i] = (int *)calloc(n,sizeof(int));
	return aux;
}

/* Functie pentru eliberarea memoriei */

void free_memory(graf **g, int *point_type, int *edges, int **orders, int **roads, int **next_hop, int *farms, int C, int farm_nr, int task)
{
	int i;
	free(point_type);
	free(edges);

	for(i = 0; i < C; i++)
		free(orders[i]);
	free(orders);

	if(task == 2)
		free(farms);

	for(i = 0; i < (*g)->n; i++)
		free(roads[i]);
	free(roads);

	for(i = 0; i < (*g)->n; i++)
		free(next_hop[i]);
	free(next_hop);

	for (i = 0; i < (*g)->n ; ++i)
		free((*g)->mat[i]);
	free((*g)->mat);
	free(*g);
	
}

/* Functie pentru adaugarea unei muchii in graf */

void add_edge(graf *g, int i, int j, int c)
{
	g -> mat[i][j] = c;
}

/* Functie pentru stergerea unei muchii din graf */


void del_edge(graf *g, int i, int j)
{
	g -> mat[i][j] = 0;
}

/* Functie pentru afisarea matricii grafului */

void print(graf *g)
{
	int i,j;

	for(i = 0; i < g->n; i++)
	{
		for(j = 0; j < g->n; j++)
			printf("%d ", g->mat[i][j]);
		printf("\n");
	}
	printf("\n");
}

/* Functie pentru initializarea matricelor de drumuri si next hop */

void init_matrices(graf *g, int **roads, int **next_hop)
{
	int i,j;

	for(i = 0; i < g->n; i++)
		for(j = 0; j < g->n; j++)
		{
			if(i == j)
				roads[i][j] = 0;
			else
				if(g->mat[i][j] != 0)
					roads[i][j] = g -> mat[i][j];
				else
					roads[i][j] = 1000;
		}

	for(i = 0; i < g->n; i++)
		for(j = 0; j < g->n; j++)
		{
			if(i == j)
				next_hop[i][j] = i;
			else
				if(g->mat[i][j] != 0)
					next_hop[i][j] = j;
				else
					next_hop[i][j] = -1;
		}
}


/* Algoritmul lui Floyd pentru generarea matricelor de drumuri minime si next hop */

void floyd(graf *g, int **roads, int **next_hop)
{
	int k,i,j;

	for(k = 0; k < g->n ; k++)
		for(i = 0; i < g -> n; i++)
			for(j = 0; j < g -> n; j++)
				if( roads[i][j] > roads[i][k] + roads[k][j])
				{
					roads[i][j] = roads[i][k] + roads[k][j];
					next_hop[i][j] = next_hop[i][k];
				}
}

/* Functie care genereaza drumul minim intre doua puncte date si returneaza distanta dintre ele */

int gen_min_road(int start, int finish, int **roads, int **next_hop, FILE *output)
{
	int d = roads[start][finish];
	while(start != finish)
	{
		fprintf(output, "%d", next_hop[start][finish]);
		start = next_hop[start][finish];
		if(start != finish)
			fprintf(output, " ");
	}
	
	return d;
}

/* Functie folosita la task-ul 2 pentru a gasi ferma prin care trebuie sa treaca drona astfel incat drumul de la un client la celalat sa fie minim */

int find_farm(int start, int finish, int *farms, int **roads, int farm_nr)
{
	int min = 100000, i, min_farm, sume[50], k = 0;

	for(i = 0; i < farm_nr; i++)
		sume[i] = 0;

	/* Crearea unui vector de sume de drumuri pentru fiecare ferma in parte */

	for(i = 0; i < farm_nr; i++)
	{
		sume[k] += roads[start][farms[i]];
		sume[k] += roads[farms[i]][finish];
		k++;
	}

	/* Determinarea fermei optime in functie de suma de drum minima */

	for(i = 0; i < k; i++)
		if(sume[i] < min)
		{
			min = sume[i];
			min_farm = farms[i];
		}

	return min_farm;
}

/* Functie care returneaza indicele fermei cea mai apropiata de un punct de start */

int find_closest_farm(int start, int *farms, int **roads, int farm_nr)
{
	int min = 100000, i, closest_farm;
	for(i = 0; i < farm_nr; i++)
		if( roads[start][farms[i]] < min)
		{
			min = roads[start][farms[i]];
			closest_farm = farms[i];
		}

	return closest_farm;
}


int main(int argc, char const *argv[])
{
	FILE *input; 
	FILE *output;

	int z = 0, farm_nr = 0, task, N, M, C, start, k = 0, i, pos, length, j, s, x = 0, y, distance = 0;
	int *point_type, *edges, **orders, **roads, **next_hop, *farms;
	graf *graf1;
	char buff[BUFF_MAX];
	input = fopen(argv[1], "r"); // Fisierul de input
	output = fopen(argv[2], "w"); // Fisierul de output

	if (!output)
   	{
       fprintf(stderr, "Eroare! Nu am putut deschide fisierul destinatie!\n");
       return 0;
   	}



	if(input == NULL)
	{
		fprintf(stderr, "Eroare, nu s-a putut deschide fisierul de input\n");
		return 0;
	}
	else
	{
		if (fgets(buff, BUFF_MAX, input) != NULL)
		{
			switch(buff[0]) // Gasirea task-ului ce trebuie rezolvat 
			{
				case '1':
					printf("%s\n", "Task1");
					task = 1;
					break;
				case '2' :
					printf("%s\n", "Task2");
					task = 2;
					break;	
				case '3' :
					printf("%s\n", "Task3");
					task = 3;
					break;
				case '4' :
					printf("%s\n", "Bonus");
					task = 4;
					break;
				default: break;
			}
		}


		if(task == 1)
		{
			/* Parsarea fisierului de input */

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				N = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					N = N * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", N);
			graf1 = init(N);

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				point_type = (int*)malloc(N*sizeof(int));
				pos = 0;
				length = strlen(buff);
				while (pos < length)
				{
					if(isdigit(buff[pos]))
					{
						point_type[k] = buff[pos] - '0';
						k++;
					}
					pos++;
				}
			}

			for(i = 0; i < k; i++)
				printf("%d ", point_type[i] );

			printf("\n");

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				M = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					M = M * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", M);

			edges = (int*)malloc(3*sizeof(int));

			for(i = 0; i < M; i++)
			{
				if (fgets(buff, BUFF_MAX, input) != NULL)
				{
					s = 0;
					for (char *p = strtok(buff," \n"); p != NULL; p = strtok(NULL, " \n"))
					{
						edges[s] = atoi(p);
						s++;
					}
					for(j = 0; j < s; j++)
						printf("%d ", edges[j]);

					add_edge(graf1, edges[0], edges[1], edges[2]);

					printf("\n");
					for(j = 0; j < s; j++)
						edges[j] = 0;
				}
			}

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				C = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					C = C * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", C);

			orders = (int **)malloc(C * sizeof(int*));
			for(i = 0; i < C; i++) 
				orders[i] = (int *)calloc(2,sizeof(int));

			for(i = 0; i < C; i++)
			{
				if (fgets(buff, BUFF_MAX, input) != NULL)
				{
					y = 0;
					for (char *p = strtok(buff," \n"); p != NULL; p = strtok(NULL, " \n"))
					{
						orders[x][y] = atoi(p);
						y++;
					}
					x++;
				}
			}

			for(i = 0; i < x; i++)
			{
				for(j = 0; j < y; j++)
					printf("%d ", orders[i][j]);
				printf("\n");
			}

			printf("\n");
			print(graf1);
			
			roads = (int **)malloc(N * sizeof(int*));
				for(i = 0; i < N; i++) 
					roads[i] = (int *)calloc(N,sizeof(int));

			next_hop = (int **)malloc(N * sizeof(int*));
				for(i = 0; i < N; i++) 
					next_hop[i] = (int *)calloc(N,sizeof(int));

			init_matrices(graf1, roads, next_hop);
			floyd(graf1, roads, next_hop); 

			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
					printf("%d ", roads[i][j]);
				printf("\n");
			}

			printf("\n");

			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
					printf("%d ", next_hop[i][j]);
				printf("\n");
			}

			printf("\n");

			/* Gasirea fermei de inceput */

			for(i = 0; i < k; i++)
				if(point_type[i] == 1)
					start = i; 
			fprintf(output, "%d ", start);

			/* Generarea drumurilor minime */

			for(i = 0; i < x - 1; i++)
			{
				distance += gen_min_road(start, orders[i][1], roads, next_hop, output);
				fprintf(output, " ");
				distance += gen_min_road(orders[i][1], start, roads, next_hop, output);
				fprintf(output, " ");
			}

			/* Apelarea inca o data in exterior a functiilor de generare a drumului pentru a nu avea spatiu la finalul primului rand in fisierul de output */

			distance += gen_min_road(start, orders[i][1], roads, next_hop, output);
			fprintf(output, " ");
			distance += gen_min_road(orders[i][1], start, roads, next_hop, output);

			fprintf(output, "\n%d\n", distance);
			farms = (int*)malloc(farm_nr*sizeof(int));
			free_memory(&graf1, point_type, edges, orders, roads, next_hop, farms, C, farm_nr, task); // Eliberarea memoriei folosite 
	
		}



		if(task == 2)
		{
			/* Parsarea fisierului de input */

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				N = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					N = N * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", N);
			graf1 = init(N);

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				point_type = (int*)malloc(N*sizeof(int));
				pos = 0;
				length = strlen(buff);
				while (pos < length)
				{
					if(isdigit(buff[pos]))
					{
						point_type[k] = buff[pos] - '0';
						k++;
					}
					pos++;
				}
			}

			for(i = 0; i < k; i++)
				printf("%d ", point_type[i] );

			printf("\n");

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				M = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					M = M * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", M);

			edges = (int*)malloc(3*sizeof(int));

			for(i = 0; i < M; i++)
			{
				if (fgets(buff, BUFF_MAX, input) != NULL)
				{
					s = 0;
					for (char *p = strtok(buff," \n"); p != NULL; p = strtok(NULL, " \n"))
					{
						edges[s] = atoi(p);
						s++;
					}
					for(j = 0; j < s; j++)
						printf("%d ", edges[j]);

					add_edge(graf1, edges[0], edges[1], edges[2]);

					printf("\n");
					for(j = 0; j < s; j++)
						edges[j] = 0;
				}
			}

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				C = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					C = C * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", C);

			orders = (int **)malloc(C * sizeof(int*));
			for(i = 0; i < C; i++) 
				orders[i] = (int *)calloc(2,sizeof(int));

			for(i = 0; i < C; i++)
			{
				if (fgets(buff, BUFF_MAX, input) != NULL)
				{
					y = 0;
					for (char *p = strtok(buff," \n"); p != NULL; p = strtok(NULL, " \n"))
					{
						orders[x][y] = atoi(p);
						y++;
					}
					x++;
				}
			}

			for(i = 0; i < x; i++)
			{
				for(j = 0; j < y; j++)
					printf("%d ", orders[i][j]);
				printf("\n");
			}

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				start = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					start = start * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n\n", start);
			print(graf1);
			
			roads = (int **)malloc(N * sizeof(int*));
				for(i = 0; i < N; i++) 
					roads[i] = (int *)calloc(N,sizeof(int));

			next_hop = (int **)malloc(N * sizeof(int*));
				for(i = 0; i < N; i++) 
					next_hop[i] = (int *)calloc(N,sizeof(int));

			init_matrices(graf1, roads, next_hop);
			floyd(graf1, roads, next_hop);

			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
					printf("%d ", roads[i][j]);
				printf("\n");
			}

			printf("\n");

			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
					printf("%d ", next_hop[i][j]);
				printf("\n");
			}

			printf("\n");

			farm_nr = 0;

			/* Gasirea numarului de ferme */
			for(i = 0; i < k; i++)
				if(point_type[i] == 1)
					farm_nr++;

			farms = (int*)malloc(farm_nr*sizeof(int));

			/* Crearea unui vector de ferme */

			for(i = 0; i < k; i++)
				if(point_type[i] == 1)
				{
					farms[z] = i;
					z++;
				}

			for(i = 0; i < z; i++)
				printf("%d ", farms[i]);
			printf("\n");

			fprintf(output, "%d ", start); // Printarea in fisierul de output a fermei de start 

			/* Generarea drumului minim */

			distance += gen_min_road(start, orders[0][1], roads, next_hop, output);
			fprintf(output, " ");
			distance += gen_min_road(orders[0][1], find_farm(orders[0][1], orders[1][1], farms, roads, farm_nr), roads, next_hop, output);
			fprintf(output, " ");
			for(i = 0; i < x - 2; i++)
			{
				distance += gen_min_road(find_farm(orders[i][1], orders[i + 1][1], farms, roads, farm_nr), orders[i + 1][1], roads, next_hop, output);
				fprintf(output, " ");
				distance += gen_min_road(orders[i + 1][1], find_farm(orders[i + 1][1], orders[i + 2][1], farms, roads, farm_nr), roads, next_hop, output);
				fprintf(output, " ");
				
			}

			/* Apelarea inca o data in exterior a functiilor de generare a drumului pentru a nu avea spatiu la finalul primului rand in fisierul de output */

			distance += gen_min_road(find_farm(orders[i][1], orders[i + 1][1], farms, roads, farm_nr), orders[i + 1][1], roads, next_hop, output);
			fprintf(output, " ");
			distance += gen_min_road(orders[i + 1][1], find_closest_farm(orders[i + 1][1],farms,roads,farm_nr), roads, next_hop, output);
			
			fprintf(output, "\n%d\n", distance);
			free_memory(&graf1, point_type, edges, orders, roads, next_hop, farms, C, farm_nr, task);
	
		}



		if(task == 3)
		{
			/* Parsarea fisierului de input */

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				N = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					N = N * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", N);
			graf1 = init(N);

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				point_type = (int*)malloc(N*sizeof(int));
				pos = 0;
				length = strlen(buff);
				while (pos < length)
				{
					if(isdigit(buff[pos]))
					{
						point_type[k] = buff[pos] - '0';
						k++;
					}
					pos++;
				}
			}

			for(i = 0; i < k; i++)
				printf("%d ", point_type[i] );

			printf("\n");

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				M = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					M = M * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", M);

			edges = (int*)malloc(3*sizeof(int));

			for(i = 0; i < M; i++)
			{
				if (fgets(buff, BUFF_MAX, input) != NULL)
				{
					s = 0;
					for (char *p = strtok(buff," \n"); p != NULL; p = strtok(NULL, " \n"))
					{
						edges[s] = atoi(p);
						s++;
					}
					for(j = 0; j < s; j++)
						printf("%d ", edges[j]);

					add_edge(graf1, edges[0], edges[1], edges[2]);

					printf("\n");
					for(j = 0; j < s; j++)
						edges[j] = 0;
				}
			}

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				C = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					C = C * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n", C);

			orders = (int **)malloc(C * sizeof(int*));
			for(i = 0; i < C; i++) 
				orders[i] = (int *)calloc(2,sizeof(int));

			for(i = 0; i < C; i++)
			{
				if (fgets(buff, BUFF_MAX, input) != NULL)
				{
					y = 0;
					for (char *p = strtok(buff," \n"); p != NULL; p = strtok(NULL, " \n"))
					{
						orders[x][y] = atoi(p);
						y++;
					}
					x++;
				}
			}

			for(i = 0; i < x; i++)
			{
				for(j = 0; j < y; j++)
					printf("%d ", orders[i][j]);
				printf("\n");
			}

			if (fgets(buff, BUFF_MAX, input) != NULL)
			{
				pos = 0;
				start = 0;
				length = strlen(buff);
				while (isdigit(buff[pos]) && pos < length)
				{
					start = start * 10 + buff[pos] - '0';
					pos++;
				}
			}

			printf("%d\n\n", start);
			print(graf1);
			
			roads = (int **)malloc(N * sizeof(int*));
				for(i = 0; i < N; i++) 
					roads[i] = (int *)calloc(N,sizeof(int));

			next_hop = (int **)malloc(N * sizeof(int*));
				for(i = 0; i < N; i++) 
					next_hop[i] = (int *)calloc(N,sizeof(int));

			init_matrices(graf1, roads, next_hop);
			floyd(graf1, roads, next_hop);

			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
					printf("%d ", roads[i][j]);
				printf("\n");
			}

			printf("\n");

			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
					printf("%d ", next_hop[i][j]);
				printf("\n");
			}

			printf("\n");

			farm_nr = 0;

			for(i = 0; i < k; i++)
				if(point_type[i] == 1)
					farm_nr++;

			farms = (int*)malloc(farm_nr*sizeof(int));

			for(i = 0; i < k; i++)
				if(point_type[i] == 1)
				{
					farms[z] = i;
					z++;
				}

			for(i = 0; i < z; i++)
				printf("%d ", farms[i]);
			printf("\n");

			fprintf(output, "%d ", start);

			distance += gen_min_road(start, orders[0][1], roads, next_hop, output);
			fprintf(output, " ");
			distance += gen_min_road(orders[0][1], find_farm(orders[0][1], orders[1][1], farms, roads, farm_nr), roads, next_hop, output);
			//fprintf(output, " ");
			for(i = 0; i < x - 2; i++)
			{
				distance += gen_min_road(find_farm(orders[i][1], orders[i + 1][1], farms, roads, farm_nr), orders[i + 1][1], roads, next_hop, output);
				fprintf(output, " ");
				distance += gen_min_road(orders[i + 1][1], find_farm(orders[i + 1][1], orders[i + 2][1], farms, roads, farm_nr), roads, next_hop, output);
				//fprintf(output, " ");
				
			}
			/*distance += gen_min_road(find_farm(orders[i][1], orders[i + 1][1], farms, roads, farm_nr), orders[i + 1][1], roads, next_hop, output);
			fprintf(output, " ");
			distance += gen_min_road(orders[i + 1][1], find_closest_farm(orders[i + 1][1],farms,roads,farm_nr), roads, next_hop, output);*/
			
			fprintf(output, "\n%d\n", distance);
			free_memory(&graf1, point_type, edges, orders, roads, next_hop, farms, C, farm_nr, task);
	
		}

	}


	fclose(input);
	fclose(output);
	return 0;
}