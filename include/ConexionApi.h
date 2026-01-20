#ifndef CONEXIONAPI_H_INCLUDED
#define CONEXIONAPI_H_INCLUDED
#include <stddef.h>
#include "Archivo.h"
#include "DataBase.h"

#define RAWG_API_KEY "df92a5b1f70b406db0a851be04a8e0a3"

#define TOP1_FILA 5
#define TOP1_COL  1    // Columna A

#define TOP2_FILA 5
#define TOP2_COL  2    // Columna B

#define TOP3_FILA 5
#define TOP3_COL  3    // Columna C

typedef struct {
    char *data;
    size_t size;
} HTTP_Response;



char* obtener_plataforma_api(const char *nombre_juego);
char* obtener_imagen_api(const char *nombre_juego);
size_t escribir_datos(void *contents, size_t size, size_t nmemb, void *userp); //Junta todos los valores consegidos en el JSON de API
char* llamar_rawg_api(const char *nombre_juego);
void probar_api(void);

void procesar_api_completo(sqlite3 *db, const char *csv_file);

int actualizarPlataformaApi(Juego *juego);
void actualizar_db_con_api(sqlite3 *db);
void actualizar_top3_con_formulas(const char *csv_file, sqlite3 *db);


//void escribir_formula_en_celda(const char *csv_file, int fila, int columna, const char *url);
//void test_formula_directo(void);

void escribir_formula_en_celda_corregida(const char *csv_file, int fila, int columna, const char *url);

#endif // CONEXIONAPI_H_INCLUDED
