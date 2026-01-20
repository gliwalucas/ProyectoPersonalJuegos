#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include "sqlite3.h"
#include <stdio.h>

int abrirDB(sqlite3 **db);
void cerrarDB(sqlite3 *db);

void listarDB(sqlite3 *db);
void actualizarDB(sqlite3 *db);

int tablaVacia(sqlite3 *db);

int insertarTodoDB(sqlite3 *db,FILE *csv);

#endif // DATABASE_H_INCLUDED
