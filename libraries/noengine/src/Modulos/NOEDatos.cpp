#include "Modulos/NOEComun.hpp"

namespace NOEDatos
{    
    
	void IniciaDatos()
	{
        // Nada por ahora
	}

	void FinalizaDatos()
    {
		Configuracion & c = ObtenConfiguracion();
        
        for(int i = 0; i < c.numDatasets; i++)
        {
            GuardaDataset(i);
        }
        
    }
    
    void _PonTamanyoDataset(int indiceDataset, int tamanyo)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        int nuevoTamanyo;
        NOE_Byte* nuevosDatos;
        
        if(tamanyo == 0)
        {
            nuevosDatos = 0;
            nuevoTamanyo = 0;
        }
        else
        {
            int numBloques = ObtenTamanyoBloques(tamanyo);        
            nuevoTamanyo = numBloques * noeTamanyoBloque;
            
            printf("Estableciendo el tamanyo del dataset %d en %d\n", indiceDataset, nuevoTamanyo);

            nuevosDatos = new NOE_Byte[nuevoTamanyo];
                        
            for(int i = 0; i < nuevoTamanyo; i ++)
            {
                if(i < dataset->tamanyo) { nuevosDatos[i] = dataset->bytes[i]; }
                else { nuevosDatos[i] = 0; }
            }

        }
        
        if(dataset->bytes != 0) { delete dataset->bytes; }

        dataset->bytes = nuevosDatos;
        dataset->tamanyo = nuevoTamanyo;
        
    }
    
   
    void AseguraTamanyoDataset(int indiceDataset, int tamanyo)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        if(tamanyo > dataset->tamanyo) { _PonTamanyoDataset(indiceDataset, tamanyo); }
        
    }
    
    void PonTamanyoDataset(int indiceDataset, int tamanyo)
    {
        _PonTamanyoDataset(indiceDataset, tamanyo);
    }
    
    void LimpiaDataset(int indiceDataset, int desde, int hasta)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        int offset1 = desde;
        int offset2 = hasta < 0 ? dataset->tamanyo - 1: hasta;
        
        if(dataset->tamanyo > 0)
        {            
            memset(&dataset->bytes[offset1], 0, offset2 - offset1 + 1);
        }
        
    }
    
    void CopiaDataset(int _indiceDatasetOrigen, int _indiceDatasetDestino, int origenDesde, int origenHasta, int destinoDesde)
    {
        int indiceDatasetOrigen = _indiceDatasetOrigen;
        int indiceDatasetDestino = _indiceDatasetDestino < 0 ? _indiceDatasetOrigen : _indiceDatasetDestino;
        
        NOEDataset* datasetOrigen = ObtenDataset(indiceDatasetOrigen);
        NOEDataset* datasetDestino = ObtenDataset(indiceDatasetDestino);

        int tamanyo = origenHasta - origenDesde + 1;
        
        AseguraTamanyoDataset(indiceDatasetDestino, destinoDesde + tamanyo);
        
        memmove(&datasetDestino->bytes[destinoDesde], &datasetOrigen->bytes[origenDesde], tamanyo);
        
        printf("Movidos %d bytes de %d a %d\n", tamanyo, origenDesde, destinoDesde);
        
    }
    
    int ObtenTamanyoBloques(int tamanyoBytes)
    {
        int resultado = tamanyoBytes / noeTamanyoBloque;
        
        if(tamanyoBytes % noeTamanyoBloque > 0) { resultado = resultado + 1; }
        
        return resultado;
        
    }

    void ObtenDatosDataset(int indiceDataset, int desde, int hasta, void* _bytes)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        NOE_Byte* bytes = (NOE_Byte *)_bytes;
                
        memcpy(bytes, &dataset->bytes[desde], hasta - desde + 1);
        
    }
    
    void PonDatosDataset(int indiceDataset, int desde, int hasta, const void* _bytes)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);

        const NOE_Byte* bytes = (const NOE_Byte *)_bytes;
        
        
        AseguraTamanyoDataset(indiceDataset, hasta + 1);        
        
        memcpy(&dataset->bytes[desde], bytes, hasta - desde + 1);
    }
    
    int ObtenEnteroDataset(int indiceDataset, int offset)
    {
        int resultado;
        
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        // if(offset + sizeof(int) > dataset->tamanyo)
        // {
        //     printf("NOEDatos::ERROR => Intentando obtener un entero mas alla del final del dataset\n");
        //     return 0;
        // }    
        
        memcpy(&resultado, &dataset->bytes[offset], sizeof(int));
        
        return resultado;
    }
    
    void PonEnteroDataset(int indiceDataset, int offset, int valor)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        AseguraTamanyoDataset(indiceDataset, offset + sizeof(int));

        memcpy(&dataset->bytes[offset], &valor, sizeof(int));
    }
    
    void ObtenTextoDataset(int indiceDataset, int offset, char texto[])
    {
        int indice = offset;

        NOEDataset* dataset = ObtenDataset(indiceDataset);

        //if(indice + 1 > dataset->tamanyo)
        //{ printf("NOEDatos::ERROR => Intentando obtener un texto mas alla del final del dataset\n"); texto[0] = 0; return; }    
        
        while(dataset->bytes[indice] != 0)
        {
            texto[indice - offset] = (char)dataset->bytes[indice];
            indice ++;
            
            //if(indice + 1 > dataset->tamanyo)
            //{ printf("NOEDatos::ERROR => Intentando obtener un texto mas alla del final del dataset\n"); texto[0] = 0; return; }    
            
        }
        
        texto[indice - offset] = 0;
        
        
    }
    
    void PonTextoDataset(int indiceDataset, int offset, const char texto[])
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        int indice = 0;
        
        AseguraTamanyoDataset(indiceDataset, offset + strlen(texto) + 1);
        
        while(texto[indice] != 0) { dataset->bytes[offset + indice] = (NOE_Byte)texto[indice]; indice ++; }
        
        dataset->bytes[offset + indice] = 0;        
    }

    void GuardaDatasetAhora(int dataset)
    {
        GuardaDataset(dataset);
    }
    
    int ObtenNumDatasets()
    {
        Configuracion & c = ObtenConfiguracion();
        
        return c.numDatasets;

    }
    
    int ObtenTamanyoDataset(int indiceDataset)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
                
        return dataset->tamanyo;
    }
    
    void ImprimeDataset(int indiceDataset, bool modoTexto)
    {
        NOEDataset* dataset = ObtenDataset(indiceDataset);
        
        if(dataset->tamanyo == 0) { return; }
        
        printf("      ");
        for(int i = 0; i < 16; i++)
        {
            printf("%02X ", i);
        }
 
        printf("\n");
        printf("      ");
        for(int i = 0; i < 16; i++)
        {
            printf(".. ", i);
        }
        
        for(int i = 0; i < dataset->tamanyo; i ++)
        {
            if(i % 16 == 0) { printf("\n%04X: ", i); }

            NOE_Byte dato = dataset->bytes[i];

            if(modoTexto)
            {   
                if(dato >= 'a' && dato <= 'z' || dato >= 'A' && dato <= 'Z' ||
                   dato == ' ' || dato == '_' || dato >= '0' && dato <= '9') { printf(" %c ", (char)dato); }
                else { printf(" . "); }
            }
            else { printf("%02X ", (int)dato); }
            
        }        
        printf("\n");        
    }    


}