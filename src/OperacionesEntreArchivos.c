#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "OperacionesEntreArchivos.h"
#include "Archivo.h"
#include "DataBase.h"

void sincronizacionArchivosDB(sqlite3 **db) {
    FILE *csv = fopen(RUTA, "r");
    if (!csv) {
        printf("Error en la apertura del archivo");
        return;
    }

    abrirDB(db);
    //Ya estan los dos abiertos
    Juego j;
    int fila = 0;
    int resultado;
    int modificados = 0;
    int insertados = 0;
    int aux=0;

    if (tablaVacia(*db)) {
        insertarTodoDB(*db, csv);
    } else {

           while ((resultado = LeerRegistroIndividual(csv, &j, &fila)) != 0)
            if (resultado == 1) {
                if (j.ID == 0) {
                    printf("ERROR: ID=0 en fila %d - REGISTRO INVÁLIDO, OMITIENDO\n", fila);
                    continue;  // Saltar este registro completamente
                }

                if (existeJuego(*db, j.ID)) {
                    aux=actualizarRegistro(*db, &j);
                    if(aux==101) //101=Exito en Update de sqlite
                    modificados++;
                } else {
                    insertarJuego(*db, &j);
                    insertados++;
                }
    }
    printf("\n--------------------------\n");
    printf("Modificados %d registros\n", modificados);
    printf("Insertados %d registros\n", insertados);
    printf("--------------------------\n");
    }
    printf("Cerrando archivo CSV y DB\n");
    printf("--------------------------\n");
    cerrarDB(*db);
    fclose(csv);
}


int insertarTodoDB(sqlite3 *db, FILE *csv)
{
    Juego j;
    int fila = 0;
    int insertados = 0;
    int resultado;

    while ((resultado = LeerRegistroIndividual(csv, &j, &fila)) != 0) {
        if (resultado == 1) {  // Solo procesar si es un registro válido (no encabezado)
            if (insertarJuego(db, &j) == SQLITE_DONE) {
                insertados++;
            }
        }
        // Si resultado == -1, es encabezado, lo ignoramos y continuamos
    }
    printf("\n--------------------------\n");
    printf("Insertados %d registros\n", insertados);
    printf("--------------------------\n");
    return insertados;
}


int LeerRegistroIndividual(FILE *csv, Juego *j, int *fila) {
    char linea[MAX_LINEA];

    if (fgets(linea, sizeof(linea), csv) == NULL) {
        return 0;  // Fin de archivo
    }

    (*fila)++;

    if (*fila <= COMIENZO_TABLA) {
        return -1;
    }

    memset(j, 0, sizeof(Juego));

    linea[strcspn(linea, "\n")] = 0;

    char *campo;
    int columna = 0;

    campo = strtok(linea, ",");
    while (campo != NULL) {
        switch (columna) {
            case 0: j->ID = atoi(campo); break;

            case 1: strcpy(j->Nombre, campo); break;

            case 2: strcpy(j->FechaFinalizacion, campo); break;

            case 3:
              if (atof(campo) == 0.0) {
                    j->PuntajeCritico = 0.0; //usaba -1 pero es incopatible con restriccion que puse en campo SQLite
                } else {
                    j->PuntajeCritico = atof(campo);
                }
                break;

            case 4:
                if (atof(campo) == 0.0) {
                    j->PuntajePersonal = 0.0; //IDEM
                } else {
                    j->PuntajePersonal = atof(campo);
                }

                break;
            case 5: strcpy(j->Genero, campo); break;

            case 6: strcpy(j->Comentarios, campo); break;

            case 7:
                    // Limpiar TODO excepto letras
                    char limpio[100];
                    int idx = 0;

                    for (int i = 0; campo[i] && idx < 99; i++) {
                        char c = campo[i];
                        // Solo mantener A-Z, a-z
                        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                            limpio[idx++] = toupper(c);
                        }
                    }
                    limpio[idx] = '\0';

                    // Ahora comparar
                    if (strcmp(limpio, "SI") == 0) {
                        strcpy(j->Rejugar, "SI");
                    } else if (strcmp(limpio, "NO") == 0) {
                        strcpy(j->Rejugar, "NO");
                    } else {
                        strcpy(j->Rejugar, "DESCONOCIDO");
                    }

                    break;
            // case 8:
        }

    campo = strtok(NULL, ",");
    columna++;
    }

return 1;
}



int actualizarRegistro(sqlite3 *db, Juego *j)
{
sqlite3_stmt *stmt;
    // Solo actualizamos los campos que vienen del CSV
    // NO actualizamos plataforma ni imagen_url (vienen de API)
    const char *sql = "UPDATE juego SET "
                      "nombre = ?, "
                      "fecha_finalizacion = ?, "
                      "puntaje_critico = ?, "
                      "puntaje_personal = ?, "
                      "genero = ?, "
                      "comentario = ?, "
                      "rejugar = ? "
                      "WHERE id_juego = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando UPDATE: %s\n", sqlite3_errmsg(db));
        return SQLITE_ERROR;
    }

    // Bind de parámetros (solo los del CSV)
    sqlite3_bind_text(stmt, 1, j->Nombre, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, j->FechaFinalizacion, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, j->PuntajeCritico);
    sqlite3_bind_double(stmt, 4, j->PuntajePersonal);
    sqlite3_bind_text(stmt, 5, j->Genero, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, j->Comentarios, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, j->Rejugar, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, j->ID);

    int resultado = sqlite3_step(stmt);

    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error actualizando juego ID %d: %s\n", j->ID, sqlite3_errmsg(db));
    } else {
        printf("Actualizado juego ID %d: %s\n", j->ID, j->Nombre);
    }

    sqlite3_finalize(stmt);
    return resultado;
}



int insertarJuego(sqlite3 *db, Juego *j)
{
sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO juego "
                      "(id_juego, nombre, fecha_finalizacion, puntaje_critico, puntaje_personal, "
                      "genero, comentario, rejugar, plataforma, imagen_url) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando INSERT: %s\n", sqlite3_errmsg(db));
        return SQLITE_ERROR;
    }

    sqlite3_bind_int(stmt, 1, j->ID);
    sqlite3_bind_text(stmt, 2, j->Nombre, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, j->FechaFinalizacion, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, j->PuntajeCritico);
    sqlite3_bind_double(stmt, 5, j->PuntajePersonal);
    sqlite3_bind_text(stmt, 6, j->Genero, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, j->Comentarios, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, j->Rejugar, -1, SQLITE_TRANSIENT);

    // Campos futuros (API) - pueden ser NULL o valores por defecto
    sqlite3_bind_null(stmt, 9);   // plataforma (futuro)
    sqlite3_bind_null(stmt, 10);  // imagen_url (futuro)


    int resultado = sqlite3_step(stmt);

    if (resultado != SQLITE_DONE) {
        fprintf(stderr, "Error insertando juego ID %d: %s\n", j->ID, sqlite3_errmsg(db));
    } else {
        printf("Insertado juego ID %d: %s\n", j->ID, j->Nombre);
    }

    sqlite3_finalize(stmt);
    return resultado;
}




int existeJuego(sqlite3 *db, int id)
{
sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM juego WHERE id_juego = ?;";
    int existe = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        existe = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return existe;
}
