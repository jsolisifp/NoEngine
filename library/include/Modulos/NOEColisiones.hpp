#ifndef __NOE_COLISIONES__
#define __NOE_COLISIONES__

namespace NOEColisiones
{
	
	extern int** colisionMap;

	void IniciaColisiones();

	void DebugDibujaCollisionMap();

	// Library functions

	int ObtenTileColision(int posicionX, int posicionY);
	void PonTileColision(int posicionX, int posicionY, int colision);
	void LimpiaMapaColision();
	bool ColisionDiscoDisco(int pX1, int pY1, int radio1, int pX2, int pY2, int radio2);
	bool ColisionDiscoPunto(int pX1, int pY1, int radio, int pX2, int pY2);
	bool ColisionIntervaloIntervalo(int a1, int b1, int a2, int b2);
	bool ColisionCajaCaja(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int ancho2, int alto2);
	bool ColisionCajaDisco(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int radio2);
	bool ColisionCajaMapaColision(int pX1, int pY1, int ancho1, int alto1);
	bool ColisionDiscoMapaColision(int pX1, int pY1, int radio1);
	void DebugDibujaCollisionMap();

}


#endif
