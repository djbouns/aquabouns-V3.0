#ifndef PinUtilisateur_h
#define PinUtilisateur_h

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Librairies $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <arduino.h>

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// entrées/sorties analogique
const uint8_t hardResetWIFI = A5; // reset du wifi
const uint8_t hardResetGSM = A6; // reset du gsm
const uint8_t pinInCoupureCourant = A7; // entree presence tension d'alimentation
const uint8_t pinInBatterie = A8; // entree niveau batterie
const uint8_t pinOutRelaisChauffage = A10; // relais coupure chauffage // ouvert = ON
const uint8_t pinOutRelaisEcumeur = A11; // relais ecumeur // ouvert = ON
const uint8_t pinOutRelaisRemontee = A13; // relais coupure remontee // ouvert = ON

//entrées/sorties digital
const uint8_t pinOutBrassage[3] {2, 3, 4}; // sortie Pwmpompe Brassage 1
const uint8_t pinOUTOscillo1 = 5; // sortie PWM ocsillateur Brassage 1
const uint8_t pinOUTOscillo2 = 6; // sortie PWM ocsillateur Brassage 2
const uint8_t pinOUTOscillo3 = 7; // sortie PWM ocsillateur Brassage 3
const uint8_t pinOutRelaisVentilateurRampe = 8; // relais ventilateur rampe
const uint8_t pinOutEclairage[4] {11, 12, 9, 10}; // sortie PWM eclairage Couleur1, color2, Couleur1, color2
#define pinInSondeDs18b20 13 // entree sonde temperature aquarium + rampe
#define nextion Serial1  // pin18 = tx1 + pin19 = rx1 >>> attention !!! la definition du port serie est a declarer dans la biblioteque NEXTION, fichier "NexConfig.h" ligne 37 !!!  la modification dans le code arduino est sans effet !!!
#define gsm Serial2 // pin18 = tx16 + pin17 = rx2
#define d1mini Serial3 // pin14 = tx3 + pin15 = rx3 
//20 sda > ds3231 + 3x MODUL ATLAS
//21 scl > ds3231 + 3x MODUL ATLAS
const uint8_t pinInFlotteurGodetEcumeur = 27; // entree flotteur godet ecumeur (ferme = ON)
const uint8_t pinOutRelaisRefuge = 28; // relais coupure refuge // ouvert = off
const uint8_t pinInFlotteurReserveBas = 29; // entree flotteur niveau bas reserve d'eau (ferme si niveau bas reserve d'eau osmose)
const uint8_t pinOutRelaisRemplissageReserve = 30; // relais ou electrovanne pour remplissage reserve
const uint8_t pinInFlotteurReserveHaut = 31; // entree flotteur disponible, non utilisé pour le momment
const uint8_t pinOutRAC = 32; // relais ou electrovanne pour RAC
const uint8_t pinInFlotteurOsmolation = 33; // entree flotteur osmolation (ferme = ON)
const uint8_t pinInFlotteurNiveauEcumeur = 35; // entree flotteur niveau ecumeur (ferme = ON)
const uint8_t pinOutRelaisDistributeurNouriture = 37; // relais distributeur nouriture
const uint8_t pinInFlotteurSecurite = 39;// entree flotteur securité decante (ouvert si niveau haut decante)
const uint8_t pinOutBatterie = 41; // relais distributeur nouriture
const uint8_t pinOutRelaisVentilateurBac = 47; // relais ventilateur bac
const uint8_t pinOutbuzzer = 48; // sortie buzzer 48
const uint8_t pinOutRelaisOsmolation = 49; // relais pompe osmolation
//50 miso > carte sd
//51 mosi > carte sd
//52 sck > carte sd
#define SD_CS_PIN // pin53  > carte sd

#endif
