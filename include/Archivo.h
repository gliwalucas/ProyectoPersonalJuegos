#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED
#include <stdio.h>

#define MAX_LINEA 1024
#define COMIENZO_TABLA 6
#define RUTA "C:\\Users\\Lucas\\OneDrive\\Desktop\\ProyectoJuegos\\JuegosExcelLectura.csv"

typedef struct{
int ID;
char Nombre[200];
char FechaFinalizacion[20];
float PuntajeCritico;
float PuntajePersonal;
char Genero[30];
char Comentarios[300];
char Rejugar[8];
char Plataforma[30]; //Esta vacio hasta que traiga el dato por API
char FotoUrl[800]; //Este campo solo se usa en escritura, cuando lo busque por API, en lectura excel generalmente es null, y no va a estar en todos los registros
}Juego;

void LeerRegistros();

#endif // ARCHIVO_H_INCLUDED
