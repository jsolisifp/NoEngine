#include "Modulos/NOEComun.hpp"

namespace NOESistema
{
	sfClock*		timer;
	int tiempoDesdeActualizacion;

	void IniciaSistema()
	{
		// Inicia reloj
		
		timer = sfClock_create();
		tiempoDesdeActualizacion = 0;
		
		// Inicia generador de números aleatorios
		
		srand(getpid());
		
	}


	void ActualizaSistema()
	{
		sfTime t = sfClock_getElapsedTime(timer);
		tiempoDesdeActualizacion = sfTime_asMilliseconds(t);
		
		if(tiempoDesdeActualizacion > ObtenConfiguracion().tiempoMinimoActualizacion)
		{
			tiempoDesdeActualizacion = sfTime_asMilliseconds(t);
			sfClock_restart(timer);
		}
		else
		{
			tiempoDesdeActualizacion = 0;
		}
		
	}

	int ObtenTiempoDesdeActualizacion()
	{
		return tiempoDesdeActualizacion;
	}

	int ObtenNumeroAleatorio(int minimo, int maximo)
	{
		return minimo + rand() % (maximo - minimo + 1);
	}


	void FinalizaSistema()
	{
		sfClock_destroy(timer);	
	}

}