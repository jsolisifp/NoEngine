#ifndef NOENTIENDODB_HPP
#define NOENTIENDODB_HPP

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                          NoEntiendo DB                              //
//                                                                     //
//      Una base de datos pedagógica para la librería NoEntiendo       //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                           LICENCIA                                  //
//                                                                     //
//    Attribution-NonCommercial-NoDerivatives 4.0 International        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

// Dependencias

#include "NoEntiendo.hpp"

// Enums

enum NOEDB_Operador
{
    NOEDB_OPERADOR_IGUAL = 0,
    NOEDB_OPERADOR_DIFERENTE,
    NOEDB_OPERADOR_MENOR,
    NOEDB_OPERADOR_MENORIGUAL,
    NOEDB_OPERADOR_MAYOR,
    NOEDB_OPERADOR_MAYORIGUAL
    
};

enum NOEDB_TipoOrdenacion
{
    NOEDB_TIPOORDENACION_ASCENDENTE,
    NOEDB_TIPOORDENACION_DESCENDENTE
};

// Funciones principales

void NOEDB_Inicia();
void NOEDB_Finaliza();

// Gestion de tablas

bool NOEDB_ExisteTabla(char* tabla);
void NOEDB_EliminaTabla(char* tabla);

// Definicion de tablas

void NOEDB_DefineTabla(char* tabla, int dataset);
void NOEDB_DefineEntero(char* columna);
void NOEDB_DefineTexto(char* columna);
void NOEDB_DefineClaveExterna(char* columna, char* tabla);
void NOEDB_FinDefineTabla();

// Insercion de filas

void NOEDB_InsertaFila(char* tabla);
void NOEDB_InsertaEntero(char* columna, int valor);
void NOEDB_InsertaTexto(char* columna, char* valor);
void NOEDB_InsertaClaveExterna(char* columna, int valor);
int NOEDB_FinInsertaFila();

// Borrado de filas

void NOEDB_BorraFilas(char* tabla);
void NOEDB_BorraSiClavePrimaria(int valor);
void NOEDB_BorraSiEntero(char* columna, NOEDB_Operador operador, int valor);
void NOEDB_BorraSiTexto(char* columna, NOEDB_Operador operador, char* valor);
void NOEDB_BorraSiClaveExterna(int valor);
int NOEDB_FinBorraFilas();

// Modificacion de filas

void NOEDB_ModificaFilas(char* tabla);
void NOEDB_ModificaSiClavePrimaria(int valor);
void NOEDB_ModificaSiEntero(char* columna, NOEDB_Operador operador, int valor);
void NOEDB_ModificaSiTexto(char* columna, NOEDB_Operador operador, char* valor);
void NOEDB_ModificaEntero(char* columna, int valor);
void NOEDB_ModificaTexto(char* columna, char* valor);
void NOEDB_ModificaClaveExterna(char* columna, int valor);
int NOEDB_FinModificaFilas();

// Ordenacion

void NOEDB_OrdenaTabla(char* tabla);
void NOEDB_OrdenaPor(char* columna, NOEDB_TipoOrdenacion ordenacion);
void NOEDB_FinOrdenaTabla();

// Busqueda de filas

void NOEDB_BuscaFilas(char* tabla);
void NOEDB_BuscaSiClavePrimaria(int valor);
void NOEDB_BuscaSiEntero(char* columna, NOEDB_Operador operador, int valor);
void NOEDB_BuscaSiTexto(char* columna, NOEDB_Operador operador, char* valor);
void NOEDB_BuscaSiClaveExterna(char* columna, int valor);
void NOEDB_FinBuscaFilas();

bool NOEDB_LeeResultado();
int NOEDB_LeeClavePrimaria();
int NOEDB_LeeEntero(char* columna);
void NOEDB_LeeTexto(char* columna, char* texto);
int NOEDB_LeeClaveExterna(char* columna);
void NOEDB_FinLeeResultado();

// Utilidades

void NOEDB_ImprimeTabla(char *tabla);

#endif

