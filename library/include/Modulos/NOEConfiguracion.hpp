#ifndef __NOE_CONFIGURACION__
#define __NOE_CONFIGURACION__

namespace NOEConfiguracion
{

	struct Configuracion
	{
		int bpp = 32;
		int tamanyoTile = 16;
		int anchoPantalla = 800;
		int altoPantalla = 400;
		int pantallaCompleta = 0;
		int anchoTilemaps = 20;
		int altoTilemaps  = 20;
		int numTiposTile = 32;
		int numTilemaps = 3;
		int numSprites = 32;
		int numDecorados = 8;
		int numFuentes = 2;
		int numCursores = 8;
		int cursor = 0;
		int anchoCursor = 32;
		int altoCursor = 32;
		int desplazamientoCursorX = 2;
		int desplazamientoCursorY = 2;
		int numMusicas = 8;
		int numSonidos = 32;
		int numCanales = 16;
		int numFormas = 32;
		int tiempoMinimoActualizacion = 10;
		int longitudLineaEntrada = 100;
        int numDatasets = 8;
	};

	void IniciaConfiguracion(); 

	Configuracion& ObtenConfiguracion();
	
}

#endif