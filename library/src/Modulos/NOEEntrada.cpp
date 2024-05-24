#include "Modulos/NOEComun.hpp"


namespace NOEEntrada
{
	
	const int noeEntradaPrimeraTecla = NOE_TECLA_ESPACIO;
	const int noeEntradaUltimaTecla = NOE_TECLA_0;

	char *entradaBuffer;
	int entradaBufferOcupadas;


	char entradaCaracteresTecla[] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
									'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
									'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3',
									'4', '5', '6', '7', '8', '9', '0' };


	void IniciaEntrada()
	{
		Configuracion &c = ObtenConfiguracion();
		
		entradaBuffer = new char[c.longitudLineaEntrada + 1];
		
		entradaBuffer[0] = '\0';
		entradaBufferOcupadas = 0;
		
	}

	void ActualizaEntrada()
	{
		Configuracion &c = ObtenConfiguracion();
		
		for(int i = noeEntradaPrimeraTecla; i <= noeEntradaUltimaTecla; i ++)
		{
			if(ObtenTeclaAbajo((NOE_Tecla)i))
			{
				if(entradaBufferOcupadas < c.longitudLineaEntrada)
				{
					entradaBuffer[entradaBufferOcupadas] =  entradaCaracteresTecla[i];
					entradaBuffer[entradaBufferOcupadas + 1] = '\0';
					entradaBufferOcupadas ++;
				}
			}
		}
		
		if(ObtenTeclaAbajo(NOE_TECLA_BORRA_ATRAS))
		{
			if(entradaBufferOcupadas > 0)
			{
				entradaBufferOcupadas --;
				entradaBuffer[entradaBufferOcupadas] = '\0';			
			}
		}	
	}

	const char *ObtenEntrada()
	{
		return entradaBuffer;
	}

	void LimpiaEntrada()
	{
		entradaBufferOcupadas = 0;
		entradaBuffer[0] = '\0';
	}

	void FinalizaEntrada()
	{
		delete entradaBuffer;
		entradaBuffer = NULL;
		
	}

}