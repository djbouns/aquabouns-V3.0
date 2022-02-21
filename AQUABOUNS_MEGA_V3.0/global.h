#ifndef global_h_
#define global_h_

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Librairies $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <Servo.h>
#include "DallasTemperature.h"
#include <OneWire.h>
#include "RTClib.h"
#include "SdFat.h"
#include "Nextion.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fichiers $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include "eeprom.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/* dans .ino */
extern uint8_t roulement1;
extern uint32_t roulementPrecedent1, roulementPrecedent2;
extern const char texteNextionDemarrage[] PROGMEM;
extern const char smsAlerteSecuriteDecante[] PROGMEM;
extern const char smsCoupureEdf[] PROGMEM;
extern const char smsDegree[] PROGMEM;
extern const char smsEdfOk[] PROGMEM;
extern const char smsGodetPlein[] PROGMEM;
extern const char smsOsmolationOk[] PROGMEM;
extern const char smsPh[] PROGMEM;
extern const char smsReserveVide[] PROGMEM;
extern const char smsSecuriteDecanteOk[] PROGMEM;
extern const char smsTemperature[] PROGMEM;
extern const char smsTropOsmolation[] PROGMEM;
extern const char smsTropRemplissage[] PROGMEM;
extern const char texteNextionAlerte[] PROGMEM;
extern const char texteNextionAnnee[] PROGMEM;
extern const char texteNextionAquaRampe[] PROGMEM;
extern const char texteNextionAucuneReponseGsm[] PROGMEM;
extern const char texteNextionAucuneReponseSD[] PROGMEM;
extern const char texteNextionAucuneReponseWifi[] PROGMEM;
extern const char texteNextionAucuneSonde[] PROGMEM;
extern const char texteNextionBIENVENUE[] PROGMEM;
extern const char texteNextionDANS[] PROGMEM;
extern const char texteNextionTemperatureBac[] PROGMEM;
extern const char texteNextionDeuxSonde[] PROGMEM;
extern const char texteNextionErreurModeRoutage[] PROGMEM;
extern const char texteNextionErreurModeTexte[] PROGMEM;
extern const char texteNextionetalonnage[] PROGMEM;
extern const char texteNextionFinAlerte[] PROGMEM;
extern const char texteAlerteActuellement[] PROGMEM;
extern const char texteNextionGodetHaut[] PROGMEM ;
extern const char texteNextionHorlogeDetectee[] PROGMEM;
extern const char texteNextionHorlogeJamaisInitialise[] PROGMEM;
extern const char texteNextionImportationDe[] PROGMEM;
extern const char texteNextionImportationEepromDefault[] PROGMEM;
extern const char texteNextionImportationEepromOK[] PROGMEM;
extern const char texteNextionInitialisationGsm[] PROGMEM;
extern const char texteNextionInitialisationGsmOK[] PROGMEM;
extern const char texteNextionInitialisationHorloge[] PROGMEM;
extern const char texteNextionInitialisationHorlogeOK[] PROGMEM;
extern const char texteNextionInitialisationSD[] PROGMEM;
extern const char texteNextionInitialisationSDOK[] PROGMEM;
extern const char texteNextionInitialisationSonde[] PROGMEM;
extern const char texteNextionInitialisationWifi[] PROGMEM;
extern const char texteNextionInitialisationWifiOK[] PROGMEM;
extern const char texteNextionLAQUABOUNS[] PROGMEM;
extern const char texteNextionModuleNonDetecte[] PROGMEM;
extern const char texteNextionNonConnecte[] PROGMEM;
extern const char texteNextionNourissage[] PROGMEM;
extern const char texteNextionOsmolationOff[] PROGMEM;
extern const char texteNextionPasDeFichier[] PROGMEM;
extern const char texteNextionPasDeReponseHorloge[] PROGMEM;
extern const char texteNextionPhBas[] PROGMEM;
extern const char texteNextionPhHaut[] PROGMEM;
extern const char texteNextionReserveVide[] PROGMEM;
extern const char texteNextionUneSonde[] PROGMEM;
extern const char texteNextionVersion[] PROGMEM;
extern const char texteSurSDDemarrage[] PROGMEM;
extern const char texteSurSDRedemarrage[] PROGMEM;
extern const char texteVide[] PROGMEM;
extern const char finDuFichier[] PROGMEM;
extern const char oec0[] PROGMEM ;
extern const char os1[] PROGMEM;

/* dans affichage.cpp */
extern boolean ChangementDEauON, NotifOn, nourissageManuelEnCours;
extern char bufferTexte[];
extern const uint8_t dix, cent, maxbufferTexte, zero, un, deux, trois, quatre, pas2Changement2Couleur;
enum pageNextion : uint8_t {menu, parametre, light, brass, horloge , wifi, tel, info, standby, demarrage, choixgraph, graph24h, graph7j, orph, sd, popup, alarme, dernierePage = 255};
extern const uint16_t mille, deuxMille;
extern uint32_t number, numero_page, recupHorraireTemporraire;
extern pageNextion pageActuelNextion;
extern NexButton onoffpomperemonte;
extern NexButton onoffalimentation1;
extern NexButton onoffalimentation2;
extern NexButton onoffpwmbrassage1;
extern NexButton onoffpwmbrassage2;
extern NexButton onoffpwmbrassage3;
extern NexButton onoffrefuge;
extern NexButton onoffpwmcouleur1;
extern NexButton onoffpwmcouleur2;
extern NexButton onoffpwmcouleur3;
extern NexButton onoffpwmcouleur4;
extern NexButton onofftempete;
extern NexButton onoffchangementdeau;
extern NexButton onoffbuzzermenu;
extern NexButton onofftempetealeatoire;
extern NexVariable variableetatboutonsmenu;
extern NexText affichageheurealimentation1;
extern NexText affichageheurealimentation2;
extern NexButton boutonalimentationmanuel;
extern NexButton boutonadressesonde;
extern NexButton boutonverstel;
extern NexButton boutonverswifi;
extern NexButton boutonversmenu;
extern NexButton boutonversinfo;
extern NexButton boutonversconfig;
extern NexButton boutonversgraph24h;
extern NexButton boutonversbrassage;
extern NexButton boutonvershorloge;
extern NexButton boutonversalarme;
extern NexButton boutonversstandby;
extern NexButton boutonversetalonnage;
extern NexButton boutonverseclairage;
extern NexButton boutonversgraph7j;
extern NexButton onoffnotif;
extern NexButton boutonenregistrerhorloge;
extern NexButton boutonenregistrerconfig;
extern NexButton boutonenregistrereclairage;
extern NexButton boutonenregistreralarme;
extern NexButton boutonenregistrerbrassage;
extern NexButton boutonenregistrertel;
extern NexButton boutonenregistrerwifi;
extern NexPicture boutoncalibrer;
extern NexButton boutonredemarrage;
extern NexButton boutonrestaureeeprom;
extern NexButton onoffbuzzeralarme;
extern NexButton onoffrac;
extern NexVariable valeuroscillo1min;
extern NexVariable valeuroscillo1max;
extern NexVariable valeuroscillo2min;
extern NexVariable valeuroscillo2max;
extern NexVariable valeuroscillo3min;
extern NexVariable valeuroscillo3max;
extern NexVariable valeurminBrassage1;
extern NexVariable valeurminBrassage2;
extern NexVariable valeurminBrassage3;
extern NexPage pagestandby;
extern NexText boutonoui;
extern NexButton boutonbas;
extern NexButton boutonhaut;
extern NexButton boutonclear;
extern NexText texte1;
extern NexText texte2;
extern NexText texte3;
extern NexText texte4;
extern NexText texte5;
extern NexText texte6;
extern NexText valeursd00;
extern NexText valeursd01;
extern NexText valeursd02;
extern NexText valeursd03;
extern NexText valeursd04;
extern const uint16_t vert, orange, rouge, batterie0;

/* autres.cpp */
extern boolean nourrissage1ON, nourrissage2ON, texteNourrissage, alarmeOnOff;
extern const uint16_t adresseMotClef, adresseDeSauvegarde;
extern const uint32_t motClef;
extern uint32_t dureeNourrissageMillis, heureNourrissage[];
enum {numeroUn, numeroDeux, numeroTrois, dernierNumero = 255}; // utiliser pour Brassage, oscillo, nourrissage ...

/* dans Brassage.cpp */
enum {Arret, Actif, Pwm, Delay, Timer, dernierMode = 255}; // utiliser pour Brassage, eclairage, pompe de remonté
extern boolean MajEcranTempeteAleatoireEnCours, nourissageEnCours, tempeteManuelEnCours, tempeteAleatoireEnCours, progressionPuissanceBrassage, MajEcranTempeteManuelEnCours;
extern uint8_t pwmBrassage[], puissanceMaxBrassage[], accalmieNocturne, puissanceTempete;
extern uint32_t dureeTempeteMillis, heureTempeteAleatoire, dureeTempeteAleatoire;

/* dans eeprom.cpp */
extern boolean demandeRestaure;

/* dans eclairage */
enum {Couleur1, Couleur2, Couleur3, Couleur4, derniereCouleur = 255};
extern uint8_t pwmEclairage[], puissanceMaxEclairage[];
extern uint16_t dureelevercoucher;
extern uint32_t leverSoleil, coucherSoleil, debutleverSoleil[], finleverSoleil[], debutcoucherSoleil[], fincoucherSoleil[];

/* dans flotteurs.cpp */
extern boolean smsAlerteOsmolation, smsAlerteGodet, smsAlerteSecurite, smsAlerteReserve, smsAlerteRemplissage;

/* dans gsm.cpp */
extern boolean gsmOn, envoyerSMS(const char * noDeTel, const char * messageAEnvoyer);
extern boolean gsmPrintlnAndWaitATCommand(const char * command, const char * endMarker, uint32_t duration, boolean verbose);
extern boolean delaisEntre2SMSOK ;
extern char numero2tel[], messageGsm[], buffer[], nomOperateur[], codePin[];
extern const char * ATString, * OKShortString, * OKLongString;
extern const uint8_t maxnumero2tel, maxMessageSize, maxPinSize, maxNomOperateur;
extern uint8_t signalGSM ;
extern const uint16_t delaisEntre2SMS;
extern uint32_t compteurEntre2SMS ;

/* dans horloge.cpp */
extern char textime[], minuteEnTexteOut[], minuteEnTexteIn[];
extern const uint8_t maxminuteEnTexte;
extern uint8_t Heure, minut, jour, mois, jourJ, moisM;
extern uint16_t M, S, annee;
extern uint32_t Time, H, tempsPrecedentNourrissage, tempsPrecedentTempete, tempsPrecedentTempeteAleatoire;

/* dans oscillo.cpp */
extern Servo oscillo[];
extern boolean oscillo1On, oscillo2On, oscillo3On;
extern uint8_t angle1Oscillo[], angle2Oscillo[], positionOscillo[];
extern uint16_t delaisMouvementOscilloMillis[];
extern uint32_t tempsPrecedentOscillo[];

/* dans sd.cpp */
extern SdFat SD;
extern File myFile;
extern const char* ssidSurSD;
extern const char* mdpSurSD;
extern const char* numero2TelSurSD;
extern const char* erreursSurSD;
extern const char* mesuresEnBinaire;
extern const char* pinSurSD;
extern const uint8_t nbr2Lignes;
extern uint8_t numero2Ligne;
extern boolean SDPrecedementVersLeBas;
enum pageActuel : uint8_t {PAGE_FIN, PAGE_SUIVANTE, PAGE_PRECEDENTE};

/* dans temperature.cpp */
extern RTC_DS3231 rtc;
extern OneWire oneWire;
extern DallasTemperature sensors;
extern DeviceAddress sondeBac, sondeRampe;
extern boolean alerteTemperature, alertePhBas, alertePhHaut, adresseSondeRampe, adresseSondeBac, tempeteAleatoireOn, etatVentilationBac, etatVentilationRampe, demande2MesureSalinite, etaitEnEtalonnage;
extern const uint8_t cent;
extern uint8_t temperatureRampe, ventilationrampe;
extern float moyennePhBac, moyennePhRac;
extern float temperatureBac, ventilationaquarium, alertetemperaturehaute, alertetemperaturebasse;

/* dans wifi.cpp */
enum typeCommande_t : uint8_t {ENVOYERsauvegarde = 0, ENVOYERvariable, ENVOYERidentifiantWifi, ENVOYERconnectionWifi,
                               RECEVOIRsauvegarde, RECEVOIRvariable, RECEVOIRidentifiantWifi, RECEVOIRconnectionWifi, AUCUNE = 255
                              }; // dans le même ordre que les commandes
extern const uint8_t maxSsid, maxMdp;
extern boolean reponseModuleWifi;

#endif
