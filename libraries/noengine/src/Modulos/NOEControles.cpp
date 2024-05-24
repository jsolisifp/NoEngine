#include "Modulos/NOEComun.hpp"

namespace NOEControles
{
	// Teclado

	const int noeTecladoNumTeclas = NOE_TECLA_BORRA_ATRAS + 1;

	sfKeyCode tecladoCodigosSFTecla[] =  { sfKeySpace, sfKeyA, sfKeyB, sfKeyC, sfKeyD, sfKeyE, sfKeyF, sfKeyG, sfKeyH, sfKeyI,
											sfKeyJ, sfKeyK, sfKeyL, sfKeyM, sfKeyN, sfKeyO, sfKeyP, sfKeyQ, sfKeyR, sfKeyS,
											sfKeyT, sfKeyU, sfKeyV, sfKeyW, sfKeyX, sfKeyY, sfKeyZ, sfKeyNum1, sfKeyNum2, sfKeyNum3,
											sfKeyNum4, sfKeyNum5, sfKeyNum6, sfKeyNum7, sfKeyNum8, sfKeyNum9, sfKeyNum0, sfKeyEscape,
											sfKeyLeft,  sfKeyRight, sfKeyUp, sfKeyDown, sfKeyReturn, sfKeyBack };

	bool tecladoTeclasPulsadas[] = { false, false, false, false, false, false, false, false, false, false,
									false, false, false, false, false, false, false, false, false, false,
									false, false, false, false, false, false, false, false, false, false,
									false, false, false, false, false, false, false, false,  false,  false,
									false, false, false, false };

	bool tecladoTeclasPulsadasAnteriores[] = { false, false, false, false, false, false, false, false, false, false,
									false, false, false, false, false, false, false, false, false, false,
									false, false, false, false, false, false, false, false, false, false,
									false, false, false, false, false, false, false, false, false, false,
									false, false, false, false };
									
	// Raton

	bool ratonBotonPrincipalPulsado = false;
	bool ratonBotonSecundarioPulsado = false;
	bool ratonBotonCentralPulsado = false;

	bool ratonBotonPrincipalPulsadoAnterior = false;
	bool ratonBotonSecundarioPulsadoAnterior = false;
	bool ratonBotonCentralPulsadoAnterior = false;

	int ratonX;
	int ratonY;
	int ratonXAnterior;
	int ratonYAnterior;

	void IniciaControles()
	{
		// Inicia teclado
		
		for(int i = 0; i < noeTecladoNumTeclas; i ++)
		{
			tecladoTeclasPulsadas[i] = false;
			tecladoTeclasPulsadasAnteriores[i] = sfKeyboard_isKeyPressed(tecladoCodigosSFTecla[i]);
		}
		
		// Inicia raton
		
		sfRenderWindow* ventana = ObtenVentana();
		
		ratonBotonPrincipalPulsado = false;
		ratonBotonSecundarioPulsado = false;
		ratonBotonCentralPulsado = false;
		ratonBotonPrincipalPulsadoAnterior = sfMouse_isButtonPressed(sfMouseLeft);	
		ratonBotonSecundarioPulsadoAnterior = sfMouse_isButtonPressed(sfMouseRight);
		ratonBotonCentralPulsadoAnterior = sfMouse_isButtonPressed(sfMouseMiddle);
		
		sfVector2i posicionRaton = sfMouse_getPosition((const sfWindow*)ventana);
		ratonX = posicionRaton.x;
		ratonY = posicionRaton.y;
		ratonXAnterior = posicionRaton.x;
		ratonYAnterior = posicionRaton.y;	
	}

	void ActualizaControles()
	{
		
		// Actualiza teclado
		
		for(int i = 0; i < noeTecladoNumTeclas; i ++)
		{
			tecladoTeclasPulsadasAnteriores[i] = tecladoTeclasPulsadas[i];
			tecladoTeclasPulsadas[i] = sfKeyboard_isKeyPressed(tecladoCodigosSFTecla[i]);
			
		}

		// Actualiza raton
		
		sfRenderWindow* ventana = ObtenVentana();
		
		ratonBotonPrincipalPulsadoAnterior = ratonBotonPrincipalPulsado;
		ratonBotonSecundarioPulsadoAnterior = ratonBotonSecundarioPulsado;
		ratonBotonCentralPulsadoAnterior = ratonBotonCentralPulsado;
		ratonBotonPrincipalPulsado = sfMouse_isButtonPressed(sfMouseLeft);
		ratonBotonSecundarioPulsado = sfMouse_isButtonPressed(sfMouseRight);
		ratonBotonCentralPulsado = sfMouse_isButtonPressed(sfMouseMiddle);
		
		sfVector2i posicionRaton = sfMouse_getPosition((const sfWindow*)ventana);
		
		ratonXAnterior = ratonX;
		ratonYAnterior = ratonY;
		ratonX = posicionRaton.x;
		ratonY = posicionRaton.y;	
		

	}

	bool ObtenTeclaPulsada(NOE_Tecla tecla)
	{
		return tecladoTeclasPulsadas[tecla];
	}

	bool ObtenTeclaAbajo(NOE_Tecla tecla)
	{	
		return tecladoTeclasPulsadas[tecla] && !tecladoTeclasPulsadasAnteriores[tecla];
	}

	bool ObtenTeclaArriba(NOE_Tecla tecla)
	{
		return !tecladoTeclasPulsadas[tecla] && tecladoTeclasPulsadasAnteriores[tecla];
	}

	int ObtenPosicionRatonX()
	{
		return ratonX;
	}

	int ObtenPosicionRatonY()
	{
		return ratonY;
	}

	int ObtenDesplazamientoRatonX()
	{
		return ratonX - ratonXAnterior;
	}

	int ObtenDesplazamientoRatonY()
	{
		return ratonY - ratonYAnterior;
	}

	bool ObtenBotonRaton(NOE_BotonRaton boton)
	{
		if(boton == NOE_BOTONRATON_PRINCIPAL) { return ratonBotonPrincipalPulsado; }
		else if(boton == NOE_BOTONRATON_SECUNDARIO) { return ratonBotonSecundarioPulsado; }
		else // boton == NOE_BOTONRATON_CENTRAL
		{ return ratonBotonCentralPulsado; }
		
	}

	bool ObtenBotonRatonAbajo(NOE_BotonRaton boton)
	{
		if(boton == NOE_BOTONRATON_PRINCIPAL)
		{ return ratonBotonPrincipalPulsado && !ratonBotonPrincipalPulsadoAnterior; }
		else if(boton == NOE_BOTONRATON_SECUNDARIO)
		{ return ratonBotonSecundarioPulsado && !ratonBotonSecundarioPulsadoAnterior; }
		else // boton == NOE_BOTONRATON_CENTRAL
		{ return ratonBotonCentralPulsado && !ratonBotonCentralPulsadoAnterior; }
	}

	bool ObtenBotonRatonArriba(NOE_BotonRaton boton)
	{
		if(boton == NOE_BOTONRATON_PRINCIPAL)
		{ return ratonBotonPrincipalPulsadoAnterior && !ratonBotonPrincipalPulsado; }
		else if(boton == NOE_BOTONRATON_SECUNDARIO)
		{ return ratonBotonSecundarioPulsadoAnterior && !ratonBotonSecundarioPulsado; }
		else // boton == BOTONRATON_CENTRAL
		{ return ratonBotonCentralPulsadoAnterior && !ratonBotonCentralPulsado; }
		
	}
}