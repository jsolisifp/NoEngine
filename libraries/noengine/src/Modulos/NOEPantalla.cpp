#include "Modulos/NOEComun.hpp"

namespace NOEPantalla
{
	// Ventana

	const int noeUIMaxCadena = 200;

	sfRenderWindow* ventana;
	sfImage* icono;
	sfColor  colorLimpiar;

	// Cursor

	int cursor;
	bool cursorMostrar;
	int cursorAncho;
	int cursorAlto;
	int cursorDesplazamientoX;
	int cursorDesplazamientoY;

	sfSprite** cursores;

	void DibujaCursor();

	bool IniciaPantalla()
	{
		// Inicia ventana
		
		Configuracion &c = ObtenConfiguracion();
		
		sfVideoMode mode = { (unsigned int)(c.anchoPantalla),
							 (unsigned int)(c.altoPantalla),
							 (unsigned int)(c.bpp) };


		char titulo[noeUIMaxCadena];

		sprintf(titulo, "%s Version %d.%d", noeNombreLibreria, noeVersionMajor, noeVersionMinor);

		int style = sfTitlebar;
		
		
		if(c.pantallaCompleta) { style |= sfFullscreen; }
		
		ventana = sfRenderWindow_create(mode, titulo, style, NULL);
		if(!ventana) { return false; }
		
		colorLimpiar = sfColor_fromRGB(0, 0, 0);

		
		char ruta[noeRecursosMaxRuta];
		sprintf(ruta, "%s%s", noeRecursosRutaBase, "icono.png");
		
		icono = sfImage_createFromFile(ruta);
		
		sfRenderWindow_setIcon(ventana, sfImage_getSize(icono).y, sfImage_getSize(icono).y, sfImage_getPixelsPtr(icono));
		
		// Inicia cursor
		
		cursor = c.cursor;
		cursorMostrar = true;
		
		cursorDesplazamientoX = c.desplazamientoCursorX;
		cursorDesplazamientoY = c.desplazamientoCursorY;
		cursorAncho = c.anchoCursor;
		cursorAlto = c.altoCursor;
		
		sfRenderWindow* ventana = ObtenVentana();
		
		sfRenderWindow_setMouseCursorVisible(ventana, sfFalse);
		
		cursores = new sfSprite*[c.numCursores];
		
		for(int i = 0; i < c.numCursores; i ++)
		{
			sfTexture* texture;

			sprintf(ruta, "%s%s%02d.png", noeRecursosRutaBase, noeRecursosRutaCursores, i);
			texture = sfTexture_createFromFile(ruta, NULL);

			cursores[i] = sfSprite_create();
			sfSprite_setTexture(cursores[i], texture, sfTrue);

		}		
		
		return true;
	}


	void LimpiaPantalla(int r, int g, int b)
	{
		sfColor color = sfColor_fromRGB(r,g,b);									
	   sfRenderWindow_clear(ventana, color);
	}

	void MuestraPantalla()
	{
		//DebugDibujaCollisionMap();
		
		DibujaCursor();
		
		sfRenderWindow_display(ventana);	
	}

	sfRenderWindow* ObtenVentana()
	{
		return ventana;
	}
	
	int ObtenAnchoPantalla()
	{
		Configuracion &c = ObtenConfiguracion();	
		return c.anchoPantalla;
	}
	
	int ObtenAltoPantalla()
	{
		Configuracion &c = ObtenConfiguracion();	
		return c.altoPantalla;
	}
	


	void PonCursor(int c)
	{
		cursor = c;
	}

	void PonMostrarCursor(bool mostrar)
	{
		cursorMostrar = mostrar;
	}

	void PonTamanyoCursor(int ancho, int alto)
	{
		cursorAncho = ancho;
		cursorAlto = alto;
	}

	void PonDesplazamientoCursor(int x, int y)
	{
		cursorDesplazamientoX = x;
		cursorDesplazamientoY = y;
	}

	void DibujaCursor()
	{
		Configuracion &c = ObtenConfiguracion();
		sfRenderWindow *ventana = ObtenVentana(); 
		
		if(cursorMostrar)
		{
			sfSprite* sprite = cursores[cursor];
			
			sfVector2f position = { (float)ratonX - cursorDesplazamientoX, (float)ratonY  - cursorDesplazamientoY};
			sfSprite_setPosition(sprite, position);
			
			const sfTexture* textura = sfSprite_getTexture(sprite);
			sfVector2u tamanyo = sfTexture_getSize(textura);

			sfVector2f escala = { (float)cursorAncho / tamanyo.x, (float)cursorAlto / tamanyo.y  };
			sfSprite_setScale(sprite, escala);

			sfRenderWindow_drawSprite(ventana, sprite, NULL);
		
		}
		
		char coordinates[noeMaxCadenaUI];

		//sprintf(coordinates, "%d %d", ratonX, ratonY);
		
		//DibujaTexto(coordinates, ratonX, ratonY, c.tamanyoTile / 5, c.tamanyoTile / 5, 0);
		
	}

	void FinalizaPantalla()
	{
		sfRenderWindow_destroy(ventana);
		sfImage_destroy(icono);
		
	}

}