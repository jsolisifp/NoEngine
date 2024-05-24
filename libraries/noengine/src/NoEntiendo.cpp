#include <NoEntiendo.hpp>
#include <Modulos/NOEComun.hpp>
#include <Modulos/NOEPrincipal.hpp>


// Principal

bool NOE_Inicia() { return NOEPrincipal::Inicia(); }
void NOE_Actualiza() { NOEPrincipal::Actualiza(); }
void NOE_Finaliza() { NOEPrincipal::Finaliza(); }

// Pantalla

void NOE_LimpiaPantalla(int r, int g, int b) { NOEPantalla::LimpiaPantalla(r, g, b); }
void NOE_MuestraPantalla() { NOEPantalla::MuestraPantalla(); }
int NOE_ObtenAnchoPantalla() { return NOEPantalla::ObtenAnchoPantalla(); }
int NOE_ObtenAltoPantalla() { return NOEPantalla::ObtenAltoPantalla(); }

void NOE_PonCursor(int cursor) { NOEPantalla::PonCursor(cursor); }
void NOE_PonMostrarCursor(bool mostrar) { NOEPantalla::PonMostrarCursor(mostrar); }
void NOE_PonTamanyoCursor(int ancho, int alto) { NOEPantalla::PonTamanyoCursor(ancho, alto); }
void NOE_PonDesplazamientoCursor(int x, int y) { NOEPantalla::PonDesplazamientoCursor(x, y); }

// Colisiones

int NOE_ObtenTileColision(int posicionX, int posicionY) { return NOEColisiones::ObtenTileColision(posicionX, posicionY); }
void NOE_PonTileColision(int posicionX, int posicionY, int colision) { NOEColisiones::PonTileColision(posicionX, posicionY, colision); }
void NOE_LimpiaMapaColision() { NOEColisiones::LimpiaMapaColision(); }

bool NOE_ColisionDiscoDisco(int pX1, int pY1, int radio1, int pX2, int pY2, int radio2) { return NOEColisiones::ColisionDiscoDisco(pX1, pY1, radio1, pX2, pY2, radio2); }
bool NOE_ColisionCajaCaja(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int ancho2, int alto2) { return NOEColisiones::ColisionCajaCaja(pX1, pY1, ancho1, alto1, pX2, pY2, ancho2, alto2); }
bool NOE_ColisionCajaDisco(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int radio2) { return NOEColisiones::ColisionCajaDisco(pX1, pY1, ancho1, alto1, pX2, pY2, radio2); }
bool NOE_ColisionCajaMapaColision(int pX1, int pY1, int ancho1, int alto1) { return NOEColisiones::ColisionCajaMapaColision(pX1, pY1, ancho1, alto1); }
bool NOE_ColisionDiscoMapaColision(int pX1, int pY1, int radio1) { return NOEColisiones::ColisionDiscoMapaColision(pX1, pY1, radio1); }

// Entrada

const char *NOE_ObtenEntrada() { return NOEEntrada::ObtenEntrada(); }
void NOE_LimpiaEntrada()  { NOEEntrada::LimpiaEntrada(); }

// Graficos

int NOE_ObtenPosicionCamaraX() { return NOEGraficos::ObtenPosicionCamaraX(); }
int NOE_ObtenPosicionCamaraY() { return NOEGraficos::ObtenPosicionCamaraY(); }
void NOE_PonPosicionCamara(int x, int y) { NOEGraficos::PonPosicionCamara(x, y); }

int NOE_ObtenTile(int tilemap, int posicionX, int posicionY) { return NOEGraficos::ObtenTile(tilemap, posicionX, posicionY); }
void NOE_PonTile(int tilemap, int posicionX, int posicionY, int tile) { NOEGraficos::PonTile(tilemap, posicionX, posicionY, tile); }
void NOE_LimpiaTilemap(int tilemap, int tile) { NOEGraficos::LimpiaTilemap(tilemap, tile); }
void NOE_DibujaTilemap(int numTilemap) { NOEGraficos::DibujaTilemap(numTilemap); }

void NOE_DibujaSprite(int sprite, int x, int y, int ancho, int alto, bool invertirX, bool invertirY) { NOEGraficos::DibujaSprite(sprite, x, y, ancho, alto, invertirX, invertirY); }

void NOE_DibujaDecorado(int decorado) { NOEGraficos::DibujaDecorado(decorado); }

void NOE_DibujaCaracter(char caracter, int x, int y, int ancho, int alto, int font) { NOEGraficos::DibujaCaracter(caracter, x, y, ancho, alto, font); }
void NOE_DibujaTexto(const char texto[], int x, int y, int anchoCaracter, int altoCaracter, int font) { NOEGraficos::DibujaTexto(texto, x, y, anchoCaracter, altoCaracter, font); }

void NOE_DibujaDisco(int x, int y, int radio, int r, int g, int b) { NOEGraficos::DibujaDisco(x, y, radio, r, g, b); }
void NOE_DibujaCaja(int x, int y, int ancho, int alto, int r, int g, int b) { NOEGraficos::DibujaCaja(x, y, ancho, alto, r, g, b); }
void NOE_DibujaForma(int forma, int x, int y, int ancho, int alto, int r, int g, int b) { NOEGraficos::DibujaForma(forma, x, y, ancho, alto, r, g, b); }

int NOE_ConviertePantallaAMundoX(int x) { return NOEGraficos::ConviertePantallaAMundoX(x); }
int NOE_ConviertePantallaAMundoY(int y) { return NOEGraficos::ConviertePantallaAMundoY(y); }
int NOE_ConvierteMundoAPantallaX(int x) { return NOEGraficos::ConvierteMundoAPantallaX(x); }
int NOE_ConvierteMundoAPantallaY(int y) { return NOEGraficos::ConvierteMundoAPantallaY(y); }
int NOE_ConvierteMundoATileX(int x) { return NOEGraficos::ConvierteMundoATileX(x); }
int NOE_ConvierteMundoATileY(int y) { return NOEGraficos::ConvierteMundoATileY(y); }
int NOE_ConvierteTileAMundoX(int x) { return NOEGraficos::ConvierteTileAMundoX(x); }
int NOE_ConvierteTileAMundoY(int y) { return NOEGraficos::ConvierteTileAMundoY(y); }

// Sistema

void NOE_Espera(int tiempo) { NOEPrincipal::Espera(tiempo); }
int NOE_ObtenTiempoDesdeActualizacion() { return NOESistema::ObtenTiempoDesdeActualizacion(); }

int NOE_ObtenNumeroAleatorio(int minimo, int maximo) { return NOESistema::ObtenNumeroAleatorio(minimo, maximo); }

// Entrada

bool NOE_ObtenTeclaPulsada(NOE_Tecla tecla) { return NOEControles::ObtenTeclaPulsada(tecla); }
bool NOE_ObtenTeclaArriba(NOE_Tecla tecla) { return NOEControles::ObtenTeclaArriba(tecla); }
bool NOE_ObtenTeclaAbajo(NOE_Tecla tecla) { return NOEControles::ObtenTeclaAbajo(tecla); }

int  NOE_ObtenPosicionRatonX() { return NOEControles::ObtenPosicionRatonX(); }
int  NOE_ObtenPosicionRatonY() { return NOEControles::ObtenPosicionRatonY(); }
int  NOE_ObtenDesplazamientoRatonX() { return NOEControles::ObtenDesplazamientoRatonX(); }
int  NOE_ObtenDesplazamientoRatonY() { return NOEControles::ObtenDesplazamientoRatonY(); }

bool NOE_ObtenBotonRaton(NOE_BotonRaton boton) { return NOEControles::ObtenBotonRaton(boton); }
bool NOE_ObtenBotonRatonAbajo(NOE_BotonRaton boton) { return NOEControles::ObtenBotonRatonAbajo(boton); }
bool NOE_ObtenBotonRatonArriba(NOE_BotonRaton boton) { return NOEControles::ObtenBotonRatonArriba(boton); }

// Sonido

void NOE_ReproduceMusica(int musica, int volumen, int pitch) { ::NOESonido::ReproduceMusica(musica, volumen, pitch); }
void NOE_ParaMusica() { ::NOESonido::ParaMusica(); }

int NOE_ReproduceSonido(int sonido, int volumen, int pitch) { return ::NOESonido::ReproduceSonido(sonido, volumen, pitch); }
void NOE_ParaSonido(int canal) { ::NOESonido::ParaSonido(canal); }

// Datos

int NOE_ObtenNumeroDatasets() { return ::NOEDatos::ObtenNumDatasets(); }
int NOE_ObtenTamanyoDataset(int dataset) { return ::NOEDatos::ObtenTamanyoDataset(dataset); }
void NOE_RedimensionaDataset(int dataset, int tamanyo) { ::NOEDatos::PonTamanyoDataset(dataset, tamanyo); }
void NOE_LimpiaDataset(int dataset, int desde, int hasta) { ::NOEDatos::LimpiaDataset(dataset, desde, hasta); }
void NOE_CopiaDataset(int indiceDatasetOrigen, int indiceDatasetDestino, int origenDesde, int origenHasta, int destinoDesde) { ::NOEDatos::CopiaDataset(indiceDatasetOrigen, indiceDatasetDestino, origenDesde, origenHasta, destinoDesde); }
void NOE_GuardaDatasetAhora(int indiceDataset) { ::NOEDatos::GuardaDatasetAhora(indiceDataset); }
void NOE_RecuperaDatosDataset(int dataset, int desde, int hasta, void* bytes) { ::NOEDatos::ObtenDatosDataset(dataset, desde, hasta, bytes); }
void NOE_AlmacenaDatosDataset(int dataset, int desde, int hasta, const void* bytes) { ::NOEDatos::PonDatosDataset(dataset, desde, hasta, bytes); }
int NOE_RecuperaEnteroDataset(int indiceDataset, int offset) { return ::NOEDatos::ObtenEnteroDataset(indiceDataset, offset); }
void NOE_AlmacenaEnteroDataset(int indiceDataset, int offset, int valor) { ::NOEDatos::PonEnteroDataset(indiceDataset, offset, valor); }
void NOE_RecuperaTextoDataset(int indiceDataset, int offset, char texto[])  { ::NOEDatos::ObtenTextoDataset(indiceDataset, offset, texto); }
void NOE_AlmacenaTextoDataset(int indiceDataset, int offset, const char texto[])  { ::NOEDatos::PonTextoDataset(indiceDataset, offset, texto); }
void NOE_ImprimeDataset(int indiceDataset, bool modoTexto) {  ::NOEDatos::ImprimeDataset(indiceDataset, modoTexto); }

