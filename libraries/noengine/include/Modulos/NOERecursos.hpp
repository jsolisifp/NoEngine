#ifndef __NOE_RECURSOS__
#define __NOE_RECURSOS__

namespace NOERecursos
{
	typedef sfTexture NOETextura;
	typedef sfSoundBuffer NOEBufferSonido;
	typedef sfMusic NOEMusica;
	typedef NOETextura** NOEFuente;
	typedef FILE NOERecurso;

	enum NOEGrupoTexturas
	{
		NOE_GRUPOTEXTURAS_SPRITES = 0,
		NOE_GRUPOTEXTURAS_TIPOSTILE = 1,
		NOE_GRUPOTEXTURAS_DECORADOS = 2,
		NOE_GRUPOTEXTURAS_TOTAL = 3
		
	};
    
    enum NOETipoRecurso
    {
        NOE_TIPORECURSO_TEXTO,
        NOE_TIPORECURSO_BINARIO
    };
    
    enum NOETipoAccesoRecurso
    {
        NOE_TIPOACCESORECURSO_ESCRIBIR,
        NOE_TIPOACCESORECURSO_LEER
    };    
	
	struct NOEDatosForma
	{
		int* puntos;
		int numPuntos;
	};
    
    struct NOEDataset
    {
        NOE_Byte* bytes;
        int tamanyo;
    };
    
    const int noeRecursosNumGlifosFuente = 37;

	const char noeRecursosRutaBase[] = "Recursos/";
	const char noeRecursosRutaTiposTile[] = "Tiles/";
	const char noeRecursosRutaMusicas[] = "Musicas/";
	const char noeRecursosRutaSonidos[] = "Sonidos/";
	const char noeRecursosRutaSprites[] = "Sprites/";
	const char noeRecursosRutaDecorados[] = "Decorados/";
	const char noeRecursosRutaFuentes[] = "Fuentes/";
	const char noeRecursosRutaCursores[] = "Cursores/";
	const char noeRecursosRutaFormas[] = "Formas/";
	const char noeRecursosRutaDatasets[] = "Datasets/";
	const int noeRecursosMaxRuta = 200;
	
	const int noeRecursosMaxLinea = 200;

	void IniciaRecursos();

	NOERecurso* AbreRecurso(const char nombre[], NOETipoRecurso tipo, NOETipoAccesoRecurso acceso);
    
    int ObtenTamanyoRecurso(NOERecurso* recurso);
	
    // Recursos tipo texto

    bool LeePropiedad(NOERecurso* recurso, char nombre[], char valor[]);
	int ObtenEntero(const char valor[]);
	int ObtenLongitudVector(const char valor[]);
	void ObtenVector(const char valor[], int vector[]);
    
    // Recursos tipo binario

    int LeeBytes(NOERecurso* recurso, int offset, NOE_Byte bytes[], int numBytes);
    int EscribeBytes(NOERecurso* recurso, int offset, NOE_Byte bytes[], int numBytes);
    int ObtenEntero(int offset, const NOE_Byte bytes[]);
    void PonEntero(int valor, int offset, NOE_Byte bytes[]);       
    void ObtenTexto(int offset, const NOE_Byte bytes[], char texto[]);    
    void PonTexto(const char texto[], int offset, NOE_Byte bytes[]);
    
	void CierraRecurso(NOERecurso* recurso);
    
	NOETextura* ObtenTextura(NOEGrupoTexturas grupo, int indice);
	NOEBufferSonido* ObtenBufferSonido(int indice);
	NOEMusica* ObtenMusica(int indice);
	NOEFuente ObtenFuente(int indice);
	NOEDatosForma* ObtenDatosForma(int indice);
    NOEDataset* ObtenDataset(int indice);
    void GuardaDataset(int indice);
    
}

#endif