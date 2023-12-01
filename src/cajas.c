#include "cajas.h"
#include "pokemon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LEER "r"
#define ESCRIBIR "w"
#define MAX_LEER 200
#define FORMATO_ESCRITURA "%s;%d;%d;%d\n"

struct _caja_t {
		pokemon_t** pokemones;
		size_t cantidad;
};

void selection(pokemon_t** vector, int n){
		int max;
		for (int i = n-1; i > 0; i--) {
				max = 0;
				for (int j = 0; j < i+1; j++) {
        				if (strcmp(pokemon_nombre(vector[j]), 
							pokemon_nombre(vector[max])) > 0) {
								max = j;
						}
				}
				if (strcmp(pokemon_nombre(vector[i]),
					pokemon_nombre(vector[max])) == 0) {
						continue;
				}
				void *aux = vector[i];
				vector[i] = vector[max];
				vector[max] = aux;
	}
}

caja_t *caja_cargar_archivo(const char *nombre_archivo)
{	
		if (!nombre_archivo){
				return NULL;
		}

		FILE *archivo = fopen(nombre_archivo, LEER);
		if (!archivo) {
				return NULL;
		}

		caja_t *caja = malloc(sizeof(caja_t));
		if (!caja) {
				fclose(archivo);
				return NULL;
		}

		caja->pokemones = malloc(0);
		if (!caja->pokemones) {
				free(caja);
				fclose(archivo);
				return NULL;
		}

		caja->cantidad = 0;

		char cadena[MAX_LEER];
		while (fgets(cadena, MAX_LEER, archivo) != NULL) {

				pokemon_t* pokemon = pokemon_crear_desde_string(cadena);
				if (!pokemon) {
						continue;
				}

				pokemon_t** nuevo_pokemones = realloc(caja->pokemones, 
										(caja->cantidad+1)*sizeof(pokemon_t*));
				if (!nuevo_pokemones) {
						caja_destruir(caja);
						fclose(archivo);
						return NULL;
				}

				caja->pokemones = nuevo_pokemones;
				caja->pokemones[caja->cantidad] = pokemon;
				caja->cantidad++;
		}
		selection(caja->pokemones, (int)caja->cantidad);
		fclose(archivo);
		return caja;
}

int caja_guardar_archivo(caja_t *caja, const char *nombre_archivo)
{
		if (!caja || !nombre_archivo) {
				return 0;
		}

		FILE *archivo = fopen(nombre_archivo, ESCRIBIR);
		if (!archivo) {
				return 0;
		}
		int contador;
		for (contador = 0; contador < caja->cantidad; contador++) {
				fprintf(archivo, FORMATO_ESCRITURA, 
				pokemon_nombre(caja->pokemones[contador]), 
				pokemon_nivel(caja->pokemones[contador]), 
				pokemon_ataque(caja->pokemones[contador]), 
				pokemon_defensa(caja->pokemones[contador]));
		}
		fclose(archivo);
		return contador;
}

caja_t *crear_nuevos_pokemones(caja_t *caja1, caja_t *caja2){
	
		char cadena[MAX_LEER];
		for (int i = 0; i < caja1->cantidad; i++) {

			sprintf(cadena, "%s;%d;%d;%d\n", 
			pokemon_nombre(caja1->pokemones[i]), 
			pokemon_nivel(caja1->pokemones[i]), 
			pokemon_ataque(caja1->pokemones[i]), 
			pokemon_defensa(caja1->pokemones[i]));		
			pokemon_t *pokemon = pokemon_crear_desde_string(cadena);
			if (!pokemon) {
					continue;
			}

			pokemon_t **nuevo_pokemones = realloc(caja2->pokemones, 
										(caja2->cantidad+1)*sizeof(pokemon_t*));
			if (!nuevo_pokemones) {
					pokemon_destruir(pokemon);
					return NULL;
			}

			caja2->pokemones = nuevo_pokemones;
			caja2->pokemones[caja2->cantidad] = pokemon;
			caja2->cantidad++;
		}

		return caja2;
}

caja_t *caja_combinar(caja_t *caja1, caja_t *caja2)
{	
		if (!caja1 || !caja2) {
				return NULL;
		}

		caja_t *caja3 = malloc(sizeof(caja_t));
		if (!caja3) {
				return NULL;
		}

		caja3->pokemones = malloc(0);
		if (!caja3->pokemones) {
				free(caja3);
				return NULL;
		}
		caja3->cantidad = 0;

		if (!crear_nuevos_pokemones(caja1, caja3) || 
			!crear_nuevos_pokemones(caja2, caja3)) {
				caja_destruir(caja3);
				return NULL;
		}

		selection(caja3->pokemones, (int)caja3->cantidad);
		return caja3;
}

int caja_cantidad(caja_t *caja)
{
		if (!caja) {
				return 0;
		}
		return (int)caja->cantidad;
}

pokemon_t *caja_obtener_pokemon(caja_t *caja, int n)
{	
		if (!caja || n < 0 || n >= caja->cantidad) {
				return NULL;
		}
		return caja->pokemones[n];
}

int caja_recorrer(caja_t *caja, void (*funcion)(pokemon_t *))
{	
		if (!caja || !funcion) {
				return 0;
		}

		int contador;
		for (contador = 0; contador < caja->cantidad; contador++) {
				funcion(caja->pokemones[contador]);
		}
		return contador;
}

void caja_destruir(caja_t *caja)
{
		if (!caja) {
				return;
		}

		caja_recorrer(caja, pokemon_destruir);
		free(caja->pokemones);
		free(caja);
}