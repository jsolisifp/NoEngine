#ifndef __NOE_GRAFICOS__
#define __NOE_GRAFICOS__

namespace NOEGraficos
{
	extern int camaraX;
	extern int camaraY;

	const int noeMaxCadenaUI = 200;

	void IniciaGraficos();
	void FinalizaGraficos();

	bool EstaEnTilemap(int posicionX, int posicionY);
	void RellenaAreaTilemap(int tilemap, int posicionX, int posicionY, int ancho, int alto, int tipoBloque);
	void RellenaTilemap(int tilemap, int tipoBloque);
	int ObtenAnchoTilemap();
	int ObtenAltoTilemap();
	int ObtenAnchoGlifo(int glifo, int fuente);
	int ObtenAltoGlifo(int glifo, int fuente);
	void DibujaGlifo(int glifo, int x, int y, int ancho, int alto, int font);
	int BuscaGlifo(char caracter);
	
	
	// Library functions
	
	int ObtenPosicionCamaraX();
	int ObtenPosicionCamaraY();
	void PonPosicionCamara(int x, int y);
	void DibujaSprite(int sprite, int x, int y, int ancho, int alto, bool invertirX, bool invertirY);
	void DibujaTilemap(int indice);
	bool EstaEnTilemap(int posicionX, int posicionY);
	void RellenaAreaTilemap(int tilemap, int posicionX, int posicionY, int ancho, int alto, int tipoBloque);
	void RellenaTilemap(int tilemap, int tipoBloque);
	void LimpiaTilemap(int tilemap, int tile);
	void PonTile(int tilemap, int posicionX, int posicionY, int tipo);
	int ObtenTile(int tilemap, int posicionX, int posicionY);
	int ObtenAnchoTilemap();
	int ObtenAltoTilemap();
	void DibujaDisco(int x, int y, int radio, int r, int g, int b);
	void DibujaCaja(int x, int y, int ancho, int alto, int r, int g, int b);
	void DibujaForma(int forma, int x, int y, int ancho, int alto, int r, int g, int b);
	void DibujaDecorado(int decorado);
	void DibujaCaracter(char caracter, int x, int y, int ancho, int alto, int font);
	void DibujaTexto(const char texto[], int x, int y, int anchoCaracter, int altoCaracter, int font);
	int ConviertePantallaAMundoX(int x);
	int ConviertePantallaAMundoY(int y);
	int ConvierteMundoAPantallaX(int x);
	int ConvierteMundoAPantallaY(int y);
	int ConvierteMundoATileX(int x);
	int ConvierteMundoATileY(int y);
	int ConvierteTileAMundoX(int x);
	int ConvierteTileAMundoY(int y);
}

#endif
