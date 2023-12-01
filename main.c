#include "src/cajas.h"
#include "src/pokemon.h"
#include <stdio.h>

void imprimir_caja(pokemon_t *pokemon){
	printf("Nombre: %s | ", pokemon_nombre(pokemon));
	printf("Nivel: %d | ", pokemon_nivel(pokemon));
	printf("Ataque: %d | ", pokemon_ataque(pokemon));
	printf("Defensa: %d\n", pokemon_defensa(pokemon));
}

int main(int argc, char *argv[]) {

		caja_t* caja1 = caja_cargar_archivo("file1.csv");
	
		caja_t* caja2 = caja_cargar_archivo("file2.csv");

		caja_recorrer(caja1, imprimir_caja);
		printf("\n");
		caja_recorrer(caja2, imprimir_caja);
		printf("\n");

		caja_t* caja3 = caja_combinar(caja1, caja2);

		caja_destruir(caja1);
		caja_destruir(caja2);

		caja_recorrer(caja3, imprimir_caja);
		printf("\n");

		caja_guardar_archivo(caja3, "file3.csv");

		caja_destruir(caja3);

	return 0;
}
