#include "NoEntiendo.hpp"
#include <stdio.h>

int main()
{
	NOE_Inicia();
	
	NOE_ReproduceMusica(0, 100, 100);
	
	while(!NOE_ObtenTeclaPulsada(NOE_TECLA_ESCAPE))
	{
		NOE_Actualiza();
		
		NOE_LimpiaPantalla(0, 0, 0);
		NOE_DibujaDecorado(0);
		NOE_DibujaSprite(0, 100, 100, 64, 64, 0, 0);
		
		NOE_MuestraPantalla();
	}

	NOE_ParaMusica();
	
	NOE_Finaliza();

    return 0;
	
}