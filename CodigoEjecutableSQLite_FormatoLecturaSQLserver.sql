--================================================================================================================
/*
Proyecto personal, tengo mi "base de datos" en excel. Tiene informacion de mis juegos pasados

Quiero lograr cargar datos de determinadas columnas por medio de APIS, tambien quiero que se actualize automaticamente
cada x tiempo, asi, si tengo una nueva entrada en el excel, se trae hacia SQL (no se si es conexion directa todavia
o tengo que hacerlo manualmente) y luego mi ejecutable en C tambien estara automatizado, realizando la conexion
y modificando los registros, validandolos de paso.

IMPORTANTE
Voy a trabajar con SQLite, tiene distintos comandos para ciertas cosas y limitaciones
las aclaro en comentarios, lo dejo en formato SQL general el ejecutable ya que
SQLite ejecuta el codigo, pero no lo guarda
*/
--================================================================================================================

/*IF  NOT EXISTS(SELECT 1 FROM SYS.DATABASES WHERE NAME = 'JUEGOS')
	CREATE DATABASE JUEGOS;
GO
USE JUEGOS
GO
set nocount on
GO
--creacion de esquema para la creación de la db
IF NOT EXISTS(SELECT 1 FROM SYS.schemas WHERE name LIKE 'dbJuegos')
	 EXEC('CREATE SCHEMA dbJuegos;');
GO
--Si creo mas tablas a futuro, debo crear su scheme correspondiente para trabajar modularmente

--**********CREACION DE TABLAS***********

IF NOT EXISTS ( SELECT 1 FROM sys.tables  WHERE name = 'juego' AND schema_id = SCHEMA_ID('dbJuegos')) 
BEGIN
	CREATE TABLE dbJuegos.juego(

	id_juego INT INTEGER PRIMARY KEY, --NO TIENE IDENTITY, SE USA INTEGER, AUTOINCREMENTAL
	nombre TEXT CHECK (length(nombre) <= 100), --NO HAY VARCHAR, SE USA TEXT
	fecha_finalizacion TEXT, --NO TIENE FORMATO FECHA DATETIME
	puntaje_critico REAL,
	puntaje_personal REAL,
	comentario TEXT CHECK(LENGTH(comentario)<=1000), 
	plataforma TEXT CHECK(LENGTH(plataforma)<=15), --TRAER DATO POR API
	imagen_url TEXT CHECK(LENGTH(imagen_url)<=2000) -- TRAER DATO POR API
	);

GO
*/
--Muy de SQLSERVER, SQLite no permite casi nada de condicionales, es mas para ejecutarlo en seco
CREATE TABLE IF NOT EXISTS juego(
    id_juego INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT CHECK(length(nombre) <= 100),
    fecha_finalizacion TEXT,
    puntaje_critico REAL CHECK(puntaje_critico>=0 AND puntaje_critico<=10),
    puntaje_personal REAL CHECK(puntaje_personal>=0 AND puntaje_personal<=10),
	genero TEXT CHECK(LENGTH(genero)<=30),
    comentario TEXT CHECK(length(comentario) <= 1000),
	rejugar BINARY,
    plataforma TEXT CHECK(length(plataforma) <= 15),
    imagen_url TEXT CHECK(length(imagen_url) <= 2000)
);


/* Errores en medio del ejecutable C
Primero, del excel me pueden llegar valores 0 en puntaje, lo que hizo que tenga que modificar el codigo en C (esta comentado igual)

Segundo, La tabla en SQL no me guarda en formato .2f los puntajes, lo que hace que sea un desastre leerlo. Por lo tanto, voy a ejecutar en el IDE de SQLite

DELETE FROM juego;                                    Para borrar los datos

UPDATE juego 
SET puntaje_critico = ROUND(puntaje_critico, 2),
 puntaje_personal = ROUND(puntaje_personal, 2);       Para actualizar la vista de los puntajes

 

*/