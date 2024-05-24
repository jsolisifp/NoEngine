#include "NoEntiendo.hpp"
#include <stdio.h>

#define FPS 30
#define ESPERA ((int)(((float)1/FPS) * 1000))
#define VELOCIDAD 3

int main()
{
	NOE_Inicia();
	

	int circulo1X = 0;
	int circulo1Y = 0;
	int circulo1Radio = 32;
	
	int circulo2X = 0;
	int circulo2Y = 0;
	int circulo2Radio = 64;


	int rectangulo1X = 0;
	int rectangulo1Y = 0;
	int rectangulo1Ancho = 64;
	int rectangulo1Alto = 128;
	
	int rectangulo2X = 0;
	int rectangulo2Y = 0;
	int rectangulo2Ancho = 128;
	int rectangulo2Alto = 64;
	
	int forma1X = 0;
	int forma1Y = 0;
	int forma1Ancho = 64;
	int forma1Alto = 128;
	
	int forma2X = 0;
	int forma2Y = 0;
	int forma2Ancho = 128;
	int forma2Alto = 64;	

	int color1R = 0;
	int color1G = 0;
	int color1B = 0;

	int color2R = 0;
	int color2G = 0;
	int color2B = 0;

	int ciclo = 0;
	int direccionX = 0;
	int direccionY = 0;
	int posicionX = 0;
	int posicionY = 0;

	int direccionCamaraX = 0;
	int direccionCamaraY = 0;
	
	int temporizador = 0;
	
	int musica = 0;
	
	int figuras = 0;
	
	
	//NOE_ReproduceMusica(musica);
	

	while(!NOE_ObtenTeclaPulsada(NOE_TECLA_ESCAPE))
	{
		NOE_Actualiza();		

		if(NOE_ObtenTeclaPulsada(NOE_TECLA_D))
		{
			direccionX = 1;			
		}
		else if(NOE_ObtenTeclaPulsada(NOE_TECLA_A))
		{
			direccionX = -1;
		}
		if(NOE_ObtenTeclaPulsada(NOE_TECLA_S))
		{
			direccionY = 1;
		}
		else if(NOE_ObtenTeclaPulsada(NOE_TECLA_W))
		{
			direccionY = -1;
		}

		if(NOE_ObtenTeclaPulsada(NOE_TECLA_DERECHA))
		{
			direccionCamaraX = 2;			
		}
		else if(NOE_ObtenTeclaPulsada(NOE_TECLA_IZQUIERDA))
		{
			direccionCamaraX = -2;
		}
		if(NOE_ObtenTeclaPulsada(NOE_TECLA_ABAJO))
		{
			direccionCamaraY = 2;
		}
		else if(NOE_ObtenTeclaPulsada(NOE_TECLA_ARRIBA))
		{
			direccionCamaraY = -2;
		}		
		
		if(NOE_ObtenTeclaAbajo(NOE_TECLA_ENTRAR))
		{
			figuras = (figuras + 1) % 4;
			
			if(musica == 0) { NOE_ReproduceMusica(3, 100, 200); musica = 3; }
			else { NOE_ReproduceMusica(0, 50, 50); musica = 0; }
			
			NOE_LimpiaEntrada();
			
		}		
		
		if(NOE_ObtenTeclaAbajo(NOE_TECLA_N))
		{
			int canal = NOE_ReproduceSonido(0, 50, 100);
			
			//printf("Reproduciendo sonido 0 en canal %d\n", canal);
			
			//printf("Detectada N\n");
		}		
		
		if(NOE_ObtenTeclaAbajo(NOE_TECLA_M))
		{
			int canal = NOE_ReproduceSonido(1, 100, 50);
			
			//printf("Reproduciendo sonido 1 en canal %d\n", canal);
			//printf("Detectada M\n");
		}

		if(NOE_ObtenBotonRaton(NOE_BOTONRATON_CENTRAL))
		{
			direccionCamaraX = - NOE_ObtenDesplazamientoRatonX();		
			direccionCamaraY = - NOE_ObtenDesplazamientoRatonY();
			
		}		

		NOE_PonTile(0, 5, 5, 7);
		
		temporizador = temporizador + NOE_ObtenTiempoDesdeActualizacion();

		if(temporizador > 10 || NOE_ObtenBotonRaton(NOE_BOTONRATON_CENTRAL))
		{
			posicionX += direccionX;
			posicionY += direccionY;
			
			direccionX = 0;
			direccionY = 0;

			int x = NOE_ObtenPosicionCamaraX();
			int y = NOE_ObtenPosicionCamaraY();

			x += direccionCamaraX;
			y += direccionCamaraY;

			NOE_PonPosicionCamara(x, y);			

			direccionCamaraX = 0;
			direccionCamaraY = 0;

			temporizador = 0;

		}
		
		int x = NOE_ObtenPosicionRatonX();
		int y = NOE_ObtenPosicionRatonY();
		x = NOE_ConviertePantallaAMundoX(x);
		y = NOE_ConviertePantallaAMundoY(y);
		posicionX = NOE_ConvierteMundoATileX(x);
		posicionY = NOE_ConvierteMundoATileY(y);

		if(NOE_ObtenBotonRaton(NOE_BOTONRATON_PRINCIPAL))
		{
			NOE_PonTile(1, posicionX, posicionY, 1);		
			NOE_PonTileColision(posicionX, posicionY, 1);
			
			circulo1X = x;
			circulo1Y = y;
			
			rectangulo1X = x;
			rectangulo1Y = y;
			
			forma1X = x;
			forma1Y = y;			
			
			
		}
		else if(NOE_ObtenBotonRaton(NOE_BOTONRATON_SECUNDARIO))
		{
			NOE_PonTile(1, posicionX, posicionY, noe_tileVacio);		
			NOE_PonTileColision(posicionX, posicionY, noe_tileColisionVacio);
			
			circulo2X = x;
			circulo2Y = y;

			rectangulo2X = x;
			rectangulo2Y = y;

			forma2X = x;
			forma2Y = y;			
		}
		else
		{
			if(NOE_ObtenTileColision(posicionX, posicionY) != noe_tileColisionVacio)
			{
					//printf("Hay colision\n");
			}
			else
			{
					//printf("No hay colision\n");
			}
		}
		
		color1R = color2R = 0;
		color1G = color2G = 255;
		color1B = color2B = 0;
		
		
		if(figuras == 0)
		{
			if(NOE_ColisionDiscoMapaColision(circulo1X, circulo1Y, circulo1Radio))
			{
				color1R = 255;
				color1G = 0;
				color1B = 0;
			}
			
			if(NOE_ColisionDiscoMapaColision(circulo2X, circulo2Y, circulo2Radio))
			{
				color2R = 255;
				color2G = 0;
				color2B = 0;
			}
			
			if(NOE_ColisionDiscoDisco(circulo1X, circulo1Y, circulo1Radio, circulo2X, circulo2Y, circulo2Radio))
			{
				color1R = color2R = 255;
				color1G = color2G = 0;
				color1B = color2B = 0;
			}

		}
		else if(figuras == 1)
		{
			if(NOE_ColisionCajaMapaColision(rectangulo1X, rectangulo1Y, rectangulo1Ancho, rectangulo1Alto))
			{
				color1R = 255;
				color1G = 0;
				color1B = 0;
			}
			
			if(NOE_ColisionCajaMapaColision(rectangulo2X, rectangulo2Y, rectangulo2Ancho, rectangulo2Alto))
			{
				color2R = 255;
				color2G = 0;
				color2B = 0;
			}
			
			if(NOE_ColisionCajaCaja(rectangulo1X, rectangulo1Y, rectangulo1Ancho, rectangulo1Alto, rectangulo2X, rectangulo2Y, rectangulo2Ancho, rectangulo2Alto))
			{
				color1R = color2R = 255;
				color1G = color2G = 0;
				color1B = color2B = 0;
			}

		}
		else if(figuras == 2)
		{
			if(NOE_ColisionCajaMapaColision(rectangulo1X, rectangulo1Y, rectangulo1Ancho, rectangulo1Alto))
			{
				color1R = 255;
				color1G = 0;
				color1B = 0;
			}

			if(NOE_ColisionDiscoMapaColision(circulo2X, circulo2Y, circulo2Radio))
			{
				color2R = 255;
				color2G = 0;
				color2B = 0;
			}			
			
			if(NOE_ColisionCajaDisco(rectangulo1X, rectangulo1Y, rectangulo1Ancho, rectangulo1Alto, circulo2X, circulo2Y, circulo2Radio))
			{
				color1R = color2R = 255;
				color1G = color2G = 0;
				color1B = color2B = 0;
			}

		}
		

		NOE_LimpiaPantalla(20,20,20);
		NOE_DibujaDecorado(1);
		NOE_DibujaTilemap(0);
		NOE_DibujaTilemap(1);
		NOE_DibujaSprite(1, circulo1X - circulo1Radio, circulo1Y - circulo1Radio, 2 * circulo1Radio, 2 * circulo1Radio, 1, 1);
		NOE_DibujaSprite(1, circulo2X - circulo2Radio, circulo2Y - circulo2Radio, 2 * circulo2Radio, 2 * circulo2Radio, 1, 1);
		
		
		NOE_DibujaSprite(1, 100, 100, 50, -1, 0, 0);
		
		NOE_DibujaTexto("HOLA", 100, 100, -1, -1, 0);

		NOE_DibujaCaracter('B', 100, 200, 30, -1, 0);
		
		NOE_DibujaTexto("HOLA", 100, 300, -1, 100, 0);
		NOE_DibujaTexto("HOLA", 100, 400, 100, -1, 0);
		
		//printf("Dimensiones pantalla %d, %d\n", NOE_ObtenAnchoPantalla(), NOE_ObtenAltoPantalla());

		if(figuras == 0)
		{
			NOE_DibujaDisco(circulo1X, circulo1Y, circulo1Radio, color1R, color1G, color1B);
			NOE_DibujaDisco(circulo2X, circulo2Y, circulo2Radio, color2R, color2G, color2B);
		}
		else if(figuras == 1)
		{
			NOE_DibujaCaja(rectangulo1X, rectangulo1Y, rectangulo1Ancho, rectangulo1Alto,color1R, color1G, color1B);
			NOE_DibujaCaja(rectangulo2X, rectangulo2Y, rectangulo2Ancho, rectangulo2Alto,color2R, color2G, color2B);
		}
		else if(figuras == 2)
		{
			NOE_DibujaCaja(rectangulo1X, rectangulo1Y, rectangulo1Ancho, rectangulo1Alto, color1R, color1G, color1B);
			NOE_DibujaDisco(circulo2X, circulo2Y, circulo2Radio, color2R, color2G, color2B);
		}
		else if(figuras == 3)
		{	
			NOE_DibujaForma(0, forma1X, forma1Y, forma1Ancho, forma1Alto, color1R, color1G, color1B);
			NOE_DibujaForma(0, forma2X, forma2Y, forma2Ancho, forma2Alto, color2R, color2G, color2B);
		}
		
		NOE_DibujaTexto("BUFFER DE ENTRADA", 0, 0, 24, 32, 0);		
		NOE_DibujaTexto(NOE_ObtenEntrada(), 0, 32, 24, 32, 0);		
		NOE_MuestraPantalla();
		

		ciclo ++;
		
	}
	
	NOE_ParaMusica();
	
	NOE_Finaliza();

    return 0;
}