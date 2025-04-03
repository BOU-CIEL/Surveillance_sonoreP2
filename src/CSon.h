#include "arduinoFFT.h"
#include <driver/i2s.h>

#define SAMPLES 512                 // Nombre d'échantillons 
#define SAMPLING_FREQUENCY 44100    // Fréquence d'échantillonnage 
#define DMA_BUF_LEN 512             // Taille du buffer DMA : 512 échantillons 
#define DMA_BUF_COUNT 8             // Nombre de buffers DMA : 8 

#ifndef CSon_H
#define CSon_H

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
    /**
     * @brief Constructeur de la classe CSon
     * 
     * Initialise les paramètres de l'échantillonnage et les buffers DMA.
     */
    CSon();

    /**
     * @brief Destructeur de la classe CSon
     * 
     * Libère les ressources utilisées par l'objet CSon.
     */
    ~CSon();

    /**
     * @brief Fonction pour démarrer l'acquisition des échantillons via I2S.
     * 
     * Cette méthode initialise le périphérique I2S, configure la fréquence d'échantillonnage et démarre la collecte des données audio.
     * 
     * @return true si l'acquisition a démarré avec succès, false sinon.
     */
    bool startAcquisition();

    /**
     * @brief Fonction pour arrêter l'acquisition des échantillons via I2S.
     * 
     * Cette méthode arrête l'acquisition des données audio et désactive le périphérique I2S.
     * 
     * @return true si l'acquisition a été arrêtée avec succès, false sinon.
     */
    bool stopAcquisition();

    /**
     * @brief Fonction pour obtenir un échantillon spécifique.
     * 
     * Cette méthode permet d'accéder à un échantillon spécifique dans le tableau `i2sData`.
     * 
     * @param index L'indice de l'échantillon souhaité (de 0 à SAMPLES-1).
     * @return L'échantillon correspondant à l'indice demandé.
     */
    int32_t getSample(uint16_t index) const;

    /**
     * @brief Fonction pour accéder à tous les échantillons.
     * 
     * Cette méthode retourne l'intégralité des échantillons collectés dans un tableau de 32 bits.
     * 
     * @return Un tableau contenant tous les échantillons collectés.
     */
    const int32_t* getSamples() const;

    /**
     * @brief Fonction pour vider les échantillons collectés dans le buffer.
     * 
     * Cette méthode réinitialise le tableau `i2sData` à zéro, permettant de récupérer de nouveaux échantillons sans interférence avec les précédents.
     */
    void clearSamples();

private:
    static const uint16_t SAMPLES = 512;              ///< Nombre d'échantillons à collecter
    static const uint32_t SAMPLING_FREQUENCY = 44100; ///< Fréquence d'échantillonnage (en Hz)
    static const uint16_t DMA_BUF_LEN = 512;          ///< Taille du buffer DMA (en nombre d'échantillons)
    static const uint16_t DMA_BUF_COUNT = 8;          ///< Nombre de buffers DMA utilisés

    int32_t i2sData[SAMPLES]; ///< Tableau d'échantillons audio sur 32 bits de taille fixe

    /**
     * @brief Initialisation des buffers DMA.
     * 
     * Cette méthode configure les buffers DMA pour le transfert des échantillons.
     */
    void initDMA();

    /**
     * @brief Traitement des échantillons dans le buffer.
     * 
     * Cette méthode est appelée pour effectuer le traitement nécessaire sur les échantillons collectés.
     */
    void processSamples();
};

#endif // CSon_H
