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