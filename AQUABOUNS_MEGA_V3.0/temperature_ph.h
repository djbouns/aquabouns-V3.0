#ifndef  _temperature_h_
#define _temperature_h_

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Librairies $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <arduino.h>

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Declarations des fonctions dans le .cpp $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void temperature();
void chauffage();
void ventilation();
void messageAlerteTemperature();
void declenchementAlarme();
void ph();
void attenteMesurePH();
void alertePhBac();
void redox();
void attenteMesureRedox();
void salinite();
void attenteMesureSalinite();
void etalonnageEnCours();
void etalonnageDesSondes(uint8_t sonde, uint16_t valeur, float valeurfloat);
void saliniteModeMesure();
void saliniteModeEtalonnage();
void initialisationEC(char*commandeIni);
#endif
