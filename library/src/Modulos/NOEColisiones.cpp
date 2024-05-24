#include "Modulos/NOEComun.hpp"

namespace NOEColisiones
{

	sfRectangleShape * debugColisionRect;
	sfColor	debugColor;

	int**  colisionMap;

	void IniciaColisiones()
	{
		Configuracion &c = ObtenConfiguracion();
		
		colisionMap = new int *[c.altoTilemaps];
		
		for(int y = 0; y < c.altoTilemaps; y ++)
		{
			colisionMap[y] = new int[c.anchoTilemaps];
			
			for(int x = 0; x < c.anchoTilemaps; x ++)
			{
				colisionMap[y][x] = noe_tileColisionVacio;
			}
		}
		
		debugColisionRect = sfRectangleShape_create();
		debugColor = sfColor_fromRGBA(0,255,0,50);
		
	}

	int ObtenTileColision(int posicionX, int posicionY)
	{
		if(EstaEnTilemap(posicionX, posicionY))
		{
			return colisionMap[posicionY][posicionX];
		}
		else
		{
			return noe_tileColisionVacio;
		}
	}

	void PonTileColision(int posicionX, int posicionY, int colision)
	{
		if(EstaEnTilemap(posicionX, posicionY))
		{
			colisionMap[posicionY][posicionX] = colision;
		}
	}

	void LimpiaMapaColision()
	{
		Configuracion &c = ObtenConfiguracion();
		
		for(int y = 0; y < c.altoTilemaps; y ++)
		{
			for(int x = 0; x < c.anchoTilemaps; x ++)
			{
				colisionMap[y][x] = noe_tileColisionVacio;
			}
		}
	}

	bool ColisionDiscoDisco(int pX1, int pY1, int radio1, int pX2, int pY2, int radio2)
	{
		float pX1F = pX1;
		float pY1F = pY1;
		float radio1F = radio1;
		float pX2F = pX2;
		float pY2F = pY2;
		float radio2F = radio2;
		
		if(pow(pX2F - pX1F, 2) + pow(pY1F - pY2F, 2) <= pow(radio1 + radio2, 2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ColisionDiscoPunto(int pX1, int pY1, int radio, int pX2, int pY2)
	{
		int d2 = pow(pX1 - pX2, 2) + pow(pY1 - pY2, 2);
		
		return d2 < pow(radio, 2);
	}

	bool ColisionIntervaloIntervalo(int a1, int b1, int a2, int b2)
	{
		if(b1 < a2 || a1 > b2) { return false; }
		else { return true;	}
	}

	bool ColisionCajaCaja(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int ancho2, int alto2)
	{
		bool colisionX = ColisionIntervaloIntervalo(pX1, pX1 + ancho1 - 1, pX2, pX2 + ancho2 -1);
		bool colisionY = ColisionIntervaloIntervalo(pY1, pY1 + alto1 - 1, pY2, pY2 + alto2 -1);
		
		//if(colisionX) { printf("ColX\n"); }
		//if(colisionY) { printf("ColY\n"); }
		
		
		if(colisionX && colisionY)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ColisionCajaDisco(int pX1, int pY1, int ancho1, int alto1, int pX2, int pY2, int radio2)
	{
		if(pX2 < pX1 && pY2 < pY1)
		{
			return ColisionDiscoPunto(pX2, pY2, radio2, pX1, pY1);
		}
		else if(pX2 > pX1 + ancho1 && pY2 < pY1)
		{
			return ColisionDiscoPunto(pX2, pY2, radio2, pX1 + ancho1, pY1);
		}
		else if(pX2 > pX1 + ancho1 && pY2 > pY1 + alto1)
		{
			return ColisionDiscoPunto(pX2, pY2, radio2, pX1 + ancho1, pY1 + alto1);
		}
		else if(pX2 < pX1 && pY2 > pY1 + alto1)
		{
			return ColisionDiscoPunto(pX2, pY2, radio2, pX1, pY1 + alto1);
		}
		else
		{
			return ColisionCajaCaja(pX1, pY1, ancho1, alto1, pX2 - radio2, pY2 - radio2, 2 * radio2, 2 * radio2);
		}
		
	}

	bool ColisionCajaMapaColision(int pX1, int pY1, int ancho1, int alto1)
	{
		bool colision = false;

		Configuracion &c = ObtenConfiguracion();
		
		for(int y = 0; y < c.altoTilemaps; y ++)
		{
			for(int x = 0; x < c.anchoTilemaps; x ++)
			{
				int tileX = x * c.tamanyoTile;
				int tileY = y * c.tamanyoTile;
				
				if(colisionMap[y][x] != noe_tileColisionVacio)
				{	
					if(ColisionCajaCaja(pX1, pY1, ancho1, alto1, tileX, tileY, c.tamanyoTile, c.tamanyoTile))
					{ colision = true; }
				}
				
			}
		}
		
		return colision;
	}

	bool ColisionDiscoMapaColision(int pX1, int pY1, int radio1)
	{
		bool colision = false;

		Configuracion &c = ObtenConfiguracion();
		
		for(int y = 0; y < c.altoTilemaps; y ++)
		{
			for(int x = 0; x < c.anchoTilemaps; x ++)
			{
				int tileX = x * c.tamanyoTile;
				int tileY = y * c.tamanyoTile;
				
				if(colisionMap[y][x] != noe_tileColisionVacio)
				{			
					if(ColisionCajaDisco(tileX, tileY, c.tamanyoTile, c.tamanyoTile, pX1, pY1, radio1))
					{
						colision = true;
					}
				}
				
			}
		}
		
		return colision;
	}

	void DebugDibujaCollisionMap()
	{
		Configuracion &c = ObtenConfiguracion();
		sfRenderWindow* ventana = ObtenVentana();
		
		char coordinates[100];
		
		for(int y = 0; y < c.altoTilemaps; y ++)
		{
			for(int x = 0; x < c.anchoTilemaps; x ++)
			{
				if(colisionMap[y][x] != noe_tileColisionVacio)
				{
					sfVector2f position = { (float)x * c.tamanyoTile - camaraX, (float)y * c.tamanyoTile  - camaraY };
					sfVector2f tamanyo = { (float)c.tamanyoTile, (float)c.tamanyoTile };
					sfRectangleShape_setPosition(debugColisionRect, position);
					sfRectangleShape_setSize(debugColisionRect, tamanyo);
					sfRectangleShape_setFillColor(debugColisionRect, debugColor);
					
					sfRenderWindow_drawRectangleShape(ventana, debugColisionRect, NULL);
				}
				
				sprintf(coordinates, "%d %d", x , y);
				
				DibujaTexto(coordinates, x * c.tamanyoTile - camaraX, y * c.tamanyoTile - camaraY, c.tamanyoTile / 5, c.tamanyoTile / 5, 0);
				
				
				
			}
		}
		
	}
	
}