#include "Modulos/NOEComun.hpp"

namespace NOEGraficos
{
	
	int camaraX = 0;
	int camaraY = 0;

	sfConvexShape** formas;

	sfSprite**		sprites;

	sfSprite**	tiposTile;
	int***		tilemaps;

	sfRectangleShape * formaCaja;
	sfCircleShape * formaDisco;

	sfSprite**		decorados;

	sfSprite***		fuentes;

	char fuenteCaracteresGlifo[] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
									 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
									 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3',
									 '4', '5', '6', '7', '8', '9', '0'	};


	void IniciaGraficos()
	{
		Configuracion & c = ObtenConfiguracion();		
		
		// Iniciar sprites
		
		sprites = new sfSprite*[c.numSprites];
		
		for(int i = 0; i < c.numSprites; i ++)
		{
			NOETextura* textura = ObtenTextura(NOE_GRUPOTEXTURAS_SPRITES, i);
			sprites[i] = sfSprite_create();
			sfSprite_setTexture(sprites[i], textura, sfTrue);
			
		}
		
		
		// Iniciar tilemaps
		
		tiposTile = new sfSprite*[c.numTiposTile];
		
		for(int i = 0; i < c.numTiposTile; i ++)
		{
			NOETextura* textura = ObtenTextura(NOE_GRUPOTEXTURAS_TIPOSTILE, i);
			tiposTile[i] = sfSprite_create();
			sfSprite_setTexture(tiposTile[i], textura, sfTrue);
			
		}
			
		
		tilemaps = new int**[c.numTilemaps];
		
		for(int t = 0; t < c.numTilemaps; t++)
		{
			tilemaps[t] = new int *[c.altoTilemaps];
			for(int y = 0; y < c.altoTilemaps; y ++)
			{	
				tilemaps[t][y] = new int[c.anchoTilemaps];			
				for(int x = 0; x < c.anchoTilemaps; x ++) { tilemaps[t][y][x] = noe_tileVacio; }
			}
			
		}
		
		// Iniciar formas
		
		formaDisco = sfCircleShape_create();
		formaCaja = sfRectangleShape_create();
		
		formas = new sfConvexShape*[c.numFormas];
		
		for(int i = 0; i < c.numFormas; i ++)
		{
			NOEDatosForma* datosForma = ObtenDatosForma(i);
			
			sfConvexShape* forma = sfConvexShape_create();
			
			sfConvexShape_setPointCount(forma, datosForma->numPuntos);
			
			for(int j = 0; j < datosForma->numPuntos; j ++)
			{
				sfVector2f punto = { (float)datosForma->puntos[j * 2],  (float)datosForma->puntos[j * 2 + 1] };
				sfConvexShape_setPoint(forma, j, punto);
				
			}
			
			formas[i] = forma;
			
		}
		
		// Iniciar decorados
		
		decorados = new sfSprite*[c.numDecorados];
		
		for(int i = 0; i < c.numDecorados; i ++)
		{
			NOETextura* textura;
			
			textura = ObtenTextura(NOE_GRUPOTEXTURAS_DECORADOS, i);
			decorados[i] = sfSprite_create();
			sfSprite_setTexture(decorados[i], textura, sfTrue);
			
		}

		// Iniciar fuentes
		
		fuentes	= new sfSprite**[c.numFuentes];
		
		for(int i = 0; i < c.numFuentes; i ++)
		{
			fuentes[i] = new sfSprite*[noeRecursosNumGlifosFuente];
			
			for(int j = 0; j < noeRecursosNumGlifosFuente; j ++)
			{
				NOETextura* textura;
				fuentes[i][j] = sfSprite_create();
				textura = ObtenFuente(i)[j];
				sfSprite_setTexture(fuentes[i][j], textura, sfTrue);
			}
		}	
		
		// printf("Fuentes iniciadas\n");
		
		
	}

	int ObtenPosicionCamaraX()
	{
		return camaraX;
	}

	int ObtenPosicionCamaraY()
	{
		return camaraY;
	}

	void PonPosicionCamara(int x, int y)
	{
		camaraX = x;
		camaraY = y;
	}

	void DibujaSprite(int sprite, int x, int y, int ancho, int alto, bool invertirX, bool invertirY)
	{
		int ancho2;
		int alto2;
		
		sfRenderWindow *ventana = ObtenVentana();
		sfSprite* sprite2 = sprites[sprite];
		
		
		const sfTexture* textura = sfSprite_getTexture(sprite2);
		sfVector2u tamanyo = sfTexture_getSize(textura);
		
		ancho2 = ancho;
		alto2 = alto;
		
		if(alto < 0) { alto2 = tamanyo.y; }
		if(ancho < 0) { ancho2 = tamanyo.x; }
		
		sfVector2f escala = { (invertirX ? -1: 1) * (float)ancho2 / tamanyo.x, (invertirY ? -1: 1) * (float)alto2 / tamanyo.y  };
		sfSprite_setScale(sprite2, escala);

		sfVector2f position = { (invertirX ? 1 : 0) * ancho2 + (float)x - camaraX, (invertirY ? 1 : 0) * alto2 + (float)y - camaraY };
		sfSprite_setPosition(sprite2, position);

		sfRenderWindow_drawSprite(ventana, sprite2, NULL);
		
	}


	void DibujaTilemap(int indice)
	{
		Configuracion &c = ObtenConfiguracion();
		sfRenderWindow * ventana = ObtenVentana();

		for(int y = 0; y < c.altoTilemaps; y++)
		{
			for(int x = 0; x < c.anchoTilemaps; x++)
			{
				int tipoTile = tilemaps[indice][y][x];
				if(tipoTile != noe_tileVacio)
				{
					sfSprite* sprite = tiposTile[tipoTile];
					
					sfVector2f position = { (float)x * c.tamanyoTile - camaraX, (float)y * c.tamanyoTile  - camaraY };
					sfSprite_setPosition(sprite, position);
					
					const sfTexture* textura = sfSprite_getTexture(sprite);
					sfVector2u tamanyo = sfTexture_getSize(textura);

					sfVector2f escala = { (float)c.tamanyoTile / tamanyo.x, (float)c.tamanyoTile / tamanyo.y  };
					sfSprite_setScale(sprite, escala);

					sfRenderWindow_drawSprite(ventana, sprite, NULL);
				}
			}
		}

	}

	// Funciones privadas

	bool EstaEnTilemap(int posicionX, int posicionY)
	{
		Configuracion &c = ObtenConfiguracion();	
		return posicionX >= 0 && posicionX < c.anchoTilemaps && posicionY >= 0 && posicionY < c.altoTilemaps;
	}

	void RellenaAreaTilemap(int tilemap, int posicionX, int posicionY, int ancho, int alto, int tipoBloque)
	{
		for(int y = 0; y < alto; y++)
		{
			for(int x = 0; x < ancho; x++)
			{
				if(EstaEnTilemap(posicionX + x, posicionY + y))
				{
					tilemaps[tilemap][posicionY + y][posicionX + x] = tipoBloque;
				}
			}
		}

	}

	void RellenaTilemap(int tilemap, int tipoBloque)
	{
		Configuracion &c = ObtenConfiguracion();	
		RellenaAreaTilemap(tilemap, 0, 0, c.anchoTilemaps, c.altoTilemaps, tipoBloque);
	}

	void LimpiaTilemap(int tilemap, int tile)
	{
		RellenaTilemap(tilemap, tile);
	}


	void PonTile(int tilemap, int posicionX, int posicionY, int tipo)
	{
		if(EstaEnTilemap(posicionX, posicionY))
		{
			tilemaps[tilemap][posicionY][posicionX] = tipo;
		}
	}

	int ObtenTile(int tilemap, int posicionX, int posicionY)
	{
		if(EstaEnTilemap(posicionX, posicionY))
		{
			return tilemaps[tilemap][posicionY][posicionX];
		}
		else
		{
			return noe_tileVacio;
		}
	}

	int ObtenAnchoTilemap()
	{
		Configuracion &c = ObtenConfiguracion();
		return c.anchoTilemaps;
	}

	int ObtenAltoTilemap()
	{
		Configuracion &c = ObtenConfiguracion();
		return c.altoTilemaps;
	}

	void DibujaDisco(int x, int y, int radio, int r, int g, int b)
	{
		sfRenderWindow *ventana = ObtenVentana();
		sfVector2f position = { (float)x - radio - camaraX, (float)y - radio - camaraY };
		sfCircleShape_setPosition(formaDisco, position);
		sfCircleShape_setRadius(formaDisco, radio);
		sfColor color = sfColor_fromRGB(r, g, b);
		sfCircleShape_setFillColor(formaDisco, color);
		
		sfRenderWindow_drawCircleShape(ventana, formaDisco, NULL);
	}

	void DibujaCaja(int x, int y, int ancho, int alto, int r, int g, int b)
	{
		sfRenderWindow *ventana = ObtenVentana();
		sfVector2f position = { (float)x - camaraX, (float)y - camaraY };
		sfRectangleShape_setPosition(formaCaja, position);
		sfVector2f tamanyo = { (float)ancho, (float)alto };
		sfRectangleShape_setSize(formaCaja, tamanyo);
		sfColor color = sfColor_fromRGB(r, g, b);
		sfRectangleShape_setFillColor(formaCaja, color);
		
		sfRenderWindow_drawRectangleShape(ventana, formaCaja, NULL);
	}
	
	void DibujaForma(int forma, int x, int y, int ancho, int alto, int r, int g, int b)
	{
		sfRenderWindow *ventana = ObtenVentana();
		sfFloatRect rect = sfConvexShape_getLocalBounds(formas[forma]);
		sfVector2f escala = { ancho / rect.width, alto / rect.height };		
		sfVector2f posicion = { (float)x - rect.left * escala.x - camaraX, (float)y  - rect.top * escala.y - camaraY };
		sfConvexShape_setPosition(formas[forma], posicion);
		sfConvexShape_setScale(formas[forma], escala);
		sfColor color = sfColor_fromRGB(r, g, b);
		sfConvexShape_setFillColor(formas[forma], color);		
		
		sfRenderWindow_drawConvexShape(ventana, formas[forma], NULL);
	}

	void DibujaDecorado(int decorado)
	{
		sfRenderWindow *ventana = ObtenVentana();
		Configuracion &c = ObtenConfiguracion();	

		sfSprite* decorado2 = decorados[decorado];
			
		const sfTexture* textura = sfSprite_getTexture(decorado2);
		sfVector2u tamanyo = sfTexture_getSize(textura);

			
		sfVector2f escala = { (float) c.anchoPantalla / tamanyo.x, (float) c.altoPantalla  / tamanyo.y  };
		sfSprite_setScale(decorado2, escala);

		sfVector2f position = { 0, 0 };
		sfSprite_setPosition(decorado2, position);

		sfRenderWindow_drawSprite(ventana, decorado2, NULL);
		
	}
	
	int ObtenAnchoGlifo(int glifo, int font)
	{
		sfSprite* sprite = fuentes[font][glifo];
		const sfTexture* textura = sfSprite_getTexture(sprite);
		sfVector2u tamanyo = sfTexture_getSize(textura);		
		
		return tamanyo.x;
	}
	
	int ObtenAltoGlifo(int glifo, int font)
	{
		sfSprite* sprite = fuentes[font][glifo];
		const sfTexture* textura = sfSprite_getTexture(sprite);
		sfVector2u tamanyo = sfTexture_getSize(textura);		
		
		return tamanyo.y;		
	}
	
	int BuscaGlifo(char caracter)
	{
		int glifo = -1;
		bool found = false;
		
		int i = 0;
		while(!found && i < noeRecursosNumGlifosFuente)
		{
			if(fuenteCaracteresGlifo[i] == caracter)
			{	
				glifo = i;
				found = true;
				
			}
			else
			{	
				i ++;
			}			
			
		}

		return glifo;		
	}
	
	void DibujaGlifo(int glifo, int x, int y, int ancho, int alto, int font)
	{
		sfRenderWindow *ventana = ObtenVentana();
		
		sfSprite* sprite2 = fuentes[font][glifo];

		int ancho2 = ancho;
		int alto2 = alto;
		
		const sfTexture* textura = sfSprite_getTexture(sprite2);
		sfVector2u tamanyo = sfTexture_getSize(textura);

		if(ancho < 0) { ancho2 = tamanyo.x; }		
		if(alto < 0) { alto2 = tamanyo.y;  }
		
		

		sfVector2f escala = { (float)ancho2 / tamanyo.x, (float)alto2 / tamanyo.y  };
		sfSprite_setScale(sprite2, escala);

		sfVector2f position = { (float)x, (float)y };
		sfSprite_setPosition(sprite2, position);

		sfRenderWindow_drawSprite(ventana, sprite2, NULL);
		
	}


	void DibujaCaracter(char caracter, int x, int y, int ancho, int alto, int font)
	{
		int glifo = BuscaGlifo(caracter);
		if(glifo < 0) { glifo = 0; }
		
		DibujaGlifo(glifo, x, y, ancho, alto, font);
		
		
	}

	void DibujaTexto(const char texto[], int x, int y, int anchoCaracter, int altoCaracter, int font)
	{
		//printf(texto);
		int i = 0;
		
		while(texto[i] != '\0')
		{
			int glifo;
			
			int ancho2 = anchoCaracter;
			int alto2 = altoCaracter;
			
			glifo = BuscaGlifo(texto[i]);
			if(glifo < 0) { glifo = 0; }
			
			if(anchoCaracter < 0) { ancho2 = ObtenAnchoGlifo(glifo, font); }
			if(altoCaracter < 0) { alto2 = ObtenAltoGlifo(glifo, font); }

			DibujaGlifo(glifo, x + i * ancho2, y, ancho2, alto2, font);
			
			i ++;
		}
	}

	int ConviertePantallaAMundoX(int x)
	{
		return x + camaraX;
	}

	int ConviertePantallaAMundoY(int y)
	{
		return y + camaraY;
	}

	int ConvierteMundoAPantallaX(int x)
	{
		return x - camaraX;
	}

	int ConvierteMundoAPantallaY(int y)
	{
		return y - camaraY;
	}

	int ConvierteMundoATileX(int x)
	{
		Configuracion &c = ObtenConfiguracion();
		return x / c.tamanyoTile;
	}

	int ConvierteMundoATileY(int y)
	{
		Configuracion &c = ObtenConfiguracion();
		return y / c.tamanyoTile;
	}

	int ConvierteTileAMundoX(int x)
	{
		Configuracion &c = ObtenConfiguracion();
		return x * c.tamanyoTile;
	}

	int ConvierteTileAMundoY(int y)
	{
		Configuracion &c = ObtenConfiguracion();
		return y * c.tamanyoTile;
	}

}