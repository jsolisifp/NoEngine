#include <NoEntiendoDB.hpp>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ERRORSI(_cond_,_mess_,_param_,_ret_) if(_cond_) { sprintf(errorStr, _mess_, _param_); printf("NOEDB:ERROR:%s\n", errorStr); _ret_; }

const char noedbNombreLibreria[] = "NoEntiendoDB";
const int noedbVersionMajor = 0;
const int noedbVersionMinor = 92;

// Constantes

const int noedbMaxNombre = 40;

const char noedbNombreColumnaClave[noedbMaxNombre] = "_Id";

const char noedbMaxCriterios = 32;
const char noedbMaxModificaciones = 16;
const char noedbMaxOrdenaciones = 16;

const int noedbMaxColumnas = 16;
const int noedbMaxTexto = 80;

const int noedbMaxTamanyoFila = sizeof(int) + (noedbMaxColumnas - 1) * (noedbMaxTexto + 1);

const int noedbMaxLineaImprimir = 80;

// Enums

enum NOEDBTipoColumna
{
    NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA,
    NOEDB_TIPOCOLUMNA_ENTERO,
    NOEDB_TIPOCOLUMNA_TEXTO,
    NOEDB_TIPOCOLUMNA_CLAVEEXTERNA,
};

enum NOEDBOperacion
{
    NOEDB_OPERACION_NINGUNA,
    NOEDB_OPERACION_DEFINE,
    NOEDB_OPERACION_INSERTA,
    NOEDB_OPERACION_MODIFICA,
    NOEDB_OPERACION_BORRA,
    NOEDB_OPERACION_ORDENA,
    NOEDB_OPERACION_BUSCA,
    NOEDB_OPERACION_LEERESULTADO
    
};

// Structs

struct NOEDBCriterio
{
    char nombreColumna[noedbMaxNombre];
    NOEDB_Operador operador;
    NOEDBTipoColumna tipoValor;
    int valorEntero;
    char valorTexto[noedbMaxTexto + 1];
};

struct NOEDBModificacion
{
    char nombreColumna[noedbMaxNombre];
    NOEDBTipoColumna tipoValor;
    int valorEntero;
    char valorTexto[noedbMaxTexto + 1];
};

struct NOEDBOrdenacion
{
    char nombreColumna[noedbMaxNombre];
    NOEDB_TipoOrdenacion tipoOrdenacion;
};

struct NOEDBDefinicionColumna
{
    char nombre[noedbMaxNombre];
    NOEDBTipoColumna tipo;
    char tablaExterna[noedbMaxNombre];
};

struct NOEDBDefinicionTabla
{        
    char nombre[noedbMaxNombre];
    int numColumnas;
    NOEDBDefinicionColumna columnas[noedbMaxColumnas];
};

struct NOEDBPropiedadesTabla
{
    int ultimaClaveGenerada;
    int numFilas;
};

// Funciones privadas

int BuscaDatasetTabla(char* tabla);
void ImprimeDefinicionTabla(NOEDBDefinicionTabla& tabla);
void ImprimeDatosTabla(int indiceDataset, NOEDBDefinicionTabla& tabla, NOEDBPropiedadesTabla& propiedades);
void ObtenCabeceraTabla(int indiceDataset, NOEDBDefinicionTabla& definicion, NOEDBPropiedadesTabla& propiedades);
int ObtenTamanyoCabeceraTabla(NOEDBDefinicionTabla& tabla);
int ObtenTamanyoFila(NOEDBDefinicionTabla& tabla);
int ObtenOffsetColumna(char* columna, NOEDBDefinicionTabla& tabla);  
bool ValorEnteroCumpleCriterio(int valor, NOEDB_Operador operador, int valorOperador);
bool ValorTextoCumpleCriterio(const char valor[], NOEDB_Operador operador, const char valorOperador[]);
bool FilaCursorCumpleCriterios();
void AnyadeCriterioClavePrimaria(int valor);
void AnyadeCriterioEntero(char* columna, NOEDB_Operador operador, int valor);
void AnyadeCriterioTexto(char* columna, NOEDB_Operador operador, char* valor);
void AnyadeCriterioClaveExterna(char* columna, int valor);
int ComparaOrdenFilas(int cursorFila1, int cursorFila2);
int BuscaColumna(char* columna, NOEDBDefinicionTabla& tabla);
bool TablaEsReferenciada(char* tabla);
bool TablaContieneClavePrimaria(char* tabla, int valor);
bool ClavePrimariaEsReferenciada(char* tabla, int clave);
bool TablaDatasetEsReferenciada(int dataset);

// Variables

NOEDBCriterio criterios[noedbMaxCriterios];
int numCriterios;

NOEDBModificacion modificaciones[noedbMaxModificaciones];
int numModificaciones;

NOEDBOrdenacion ordenaciones[noedbMaxOrdenaciones];
int numOrdenaciones;

NOE_Byte bufferFila[noedbMaxTamanyoFila];

NOEDBDefinicionTabla definicionTabla;
NOEDBPropiedadesTabla propiedadesTabla;

int indiceDataset;
int cursorFila;

int tamanyoFila;
int tamanyoCabecera;

NOEDBOperacion operacionActual;

char errorStr[noedbMaxLineaImprimir];

// Funciones de la libreria

void NOEDB_Inicia()
{
    printf("----------------------------------------------\n");
    printf("  %s Version %d.%d\n", noedbNombreLibreria, noedbVersionMajor, noedbVersionMinor);
    printf("----------------------------------------------\n");
    printf("\n");
    printf("Base de datos iniciada\n");        
    printf("\n");
    
    operacionActual = NOEDB_OPERACION_NINGUNA;

    sprintf(errorStr, "");
    
    
}
void NOEDB_Finaliza()
{
    // Nada que hacer por ahora
}


bool NOEDB_ExisteTabla(char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "Esta operacion no se puede realizar ahora porque hay otra pendiente de finalizar","",return false)
    
    int resultado = BuscaDatasetTabla(tabla);

    return resultado >= 0;
}

void NOEDB_EliminaTabla(char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "Esta operacion no se puede realizar ahora porque hay otra pendiente de finalizar","",return)

    int indiceDataset = BuscaDatasetTabla(tabla);
    
    ERRORSI(indiceDataset < 0,"La tabla %s no se puede eliminar porque no existe",tabla,return)
    ERRORSI(TablaEsReferenciada(tabla),"La tabla %s no se puede eliminar porque esta referenciada por una clave externa en otra tabla", tabla,return)
    
    NOE_RedimensionaDataset(indiceDataset, 0);
    
    NOE_GuardaDatasetAhora(indiceDataset);
}

void NOEDB_DefineTabla(char* tabla, int dataset)
{     
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "No puedes iniciar la definicion porque hay otra operacion pendiente de finalizar","",return)
    ERRORSI(TablaDatasetEsReferenciada(dataset), "La tabla %s no se puede definir porque existe una tabla definida en el dataset a la cual otras tablas hacen referencia","",return)
    ERRORSI(BuscaDatasetTabla(tabla) >= 0, "La tabla %s no se puede definir porque ya existe\n", tabla,return)
     
    indiceDataset = dataset;
    
    NOEDBDefinicionTabla &defT = definicionTabla;
    
    strcpy(defT.nombre, tabla);
    
    NOEDBDefinicionColumna &defC = defT.columnas[0];

    strcpy(defC.nombre, noedbNombreColumnaClave);
    defC.tipo = NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA;
    
    defT.numColumnas = 1;
    
    operacionActual = NOEDB_OPERACION_DEFINE;
}

void NOEDB_DefineEntero(char* columna)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_DEFINE, "Esta operacion debe ir precedida del inicio de definicion de tabla","",return)
    
    ERRORSI(BuscaColumna(columna, definicionTabla) >= 0,"La columna %s ya existe",columna,return)
    
    NOEDBDefinicionTabla &defT = definicionTabla;
    NOEDBDefinicionColumna &defC = defT.columnas[defT.numColumnas];
    
    strcpy(defC.nombre, columna);
    defC.tipo = NOEDB_TIPOCOLUMNA_ENTERO;
    defT.numColumnas ++;
}

void NOEDB_DefineTexto(char* columna)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_DEFINE, "Esta operacion debe ir precedida del inicio de definicion de tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) >= 0,"La columna %s ya existe",columna,return)

    NOEDBDefinicionTabla &defT = definicionTabla;
    NOEDBDefinicionColumna &defC = defT.columnas[defT.numColumnas];
    
    strcpy(defC.nombre, columna);
    defC.tipo = NOEDB_TIPOCOLUMNA_TEXTO;
    defT.numColumnas ++;
}

void NOEDB_DefineClaveExterna(char* columna, char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_DEFINE, "Esta operacion debe ir precedida del inicio de definicion de tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) >= 0,"La columna %s ya existe",columna,return)
    ERRORSI(BuscaDatasetTabla(tabla) < 0, "La tabla a que hace referencia la clave externa %s no existe\n", columna,return)

    NOEDBDefinicionTabla &defT = definicionTabla;
    NOEDBDefinicionColumna &defC = defT.columnas[defT.numColumnas];
    
    strcpy(defC.nombre, columna);
    defC.tipo = NOEDB_TIPOCOLUMNA_CLAVEEXTERNA;
    strcpy(defC.tablaExterna, tabla);
    defT.numColumnas ++;
}    

void NOEDB_FinDefineTabla()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_DEFINE, "Esta operacion debe ir precedida del inicio de definicion de tabla","",return)

    NOEDBDefinicionTabla &defT = definicionTabla; 

    // Eliminamos el dataset anterior
    
    NOE_RedimensionaDataset(indiceDataset, 0);

    // Calculamos el tamanyo del nuevo dataset
    
    int tamanyoCabecera = ObtenTamanyoCabeceraTabla(defT);
                            
    // Alocatamos el dataset
        
    NOE_RedimensionaDataset(indiceDataset, tamanyoCabecera);
    NOE_LimpiaDataset(indiceDataset, 0, -1);
    
    // Rellenamos el dataset

    int offset = 0;

    //printf("Offset: %d\n", offset);
    //printf("Datos: %s\n", defT.nombre);
    
    NOE_AlmacenaTextoDataset(indiceDataset, offset, defT.nombre);        
    offset += (noedbMaxNombre + 1);
    
    //printf("Offset: %d\n", offset);
    //printf("Datos: %d\n", defT.numColumnas);
    
    NOE_AlmacenaEnteroDataset(indiceDataset, offset, defT.numColumnas);        
    offset += sizeof(int);
    
    for(int i = 0; i < defT.numColumnas; i ++)
    {
        NOEDBDefinicionColumna &defC = defT.columnas[i];
        
        NOE_AlmacenaTextoDataset(indiceDataset, offset, defC.nombre);
        offset += (noedbMaxNombre + 1);
        
        NOE_AlmacenaEnteroDataset(indiceDataset, offset, (int)defC.tipo);
        offset += sizeof(int);

        if(defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA)
        {
            NOE_AlmacenaTextoDataset(indiceDataset, offset, defC.tablaExterna);
            offset += (noedbMaxNombre + 1);
        }
    }
    
    //ImprimeDefinicionTabla(defT);
    
    NOE_AlmacenaEnteroDataset(indiceDataset, offset, 0);
    offset += sizeof(int);
    NOE_AlmacenaEnteroDataset(indiceDataset, offset, 0);
    
    // Guardamos el dataset
    
    NOE_GuardaDatasetAhora(indiceDataset);
    
    operacionActual = NOEDB_OPERACION_NINGUNA;
    
}

void NOEDB_InsertaFila(char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "No puedes iniciar la insercion porque hay otra operacion pendiente de finalizar","",return)

    //printf("Inserta fila\n");
    
    indiceDataset = BuscaDatasetTabla(tabla);
    
    ERRORSI(indiceDataset < 0,"No se pueden insertar filas en la tabla %s porque no existe",tabla,return)
    
    ObtenCabeceraTabla(indiceDataset, definicionTabla, propiedadesTabla);
    //ImprimeDefinicionTabla(definicionTabla);

    int offset = ObtenTamanyoCabeceraTabla(definicionTabla);
    
    //printf("Tamanyo cabecera tabla %d\n", offset);
    
    //printf("Ultima clave generada %d\n", propiedadesTabla.ultimaClaveGenerada);
   
    //printf("Num filas actuales %d\n", propiedadesTabla.numFilas);
            
    tamanyoFila = ObtenTamanyoFila(definicionTabla);
    
    //printf("Tamanyo fila %d\n", tamanyoFila);
    
    cursorFila = offset + propiedadesTabla.numFilas * tamanyoFila;
    
    //printf("Cursor fila %d\n", cursorFila);
    
    NOE_AlmacenaEnteroDataset(indiceDataset, cursorFila, propiedadesTabla.ultimaClaveGenerada + 1);

    operacionActual = NOEDB_OPERACION_INSERTA;
}


void NOEDB_InsertaEntero(char* columna, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_INSERTA, "Esta operacion debe ir precedida del inicio de la insercion en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede insertar un entero porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_ENTERO, "No se puede insertar un entero porque la columna %s no es de tipo entero", columna,return)
    
    
    int offsetColumna = ObtenOffsetColumna(columna, definicionTabla);
    
    NOE_AlmacenaEnteroDataset(indiceDataset, cursorFila + offsetColumna, valor);
    
}

void NOEDB_InsertaTexto(char* columna, char* valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_INSERTA, "Esta operacion debe ir precedida del inicio de la insercion en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede insertar un texto porque la columna %s no existe",columna,return)    
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_TEXTO, "No se puede insertar un texto porque la columna %s no es de tipo texto", columna,return)

    int offsetColumna = ObtenOffsetColumna(columna, definicionTabla);
    
    NOE_AlmacenaTextoDataset(indiceDataset, cursorFila + offsetColumna, valor);
    
}

void NOEDB_InsertaClaveExterna(char* columna, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_INSERTA, "Esta operacion debe ir precedida del inicio de la insercion en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede insertar una clave externa porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_CLAVEEXTERNA, "No se puede insertar una clave externa porque la columna %s no es de tipo clave externa", columna,return)
    ERRORSI(valor != 0 && !TablaContieneClavePrimaria(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tablaExterna, valor),"No se puede insertar ese valor para la clave externa en la columna %s porque no existe una fila con esa clave primaria en la tabla referenciada", columna,return)

    int offsetColumna = ObtenOffsetColumna(columna, definicionTabla);
    
    NOE_AlmacenaEnteroDataset(indiceDataset, cursorFila + offsetColumna, valor);
    
}    

int NOEDB_FinInsertaFila()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_INSERTA, "Esta operacion debe ir precedida del inicio de la insercion en la tabla","",return 0)

    int offset = ObtenTamanyoCabeceraTabla(definicionTabla) - sizeof(int) * 2;
    
    // Actualizamos la ultima clave generada
    int ultimaClave = NOE_RecuperaEnteroDataset(indiceDataset, offset) + 1;        
    NOE_AlmacenaEnteroDataset(indiceDataset, offset, ultimaClave);        
    offset += sizeof(int);
    
    // Actualizamos el numero de filas
    int numFilas = NOE_RecuperaEnteroDataset(indiceDataset, offset) + 1;        
    NOE_AlmacenaEnteroDataset(indiceDataset, offset, numFilas);        
    offset += sizeof(int);    
    
    NOE_GuardaDatasetAhora(indiceDataset);
    
    operacionActual = NOEDB_OPERACION_NINGUNA;
    
    return ultimaClave;
}

void NOEDB_BorraFilas(char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "No puedes iniciar el borrado porque hay otra operacion pendiente de finalizar","",return)
    
    numCriterios = 0;
    
    indiceDataset = BuscaDatasetTabla(tabla);    
    
    ERRORSI(indiceDataset < 0,"No se pueden borrar filas de la tabla %s porque no existe",tabla,return)
    

    ObtenCabeceraTabla(indiceDataset, definicionTabla, propiedadesTabla);    
    tamanyoCabecera = ObtenTamanyoCabeceraTabla(definicionTabla);
    tamanyoFila = ObtenTamanyoFila(definicionTabla);

    operacionActual = NOEDB_OPERACION_BORRA;
}

void NOEDB_BorraSiClavePrimaria(int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BORRA, "Esta operacion debe ir precedida del inicio del borrado en la tabla","",return)
    
    AnyadeCriterioClavePrimaria(valor);
}

void NOEDB_BorraSiEntero(char* columna, NOEDB_Operador operador, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BORRA, "Esta operacion debe ir precedida del inicio del borrado en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion para borrar porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_ENTERO, "No se puede usar la condicion para borrar porque la columna %s no es de tipo entero", columna,return)

    AnyadeCriterioEntero(columna, operador, valor);
}

void NOEDB_BorraSiTexto(char* columna, NOEDB_Operador operador, char* valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BORRA, "Esta operacion debe ir precedida del inicio del borrado en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion para borrar porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_TEXTO, "No se puede usar la condicion para borrar porque la columna %s no es de tipo texto", columna,return)

    AnyadeCriterioTexto(columna, operador, valor);
}

void NOEDB_BorraSiClaveExterna(char* columna, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BORRA, "Esta operacion debe ir precedida del inicio del borrado en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion para borrar porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_CLAVEEXTERNA, "No se puede usar la condicion para borrar porque la columna %s no es de tipo clave externa", columna,return)

    AnyadeCriterioClaveExterna(columna, valor);
}    

int NOEDB_FinBorraFilas()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BORRA, "Esta operacion debe ir precedida del inicio del borrado en la tabla","",return 0)

    int numFilasIniciales = propiedadesTabla.numFilas;
    
    cursorFila = tamanyoCabecera;

    while((cursorFila - tamanyoCabecera) / tamanyoFila < propiedadesTabla.numFilas)
    {
        bool cumple = FilaCursorCumpleCriterios();
        
        if(cumple)
        {
            int clave = NOE_RecuperaEnteroDataset(indiceDataset, cursorFila);
            
            ERRORSI(ClavePrimariaEsReferenciada(definicionTabla.nombre, clave),"No se puede borrar la fila de la tabla %s porque esta referenciada como clave externa en otra tabla",definicionTabla.nombre,return 0)
            
            // printf("Borrando fila con clave %d\n", clave);
            
            NOE_LimpiaDataset(indiceDataset, cursorFila, cursorFila + tamanyoFila - 1);
            
            propiedadesTabla.numFilas --;
            
            int indiceFilaBorrada = (cursorFila - tamanyoCabecera) / tamanyoFila;
            int numFilasRestantes = propiedadesTabla.numFilas - indiceFilaBorrada;
            
            // printf("Filas restantes %d\n", numFilasRestantes);
            
            if(numFilasRestantes > 0)
            {
                int cursorFilaSiguiente = cursorFila + tamanyoFila;
                
                NOE_CopiaDataset(indiceDataset, -1, cursorFilaSiguiente, cursorFilaSiguiente + numFilasRestantes * tamanyoFila - 1, cursorFila);
                
                // printf("Desplazadas %d filas\n", numFilasRestantes); 
                // printf("Tamanyo de fila %d\n", tamanyoFila);
            } 
            
            // No avanzo el cursor porque hemos movido la siguiente fila a su
            // posicion actual
            
            
        }
        else
        {            
            cursorFila += tamanyoFila;
        }
    }
    
    // Actualizar numFilas
    
    int offset = tamanyoCabecera - sizeof(int);
    NOE_AlmacenaEnteroDataset(indiceDataset, offset, propiedadesTabla.numFilas);
    
    NOE_GuardaDatasetAhora(indiceDataset);
    
    operacionActual = NOEDB_OPERACION_NINGUNA;
    
    return numFilasIniciales - propiedadesTabla.numFilas;
}

void NOEDB_OrdenaTabla(char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "No puedes iniciar la ordenacion porque hay otra operacion pendiente de finalizar","",return)
    
    numOrdenaciones = 0;
    
    indiceDataset = BuscaDatasetTabla(tabla);   

    ERRORSI(indiceDataset < 0,"No se puede ordenar la tabla %s porque no existe",tabla,return)
    

    ObtenCabeceraTabla(indiceDataset, definicionTabla, propiedadesTabla);    
    tamanyoCabecera = ObtenTamanyoCabeceraTabla(definicionTabla);
    tamanyoFila = ObtenTamanyoFila(definicionTabla);      
    
    operacionActual = NOEDB_OPERACION_ORDENA;
}

void NOEDB_OrdenaPor(char* columna, NOEDB_TipoOrdenacion ordenacion)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_ORDENA, "Esta operacion debe ir precedida del inicio de la ordenacion de la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se ordenar por la columna %s porque no existe",columna,return)    
    
    strcpy(ordenaciones[numOrdenaciones].nombreColumna, columna);
    ordenaciones[numOrdenaciones].tipoOrdenacion = ordenacion;
    
    numOrdenaciones ++;
}

void NOEDB_FinOrdenaTabla()
{    
    ERRORSI(operacionActual != NOEDB_OPERACION_ORDENA, "Esta operacion debe ir precedida del inicio de la ordenacion de la tabla","",return)

    for(int i = 0; i < propiedadesTabla.numFilas; i ++)
    {
        int cursorFila = tamanyoCabecera + tamanyoFila * i;
        int cursorFilaMenor = cursorFila;
        
        for(int j = i; j < propiedadesTabla.numFilas; j ++)
        {
            int cursorFilaCandidata = tamanyoCabecera + tamanyoFila * j;
            
            if(ComparaOrdenFilas(cursorFilaCandidata, cursorFilaMenor) < 0)
            {
                cursorFilaMenor = cursorFilaCandidata;
            }
        }
        
        NOE_RecuperaDatosDataset(indiceDataset, cursorFila, cursorFila + tamanyoFila - 1, bufferFila);
        NOE_CopiaDataset(indiceDataset, -1, cursorFilaMenor, cursorFilaMenor + tamanyoFila - 1, cursorFila);
        NOE_AlmacenaDatosDataset(indiceDataset, cursorFilaMenor, cursorFilaMenor + tamanyoFila - 1, bufferFila);
        
        
    }
    
    operacionActual = NOEDB_OPERACION_NINGUNA;
}


void NOEDB_ModificaFilas(char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "No puedes iniciar la modificacion porque hay otra operacion pendiente de finalizar","",return)
        
    numCriterios = 0;
    numModificaciones = 0;
    
    indiceDataset = BuscaDatasetTabla(tabla);    

    ERRORSI(indiceDataset < 0,"No se pueden modificar las filas de la tabla %s porque no existe",tabla,return)


    ObtenCabeceraTabla(indiceDataset, definicionTabla, propiedadesTabla);    
    tamanyoCabecera = ObtenTamanyoCabeceraTabla(definicionTabla);
    tamanyoFila = ObtenTamanyoFila(definicionTabla); 

    operacionActual = NOEDB_OPERACION_MODIFICA;
}

void NOEDB_ModificaSiClavePrimaria(int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return)
    
    AnyadeCriterioClavePrimaria(valor);
}

void NOEDB_ModificaSiEntero(char* columna, NOEDB_Operador operador, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion para modificar porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_ENTERO, "No se puede usar la condicion para modificar porque la columna %s no es de tipo entero", columna,return)
    
    
    AnyadeCriterioEntero(columna, operador, valor);
}

void NOEDB_ModificaSiTexto(char* columna, NOEDB_Operador operador, char* valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion para modificar porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_TEXTO, "No se puede usar la condicion para modificar porque la columna %s no es de tipo texto", columna,return)    
    
    AnyadeCriterioTexto(columna, operador, valor);
}

void NOEDB_ModificaSiClaveExterna(char* columna, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion para modificar porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_CLAVEEXTERNA, "No se puede usar la condicion para modificar porque la columna %s no es de tipo clave externa", columna,return)
    
    AnyadeCriterioClaveExterna(columna, valor);
}    

void NOEDB_ModificaEntero(char* columna, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede modificar la columna %s porque no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_ENTERO, "No se puede modificar la columna %s porque la columna no es de tipo entero", columna,return)

    
    strcpy(modificaciones[numModificaciones].nombreColumna, columna);
    modificaciones[numModificaciones].tipoValor = NOEDB_TIPOCOLUMNA_ENTERO;
    modificaciones[numModificaciones].valorEntero = valor;
    
    numModificaciones ++;
}

void NOEDB_ModificaTexto(char* columna, char* valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede modificar la columna %s porque no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_TEXTO, "No se puede modificar la columna %s porque no es de tipo texto", columna,return)    
    
    
    strcpy(modificaciones[numModificaciones].nombreColumna, columna);
    modificaciones[numModificaciones].tipoValor = NOEDB_TIPOCOLUMNA_TEXTO;
    strcpy(modificaciones[numModificaciones].valorTexto, valor);
    
    numModificaciones ++;
}

void NOEDB_ModificaClaveExterna(char* columna, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede modificar la columna %s porque no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_CLAVEEXTERNA, "No se puede modificar la columna %s porque no es de tipo clave externa", columna,return)
    ERRORSI(valor != 0 && !TablaContieneClavePrimaria(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tablaExterna, valor),"No se puede modificar la columna %s porque es una clave externa y no existe una fila con esa clave primaria en la tabla referenciada", columna,return)

    strcpy(modificaciones[numModificaciones].nombreColumna, columna);
    modificaciones[numModificaciones].tipoValor = NOEDB_TIPOCOLUMNA_CLAVEEXTERNA;
    modificaciones[numModificaciones].valorEntero = valor;

    numModificaciones ++;
}


int NOEDB_FinModificaFilas()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_MODIFICA, "Esta operacion debe ir precedida del inicio de la modificacion de la tabla","",return 0)
    
    int filasModificadas = 0;
    
    cursorFila = tamanyoCabecera;

    while((cursorFila - tamanyoCabecera) / tamanyoFila < propiedadesTabla.numFilas)
    {
        bool cumple = FilaCursorCumpleCriterios();
        
        if(cumple)
        {
            for(int i = 0; i < numModificaciones; i++)
            {
                NOEDBModificacion& mod = modificaciones[i];
                
                int offsetColumna = ObtenOffsetColumna(mod.nombreColumna, definicionTabla);
                
                if(mod.tipoValor == NOEDB_TIPOCOLUMNA_ENTERO)
                {
                    NOE_AlmacenaEnteroDataset(indiceDataset, cursorFila + offsetColumna, mod.valorEntero);
                }
                else if(mod.tipoValor == NOEDB_TIPOCOLUMNA_TEXTO)
                {
                    NOE_AlmacenaTextoDataset(indiceDataset, cursorFila + offsetColumna, mod.valorTexto);
                }
                else // mod.tipoValor == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA
                {
                    NOE_AlmacenaEnteroDataset(indiceDataset, cursorFila + offsetColumna, mod.valorEntero);
                }
                
                
            }
            
            filasModificadas ++;
            
        }

        cursorFila += tamanyoFila;
    }
    
    NOE_GuardaDatasetAhora(indiceDataset);
    
    operacionActual = NOEDB_OPERACION_NINGUNA;
    
    return filasModificadas;
}


void NOEDB_BuscaFilas(char* tabla)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "No puedes iniciar la busqueda porque hay otra operacion pendiente de finalizar","",return)
    
    numCriterios = 0;
    
    indiceDataset = BuscaDatasetTabla(tabla);    
    
    ERRORSI(indiceDataset < 0,"No se puede buscar en la tabla %s porque no existe",tabla,return)    

    ObtenCabeceraTabla(indiceDataset, definicionTabla, propiedadesTabla);
    
    operacionActual = NOEDB_OPERACION_BUSCA;
}

void NOEDB_BuscaSiClavePrimaria(int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BUSCA, "Esta operacion debe ir precedida del inicio de la busqueda en la tabla","",return)
    
    AnyadeCriterioClavePrimaria(valor);
}

void NOEDB_BuscaSiEntero(char* columna, NOEDB_Operador operador, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BUSCA, "Esta operacion debe ir precedida del inicio de la busqueda en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion de busqueda porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_ENTERO, "No se puede usar la condicion de busqueda porque la columna %s no es de tipo entero", columna,return)

    AnyadeCriterioEntero(columna, operador, valor);
}

void NOEDB_BuscaSiTexto(char* columna, NOEDB_Operador operador, char* valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BUSCA, "Esta operacion debe ir precedida del inicio de la busqueda en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion de busqueda porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_TEXTO, "No se puede usar la condicion de busqueda porque la columna %s no es de tipo texto", columna,return)    

    AnyadeCriterioTexto(columna, operador, valor);
}

void NOEDB_BuscaSiClaveExterna(char* columna, int valor)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BUSCA, "Esta operacion debe ir precedida del inicio de la busqueda en la tabla","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede usar la condicion para la busqueda porque la columna %s no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_CLAVEEXTERNA, "No se puede usar la condicion para la busqueda porque la columna %s no es de tipo clave externa", columna,return)
    
    AnyadeCriterioClaveExterna(columna, valor);    
}


void NOEDB_FinBuscaFilas()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_BUSCA, "Esta operacion debe ir precedida del inicio de la busqueda en la tabla","",return)

    tamanyoCabecera = ObtenTamanyoCabeceraTabla(definicionTabla);
    cursorFila = tamanyoCabecera;
    tamanyoFila = ObtenTamanyoFila(definicionTabla);
    
    operacionActual = NOEDB_OPERACION_NINGUNA;
}


bool NOEDB_LeeResultado()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_NINGUNA, "No puedes leer el resultado porque hay otra operacion pendiente de finalizar","",return false)
    
    bool cumple = false;
    
    while((cursorFila - tamanyoCabecera) / tamanyoFila < propiedadesTabla.numFilas && !cumple)
    {
        cumple = FilaCursorCumpleCriterios();
        
        if(!cumple)
        { cursorFila += tamanyoFila; }
    }
    
    if(cumple)
    {
        operacionActual = NOEDB_OPERACION_LEERESULTADO;
    }
    
    return cumple;
}

int NOEDB_LeeClavePrimaria()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_LEERESULTADO, "Esta operacion debe ir precedida de la lectura exitosa de un resultado","",return 0)
    
    return NOE_RecuperaEnteroDataset(indiceDataset, cursorFila);
}

int NOEDB_LeeEntero(char* columna)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_LEERESULTADO, "Esta operacion debe ir precedida de la lectura exitosa de un resultado","",return 0)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede leer la columna %s porque no existe",columna,return 0)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_ENTERO, "No se puede leer la columna %s porque no es de tipo entero", columna,return 0)
    
    int offsetColumna = ObtenOffsetColumna(columna, definicionTabla);
    
    return NOE_RecuperaEnteroDataset(indiceDataset, cursorFila + offsetColumna);
    
}

void NOEDB_LeeTexto(char* columna, char* texto)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_LEERESULTADO, "Esta operacion debe ir precedida de la lectura exitosa de un resultado","",return)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede leer la columna %s porque no existe",columna,return)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_TEXTO, "No se puede leer la columna %s porque no es de tipo texto", columna,return)    

    int offsetColumna = ObtenOffsetColumna(columna, definicionTabla);

    NOE_RecuperaTextoDataset(indiceDataset, cursorFila + offsetColumna, texto);
}

int NOEDB_LeeClaveExterna(char* columna)
{
    ERRORSI(operacionActual != NOEDB_OPERACION_LEERESULTADO, "Esta operacion debe ir precedida de la lectura exitosa de un resultado","",return 0)

    ERRORSI(BuscaColumna(columna, definicionTabla) < 0,"No se puede leer la columna %s porque no existe",columna,return 0)
    ERRORSI(definicionTabla.columnas[BuscaColumna(columna, definicionTabla)].tipo != NOEDB_TIPOCOLUMNA_CLAVEEXTERNA, "No se puede leer la columna %s porque no es de tipo clave externa", columna,return 0)

    int offsetColumna = ObtenOffsetColumna(columna, definicionTabla);
    return NOE_RecuperaEnteroDataset(indiceDataset, cursorFila + offsetColumna);
}

void NOEDB_FinLeeResultado()
{
    ERRORSI(operacionActual != NOEDB_OPERACION_LEERESULTADO, "Esta operacion debe ir precedida de la lectura exitosa de un resultado","",return)

    cursorFila += tamanyoFila;
    
    operacionActual = NOEDB_OPERACION_NINGUNA;
}

// Funciones privadas

int BuscaDatasetTabla(char* tabla)
{
    int numDatasets = NOE_ObtenNumeroDatasets();
    
    int resultado = -1;
    
    char nombreTabla[noedbMaxNombre];
    
    int i = 0;
    while(i < numDatasets && resultado < 0)
    {
        int encontrado = 0;
        
        if(NOE_ObtenTamanyoDataset(i) > 0)
        {
            NOE_RecuperaTextoDataset(i, 0, nombreTabla);
            if(strcmpi(nombreTabla, tabla) == 0) { resultado = i; encontrado = 1; }
        }
        
        if(!encontrado) { i ++; }
    }
        
    return resultado;        
}

void ObtenCabeceraTabla(int indiceDataset, NOEDBDefinicionTabla& tabla, NOEDBPropiedadesTabla& propiedades)
{
    
    int offset = 0;
    
    NOE_RecuperaTextoDataset(indiceDataset, offset, tabla.nombre);
    offset += (noedbMaxNombre + 1);
    
    // printf("Tabla %s\n", tabla.nombre);

    tabla.numColumnas = NOE_RecuperaEnteroDataset(indiceDataset, offset);
    offset += sizeof(int);
    
    // printf("Columnas %d\n", tabla.numColumnas);
    
    for(int i = 0; i < tabla.numColumnas; i ++)
    {
        NOEDBDefinicionColumna& columna = tabla.columnas[i];
        
        NOE_RecuperaTextoDataset(indiceDataset, offset, columna.nombre);
        offset += (noedbMaxNombre + 1);

        columna.tipo = (NOEDBTipoColumna)NOE_RecuperaEnteroDataset(indiceDataset, offset);
        offset += sizeof(int);
        
        if(columna.tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA)
        {
            NOE_RecuperaTextoDataset(indiceDataset, offset, columna.tablaExterna);
            offset += (noedbMaxNombre + 1);
        }
        
    }
    
    propiedades.ultimaClaveGenerada = NOE_RecuperaEnteroDataset(indiceDataset, offset);
    offset += sizeof(int);
    
    //printf("Clave generada %d\n", propiedades.ultimaClaveGenerada);
        
    propiedades.numFilas = NOE_RecuperaEnteroDataset(indiceDataset, offset);
    offset += sizeof(int);
    
}

int ObtenTamanyoCabeceraTabla(NOEDBDefinicionTabla& tabla)
{
    int resultado;
    
    int numClavesExternas = 0;
    for(int i = 0; i < tabla.numColumnas; i++)
    {
        if(tabla.columnas[i].tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA) { numClavesExternas ++; }
    }

    resultado = (noedbMaxNombre + 1) + // Nombre tabla
                sizeof(int) + // Num columnas
                (noedbMaxNombre + 1 + sizeof(NOEDBTipoColumna)) * tabla.numColumnas + // Nombre y tipo de cada columna
                (noedbMaxNombre + 1) * numClavesExternas + // Tablas externas de columnas que sean clave
                + sizeof(int) * 2; // Ultima clave generada y numero de filas
                
                            
    return resultado;
}

int ObtenTamanyoFila(NOEDBDefinicionTabla& tabla)
{
    int resultado = 0;
    
    for(int i = 0; i < tabla.numColumnas; i++)
    {
        if(tabla.columnas[i].tipo == NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA) { resultado += sizeof(int); }
        else if(tabla.columnas[i].tipo == NOEDB_TIPOCOLUMNA_ENTERO) { resultado += sizeof(int); }
        else if(tabla.columnas[i].tipo == NOEDB_TIPOCOLUMNA_TEXTO) { resultado += noedbMaxTexto + 1; }
        else // tabla.columnas[i].tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA
        { resultado += sizeof(int); }
    }
    
    return resultado;
}

int BuscaColumna(char* columna, NOEDBDefinicionTabla& tabla)
{
    int resultado = 0;
    int encontrada = 0;
    int i = 0;
    while(i < tabla.numColumnas && !encontrada)
    {
        NOEDBDefinicionColumna& defC = tabla.columnas[i];
        
        if(strcmpi(columna, defC.nombre) == 0) { encontrada = 1; resultado = i; } 
        else { i ++; }
    }
    
    if(encontrada) { return resultado; }
    else { return -1; }
}

int ObtenOffsetColumna(char* columna, NOEDBDefinicionTabla& tabla)
{
    int indiceColumna = BuscaColumna(columna, tabla);
    
    int resultado = 0;
    int i = 0;
    while(i < indiceColumna)
    {
        NOEDBDefinicionColumna& defC = tabla.columnas[i];

        if(defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA) { resultado += sizeof(int); }
        else if(defC.tipo == NOEDB_TIPOCOLUMNA_ENTERO) { resultado += sizeof(int); }
        else if(defC.tipo == NOEDB_TIPOCOLUMNA_TEXTO) { resultado += noedbMaxTexto + 1; }
        else // defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA
        { resultado += sizeof(int); }
            
        i ++;

    }
    
    return resultado;
}

void NOEDB_ImprimeTabla(char* tabla)
{
    int indiceDataset = BuscaDatasetTabla(tabla);
    
    NOEDBDefinicionTabla definicion;        
    NOEDBPropiedadesTabla propiedades;        
    ObtenCabeceraTabla(indiceDataset, definicion, propiedades);
    
    ImprimeDefinicionTabla(definicion);
    
    ImprimeDatosTabla(indiceDataset, definicion, propiedades);
}    

void ImprimeDefinicionTabla(NOEDBDefinicionTabla& tabla)
{
    char str[noedbMaxLineaImprimir];
    
    printf("+----------");    
    for(int j = 0; j < tabla.numColumnas - 1; j ++)
    { printf("-----------"); }
    printf("+\n");
    sprintf(str, "|%%-%ds", 10 * tabla.numColumnas + tabla.numColumnas - 1);
    printf(str, tabla.nombre);
    printf("|\n");
    
    for(int i = 0; i < tabla.numColumnas; i++)
    { printf("+----------"); }
    printf("+\n");   
    for(int i = 0; i < tabla.numColumnas; i ++)
    {   NOEDBDefinicionColumna &columna = tabla.columnas[i];
        printf("|%10s", columna.nombre);
    }
    printf("|\n");
    for(int i = 0; i < tabla.numColumnas; i ++)
    {   NOEDBDefinicionColumna &columna = tabla.columnas[i];
        if(columna.tipo == NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA) { printf("|%10s", "CLAVEPR"); }
        else if(columna.tipo == NOEDB_TIPOCOLUMNA_ENTERO) { printf("|%10s", "ENTERO"); }
        else if(columna.tipo == NOEDB_TIPOCOLUMNA_TEXTO) { printf("|%10s", "TEXTO"); }
        else
        {   
            sprintf(str, "CLAVEEX:");            
            strcat(str, columna.tablaExterna);
            if(strlen(str) > 10) { str[10] = 0; }
            printf("|%10s", str);
        }
    }
    printf("|\n");

    for(int j = 0; j < tabla.numColumnas; j ++)
    { printf("+----------"); }
    printf("+\n");    
    
}

void ImprimeDatosTabla(int indiceDataset, NOEDBDefinicionTabla& tabla, NOEDBPropiedadesTabla& propiedades)
{
    char str[noedbMaxLineaImprimir];    
    
    int offset = ObtenTamanyoCabeceraTabla(tabla);
    
    int longitudLinea1 = tabla.numColumnas * 10 + tabla.numColumnas + 1;

    sprintf(str, "| UltimaClave:%d NumFilas:%d", propiedades.ultimaClaveGenerada, propiedades.numFilas);
    printf(str);

    int longitudLinea2 = strlen(str);
    
    if(longitudLinea1 > longitudLinea2)
    {   for(int i = 0;i < longitudLinea1 - longitudLinea2 - 1; i ++) { printf(" "); }
        printf("|\n");
    }
    
    for(int j = 0; j < tabla.numColumnas; j ++)
    { printf("+----------"); }
    printf("+\n");    
    
    int tamanyoFila = ObtenTamanyoFila(tabla);
    
    for(int i = 0; i < propiedades.numFilas; i ++)
    {
        for(int j = 0; j < tabla.numColumnas; j ++)
        {
            NOEDBDefinicionColumna& defC = tabla.columnas[j];
            int offsetColumna = ObtenOffsetColumna(defC.nombre, tabla);
            
            if(defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA)
            {
                int valor = NOE_RecuperaEnteroDataset(indiceDataset, offset + offsetColumna);
                sprintf(str, "%d", valor);
                if(strlen(str) > 10) { str[10] = 0; }
                printf("|%10s", str);

            }
            else if(defC.tipo == NOEDB_TIPOCOLUMNA_ENTERO)
            {
                int valor = NOE_RecuperaEnteroDataset(indiceDataset, offset + offsetColumna);
                sprintf(str, "%d", valor);
                if(strlen(str) > 10) { str[10] = 0; }
                printf("|%10s", str);
            }
            else if(defC.tipo == NOEDB_TIPOCOLUMNA_TEXTO)
            {
                char valor[noedbMaxTexto + 1];

                NOE_RecuperaTextoDataset(indiceDataset, offset + offsetColumna, valor);
                sprintf(str, "%s", valor);
                if(strlen(str) >= 10) { str[10] = 0; }
                printf("|%10s", str);
            }
            else // defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA
            {
                int valor = NOE_RecuperaEnteroDataset(indiceDataset, offset + offsetColumna);
                if(valor != 0) { sprintf(str, "%d", valor); }
                else { sprintf(str, "%s", "NULO"); }
                if(strlen(str) > 10) { str[10] = 0; }
                printf("|%10s", str);
            }
           
        }
        
        printf("|\n");
        
        // NOE_Byte byte;
        // for(int k = 0; k < tamanyoFila; k ++)
        // {
            // NOE_RecuperaDatosDataset(indiceDataset, offset + k, offset + k, &byte);
            // printf("%d,", (int)byte);
        // }                
        // printf("\n");

        offset += tamanyoFila;
    }
    
    for(int j = 0; j < tabla.numColumnas; j ++)
    { printf("+----------"); }
    printf("+\n");
    
}

bool ValorEnteroCumpleCriterio(int valor, NOEDB_Operador operador, int valorOperador)
{
    if(operador == NOEDB_OPERADOR_IGUAL) { return valor == valorOperador; }
    else if(operador == NOEDB_OPERADOR_DIFERENTE) { return valor != valorOperador; }
    else if(operador == NOEDB_OPERADOR_MENOR) { return valor < valorOperador; }
    else if(operador == NOEDB_OPERADOR_MENORIGUAL) { return valor <= valorOperador; }
    else if(operador == NOEDB_OPERADOR_MAYOR) { return valor > valorOperador; }
    else // operador == NOEDB_OPERADOR_MAYORIGUAL
    { return valor >= valorOperador; }

}

bool ValorTextoCumpleCriterio(const char valor[], NOEDB_Operador operador, const char valorOperador[])
{
    if(operador == NOEDB_OPERADOR_IGUAL) { return strcmpi(valor, valorOperador) == 0; }
    else if(operador == NOEDB_OPERADOR_DIFERENTE) { return strcmpi(valor, valorOperador) != 0; }
    else if(operador == NOEDB_OPERADOR_MENOR) { return strcmpi(valor, valorOperador) < 0; }
    else if(operador == NOEDB_OPERADOR_MENORIGUAL) { return strcmpi(valor, valorOperador) <= 0; }
    else if(operador == NOEDB_OPERADOR_MAYOR) { return strcmpi(valor, valorOperador) > 0; }
    else // operador == NOEDB_OPERADOR_MAYORIGUAL
    { return strcmpi(valor, valorOperador) >= 0; }

}

void AnyadeCriterioClavePrimaria(int valor)
{
    strcpy(criterios[numCriterios].nombreColumna, noedbNombreColumnaClave);
    criterios[numCriterios].tipoValor = NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA;
    criterios[numCriterios].valorEntero = valor;
    criterios[numCriterios].operador = NOEDB_OPERADOR_IGUAL;
    
    numCriterios ++;    
}

void AnyadeCriterioEntero(char* columna, NOEDB_Operador operador, int valor)
{
    strcpy(criterios[numCriterios].nombreColumna, columna);
    criterios[numCriterios].tipoValor = NOEDB_TIPOCOLUMNA_ENTERO;
    criterios[numCriterios].valorEntero = valor;
    criterios[numCriterios].operador = operador;
    
    numCriterios ++;
    
}

void AnyadeCriterioTexto(char* columna, NOEDB_Operador operador, char* valor)
{
    strcpy(criterios[numCriterios].nombreColumna, columna);
    criterios[numCriterios].tipoValor = NOEDB_TIPOCOLUMNA_TEXTO;
    strcpy(criterios[numCriterios].valorTexto, valor);
    criterios[numCriterios].operador = operador;
    
    numCriterios ++;    
    
}

void AnyadeCriterioClaveExterna(char* columna, int valor)
{
    strcpy(criterios[numCriterios].nombreColumna, columna);
    criterios[numCriterios].tipoValor = NOEDB_TIPOCOLUMNA_CLAVEEXTERNA;
    criterios[numCriterios].valorEntero = valor;
    criterios[numCriterios].operador = NOEDB_OPERADOR_IGUAL;
    
}



bool FilaCursorCumpleCriterios()
{   
    bool cumple = true;
    int indiceCriterio = 0;

    int valorEntero = 0;
    char valorTexto[noedbMaxTexto + 1];
    valorTexto[0] = 0;
    
    while(indiceCriterio < numCriterios && cumple)
    {
        NOEDBCriterio& c = criterios[indiceCriterio];
        
        int indiceColumna = BuscaColumna(c.nombreColumna, definicionTabla);
        int offsetColumna = ObtenOffsetColumna(c.nombreColumna, definicionTabla);
        
        bool columnaCumple = true;
        int esValorEntero = 0;
         
        if(definicionTabla.columnas[indiceColumna].tipo == NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA)
        {
            valorEntero = NOE_RecuperaEnteroDataset(indiceDataset, cursorFila + offsetColumna);
            esValorEntero = 1;
        }
        else if(definicionTabla.columnas[indiceColumna].tipo == NOEDB_TIPOCOLUMNA_TEXTO)
        {
            NOE_RecuperaTextoDataset(indiceDataset, cursorFila + offsetColumna, valorTexto);
            esValorEntero = 0;
        }
        else if(definicionTabla.columnas[indiceColumna].tipo == NOEDB_TIPOCOLUMNA_ENTERO)
        {
            valorEntero = NOE_RecuperaEnteroDataset(indiceDataset, cursorFila + offsetColumna);
            esValorEntero = 1;
        }
        else // definicionTabla.columnas[indiceColumna].tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA
        {
            valorEntero = NOE_RecuperaEnteroDataset(indiceDataset, cursorFila + offsetColumna);
            esValorEntero = 1;            
        }
        
        if(esValorEntero)
        {
            columnaCumple = ValorEnteroCumpleCriterio(valorEntero, c.operador, c.valorEntero);
        }
        else
        {
            columnaCumple = ValorTextoCumpleCriterio(valorTexto, c.operador, c.valorTexto);
        }
        
        cumple = cumple && columnaCumple;
        
        if(cumple) { indiceCriterio ++; }
        
    }
    
    return cumple;
    
    
}

int ComparaOrdenFilas(int cursorFila1, int cursorFila2)
{
    int resultado = 0;
    
    int ordenacion = 0;
    
    while(ordenacion < numOrdenaciones && resultado == 0)
    {
        NOEDBOrdenacion& o = ordenaciones[ordenacion];
        
        int indiceColumna = BuscaColumna(o.nombreColumna, definicionTabla);
        
        NOEDBDefinicionColumna& defC = definicionTabla.columnas[indiceColumna];
        
        int offsetColumna = ObtenOffsetColumna(o.nombreColumna, definicionTabla);
        
        if(defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEPRIMARIA ||
            defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA || 
            defC.tipo == NOEDB_TIPOCOLUMNA_ENTERO)
        {
            int valor1 = NOE_RecuperaEnteroDataset(indiceDataset, cursorFila1 + offsetColumna);
            int valor2 = NOE_RecuperaEnteroDataset(indiceDataset, cursorFila2 + offsetColumna);
            
            if(valor1 < valor2) { resultado = -1; }
            else if(valor1 > valor2) { resultado = 1; }
        }
        else
        {
            char valor1[noedbMaxTexto];
            char valor2[noedbMaxTexto];

            NOE_RecuperaTextoDataset(indiceDataset, cursorFila1 + offsetColumna, valor1);
            NOE_RecuperaTextoDataset(indiceDataset, cursorFila2 + offsetColumna, valor2);
            
            resultado = strcmpi(valor1, valor2);
        }
        
        resultado *= (o.tipoOrdenacion == NOEDB_TIPOORDENACION_ASCENDENTE ? 1 : -1);
        
        ordenacion ++;
            
    }
    
    return resultado;
    

}

bool TablaDatasetEsReferenciada(int dataset)
{
    if(NOE_ObtenTamanyoDataset(dataset) > 0)
    {
        NOEDBDefinicionTabla defT;
        NOEDBPropiedadesTabla propsT;
        
        ObtenCabeceraTabla(dataset, defT, propsT);

        return TablaEsReferenciada(defT.nombre);
    }
    else
    {
        return false;
    }
}

bool TablaEsReferenciada(char* tabla)
{
    int numDatasets = NOE_ObtenNumeroDatasets();
    NOEDBDefinicionTabla defT;
    NOEDBPropiedadesTabla propsT;
    
    bool resultado = false;
    int i = 0;
    
    while(i < numDatasets && !resultado)
    {
        if(NOE_ObtenTamanyoDataset(i) > 0)
        {
            ObtenCabeceraTabla(i, defT, propsT);
            
            int j = 0;
            while(j < defT.numColumnas && !resultado)
            {
                NOEDBDefinicionColumna& defC = defT.columnas[j];
                
                if(defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA)
                {
                    if(strcmpi(defC.tablaExterna, tabla) == 0) { resultado = true; }
                }
                
                if(!resultado) { j++; }

            }
            
            
        }
        
        if(!resultado) { i ++; }
    }
    
    return resultado;
}

bool TablaContieneClavePrimaria(char* tabla, int valor)
{    
    bool resultado = false;
    
    int indiceDataset = BuscaDatasetTabla(tabla);
    NOEDBDefinicionTabla defT;
    NOEDBPropiedadesTabla propsT;

    ObtenCabeceraTabla(indiceDataset, defT, propsT);
    int tamanyoCabecera = ObtenTamanyoCabeceraTabla(defT);
    int tamanyoFila = ObtenTamanyoFila(defT);
    
    int i = 0;
    
    while(i < propsT.numFilas && !resultado)
    {
        int clave = NOE_RecuperaEnteroDataset(indiceDataset, tamanyoCabecera + i * tamanyoFila);
        
        if(clave == valor) { resultado = true; }
        else { i ++; }
    }
    
    return resultado;
    
}

bool ClavePrimariaEsReferenciada(char* tabla, int clave)
{
    int numDatasets = NOE_ObtenNumeroDatasets();
    NOEDBDefinicionTabla defT;
    NOEDBPropiedadesTabla propsT;
    
    bool resultado = false;
    int i = 0;
    
    while(i < numDatasets && !resultado)
    {
        if(NOE_ObtenTamanyoDataset(i) > 0)
        {
            ObtenCabeceraTabla(i, defT, propsT);
            
            int tamanyoCabecera = ObtenTamanyoCabeceraTabla(defT);
            int tamanyoFila = ObtenTamanyoFila(defT);
            
            int j = 0;
            while(j < defT.numColumnas && !resultado)
            {
                NOEDBDefinicionColumna& defC = defT.columnas[j];
                
                if(defC.tipo == NOEDB_TIPOCOLUMNA_CLAVEEXTERNA)
                {
                    if(strcmpi(defC.tablaExterna, tabla) == 0)
                    {
                        int k = 0;
                        
                        while(k < propsT.numFilas && !resultado)
                        {
                            int offsetColumna = ObtenOffsetColumna(defC.nombre, defT);
                            int offset = tamanyoCabecera + k * tamanyoFila + offsetColumna;
                            
                            int claveExterna = NOE_RecuperaEnteroDataset(i, offset);
                            
                            //printf("Comparando clave externa %d con primaria %d\n", claveExterna, clave);
                            
                            if(claveExterna == clave) { resultado = true; }
                            else { k ++; }
                            
                        }
                    }
                }
                
                if(!resultado) { j++; }

            }
            
            
        }
        
        if(!resultado) { i ++; }
    }
    
    return resultado;
    
}

