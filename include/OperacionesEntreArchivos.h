#ifndef OPERACIONESENTREARCHIVOS_H_INCLUDED
#define OPERACIONESENTREARCHIVOS_H_INCLUDED
#include "sqlite3.h"
#define RUTA "C:\\Users\\Lucas\\OneDrive\\Desktop\\ProyectoJuegos\\JuegosExcelLectura.csv"
#include "Archivo.h"

void sincronizacionArchivosDB(sqlite3 **db);

int insertarTodoDB(sqlite3 *db,FILE *csv);

int actualizarRegistro(sqlite3 *db, Juego *j);
void actualizarJuego(sqlite3 *db, Juego *j);

int LeerRegistroIndividual(FILE *csv, Juego *j, int* fila);
int existeJuego(sqlite3 *db, int id);
int insertarJuego(sqlite3 *db, Juego *j);

#endif // OPERACIONESENTREARCHIVOS_H_INCLUDED
