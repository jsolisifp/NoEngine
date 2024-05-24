#include "Modulos/NOEComun.hpp"

namespace NOESonido
{
	sfSound **canalesSonido;

	void IniciaSonido()
	{
		Configuracion & c = ObtenConfiguracion();
		

		canalesSonido = new sfSound*[c.numCanales];

		for(int i = 0; i < c.numCanales; i ++)
		{
			canalesSonido[i] = sfSound_create();
			sfSound_setLoop(canalesSonido[i], sfFalse);
			
		}
		
	}

	int ReproduceSonido(int sonido, int volumen, int pitch)
	{
		Configuracion &c = ObtenConfiguracion();

		bool encontrado = false;
		int canal = -1;
		int i = 0;
		
		while(i < c.numCanales && !encontrado)
		{
			if(sfSound_getStatus(canalesSonido[i]) != sfSoundStatus::sfPlaying)
			{
				encontrado = true;
				canal = i;
			}
			
			i ++;
		}
		
		if(encontrado)
		{
			NOEBufferSonido *s = ObtenBufferSonido(sonido);
			sfSound_setBuffer(canalesSonido[canal], s);
			sfSound_setVolume(canalesSonido[canal], volumen);
			sfSound_setPitch(canalesSonido[canal], (float)pitch / 100);
			sfSound_play(canalesSonido[canal]);
		}
		
		return canal;
		
	}

	void ParaSonido(int canal)
	{
		if(canal >= 0) { sfSound_stop(canalesSonido[canal]); }
	}

	void ReproduceMusica(int musica, int volumen, int pitch)
	{
		Configuracion &c = 	ObtenConfiguracion();
		for(int i = 0; i < c.numMusicas; i ++)
		{	NOEMusica *m = ObtenMusica(i);
			sfMusic_stop(m);	
		}
		
		NOEMusica *m = ObtenMusica(musica);
		sfMusic_setLoop(m, true);
		sfMusic_setVolume(m, volumen);
		sfMusic_setPitch(m, (float)pitch / 100);
		
		sfMusic_play(m);
	}

	void ParaMusica()
	{
		Configuracion &c = ObtenConfiguracion();	
		for(int i = 0; i < c.numMusicas; i ++)
		{	NOEMusica *m = ObtenMusica(i);
			sfMusic_stop(m);	
		}		
	}

	void FinalizaSonido()
	{
		Configuracion & c = ObtenConfiguracion();

		for(int i = 0; i < c.numCanales; i ++) { sfSound_destroy(canalesSonido[i]); }
		delete canalesSonido;
		

		
	}

}

