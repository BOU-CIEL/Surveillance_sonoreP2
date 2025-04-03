#include "arduinoFFT.h"
#include <driver/i2s.h>

#define SAMPLES 512                 // Nombre d'échantillons 
#define SAMPLING_FREQUENCY 44100    // Fréquence d'échantillonnage 
#define DMA_BUF_LEN 512             // Taille du buffer DMA : 512 échantillons 
#define DMA_BUF_COUNT 8             // Nombre de buffers DMA : 8 


#include <cstdint>

/**
 * @class CSon
 * @brief Classe représentant un système de gestion du son avec un buffer DMA pour l'acquisition et le traitement des échantillons.
 * 
 * Cette classe gère un tableau d'échantillons provenant d'un convertisseur numérique-analogique (DAC) via l'I2S.
 * Elle permet de manipuler les échantillons audio capturés, ainsi que la gestion des buffers DMA pour les transferts.
 */
class CSon {
public:
    float niveauSonoreMoyen;
    float niveauSonoreCrete;
    float vReal[SAMPLES];
    float vImag[SAMPLES];

    i2s_config_t i2sConfig;
    i2s_pin_config_t pinConfig;
    ArduinoFFT<double> FFT;

    CSon();
    /**
     * @brief Fonction pour démarrer l'acquisition des échantillons via I2S.
     * 
     * Cette méthode initialise le périphérique I2S, configure la fréquence d'échantillonnage et démarre la collecte des données audio.
     * 
     * @return true si l'acquisition a démarré avec succès, false sinon.
     */

    esp_err_t Setup();
    /**
     * @brief Initialisation des buffers DMA.
     * 
     * Cette méthode configure les buffers DMA pour le transfert des échantillons.
     */
    esp_err_t SamplesDmaAcquisition();

private:
    i2s_config_t i2sConfig;
    i2s_pin_config_t pinConfig;
    ArduinoFFT<double> FFT; 
};