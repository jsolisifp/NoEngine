#ifndef NOENTIENDO_HPP
#define NOENTIENDO_HPP

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                          NoEntiendo                                 //
//                                                                     //
//              Una librería 2D pedagógica basada en SFML              //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                           LICENCIA                                  //
//                                                                     //
//    Attribution-NonCommercial-NoDerivatives 4.0 International        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////


////////////
// TIPOS ///
////////////

// Tipos

typedef unsigned char NOE_Byte;

// Constantes

const int noe_tileVacio    = 0;

const int noe_tileColisionVacio    = 0;


// Enums

///
/// Identificadores de teclas
///
enum NOE_Tecla
{
	NOE_TECLA_ESPACIO = 0,
	NOE_TECLA_A,
	NOE_TECLA_B,
	NOE_TECLA_C,
	NOE_TECLA_D,
	NOE_TECLA_E,
	NOE_TECLA_F,
	NOE_TECLA_G,
	NOE_TECLA_H,
	NOE_TECLA_I,
	NOE_TECLA_J,
	NOE_TECLA_K,
	NOE_TECLA_L,
	NOE_TECLA_M,
	NOE_TECLA_N,
	NOE_TECLA_O,
	NOE_TECLA_P,
	NOE_TECLA_Q,
	NOE_TECLA_R,
	NOE_TECLA_S,
	NOE_TECLA_T,
	NOE_TECLA_U,
	NOE_TECLA_V,
	NOE_TECLA_W,
	NOE_TECLA_X,
	NOE_TECLA_Y,
	NOE_TECLA_Z,
	NOE_TECLA_1,
	NOE_TECLA_2,
	NOE_TECLA_3,
	NOE_TECLA_4,
	NOE_TECLA_5,
	NOE_TECLA_6,
	NOE_TECLA_7,
	NOE_TECLA_8,
	NOE_TECLA_9,
	NOE_TECLA_0,
	NOE_TECLA_ESCAPE,
	NOE_TECLA_IZQUIERDA,
	NOE_TECLA_DERECHA,
	NOE_TECLA_ARRIBA,
	NOE_TECLA_ABAJO,
	NOE_TECLA_ENTRAR,
	NOE_TECLA_BORRA_ATRAS
	
};

enum NOE_BotonRaton
{
	NOE_BOTONRATON_PRINCIPAL = 0,
	NOE_BOTONRATON_SECUNDARIO,
	NOE_BOTONRATON_CENTRAL

};

//////////////////////
// MODULO PRINCIPAL //
//////////////////////

// Funciones principales

bool NOE_Inicia();
void NOE_Actualiza();
void NOE_Finaliza();

/////////////////////
// MODULO PANTALLA //
/////////////////////

// Pantalla

void NOE_LimpiaPantalla(int r, int g, int b);
void NOE_MuestraPantalla();
int NOE_ObtenAnchoPantalla();
int NOE_ObtenAltoPantalla();

// Cursor

void NOE_PonCursor(int cursor);
void NOE_PonMostrarCursor(bool mostrar);
void NOE_PonTamanyoCursor(int ancho, int alto);
void NOE_PonDesplazamientoCursor(int x, int y);


/////////////////////
// MODULO ENTRADA  //
/////////////////////

// Entrada

const char *NOE_ObtenEntrada();
void NOE_LimpiaEntrada();

/////////////////////
// MODULO GRAFICOS //
/////////////////////

// Camara

int NOE_ObtenPosicionCamaraX();
int NOE_ObtenPosicionCamaraY();
void NOE_PonPosicionCamara(int x, int y);


// Tilemaps

int NOE_ObtenTile(int tilemap, int posicionX, int posicionY);
void NOE_PonTile(int tilemap, int posicionX, int posicionY, int tile);
void NOE_LimpiaTilemap(int tilemap, int tile);
void NOE_DibujaTilemap(int numTilemap);

// Sprites

void NOE_DibujaSprite(int sprite, int x, int y, int ancho, int alto, bool invertirX, bool invertirY);

// Decorados

void NOE_DibujaDecorado(int decorado);

// Texto

void NOE_DibujaCaracter(char caracter, int x, int y, int ancho, int alto, int font);
void NOE_DibujaTexto(const char texto[], int x, int y, int anchoCaracter, int altoCaracter, int font);

// Formas

void NOE_DibujaDisco(int x, int y, int radio, int r, int g, int b);
void NOE_DibujaCaja(int x, int y, int ancho, int alto, int r, int g, int b);
void NOE_DibujaForma(int forma, int x, int y, int ancho, int alto, int r, int g, int b);

// Cambios de coordenadas

int NOE_ConviertePantallaAMundoX(int x);
int NOE_ConviertePantallaAMundoY(int y);
int NOE_ConvierteMundoAPantallaX(int x);
int NOE_ConvierteMundoAPantallaY(int y);
int NOE_ConvierteMundoATileX(int x);
int NOE_ConvierteMundoATileY(int y);
int NOE_ConvierteTileAMundoX(int x);
int NOE_ConvierteTileAMundoY(int y);

////////////////////
// MODULO SISTEMA //
////////////////////


// Tiempo

void NOE_Espera(int tiempo);
int NOE_ObtenTiempoDesdeActualizacion();

// Aleatoriedad

int NOE_ObtenNumeroAleatorio(int minimo, int maximo);

///////////////////////
// MODULO COLISIONES //
///////////////////////

// Mapa de colision

int NOE_ObtenTileColision(int posicionX, int posicionY);
void NOE_PonTileColision(int posicionX, int posicionY, int colision);
void NOE_LimpiaMapaColision();

// Colisiones

bool NOE_ColisionDiscoDisco(int pX1, int pY1, int radio1, int pX2, int pY2, int radio2);
bool NOE_ColisionCajaCaja(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int ancho2, int alto2);
bool NOE_ColisionCajaDisco(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int radio2);
bool NOE_ColisionCajaMapaColision(int pX1, int pY1, int ancho1, int alto1);
bool NOE_ColisionDiscoMapaColision(int pX1, int pY1, int radio1);


//////////////////////
// MODULO CONTROLES //
//////////////////////

// Teclado

bool NOE_ObtenTeclaPulsada(NOE_Tecla tecla);
bool NOE_ObtenTeclaArriba(NOE_Tecla tecla);
bool NOE_ObtenTeclaAbajo(NOE_Tecla tecla);

// Raton

int  NOE_ObtenPosicionRatonX();
int  NOE_ObtenPosicionRatonY();
int  NOE_ObtenDesplazamientoRatonX();
int  NOE_ObtenDesplazamientoRatonY();

bool NOE_ObtenBotonRaton(NOE_BotonRaton boton);
bool NOE_ObtenBotonRatonAbajo(NOE_BotonRaton boton);
bool NOE_ObtenBotonRatonArriba(NOE_BotonRaton boton);

////////////////////
// MODULO SONIDO  //
////////////////////

// Músicas

void NOE_ReproduceMusica(int musica, int volumen, int pitch);
void NOE_ParaMusica();

// Sonidos

int NOE_ReproduceSonido(int sonido, int volumen, int pitch);
void NOE_ParaSonido(int canal);

/////////////////////
// MODULO DATOS    //
/////////////////////

int NOE_ObtenNumeroDatasets();
void NOE_RedimensionaDataset(int dataset, int tamanyoMinimo);
int NOE_ObtenTamanyoDataset(int dataset);
void NOE_LimpiaDataset(int dataset, int desde, int hasta);
void NOE_CopiaDataset(int datasetOrigen, int datasetDestino, int origenDesde, int origenHasta, int destinoDesde);
void NOE_RecuperaDatosDataset(int dataset, int desde, int hasta, void* bytes);
void NOE_AlmacenaDatosDataset(int dataset, int desde, int hasta, const void* bytes);
int NOE_RecuperaEnteroDataset(int dataset, int offset);
void NOE_AlmacenaEnteroDataset(int dataset, int offset, int valor);
void NOE_RecuperaTextoDataset(int dataset, int offset, char texto[]);
void NOE_AlmacenaTextoDataset(int dataset, int offset, const char texto[]);
void NOE_GuardaDatasetAhora(int dataset);
void NOE_ImprimeDataset(int dataset, bool modoTexto);

#endif

