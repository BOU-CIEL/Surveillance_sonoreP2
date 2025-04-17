#include "CSon.h"
#include <iotream>
#include <string.h>                         
    /**
     * @brief Constructeur de la classe CSon
     * 
     * Initialise les paramètres de l'échantillonnage et les buffers DMA.
     */
CSon::CSon()
    : niveauSonoreMoyen(0.0f),
      niveauSonoreCrete(0.0f),
      bytesRead(0)

{
    //Initialisation des tableaux
    memset(vReal, 0, sizeof(vReal));
    memset(vImag, 0, sizeof(vImag));
    memset(i2sData, 0, sizeof(i2sData));


    this->pinCconfig = { 
    .bck_io_num = 14,   // Numéro de broche pour le signal de l'horloge de la base (Bit Clock)
    .ws_io_num = 13,    // Numéro de broche pour le signal de sélection de mot (Word Select)
    .data_out_num = I2S_PIN_NO_CHANGE, // Pas de data out 
    .data_in_num = 12   // Numéro de broche pour l'entrée des données (Data In)
    }; 
    

    this->i2sConfig = { 
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX), // Mode maître, réception uniquement
        .sample_rate = SAMPLING_FREQUENCY, // Fréquence d'échantillonnage
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // Bits par échantillon 
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,  // Format de canal, uniquement le canal droit 
        .communication_format =  i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S), 
        .intr_alloc_flags = 0,//ESP_INTR_FLAG_LEVEL1,  // Niveau d'interruption 
        .dma_buf_count = 8,  // Nombre de buffers DMA 
        .dma_buf_len = DMA_BUF_LEN,  // Taille du buffer DMA 
        .use_apll = false, // Ne pas utiliser l'APLL (Audio PLL)
        .tx_desc_auto_clear = false, // Ne pas effacer automatiquement le descripteur TX
        .fixed_mclk = 0 // Pas de fréquence MCLK fixe
    };

    this->FFT = ArduinoFFT<double>(this->vReal, this->vImag, SAMPLES, SAMPLING_FREQUENCY); 
}

CSon::Setup()
{
    int result = i2s_driver_install(I2S_NUM_0, &this->i2sConfig, 0, NULL); 
      result = i2s_set_pin(I2S_NUM_0, &this->pinCconfig); 
      result = i2s_zero_dma_buffer(I2S_NUM_0); 
      return result; 
}

esp_err_t CSon::SamplesDmaAcquisition() 
{ 
    // Nombre d'octets lus en mémoire DMA 
    size_t bytesRead; 
    // Capture des données audio 
    result = i2s_read(I2S_NUM_0, &this->i2sData, sizeof(this->i2sData), &bytesRead, portMAX_DELAY); 
   
    if (result == ESP_OK) 
    { 
        int16_t samplesRead = bytesRead / 4; // Conversion des octets lus en nombre d'échantillons (chaque échantillon est sur 4 octets : 2 canaux de 16 bits).
        if (samplesRead > 0)  
        { 
            float mean = 0; 
            for (int16_t i = 0; i < samplesRead; ++i)  
            { 
                i2sData[i] = i2sData[i] >> 8;  // Réduction de la résolution des échantillons (passage de 16 bits à 8 bits pour simplifier le traitement).
                mean += abs(i2sData[i]); 
                if (abs(i2sData[i]) > niveauSonoreCrete) 
                    niveauSonoreCrete = abs(i2sData[i]); // Mise à jour du niveau sonore crête si un échantillon dépasse la valeur actuelle.
            } 
            this->niveauSonoreMoyen = mean / samplesRead; // Calcul du niveau sonore moyen en divisant la somme des amplitudes par le nombre d'échantillons.
        }     
    } 
    return result; 
}

    