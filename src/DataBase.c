#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataBase.h"

int abrirDB(sqlite3 **db)
{
    int rc = sqlite3_open("juegos.db", db);

    if (rc != SQLITE_OK) {
        printf("Error al abrir DB: %s\n", sqlite3_errmsg(*db));
        return 0;
    }

    printf("DB abierta correctamente\n");
    return 1;
}



void cerrarDB(sqlite3 *db)
{
    sqlite3_close(db);
    printf("DB cerrada\n");
    printf("-------------------------------------------------------------------\n");
}


void listarDB(sqlite3 *db)
{
    const char *sql =
        "SELECT id_juego, nombre, fecha_finalizacion, "
        "puntaje_critico, puntaje_personal, genero, comentario, "
        "rejugar, plataforma, imagen_url "
        "FROM juego;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error en SELECT: %s\n", sqlite3_errmsg(db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *nombre = sqlite3_column_text(stmt, 1);
        const unsigned char *fecha = sqlite3_column_text(stmt, 2);
        double critico = sqlite3_column_double(stmt, 3);
        double personal = sqlite3_column_double(stmt, 4);
        const unsigned char *genero = sqlite3_column_text(stmt, 5);
        const unsigned char *comentario = sqlite3_column_text(stmt, 6);
        int rejugar = sqlite3_column_int(stmt, 7);
        const unsigned char *plataforma = sqlite3_column_text(stmt, 8);
        const unsigned char *imagen = sqlite3_column_text(stmt, 9);

        printf("ID: %d\n", id);
        printf("Nombre: %s\n", nombre ? nombre : (unsigned char *)"(NULL)");
        printf("Fecha: %s\n", fecha ? fecha : (unsigned char *)"(NULL)");
        printf("Puntaje critico: %.1f\n", critico);
        printf("Puntaje personal: %.1f\n", personal);
        printf("Genero: %s\n", genero ? genero : (unsigned char *)"(NULL)");
        printf("Comentario: %s\n", comentario ? comentario : (unsigned char *)"(NULL)");
        printf("Rejugar: %s\n", rejugar ? "SI" : "NO");
        printf("Plataforma: %s\n", plataforma ? plataforma : (unsigned char *)"(NULL)");
        printf("Imagen URL: %s\n", imagen ? imagen : (unsigned char *)"(NULL)");
        printf("---------------------------------\n");
    }

sqlite3_finalize(stmt);
}

int tablaVacia(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    int vacia = 1;

    const char *sql = "SELECT COUNT(*) FROM juego;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) > 0) {
            vacia = 0;
        }
    }

    sqlite3_finalize(stmt);
    return vacia;
}


