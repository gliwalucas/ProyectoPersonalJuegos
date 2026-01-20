#ifndef FORMATOEXCEL_H_INCLUDED
#define FORMATOEXCEL_H_INCLUDED
#include <xlsxwriter.h>

#define RUTAexcel "C:/Users/Lucas/OneDrive/Desktop/JuegosFinal.xlsx"

void crear_excel_desde_csv(const char *csv_file, const char *excel_file);
void procesar_fila_estadisticas(lxw_worksheet *ws, char *linea,lxw_format *fmt_normalDos, lxw_format *fmt_especial) ;
void procesar_fila_imagenes(lxw_worksheet *ws, char *linea, lxw_format *fmt);
void procesar_fila_datos(lxw_worksheet *ws, int excel_fila, char *linea, lxw_format *fmt,lxw_format *fmt2);
void procesar_fila_instrucciones(lxw_worksheet *ws, char *linea, lxw_format *fmt);
void procesar_fila_top(lxw_worksheet *ws, char *linea, lxw_format *fmt);
void procesar_encabezados_tabla(lxw_worksheet *ws, char *linea, lxw_format *fmt);

//BORRAR SI NO SIRVE


#endif // FORMATOEXCEL_H_INCLUDED
