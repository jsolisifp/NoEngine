#ifndef __NOE_DATOS__
#define __NOE_DATOS__

namespace NOEDatos
{        
    const int noeTamanyoBloque = 1024;
    
	void IniciaDatos();
	void FinalizaDatos();
    
    int ObtenTamanyoBloques(int tamanyoBytes);    
    
	// Library functions
    
    int ObtenNumDatasets();
    int ObtenTamanyoDataset(int dataset);
    void PonTamanyoDataset(int dataset, int tamanyo);
    void LimpiaDataset(int dataset, int desde, int hasta);
    void CopiaDataset(int indiceDatasetOrigen, int indiceDatasetDestino, int origenDesde, int origenHasta, int destinoDesde);
    void GuardaDatasetAhora(int dataset);
    void ObtenDatosDataset(int dataset, int desde, int hasta, void* bytes);
    void PonDatosDataset(int dataset, int desde, int hasta, const void* bytes);    
    int ObtenEnteroDataset(int indiceDataset, int offset);
    void PonEnteroDataset(int indiceDataset, int offset, int valor);
    void ObtenTextoDataset(int indiceDataset, int offset, char texto[]);
    void PonTextoDataset(int indiceDataset, int offset, const char texto[]);
    void ImprimeDataset(int indiceDataset, bool modoTexto);
}

#endif
