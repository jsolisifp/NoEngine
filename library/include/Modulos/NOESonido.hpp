#ifndef __NOE_SONIDO__
#define __NOE_SONIDO__

namespace NOESonido
{
	void IniciaSonido();
	void FinalizaSonido();
	

	// Library functions
	
	int ReproduceSonido(int sonido, int volumen, int pitch);
	void ParaSonido(int canal);
	void ReproduceMusica(int musica, int volumen, int pitch);
	void ParaMusica();

}


#endif