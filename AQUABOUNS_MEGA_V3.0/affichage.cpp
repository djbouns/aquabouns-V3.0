
/* AQUABOUNS, Automate pour la gestion d'aquarium recifal
   Créé par djbouns, Copyright (C) 2022

   Ce programme est un logiciel libre: vous pouvez le redistribuer
   et/ou le modifier selon les termes de la "GNU General Public
   License", tels que publiés par la "Free Software Foundation"; soit
   la version 2 de cette licence ou (à votre choix) toute version
   ultérieure.

   Ce programme est distribué dans l'espoir qu'il sera utile, mais
   SANS AUCUNE GARANTIE, ni explicite ni implicite; sans même les
   garanties de commercialisation ou d'adaptation dans un but spécifique.

   Se référer à la "GNU General Public License" pour plus de détails.
   Vous la trouverez dans l'onglet GNU_General_Public_License.h

   Vous devriez avoir reçu une copie de la "GNU General Public License"
   en même temps que ce programme; sinon, écrivez a la "Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA".*/

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Librairies $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <EEPROM.h>
#include "Nextion.h"
#include "TimerFreeTone.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fichiers $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include "config.h"
#include "affichage.h"
#include "global.h"
#include "horloge.h"
#include "autres.h"
#include "debug.h"
#include "eclairage.h"
#include "brassage.h"
#include "carteSD.h"
#include "gsm.h"
#include "temperature_ph.h"
#include "oscillo.h"
#include "wifi.h"
#include "eeprom.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ *** ECRAN NEXTION 800*480 *** $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/* Vous avez la possibilité d'utiliser plusieurs model d'ecran nextion du momment que la resolution est 800*480
*/
// !!! IMPORTANT !!! >>> il faut imperativement utiliser un ecran nextion ORIGINAL, les copies ne reconnaise pas les fichier fait avec nextion editor <<< !!! IMPORTANT !!!
/*
  https://nextion.itead.cc/nextion-shop/

  Afin de faire votre choix il faut evaluer vos besoins :
  > taille 5" ou 7"
  > version standard, enhanced ou inteligent
  > avec ou sans boitier
  > ecran resistif ou capacitif

  >>> Le principe de l'écran tactile résistif : Un écran résistif est constitué de deux surfaces séparées par de minuscules entretoises et parcourues de connecteurs.
  Le touché du doigt ou d'une pointe quelconque déforme la surface supérieure et met en contact ses connecteurs avec ceux de la surface intérieure. L'utilisation d'un stylet est souvent necessaire.
  >>> Le principe de l'écran tactile capacitif : Quand le doigt, conducteur d'électricité, touche l'écran, des charges électriques lui sont transférées.

  voici les models compatibles avec l'Aquabouns V3 :
  ecran 5" NX8048P050-011R <<< version inteligent resistif
  ecran 5" NX8048P050-011C <<< version inteligent capacitif
  ecran 5" NX8048P050-011R <<< version inteligent resistif
  ecran 5" NX8048P050-011C <<< version inteligent capacitif
  ecran 7" NX8048P070-011R <<< version inteligent resistif
  ecran 7" NX8048P070-011C <<< version inteligent capacitif
  ecran 7" NX8048P070-011R <<< version inteligent resistif
  ecran 7" NX8048P070-011C <<< version inteligent capacitif
  ecran 5" NX8048T050_011R <<< version standard resistif
  ecran 7" NX8048T070_011R <<< version standard resistif
  ecran 5" NX8048K050_011R <<< version enhanced resistif
  ecran 7" NX8048K070_011R <<< version enhanced resistif
  ecran 7" NX8048K070_011C <<< version enhanced capacitif
*/

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
pageNextion pageActuelNextion;
uint32_t number; // pour stocker les numerique lu du nextion
uint32_t recupHorraireTemporraire; // pour recuperer calcul de l'horraire
uint8_t statutIconeTemperatureBac, statutIconeBatterie; // pour le statut des icones sur l'ecran
boolean statuticonetemperaturerampe = false;
boolean inverseSonde = true;
boolean nourissageManuelEnCours = false;
const uint16_t mille = 1000u, deuxMille = 2000u, cinqMille = 5000u;
boolean alerteOsmolation = true; // pour ne pas avoir d'alerte a repetition
boolean ChangementDEauON = false; // pour activer le changement d'eau
uint8_t Mprecedent; // stock la minute actuel pour mise a jour des minutes dans nextion quand minute != Mprecedent
const uint8_t maxbufferTexte = 75; // place pour recuperer les message en progmem
char bufferTexte[maxbufferTexte + 1]; // place pour recuperer les message en progmem + carractere de fin
boolean initialisationPhDemarrage = true;
boolean mesurePhDemarrage = true;
const uint16_t vert = 46851u; // code couleur du texte sur nextion
const uint16_t orange = 60740u; // code couleur du texte sur nextion
const uint16_t rouge = 57577u; // code couleur du texte sur nextion
const uint16_t blanc = 65535u;  // code couleur du texte sur nextion
const uint8_t pas2Changement2Couleur = 0; // garde le texte de la meme couleur que precedement
const uint16_t batterieNonConnecte = 890u; // batterie non connecté 8.4V = avec pont diviseur 890 sur l'echelle analogique de 1024
const uint16_t batterie100 = 761u; // batterie 7.2V NiMh chargée a 7.2V = 761 avec un pont diviseur sur l'echelle analogique de 1024
const uint16_t batterie0 = 687u; // batterie 7.2V NiMh vide(6.5V) = 687 avec un pont diviseur sur l'echelle analogique de 1024
enum {Ibatterie0, Ibatterie16, Ibatterie32, Ibatterie50, Ibatterie68, Ibatterie84, Ibatterie100, finBatterie = 255 };


enum {ItempeteOn = 4, ItempeteOff = 5, Ion = 6, Ioff = 7, Ipwm = 8, IpetitVentOn = 9, IpetitVentOff = 10, IgrandVentOn = 11, IgrandVentOff = 12, Iwave = 13, Ipompedelay = 15,
      INotifOff = 16, INotifOn = 17, Ipile100 = 29, Ipile84 = 30, Ipile68 = 31, Ipile50 = 32, Ipile32 = 33, Ipile16 = 34, Ipile0 = 35, IpoissonmortOn = 46, IpoissonmortOff = 47,
      InourissageOn = 48, InourissageOff = 49, Isignal1 = 93, Isignal2 = 94, Isignal3 = 95, Isignal4 = 96, Isignal5 = 97, Itimer = 119, Isonde1 = 117, Isonde2 = 118, IFin = 255
     }; // Attribut un nom au numero d'image dans nextion


enum { VRemonteOn = 2, VRemonteOff = 3, VRemonteDelay = 1, VTempeteMOn = 4, VTempeteMOff = 5, VTempeteAOn = 7, VTempeteAOff = 6, VAlimMOn = 9, VAlimMOff = 8, VAlim1On = 11, VAlim1Off = 10,
       VAlim2On = 13, VAlim2Off = 12, VCouleur1On = 16, VCouleur1Off = 14, VCouleur1PWM = 15, VCouleur2On = 19, VCouleur2Off = 17, VCouleur2PWM = 18, VCouleur3On = 22, VCouleur3Off = 20,
       VCouleur3PWM = 21, VCouleur4On = 25, VCouleur4Off = 23, VCouleur4PWM = 24, VBrassage1On = 28, VBrassage1Off = 26,  VBrassage1PWM = 27, VBrassage2On = 31, VBrassage2Off = 29,
       VBrassage2PWM = 30, VBrassage3On = 34, VBrassage3Off = 32, VBrassage3PWM = 33, VChangementDEauOn = 35, VChangementDEauOff = 36, VBuzzerOn = 7, VBuzzerOff = 8, VRefugeOn = 41,
       VRefugeOff = 39, VRefugeTimer = 40, VFin = 255
     }; // Attribut une valeurs a chaque bouton

/*déclarations de tout les elements de l'ecran etant en interaction avec l'arduino */
// page demarrage
NexPage pagedemarrage = NexPage(0, 0, "demarrage"); // adresse de la page demarage
NexText texte1 = NexText(0, 1, "t0"); // champ texte vierge
NexText texte2 = NexText(0, 2, "t1"); // champ texte vierge
NexText texte3 = NexText(0, 3, "t2"); // champ texte vierge
NexText texte4 = NexText(0, 4, "t3"); // champ texte vierge
NexText texte5 = NexText(0, 5, "t4"); // champ texte vierge
NexText texte6 = NexText(0, 6, "t5"); // champ texte vierge
// page menu
NexPage pageMenu = NexPage(1, 0, "menu"); // adresse de la page menu
NexVariable affichagepourcentcouleur1 = NexVariable(1, 14, "n0"); // adresse pour l'affichage % couleur1
NexVariable affichagepourcentcouleur2 = NexVariable(1, 15, "n1"); // adresse pour l'affichage % couleur2
NexVariable affichagepourcentcouleur3 = NexVariable(1, 34, "n7"); // adresse pour l'affichage % couleur3
NexVariable affichagepourcentcouleur4 = NexVariable(1, 35, "n8"); // adresse pour l'affichage % couleur4
NexText affichageheurelevee = NexText(1, 25, "t0"); // adresse pour l'affichage heure
NexText affichageheurecouchee = NexText(1, 36, "t2"); // adresse pour l'affichage heure
NexPicture iconetemperaturerampe = NexPicture(1, 5, "p10"); // adresse de l'icone ventilation temperature rampe
NexVariable affichagetemperaturerampe = NexVariable(1, 21, "n5"); // adresse pour l'affichage temperature rampe
NexVariable affichagepourcentbrassage1 = NexVariable(1, 1, "n2"); // adresse pour l'affichage % Brassage 1
NexVariable affichagepourcentbrassage2 = NexVariable(1, 2, "n3"); // adresse pour l'affichage % Brassage 2
NexVariable affichagepourcentbrassage3 = NexVariable(1, 16, "n4"); // adresse pour l'affichage % Brassage 3
NexButton onofftempete = NexButton(1, 49, "b16"); // adresse du bouton on/off tempete
NexPicture iconetempetealeatoire = NexPicture(1, 50, "p2"); // adresse de l'icone tempete aleatoire
NexButton onofftempetealeatoire = NexButton(1, 23, "b9"); // adresse du bouton on/off tempete aleatoire
NexText affichageheuretempete = NexText(1, 19, "t6"); // adresse pour l'affichage heure tempete
NexText affichagedureetempete = NexText(1, 20, "t7"); // adresse pour l'affichage duree tempete
NexPicture iconealimentationencours = NexPicture(1, 41, "p1"); // adresse de l'icone alimentation en cours
NexButton boutonalimentationmanuel = NexButton(1, 37, "b10"); // adresse du bouton alimentation manuel
NexButton onoffalimentation1 = NexButton(1, 7, "b1"); // adresse du bouton on/off alimentation 1
NexText affichageheurealimentation1 = NexText(1, 17, "t4"); // adresse pour l'affichage heure alimentation 1
NexButton onoffalimentation2 = NexButton(1, 8, "b2"); // adresse du bouton on/off alimentation 2
NexText affichageheurealimentation2 = NexText(1, 18, "t5"); // adresse pour l'affichage heure alimentation 2
NexButton onoffpomperemonte = NexButton(1, 6, "b0"); // adresse du bouton on/off pompe remonté
NexVariable affichagetemperaturebac = NexVariable(1, 44, "x2"); // adresse pour l'affichage temperature aquarium
NexPicture iconeventilateurbac = NexPicture(1, 4, "p9"); // adresse de l'icone ventilation temperature aquarium
NexPicture iconemort = NexPicture(1, 42, "p3"); // adresse de l'icone temperature rampe
NexVariable affichagephbac = NexVariable(1, 32, "x1"); // adresse pour l'affichage PH BAC
NexVariable affichageredox = NexVariable(1, 22, "n11"); // adresse pour l'affichage redox
NexVariable affichageSalinite = NexVariable(1, 31, "x0"); // adresse pour l'affichage salinite
NexButton onoffpwmcouleur1 = NexButton(1, 12, "b6"); // adresse du bouton on/off/pwm couleur1
NexButton onoffpwmcouleur2 = NexButton(1, 13, "b7"); // adresse du bouton on/off/pwm couleur2
NexButton onoffpwmcouleur3 = NexButton(1, 38, "b11"); // adresse du bouton on/off/pwm couleur3
NexButton onoffpwmcouleur4 = NexButton(1, 39, "b12"); // adresse du bouton on/off/pwm couleur4
NexButton onoffpwmbrassage1 = NexButton(1, 9, "b3"); // adresse du bouton on/off/pwm Brassage 1
NexButton onoffpwmbrassage2 = NexButton(1, 10, "b4"); // adresse du bouton on/off/pwm Brassage 2
NexButton onoffpwmbrassage3 = NexButton(1, 11, "b5"); // adresse du bouton on/off/pwm Brassage 3
NexButton onoffrefuge = NexButton(1, 48, "b15"); // adresse du bouton refuge
NexButton onoffchangementdeau = NexButton(1, 40, "b13"); // adresse du bouton config vers Brassage
NexButton onoffbuzzermenu = NexButton(1, 47, "b14"); // adresse du bouton on/off du buzzer
NexText affichageheure = NexText(1, 43, "t3"); // adresse pour l'affichage heure
NexPicture iconebatterie = NexPicture(1, 45, "p4"); // adresse de l'icone temperature rampe
NexButton onoffnotif = NexButton(1, 46, "p0"); // adresse du bouton menu vers notif
NexButton boutonverseclairage = NexButton(1, 33, "m6"); // adresse du bouton config vers Eclairage
NexButton boutonversbrassage = NexButton(1, 30, "m5"); // adresse du bouton config vers Brassage
NexButton boutonversetalonnage = NexButton(1, 28, "m3"); // adresse du bouton vers calibration
NexButton boutonversgraph24h = NexButton(1, 29, "m4"); // adresse du bouton config vers graph
NexButton boutonversconfig = NexButton(1, 3, "m0"); // adresse du bouton vers config
NexVariable variableetatboutonsmenu = NexVariable(1, 24, "va0"); // adresse de la variable qui retourne etat bouton
NexButton boutonversstandby = NexButton(1, 27, "m1"); // adresse du bouton vers standby
// page config
NexPage pageConfig = NexPage(2, 0, "parametre"); // adresse de la page menu
NexVariable valeurtemperatureaquarium = NexVariable(2, 16, "x2"); // adresse pour l'affichage temperature ventilation bac
NexVariable valeurventilationrampe = NexVariable(2, 35, "n2"); // adresse pour l'affichage temperature ventilation rampe
NexVariable valeurventilationaquarium = NexVariable(2, 17, "x3"); // adresse pour l'affichage temperature ventilation bac
NexButton boutonadressesonde = NexButton(2, 1, "b30"); // adresse pour le bouton adresse sonde temperature bac / rampe
NexVariable valeurheureNourrissage1h = NexVariable(2, 12, "n0"); // adresse pour l'affichage heure nourissage 1
NexVariable valeurheureNourrissage1m = NexVariable(2, 13, "n4"); // adresse pour l'affichage minute nourissage 1
NexVariable valeurheureNourrissage2h = NexVariable(2, 15, "n8"); // adresse pour l'affichage heure nourissage 2
NexVariable valeurheureNourrissage2m = NexVariable(2, 14, "n6"); // adresse pour l'affichage minute nourissage 2
NexVariable valeurdureeNourrissageMillis = NexVariable(2, 34, "n1"); // adresse pour l'affichage duree nourissage
NexProgressBar progressbarconfig = NexProgressBar(2, 19, "j0"); // champ texte
NexButton boutonenregistrerconfig = NexButton(2, 9, "b0"); // // adresse du bouton enregistrer
NexButton boutonrestaureeeprom = NexButton(2, 21, "m4"); // adresse du bouton restaure eeprom
NexButton boutonverstel = NexButton(2, 7, "m8"); // adresse du bouton vers tel
NexButton boutonverswifi = NexButton(2, 6, "m6"); // adresse du bouton vers wifi
NexButton boutonversinfo = NexButton(2, 4, "m3"); // adresse du bouton vers info
NexButton boutonvershorloge = NexButton(2, 5, "m5"); // adresse du bouton vers horloge
NexButton boutonversalarme = NexButton(2, 20, "m2"); // adresse du bouton vers alarme
NexButton boutonredemarrage = NexButton(2, 11, "m0"); // adresse du bouton redemarrage
NexButton boutonversmenu = NexButton(2, 10, "m1"); // adresse du bouton vers calibration
// page Brassage
NexPage pageBrassage = NexPage(3, 0, "brassage"); // adresse de la page Brassage
NexVariable valeuroscillo1min = NexVariable(3, 3, "n9"); // adresse pour l'affichage oscillo 1 angle min
NexVariable valeuroscillo1max = NexVariable(3, 4, "n11"); // adresse pour l'affichage oscillo 1 angle max
NexVariable valeurmouvementoscillo1 = NexVariable(3, 13, "n7"); // adresse pour l'affichage vitesse oscillo 1
NexVariable valeurminBrassage1 = NexVariable(3, 17, "n3"); // adresse pour l'affichage Brassage 1
NexVariable valeurBrassage1 = NexVariable(3, 2, "n8"); // adresse pour l'affichage Brassage 1
NexVariable valeuroscillo2min = NexVariable(3, 6, "n13"); // adresse pour l'affichage oscillo 2 angle min
NexVariable valeuroscillo2max = NexVariable(3, 7, "n14"); // adresse pour l'affichage oscillo 2 angle max
NexVariable valeurmouvementoscillo2 = NexVariable(3, 14, "n1"); // adresse pour l'affichage vitesse oscillo 2
NexVariable valeurminBrassage2 = NexVariable(3, 18, "n4"); // adresse pour l'affichage Brassage 2
NexVariable valeurBrassage2 = NexVariable(3, 5, "n12"); // adresse pour l'affichage Brassage 2
NexVariable valeuroscillo3min = NexVariable(3, 9, "n16"); // adresse pour l'affichage oscillo 3 angle min
NexVariable valeuroscillo3max = NexVariable(3, 10, "n17"); // adresse pour l'affichage oscillo 3 angle max
NexVariable valeurmouvementoscillo3 = NexVariable(3, 15, "n2"); // adresse pour l'affichage vitesse oscillo 3
NexVariable valeurminBrassage3 = NexVariable(3, 19, "n5"); // adresse pour l'affichage Brassage 3
NexVariable valeurBrassage3 = NexVariable(3, 8, "n15"); // adresse pour l'affichage Brassage 3
NexVariable valeurtempete = NexVariable(3, 11, "n20"); // adresse pour l'affichage puissance tempete
NexVariable valeurdureetempete = NexVariable(3, 1, "n6");  // adresse pour l'affichage duree tempete
NexVariable valeuraccalemiealimentation = NexVariable(3, 21, "n10");  // adresse pour l'affichage accalmie
NexVariable valeuraccalemienocturne = NexVariable(3, 16, "n0");  // adresse pour l'affichage accalmie
NexProgressBar progressbarbrassage = NexProgressBar(3, 42, "j0"); // champ texte
NexButton boutonenregistrerbrassage = NexButton(3, 23, "b0"); // bouton enregistrer
// page horloge
NexPage pageHorloge = NexPage(4, 0, "horloge"); // adresse de la page horloge
NexVariable affichageH = NexVariable(4, 19, "n0"); // champ texte heure
NexVariable affichageM = NexVariable(4, 20, "n1"); // champ texte minute
NexVariable affichagejour = NexVariable(4, 21, "n2"); // champ texte jour
NexVariable affichagemois = NexVariable(4, 22, "n3"); // champ texte mois
NexVariable affichageannee = NexVariable(4, 23, "n4"); // champ texte annee
NexProgressBar progressbarhorloge = NexProgressBar(4, 26, "j0"); // champ texte
NexButton boutonenregistrerhorloge = NexButton(4, 1, "b0"); // bouton enregistrer + horloge vers parametre
// page wifi
NexPage pageWifi = NexPage(5, 0, "wifi"); // adresse de la page wifi
NexButton boutonenregistrerwifi = NexButton(5, 6, "b0"); // bouton enregistrer + wifi vers parametre
NexText valeurssid = NexText(5, 2, "t0"); // champ texte SSID
NexText valeurmdp = NexText(5, 43, "t1"); // champ texte MDP
NexText valeurconnecte = NexText(5, 44, "t2"); // champ texte connecte
NexPicture dbsignalwifi = NexPicture(5, 45, "p0"); // gauge niveau signal
NexProgressBar progressbarwifi = NexProgressBar(5, 28, "j0"); // champ texte
// page tel
NexPage pageTel = NexPage(6, 0, "tel"); // adresse de la page tel
NexText valeurtel = NexText(6, 1, "t0"); // champ texte tel
NexPicture dbsignalgsm = NexPicture(6, 21, "p0"); // gauge niveau signal
NexText valeuroperateur = NexText(6, 2, "t4"); // champ texte connecte
NexText valeurpin = NexText(6, 25, "t1"); // champ pin
NexProgressBar progressbartel = NexProgressBar(6, 23, "j0"); // champ texte
NexButton boutonenregistrertel = NexButton(6, 20, "b0"); // bouton enregistrer + tel vers parametre
// page info
NexPage pageInfo = NexPage(7, 0, "info"); // adresse de la page info
// page standby
NexPage pagestandby = NexPage(8, 0, "standby"); // adresse de la page standby
// page graph 7j
NexPage pageGraph7J = NexPage(9, 0, "graph7j"); // adresse de la page standby
NexText affichagejour1 = NexText(9, 9, "t6"); // champ texte jour
NexText affichagejour2 = NexText(9, 7, "t5"); // champ texte jour
NexText affichagejour3 = NexText(9, 6, "t4"); // champ texte jour
NexText affichagejour4 = NexText(9, 5, "t3"); // champ texte jour
NexText affichagejour5 = NexText(9, 4, "t2"); // champ texte jour
NexText affichagejour6 = NexText(9, 3, "t1"); // champ texte jour
NexText affichagejour7 = NexText(9, 2, "t0"); // champ texte jour
NexWaveform graph7J1 = NexWaveform(9, 1, "s0"); // graph sur 7 j
NexWaveform graph7J2 = NexWaveform(9, 8, "s1"); // graph sur 7 j
// page graph 24h
NexPage pageGraph24H = NexPage(10, 0, "graph24h"); // adresse de la page graph
NexText affichageheure1 = NexText(10, 1, "t0"); // champ texte heure
NexText affichageheure2 = NexText(10, 2, "t1"); // champ texte heure
NexText affichageheure3 = NexText(10, 3, "t2"); // champ texte heure
NexText affichageheure4 = NexText(10, 4, "t3"); // champ texte heure
NexText affichageheure5 = NexText(10, 5, "t4"); // champ texte heure
NexText affichageheure6 = NexText(10, 6, "t5"); // champ texte heure
NexText affichageheure7 = NexText(10, 7, "t12"); // champ texte heure
NexText affichageheure8 = NexText(10, 8, "t13"); // champ texte heure
NexText affichageheure9 = NexText(10, 9, "t14"); // champ texte heure
NexText affichageheure10 = NexText(10, 10, "t15"); // champ texte heure
NexText affichageheure11 = NexText(10, 11, "t16"); // champ texte heure
NexText affichageheure12 = NexText(10, 12, "t17"); // champ texte heure
NexWaveform graph24H1 = NexWaveform(10, 13, "s0"); // graph temperature
NexWaveform graph24H2 = NexWaveform(10, 14, "s1"); // graph temperature
NexButton boutonversgraph7j = NexButton(10, 19, "m2"); // bouton graph vers menu
// page éclairage
NexPage pageEclairage = NexPage(11, 0, "eclairage"); // adresse de la page eclairage
NexVariable valeurcouleur1 = NexVariable(11, 5, "n0"); // adresse pour l'affichage % couleur1
NexVariable valeurcouleur2 = NexVariable(11, 6, "n1"); // adresse pour l'affichage % couleur2
NexVariable valeurcouleur3 = NexVariable(11, 7, "n2"); // adresse pour l'affichage % couleur3
NexVariable valeurcouleur4 = NexVariable(11, 8, "n3"); // adresse pour l'affichage % couleur4
NexVariable valeurleverSoleilh = NexVariable(11, 9, "n4"); // adresse pour l'affichage heure lever soleil
NexVariable valeurleverSoleilm = NexVariable(11, 10, "n5"); // adresse pour l'affichage minute lever soleil
NexVariable valeurcoucherSoleilh = NexVariable(11, 11, "n6"); // adresse pour l'affichage heure coucher soleil
NexVariable valeurcoucherSoleilm = NexVariable(11, 15, "n7"); // adresse pour l'affichage minute coucher soleil
NexVariable valeurdureelevercoucher = NexVariable(11, 16, "n8"); // adresse pour l'affichage duree lever/coucher soleil
NexVariable valeurdebutrefugeh = NexVariable(11, 33, "n9"); // adresse pour l'affichage heure debut refuge
NexVariable valeurdebutrefugem = NexVariable(11, 34, "n10"); // adresse pour l'affichage minute debut refuge
NexVariable valeurfinrefugeh = NexVariable(11, 35, "n11"); // adresse pour l'affichage heure fin refuge
NexVariable valeurfinrefugem = NexVariable(11, 36, "n12"); // adresse pour l'affichage minute fin refuge
NexProgressBar progressbareclairage = NexProgressBar(11, 37, "j0"); // champ texte
NexButton boutonenregistrereclairage = NexButton(11, 12, "b0"); // bouton sauvegarde eclairage
// page orph
NexPage pageEtalonnage = NexPage(12, 0, "orph"); // adresse de la page graph
NexVariable affichageetalonnagephbac = NexVariable(12, 30, "x1");
NexVariable valeuretalonnagePh = NexVariable(12, 12, "x0");
NexVariable affichageetalonnageredox = NexVariable(12, 1, "n1");
NexVariable valeuretalonnageRedox = NexVariable(12, 5, "n4");
NexVariable affichageetalonnagesalinite = NexVariable(12, 6, "n5");
NexVariable valeuretalonnageSalinite = NexVariable(12, 2, "n2");
NexPicture boutoncalibrer = NexPicture(12, 7, "p0");
NexVariable variableetalonnage = NexVariable(12, 4, "va0");
NexProgressBar progressbarorph = NexProgressBar(12, 28, "j0"); // champ texte
// page SD
NexPage pageSd = NexPage(13, 0, "sd"); // adresse de la page sd
NexText valeursd00 = NexText(13, 1, "t0"); // champ du fichier
NexText valeursd01 = NexText(13, 2, "t1"); // champ du fichier
NexText valeursd02 = NexText(13, 3, "t2"); // champ du fichier
NexText valeursd03 = NexText(13, 4, "t3"); // champ du fichier
NexText valeursd04 = NexText(13, 5, "t4"); // champ du fichier
NexButton boutonhaut = NexButton(13, 8, "bo"); // bouton vers le haut des evenements
NexButton boutonbas = NexButton(13, 9, "b1"); // bouton vers le haut des evenements
NexButton boutonclear = NexButton(13, 6, "m1"); // bouton efface la carte sd
// page popup
NexPage pagePopup = NexPage(14, 0, "popup"); // adresse de la page popup
// page alarme
NexPage pageAlarme = NexPage(15, 0, "alarme"); // adresse de la page popup²
NexVariable valeuralertetemperaturehaute = NexVariable(15, 11, "x0"); // adresse pour l'affichage alerte temperature bac haute
NexVariable valeuralertetemperaturebasse = NexVariable(15, 12, "x1"); // adresse pour l'affichage alerte temperature bac basse
NexVariable valeurdureeosmolation = NexVariable(15, 9, "n7"); // adresse pour l'affichage duree osmolation
NexVariable valeurdureerenplissage = NexVariable(15, 10, "n0"); // adresse pour l'affichage duree de rempliassage
NexVariable valeuralertephbachaut = NexVariable(15, 7, "x4"); // adresse pour l'affichage ph bac haut
NexVariable valeuralertephbacbas = NexVariable(15, 8, "x6"); // adresse pour l'affichage ph bac bas
NexButton onoffrac = NexButton(15, 32, "b16"); // adresse du bouton on/off du RAC
NexButton onoffbuzzeralarme = NexButton(15, 31, "b14"); // adresse du bouton on/off du buzzer
NexVariable affichageHdebutalerte = NexVariable(15, 5, "n5"); // champ texte heure
NexVariable affichageHfinalerte = NexVariable(15, 6, "n6"); // champ texte heure
NexProgressBar progressbaralarme = NexProgressBar(15, 17, "j0"); // champ texte
NexButton boutonenregistreralarme = NexButton(15, 16, "b0"); // bouton sauvegarde eclairage

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fonction $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< affichage des pages
/*  page menu, lance toute les fonction pour connaitre les variables a afficher et l'etat des boutons */
void affichePageMenu() {
  const uint8_t minuteEnSeconde = 60;
  saliniteModeMesure(); // remet la mesure en mode mesure
  pageActuelNextion = menu; // indique a l'arduino que la page actuel est menu
  pageMenu.show(); // affiche la page menu
  Mprecedent = minuteEnSeconde; // Mprecedent = 60 pour rafraichir quoi qu'il en soit
  rafraichirEclairageCouleur1Nextion(); // affiche variable
  rafraichirEclairageCouleur2Nextion(); // affiche variable
  rafraichirEclairageCouleur3Nextion(); // affiche variable
  rafraichirEclairageCouleur4Nextion(); // affiche variable
  TIMEenTEXTE(sauvegarde.leverSoleil); // convertie numerique en char
  affichageheurelevee.setText(textime); // affiche l'heure
  TIMEenTEXTE(sauvegarde.coucherSoleil); // convertie numerique en char
  affichageheurecouchee.setText(textime); // affiche l'heure
  rafraichirTemperatureRampeNextion(); // affiche variable
  rafraichirBrassage1Nextion(); // affiche variable
  rafraichirBrassage2Nextion(); // affiche variable
  rafraichirBrassage3Nextion(); // affiche variable
  statutTempete(); // affiche l'icone
  statutTempeteAleatoire(); // statut + ecrit heure et duree
  iconNourrissageEnCours();
  rafraichirboutonalimentationmanuel();
  statutOnOffAlim1(); // affiche le bouton
  statutOnOffAlim2();// affiche le bouton
  statutOnOffDelay();// affiche le bouton
  rafraichirTemperatureBacNextion(); // affiche variable
  rafraichirPhBacNextion(); // affiche variable
  rafraichirRedoxNextion(); // affiche variable
  attenteMesureSalinite(); // attend la reception des mesures
  rafraichirSaliniteNextion(); // affiche variable
  statutCouleur1();// affiche le bouton
  statutCouleur2();// affiche le bouton
  statutCouleur3();// affiche le bouton
  statutCouleur4();// affiche le bouton
  statutBrassage1();// affiche le bouton
  statutBrassage2();// affiche le bouton
  statutBrassage3();// affiche le bouton
  statutChangementDEau();// affiche le bouton
  statutBuzzerOnOffMenu();// affiche le bouton
  statutRefuge();// affiche le bouton
  affichageHeureNextion();
  rafraichirIconeBatterie();// affiche l'icone
  statutNotifOnOff();// affiche l'icone
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page menu <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/* page config, envoie les variables a afficher */
void affichePageConfig() {
  uint32_t temporairedureeNourrissageMillis;
  const uint32_t minuteEnMillis = 60000ul; // correspond a 1 minute en millisseconde
  pageConfig.show(); // affiche la page config
  saliniteModeMesure(); // remet la mesure en mode mesure
  pageActuelNextion = parametre; // indique a l'arduino que la page actuel est config
  valeurtemperatureaquarium.setValue(sauvegarde.temperatureaquarium * dix); // converti pour avoir 1 seul decimal
  valeurventilationaquarium.setValue(sauvegarde.ventilationaquarium * dix); // converti pour avoir 1 seul decimal
  valeurventilationrampe.setValue(sauvegarde.ventilationrampe);
  if (sauvegarde.adresseSondeBac == 0) {
    inverseSonde = true;
    boutonadressesonde.Set_background_image_pic(Isonde1);
  }
  else {
    inverseSonde = false;
    boutonadressesonde.Set_background_image_pic(Isonde2);
  }
  secondeEnHorraire(sauvegarde.heureNourrissage[numeroUn]) ;
  valeurheureNourrissage1h.setValue(number);
  valeurheureNourrissage1m.setValue(recupHorraireTemporraire);
  secondeEnHorraire(sauvegarde.heureNourrissage[numeroDeux]) ;
  valeurheureNourrissage2h.setValue(number);
  valeurheureNourrissage2m.setValue(recupHorraireTemporraire);
  temporairedureeNourrissageMillis = sauvegarde.dureeNourrissageMillis / minuteEnMillis; // affiche la duree en minute
  valeurdureeNourrissageMillis.setValue(temporairedureeNourrissageMillis);
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page config <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page info, affiche la page */
void affichePageInfo() {
  pageInfo.show();// affiche la page info
  pageActuelNextion = info; // indique a l'arduino que la page actuel est info
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page info <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page tel, affiche la page */
void affichePageTel() {
  pageTel.show();// affiche la page tel
  pageActuelNextion = tel; // indique a l'arduino que la page actuel est tel
  lisSurSd(numero2TelSurSD, numero2tel, maxnumero2tel); // lance la lecture/copie du fichier
  valeurtel.setText(numero2tel); // envoie le num de tel actuel
  lisSurSd(pinSurSD, codePin, maxPinSize); // lance la lecture/copie du fichier
  valeurpin.setText(codePin); // envoie le num de tel actuel
  rafraichirSignalGSM();
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page tel <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page wifi, affiche la page */
void affichePageWifi() {
  pageWifi.show();// affiche la page wifi
  pageActuelNextion = wifi; // indique a l'arduino que la page actuel est wifi
  lisSurSd(ssidSurSD, identifiantWifi.ssid, maxSsid); // lance la lecture/copie du fichier
  valeurssid.setText(identifiantWifi.ssid); // recupere la valeur dans nextion
  lisSurSd(mdpSurSD, identifiantWifi.mdp, maxMdp); // lance la lecture/copie du fichier
  valeurmdp.setText(identifiantWifi.mdp); // recupere la valeur dans nextion
  rafraichirSignalWifi();
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page wifi <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page Eclairage, envoie les variables a afficher */
void affichePageEclairage() {
  const uint8_t minuteEnSeconde = 60;
  pageEclairage.show();// affiche la page eclairage
  saliniteModeMesure(); // remet la mesure en mode mesure
  pageActuelNextion = light; // indique a l'arduino que la page actuel est eclairage
  valeurcouleur1.setValue(sauvegarde.puissanceMaxEclairage[Couleur1]);
  valeurcouleur2.setValue(sauvegarde.puissanceMaxEclairage[Couleur2]);
  valeurcouleur3.setValue(sauvegarde.puissanceMaxEclairage[Couleur3]);
  valeurcouleur4.setValue(sauvegarde.puissanceMaxEclairage[Couleur4]);
  secondeEnHorraire(sauvegarde.leverSoleil); // decoupe horraire en deux, les heure et les minutes
  valeurleverSoleilh.setValue(number); // heure en numerique
  valeurleverSoleilm.setValue(recupHorraireTemporraire); // minute
  secondeEnHorraire(sauvegarde.coucherSoleil); // decoupe horraire en deux, les heure les minutes en texte
  valeurcoucherSoleilh.setValue(number); // heure en numerique
  valeurcoucherSoleilm.setValue(recupHorraireTemporraire); // minute
  valeurdureelevercoucher.setValue(sauvegarde.dureelevercoucher / minuteEnSeconde); // affiche la duree du lever en minute
  secondeEnHorraire(sauvegarde.debutRefuge); // decoupe horraire en deux, les heure et les minutes
  valeurdebutrefugeh.setValue(number); // heure en numerique
  valeurdebutrefugem.setValue(recupHorraireTemporraire); // minute
  secondeEnHorraire(sauvegarde.finRefuge); // decoupe horraire en deux, les heure les minutes en texte
  valeurfinrefugeh.setValue(number); // heure en numerique
  valeurfinrefugem.setValue(recupHorraireTemporraire); // minute
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page eclairage <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page Brassage, envoie les variables a afficher */
void affichePageBrassage() {
  const uint32_t minuteEnMillis = 60000ul; // correspond a 1 minute en millisseconde
  saliniteModeMesure(); // remet la mesure en mode mesure
  pageBrassage.show(); // affiche la page Brassage
  pageActuelNextion = brass;  // indique a l'arduino que la page actuel est
  valeurminBrassage1.setValue(sauvegarde.puissanceDemarageBrassage[numeroUn]);
  valeurminBrassage2.setValue(sauvegarde.puissanceDemarageBrassage[numeroDeux]);
  valeurminBrassage3.setValue(sauvegarde.puissanceDemarageBrassage[numeroTrois]);
  valeurBrassage1.setValue(sauvegarde.puissanceMaxBrassage[numeroUn]);
  valeurBrassage2.setValue(sauvegarde.puissanceMaxBrassage[numeroDeux]);
  valeurBrassage3.setValue(sauvegarde.puissanceMaxBrassage[numeroTrois]);
  valeuroscillo1min.setValue(sauvegarde.angle1Oscillo[numeroUn]);
  valeuroscillo2min.setValue(sauvegarde.angle1Oscillo[numeroDeux]);
  valeuroscillo3min.setValue(sauvegarde.angle1Oscillo[numeroTrois]);
  valeuroscillo1max.setValue(sauvegarde.angle2Oscillo[numeroUn]);
  valeuroscillo2max.setValue(sauvegarde.angle2Oscillo[numeroDeux]);
  valeuroscillo3max.setValue(sauvegarde.angle2Oscillo[numeroTrois]);
  valeurmouvementoscillo1.setValue(sauvegarde.delaisMouvementOscilloMillis[numeroUn] / mille); // si delais mouvement = 0 affichage OFF
  valeurmouvementoscillo2.setValue(sauvegarde.delaisMouvementOscilloMillis[numeroDeux] / mille); // si delais mouvement = 0 affichage OFF
  valeurmouvementoscillo3.setValue(sauvegarde.delaisMouvementOscilloMillis[numeroTrois] / mille); // si delais mouvement = 0 affichage OFF
  valeurdureetempete.setValue(sauvegarde.dureeTempeteMillis / minuteEnMillis); // affiche en minute
  valeurtempete.setValue(sauvegarde.puissanceTempete);
  valeuraccalemienocturne.setValue(sauvegarde.accalmieNocturne);
  valeuraccalemiealimentation.setValue(sauvegarde.accalmieAlimentation);
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page brassage <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page horloge, envoie les variables a afficher */
void affichePageHorloge() {
  uint32_t stockNumerique;
  pageHorloge.show(); // affiche la page horloge
  pageActuelNextion = horloge; // indique a l'arduino que la page actuel est
  affichageH.setValue(variable.Heure);
  affichageM.setValue(variable.minut);
  affichagejour.setValue(jour);
  affichagemois.setValue(mois);
  stockNumerique = annee - deuxMille; // pour ne garder que les 2 derniers chiffres des annees
  affichageannee.setValue(stockNumerique);
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page horloge <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page etalonnage */
void affichePageEtalonnage() {
  pageEtalonnage.show(); // affiche la page graph
  pageActuelNextion = orph; // indique a l'arduino que la page actuel est
  saliniteModeEtalonnage();
  rafraichirMesuresEtalonnage();
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page etalonnage <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page graphique sur 24 heures, envoie les variables a afficher */
void affichePageGraph24H() {
  pageGraph24H.show(); // affiche la page graph
  saliniteModeMesure(); // remet la mesure en mode mesure
  pageActuelNextion = graph24h; // indique a l'arduino que la page actuel est
  for (int8_t i = 0; i >= -23; i--) {
    heureGraph(i);
    if (i == 0) {
      affichageheure12.setText(textime);
    }
    else if (i == -2) {
      affichageheure11.setText(textime);
    }
    else if (i == -4) {
      affichageheure10.setText(textime);
    }
    else if (i == -6) {
      affichageheure9.setText(textime);
    }
    else if (i == -8) {
      affichageheure8.setText(textime);
    }
    else if (i == -10) {
      affichageheure7.setText(textime);
    }
    else if (i == -12) {
      affichageheure6.setText(textime);
    }
    else if (i == -14) {
      affichageheure5.setText(textime);
    }
    else if (i == -16) {
      affichageheure4.setText(textime);
    }
    else if (i == -18) {
      affichageheure3.setText(textime);
    }
    else if (i == -20) {
      affichageheure2.setText(textime);
    }
    else if (i == -22) {
      affichageheure1.setText(textime);
    }
  }
  lisMesuresEnBinaire24h();
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page graph 24h <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page graphique sur 7 jours, envoie les variables a afficher */
void affichePageGraph7J() {
  pageGraph7J.show(); // affiche la page graph
  pageActuelNextion = graph7j; // indique a l'arduino que la page actuel est
  for (int8_t i = 0; i >= -6; i--) {
    dateGraph(i);
    if (i == 0) {
      affichagejour7.setText(textime);
    }
    else if (i == -1) {
      affichagejour6.setText(textime);
    }
    else if (i == -2) {
      affichagejour5.setText(textime);
    }
    else if (i == -3) {
      affichagejour4.setText(textime);
    }
    else if (i == -4) {
      affichagejour3.setText(textime);
    }
    else if (i == -5) {
      affichagejour2.setText(textime);
    }
    else if (i == -6) {
      affichagejour1.setText(textime);
    }
  }
  lisMesuresEnBinaire7J();
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page graph 7j <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page sd */
void affichePageSd() {
  const uint8_t nombre2Ligne = 5; // nombre de ligne dans nextion
  pageSd.show(); // affiche la page graph
  pageActuelNextion = sd; // indique a l'arduino que la page actuel est
  lirePage(PAGE_FIN);
  for (uint8_t i = 1; i <= nombre2Ligne; i++) {
    lirePage(PAGE_PRECEDENTE);
    numero2Ligne = i;
  }
  numero2Ligne = 0;
  NotifOn = false;
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page sd <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page popup */
void affichePagePopup() {
  pagePopup.show(); // affiche la page popup
  pageActuelNextion = popup; // indique a l'arduino que la page actuel est
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page popup <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/*  page alarme */
void affichePageAlarme() {
  uint32_t stockNumerique;
  const uint16_t HeureEnSeconde = 3600u;
  const uint8_t minuteEnSeconde = 60;
  pageAlarme.show(); // affiche la page popup
  pageActuelNextion = alarme; // indique a l'arduino que la page actuel est
  valeuralertetemperaturebasse.setValue(sauvegarde.alertetemperaturebasse * dix); // converti pour avoir 1 seul decimal
  valeuralertetemperaturehaute.setValue(sauvegarde.alertetemperaturehaute * dix); // converti pour avoir 1 seul decimal
  valeurdureeosmolation.setValue(sauvegarde.dureeOsmolationMillis / mille); // affiche la duree en minute
  stockNumerique = sauvegarde.dureeremplissage / minuteEnSeconde; // transforme millis en minute
  valeurdureerenplissage.setValue(stockNumerique); // affiche la duree en minute
  valeuralertephbacbas.setValue(sauvegarde.alertephbacbas);
  valeuralertephbachaut.setValue(sauvegarde.alertephbachaut);
  statutBuzzerOnOffAlarme();
  statutOnOffRAC();
  stockNumerique = sauvegarde.heureDebutAlerte / HeureEnSeconde;// transforme millis en minute
  affichageHdebutalerte.setValue(stockNumerique);
  stockNumerique = sauvegarde.heureFinAlerte / HeureEnSeconde;// transforme millis en minute
  affichageHfinalerte.setValue(stockNumerique);
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page alarme <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}

/* rafraichi l'heure a chaque changement de minute si la page menu est en cours */
void rafraichirHeureSiSurMenu() {
  if (Mprecedent != variable.minut) {
    if  (pageActuelNextion == menu) { // MAJ de l'heure sur nextion a chaque changement de minute.
      affichageHeureNextion();
      DPRINTF("refresh heure");  DPRINTLN(); // debug
      Mprecedent = variable.minut;
    }
  }
}

void rafraichirboutonalimentationmanuel() {
  if (nourissageManuelEnCours) {
    boutonalimentationmanuel.Set_background_image_pic(Ion);
  }
  else {
    boutonalimentationmanuel.Set_background_image_pic(Ioff);
  }
}

/* MAJ des temperature rampe + thermometre rampe si les valeurs on changées */
void rafraichirTemperatureRampeNextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagetemperaturerampe.setValue(variable.temperatureRampe); // affiche la nouvelle valeur
    if (etatVentilationRampe) { // variation de l'icone temperature rampe
      if (statuticonetemperaturerampe == false) {
        iconetemperaturerampe.setPic(IpetitVentOn);
        statuticonetemperaturerampe = true;
        DPRINTF("icone ventilo rampe ON");  DPRINTLN(); // debug
      }
    }
    else {
      if (statuticonetemperaturerampe ) { // variation de l'icone temperature rampe
        iconetemperaturerampe.setPic(IpetitVentOff);
        statuticonetemperaturerampe = false;
        DPRINTF("icone ventilo rampe OFF");  DPRINTLN(); // debug
      }
    }
  }
  DPRINTF("refresh temperature rampe : ")DPRINT(variable.temperatureRampe);  DPRINTLN(); // debug
}

/* MAJ des temperature bac + thermometre bac si les valeurs on changées */
void rafraichirTemperatureBacNextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagetemperaturebac.setValue(variable.temperatureBac * dix); // affiche temperature aquarium x 10 pour avoir la virgule dans nextion
    if (etatVentilationBac && variable.temperatureBac < sauvegarde.alertetemperaturehaute) {// variation de l'icone temperature bac
      if (statutIconeTemperatureBac != IgrandVentOn) {
        iconeventilateurbac.setPic(IgrandVentOn); // affiche la nouvelle icone
        iconemort.setPic(IpoissonmortOff); // affiche la nouvelle icone
        statutIconeTemperatureBac = IgrandVentOn;
      }
    }
    else if (variable.temperatureBac >= sauvegarde.alertetemperaturehaute) {// variation de l'icone temperature bac
      if (statutIconeTemperatureBac != IpoissonmortOn) {
        iconeventilateurbac.setPic(IgrandVentOn); // affiche la nouvelle icone
        iconemort.setPic(IpoissonmortOn); // affiche la nouvelle icone
        statutIconeTemperatureBac = IpoissonmortOn;
      }
    }
    else if (variable.temperatureBac <= sauvegarde.alertetemperaturebasse) {// variation de l'icone temperature bac
      if (statutIconeTemperatureBac != IpoissonmortOn) {
        iconeventilateurbac.setPic(IgrandVentOff); // affiche la nouvelle icone
        iconemort.setPic(IpoissonmortOn); // affiche la nouvelle icone
        statutIconeTemperatureBac = IpoissonmortOn;
      }
    }
    else {
      if (statutIconeTemperatureBac != IgrandVentOff) {// variation de l'icone temperature bac
        iconeventilateurbac.setPic(IgrandVentOff); // affiche la nouvelle icone
        iconemort.setPic(IpoissonmortOff); // affiche la nouvelle icone
        statutIconeTemperatureBac = IgrandVentOff;
      }
    }
    DPRINTF("refresh temperature aquarium : "); DPRINT(variable.temperatureBac);  DPRINTLN(); // debug
  }
  messageAlerteTemperature();
}

/* MAJ de l'eclairage couleur 1 */
void rafraichirEclairageCouleur1Nextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagepourcentcouleur1.setValue(variable.pwmEclairage[Couleur1]);
    DPRINTF("refresh couleur 1");  DPRINTLN(); // debug
  }
}
/* MAJ de l'eclairage couleur 2 */
void rafraichirEclairageCouleur2Nextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagepourcentcouleur2.setValue(variable.pwmEclairage[Couleur2]);
    DPRINTF("refresh couleur 2");  DPRINTLN(); // debug
  }
}

/* MAJ de l'eclairage couleur 3 */
void rafraichirEclairageCouleur3Nextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagepourcentcouleur3.setValue(variable.pwmEclairage[Couleur3]);
    DPRINTF("refresh couleur 3");  DPRINTLN(); // debug
  }
}

/* MAJ de l'eclairage couleur 4 */
void rafraichirEclairageCouleur4Nextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagepourcentcouleur4.setValue(variable.pwmEclairage[Couleur4]);
    DPRINTF("refresh couleur 4");  DPRINTLN(); // debug
  }
}

/* MAJ du Brassage 1 */
void rafraichirBrassage1Nextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagepourcentbrassage1.setValue(variable.pwmBrassage[numeroUn]); // affiche % Brassage 1
    DPRINTF("refresh brassage1");  DPRINTLN(); // debug
  }
}

/* MAJ du Brassage 2 */
void rafraichirBrassage2Nextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagepourcentbrassage2.setValue(variable.pwmBrassage[numeroDeux]); // affiche % Brassage 2
    DPRINTF("refresh brassage2");  DPRINTLN(); // debug
  }
}

/* MAJ du Brassage 3 */
void rafraichirBrassage3Nextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagepourcentbrassage3.setValue(variable.pwmBrassage[numeroTrois]); // affiche % Brassage 3
    DPRINTF("refresh brassage3");  DPRINTLN(); // debug
  }
}

/* MAJ du ph bac */
void rafraichirPhBacNextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichagephbac.setValue(variable.phBac); // affiche le ph du bac
    DPRINTF("refresh ph bac : ");  DPRINTLN(variable.phBac); // debug
  }
}

/* MAJ du redox */
void rafraichirRedoxNextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichageredox.setValue(variable.redox); // affiche le redox
    DPRINTF("refresh Redox : ");  DPRINTLN(variable.redox); // debug
  }
}

/* MAJ de la salinite */
void rafraichirSaliniteNextion() {
  if (pageActuelNextion == menu) { // si sur page menu
    affichageSalinite.setValue(variable.salinite * cent); // affiche la conductivité
    DPRINTF("refresh Salinite : ");  DPRINTLN(variable.salinite); // debug
  }
}

void rafraichirIconeBatterie() {
  const uint8_t margeBatterie = 20; // 20 = marge de 0.097V apres pont diviseur
  uint16_t mesureNiveauBatterie = analogRead(pinInBatterie);
  DPRINTF("mesureNiveauBatterie : "); DPRINT(mesureNiveauBatterie);  DPRINTLN(); // debug
#ifdef TestBatterie
  mesureNiveauBatterie = random(650, 900);
#endif
  if ( mesureNiveauBatterie > (batterieNonConnecte - margeBatterie)) {
    mesureNiveauBatterie = Ibatterie0;
  }
  else if (mesureNiveauBatterie < batterie0) {
    mesureNiveauBatterie = Ibatterie0;
  }
  else {
    mesureNiveauBatterie = map(mesureNiveauBatterie, batterie0, batterie100, Ibatterie0, Ibatterie100);
    DPRINTF(" Map batterie : "); DPRINT(mesureNiveauBatterie);  DPRINTLN(); // debug
  }
  if (mesureNiveauBatterie > Ibatterie84) {
    iconebatterie.setPic(Ipile100); // affiche la nouvelle icone
    DPRINTF("icone batterie 100%");  DPRINTLN(); // debug
  }
  else if (mesureNiveauBatterie > Ibatterie68) {
    iconebatterie.setPic(Ipile84); // affiche la nouvelle icone
    DPRINTF("icone batterie 84%");  DPRINTLN(); // debug
  }
  else if (mesureNiveauBatterie > Ibatterie50) {
    iconebatterie.setPic(Ipile68); // affiche la nouvelle icone
    DPRINTF("icone batterie 68%");  DPRINTLN(); // debug
  }
  else if (mesureNiveauBatterie > Ibatterie32) {
    iconebatterie.setPic(Ipile50); // affiche la nouvelle icone
    DPRINTF("icone batterie 50%");  DPRINTLN(); // debug
  }
  else if (mesureNiveauBatterie > Ibatterie16) {
    iconebatterie.setPic(Ipile32); // affiche la nouvelle icone
    DPRINTF("icone batterie 32%");  DPRINTLN(); // debug
  }
  else if (mesureNiveauBatterie > Ibatterie0) {
    iconebatterie.setPic(Ipile16); // affiche la nouvelle icone
    DPRINTF("icone batterie 16%");  DPRINTLN(); // debug
  }
  else {
    iconebatterie.setPic(Ipile0); // affiche la nouvelle icone
    DPRINTF("icone batterie 0%");  DPRINTLN(); // debug
  }
}

void rafraichirSignalWifi() { // affiche le statut de connection et niveau du signal
  //qualité du signal en DB negatif, -80 signal trop faible, -50DB signal tres fort // convertie en positif
  uint8_t db65 = 65;
  uint8_t db70 = 70;
  uint8_t db75 = 75;
  uint8_t db80 = 80;
  uint8_t db85 = 85;
  executer(RECEVOIRconnectionWifi, true); // demande le statut de connection, signal, IP
  if (connectionWifi.presenceModule ) {
    if (connectionWifi.connecter == false) { // si non connecter
      connectionWifi.puissanceSignal = db85;
      texteProgmemAuNextion(valeurconnecte, texteNextionNonConnecte, orange); // champ, texte, couleur
    }
    else { // si conecte on affiche l'IP
      valeurconnecte.Set_font_color_pco(blanc); // texte couleur vert
      valeurconnecte.setText(connectionWifi.adresseIP);// envoi le texte a l'ecran nextion
      DPRINT(connectionWifi.adresseIP);  DPRINTLN(); // debug
    }
  }
  else {
    connectionWifi.puissanceSignal = db85;
    texteProgmemAuNextion(valeurconnecte, texteNextionModuleNonDetecte, rouge); // champ, texte, couleur
    connectionWifi.connecter = false; // "non connecter au wifi" // passe en true si connecter au reseau wifi
    connectionWifi.presenceModule = false; // "non connecter au wifi" // passe en true si reponse du module
    etatWifi();
  }
  if (connectionWifi.puissanceSignal < db85) { // si signal >
    dbsignalwifi.setPic(Isignal1);
  }
  if (connectionWifi.puissanceSignal < db80) {// si signal >
    dbsignalwifi.setPic(Isignal2);
  }
  if (connectionWifi.puissanceSignal < db75) {// si signal >
    dbsignalwifi.setPic(Isignal3);
  }
  if (connectionWifi.puissanceSignal < db70) {// si signal >
    dbsignalwifi.setPic(Isignal4);
  }
  if (connectionWifi.puissanceSignal < db65) {// si signal >
    dbsignalwifi.setPic(Isignal5);
  }
  DPRINTF("niveau signal Wifi : "); DPRINT(connectionWifi.puissanceSignal);  DPRINTLN(); // debug
}

void rafraichirSignalGSM() { // affiche le statut de connection et niveau du signal
  uint32_t compteurMillis;
  const uint8_t db50 = 31;
  const uint8_t db60 = 26;
  const uint8_t db70 = 21;
  const uint8_t db80 = 16;
  const uint8_t db90 = 11;
  signalGSM = 0;
  if (gsmPrintlnAndWaitATCommand(ATString, OKLongString, 2000, true)) { // 5000 ?!
    effaceBufferTexte();
    memset(nomOperateur, '\0', maxNomOperateur); // effacer buffer
    boolean timeOut = true;
    compteurMillis = millis();
    gsmPrintlnATCommand("AT+COPS?");
    while (millis() - compteurMillis < cinqMille ) {
      ecouterGSM();
      if (nomOperateurRecu()) {
        DPRINTF("operateur recu delais timer : ");  DPRINTLN(((float)millis() - compteurMillis) / 1000); // debug
        timeOut = false;
        texteAuNextion(valeuroperateur, nomOperateur, blanc); // champ, texte, couleur
        break;
      }
    }
    if (timeOut) {
      texteProgmemAuNextion(valeuroperateur, texteNextionNonConnecte , orange); // champ, texte, couleur
    }
    else {
      compteurMillis = millis();
      gsmPrintlnATCommand("AT+CSQ");
      while (millis() - compteurMillis < cinqMille ) {
        ecouterGSM();
        if (signalGSMRecu()) {
          DPRINTF("puissance signal recu delais timer : ");  DPRINTLN(((float)millis() - compteurMillis) / 1000); // debug
          break;
        }
      }
    }
  }
  else {
    texteProgmemAuNextion(valeuroperateur, texteNextionModuleNonDetecte , rouge); // champ, texte, couleur
  }
  if (signalGSM > db90) { // si signal >
    dbsignalgsm.setPic(Isignal1);
  }
  if (signalGSM > db80) {// si signal >
    dbsignalgsm.setPic(Isignal2);
  }
  if (signalGSM > db70) {// si signal >
    dbsignalgsm.setPic(Isignal3);
  }
  if (signalGSM > db60) {// si signal >
    dbsignalgsm.setPic(Isignal4);
  }
  if (signalGSM > db50) {// si signal >
    dbsignalgsm.setPic(Isignal5);
  }
}

void rafraichirMesuresEtalonnage() {
  if (pageActuelNextion == orph) {
    affichageetalonnagephbac.setValue(variable.phBac); // affiche le ph du bac
    affichageetalonnageredox.setValue(variable.redox); // affiche le redox
    affichageetalonnagesalinite.setValue(variable.salinite); // affiche la sali
  }
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< STATUTS DES BOUTONS
/* regarde le statut des fonctions et affiche les bouton en consequence */

/* statut de l'alimentation 1, ON ou OFF */
void statutOnOffAlim1() {
  if (sauvegarde.nourrissageON[numeroUn] ) { // Si nourissage 1 ON
    onoffalimentation1.Set_background_image_pic(Ion); // affiche le bouton ON
    TIMEenTEXTE(sauvegarde.heureNourrissage[numeroUn]); // convertie numerique en char
    affichageheurealimentation1.setText(textime); // affichage de l'heure de la tempete Aleatoire
    DPRINTF("Nourrissage 1 ON");  DPRINTLN(); // debug //debug
  }
  else { // si nourissage 1 OFF
    onoffalimentation1.Set_background_image_pic(Ioff); // affiche le bouton OFF
    texteProgmemAuNextion(affichageheurealimentation1, texteVide , pas2Changement2Couleur); // champ, texte, couleur
    DPRINTF("Nourrissage 1 OFF");  DPRINTLN(); // debug //debug
  }
}

/* statut de l'alimentation 2, ON ou OFF */
void statutOnOffAlim2() {
  if (sauvegarde.nourrissageON[numeroDeux] ) { // si nourissage 2 ON
    onoffalimentation2.Set_background_image_pic(Ion); // affiche le bouton ON
    TIMEenTEXTE(sauvegarde.heureNourrissage[numeroDeux]); // convertie numerique en char
    affichageheurealimentation2.setText(textime); // affichage de l'heure de la tempete Aleatoire
    DPRINTF("Nourrissage 2 ON");  DPRINTLN(); // debug //debug
  }
  else { // si nourissage OFF
    onoffalimentation2.Set_background_image_pic(Ioff); // affiche le bouton OFF
    texteProgmemAuNextion(affichageheurealimentation2, texteVide , pas2Changement2Couleur); // champ, texte, couleur
    DPRINTF("Nourrissage 2 OFF");  DPRINTLN(); // debug //debug
  }
}

/* statut pompe remonté */
void statutOnOffDelay() {
  if (ChangementDEauON) {
    onoffpomperemonte.Set_background_image_pic(Ioff); // affiche le bouton OFF
    DPRINTF("remonté en off // changement d'eau");  DPRINTLN(); // debug //debug
  }
  else {
    if (sauvegarde.remonteDelay == Delay) {
      onoffpomperemonte.Set_background_image_pic(Ipompedelay); // affiche le bouton delay
      DPRINTF("remonté en delay");  DPRINTLN(); // debug //debug
    }
    else if (sauvegarde.remonteDelay == Actif) {
      onoffpomperemonte.Set_background_image_pic(Ion); // affiche le bouton ON
      DPRINTF("remonté en on");  DPRINTLN(); // debug //debug
    }
    else if (sauvegarde.remonteDelay == Arret) {
      onoffpomperemonte.Set_background_image_pic(Ioff); // affiche le bouton OFF
      DPRINTF("remonté en off");  DPRINTLN(); // debug //debug
    }
  }
}

/* statut tempete aleatoire, ON ou OFF */
void statutTempeteAleatoire() {
  if (pageActuelNextion == menu) { // si sur page menu
    if (sauvegarde.tempeteAleatoireOn ) { // Si tempete aleatoir ON
      onofftempetealeatoire.Set_background_image_pic(Ion); // affiche bouton ON
      tempeteNextion();// ecrit heure et duree tout deux determiné aleatoirement
      DPRINTF("tempete aléatoir ON");  DPRINTLN(); // debug //debug
    }
    else { // Si tempete aleatoir OFF
      onofftempetealeatoire.Set_background_image_pic(Ioff); // affiche bouton OFF
      texteProgmemAuNextion(affichageheuretempete, texteVide , pas2Changement2Couleur); // champ, texte, couleur
      texteProgmemAuNextion(affichagedureetempete, texteVide , pas2Changement2Couleur); // champ, texte, couleur
      DPRINTF("tempete aléatoir OFF");  DPRINTLN(); // debug //debug
    }
  }
}

/* statut eclairage couleur1, ON, PWM ou OFF */
void statutCouleur1() {
  if (sauvegarde.eclairageOnOffPwm[Couleur1] == Actif) { // si ON
    onoffpwmcouleur1.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("couleur1 on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur1] == Arret) { // si OFF
    onoffpwmcouleur1.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("couleur1 off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur1] == Pwm) { // si pwm
    onoffpwmcouleur1.Set_background_image_pic(Ipwm); // affiche bouton PWM
    DPRINTF("couleur1 pwm");  DPRINTLN(); // debug //debug
  }
}

/* statut eclairage couleur2, ON, PWM ou OFF */
void statutCouleur2() {
  if (sauvegarde.eclairageOnOffPwm[Couleur2] == Actif) { // si ON
    onoffpwmcouleur2.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("couleur2 on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur2] == Arret) { // si OFF
    onoffpwmcouleur2.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("couleur2 off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur2] == Pwm) { // Si PWM
    onoffpwmcouleur2.Set_background_image_pic(Ipwm); // affiche bouton PWM
    DPRINTF("couleur2 pwm");  DPRINTLN(); // debug //debug
  }
}

/* statut eclairage couleur3, ON, PWM ou OFF */
void statutCouleur3() {
  if (sauvegarde.eclairageOnOffPwm[Couleur3] == Actif) { // si ON
    onoffpwmcouleur3.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("couleur3 on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur3] == Arret) { // si OFF
    onoffpwmcouleur3.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("couleur3 off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur3] == Pwm) { // si pwm
    onoffpwmcouleur3.Set_background_image_pic(Ipwm); // affiche bouton PWM
    DPRINTF("couleur3 pwm");  DPRINTLN(); // debug //debug
  }
}

/* statut eclairage couleur4, ON, PWM ou OFF */
void statutCouleur4() {
  if (sauvegarde.eclairageOnOffPwm[Couleur4] == Actif) { // si ON
    onoffpwmcouleur4.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("couleur4 on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur4] == Arret) { // si OFF
    onoffpwmcouleur4.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("couleur4 off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.eclairageOnOffPwm[Couleur4] == Pwm) { // Si PWM
    onoffpwmcouleur4.Set_background_image_pic(Ipwm); // affiche bouton PWM
    DPRINTF("couleur4 pwm");  DPRINTLN(); // debug //debug
  }
}

/* statut Brassage 1, ON, PWM ou OFF */
void statutBrassage1() {
  if (sauvegarde.BrassageOnOffPwm[numeroUn] == Actif) { // Si ON
    onoffpwmbrassage1.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("Brassage1 on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.BrassageOnOffPwm[numeroUn] == Arret) { // si OFF
    onoffpwmbrassage1.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("Brassage1 off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.BrassageOnOffPwm[numeroUn] == Pwm) { // si PWM
    onoffpwmbrassage1.Set_background_image_pic(Iwave); // affiche bouton PWM
    DPRINTF("Brassage1 pwm");  DPRINTLN(); // debug //debug
  }
}

/* statut Brassage 2, ON, PWM ou OFF */
void statutBrassage2() {
  if (sauvegarde.BrassageOnOffPwm[numeroDeux] == Actif) { // si ON
    onoffpwmbrassage2.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("Brassage2 on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.BrassageOnOffPwm[numeroDeux] == Arret) { // si OFF
    onoffpwmbrassage2.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("Brassage2 off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.BrassageOnOffPwm[numeroDeux] == Pwm) { // si PWM
    onoffpwmbrassage2.Set_background_image_pic(Iwave); // affiche bouton PWM
    DPRINTF("Brassage2 pwm");  DPRINTLN(); // debug //debug
  }
}

/* statut Brassage 3, ON, PWM ou OFF */
void statutBrassage3() {
  if (sauvegarde.BrassageOnOffPwm[numeroTrois] == Actif) { // si ON
    onoffpwmbrassage3.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("Brassage3 on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.BrassageOnOffPwm[numeroTrois] == Arret) { // si OFF
    onoffpwmbrassage3.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("Brassage3 off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.BrassageOnOffPwm[numeroTrois] == Pwm) { // si PWM
    onoffpwmbrassage3.Set_background_image_pic(Iwave); // affiche bouton PWM
    DPRINTF("Brassage3 pwm");  DPRINTLN(); // debug //debug
  }
}

/* statut refuge, ON, time ou OFF */
void statutRefuge() {
  if (sauvegarde.RefugeOnOffTimer == Actif) { // si ON
    onoffrefuge.Set_background_image_pic(Ion); // affiche bouton ON
    DPRINTF("refuge on");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.RefugeOnOffTimer == Arret) { // si OFF
    onoffrefuge.Set_background_image_pic(Ioff); // affiche bouton OFF
    DPRINTF("refuge off");  DPRINTLN(); // debug //debug
  }
  else if (sauvegarde.RefugeOnOffTimer == Timer) { // si time
    onoffrefuge.Set_background_image_pic(Itimer); // affiche bouton PWM
    DPRINTF("refuge time");  DPRINTLN(); // debug //debug
  }
}

/* statut tempete manuel, ON ou OFF */
void statutTempete() {
  if (pageActuelNextion == menu) { // si sur page menu
    if (tempeteManuelEnCours ) { // si ON
      onofftempete.Set_background_image_pic(Ion); // affiche le bouton ON
      DPRINTF("tempete on");  DPRINTLN(); // debug //debug
    }
    else { // si OFF
      onofftempete.Set_background_image_pic(Ioff); // affiche bouton OFF
      DPRINTF("tempete off");  DPRINTLN(); // debug //debug
    }
  }
}

/* statut icone tempete aleatoire, ON ou OFF */
void statutIconeTempeteAleatoire() {
  if (pageActuelNextion == menu) { // si sur page menu
    if (MajEcranTempeteAleatoireEnCours ) { // si ON
      iconetempetealeatoire.setPic(ItempeteOn); // affiche le bouton ON
      DPRINTF("tempete aleatoire on");  DPRINTLN(); // debug //debug
    }
    else { // si OFF
      iconetempetealeatoire.setPic(ItempeteOff); // affiche bouton OFF
      DPRINTF("tempete aleatoire off");  DPRINTLN(); // debug //debug
    }
  }
}

/* statut changement d'eau, ON ou OFF */
void statutChangementDEau() {
  if (ChangementDEauON) { // changement d'eau On
    onoffchangementdeau.Set_background_image_pic(Ion); // affiche le bouton ON
    DPRINTF("changement eau on");  DPRINTLN(); // debug //debug
  }
  else {
    onoffchangementdeau.Set_background_image_pic(Ioff); // affiche le bouton Off
    DPRINTF("changement eau off");  DPRINTLN(); // debug //debug
  }
}

/* statut Buzzer, ON ou OFF */
void statutBuzzerOnOffMenu() {
  if (sauvegarde.BuzzerOn) { // buzzer On
    onoffbuzzermenu.Set_background_image_pic(Ion); // affiche le bouton ON
    DPRINTF("buzzer on");  DPRINTLN(); // debug //debug
  }
  else {
    onoffbuzzermenu.Set_background_image_pic(Ioff); // affiche le bouton Off
    DPRINTF("buzzer off");  DPRINTLN(); // debug //debug
  }
}

/* statut Buzzer, ON ou OFF */
void statutBuzzerOnOffAlarme() {
  if (sauvegarde.BuzzerOn) { // buzzer On
    onoffbuzzeralarme.Set_background_image_pic(Ion); // affiche le bouton ON
    DPRINTF("buzzer on");  DPRINTLN(); // debug //debug
  }
  else {
    onoffbuzzeralarme.Set_background_image_pic(Ioff); // affiche le bouton Off
    DPRINTF("buzzer off");  DPRINTLN(); // debug //debug
  }
}

/* statut RAC, ON ou OFF */
void statutOnOffRAC() {
  if (sauvegarde.PhRAC) { // buzzer On
    onoffrac.Set_background_image_pic(Ion); // affiche le bouton ON
    DPRINTF("onoffrac on");  DPRINTLN(); // debug //debug
  }
  else {
    onoffrac.Set_background_image_pic(Ioff); // affiche le bouton Off
    DPRINTF("onoffrac off");  DPRINTLN(); // debug //debug
  }
}

/* statut notif, ON ou OFF */
void statutNotifOnOff() {
  if (NotifOn) { // notif On
    onoffnotif.Set_background_image_pic(INotifOn); // affiche icone ON
    DPRINTF("notif on");  DPRINTLN(); // debug //debug
  }
  else {
    onoffnotif.Set_background_image_pic(INotifOff); // affiche icone Off
    DPRINTF("notif off");  DPRINTLN(); // debug //debug
  }
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< boutons et fonction associé
/* liste des boutons */
NexTouch *nex_listen_list[] = {
  // boutons page menu
  &onoffpwmcouleur1, &onoffpwmcouleur2, &onoffpwmcouleur3, &onoffpwmcouleur4, &onoffpwmbrassage1, &onoffpwmbrassage2, &onoffpwmbrassage3, &onofftempete,
  &onofftempetealeatoire, &boutonalimentationmanuel, &onoffalimentation1, &onoffalimentation2, &onoffpomperemonte, &onoffchangementdeau, &onoffbuzzermenu,
  &boutonverseclairage, &boutonversbrassage, &boutonversetalonnage, &boutonversgraph24h, &boutonversconfig, &boutonversstandby, &onoffrefuge,
  &onoffnotif,
  // boutons page config
  &boutonverstel, &boutonverswifi, &boutonversmenu, &boutonversinfo, &boutonvershorloge, &boutonenregistrerconfig, &boutonredemarrage, &boutonversalarme,
  &boutonrestaureeeprom, &boutonadressesonde,
  // boutons page wifi
  &boutonenregistrerwifi,
  // boutons page tel
  &boutonenregistrertel,
  // boutons page Brassage
  &boutonversmenu, &boutonenregistrerbrassage, &valeuroscillo1min, &valeuroscillo1max, &valeuroscillo2min, &valeuroscillo2max, &valeuroscillo3min, &valeuroscillo3max,
  &valeurminBrassage1, &valeurminBrassage2, &valeurminBrassage3,
  // boutons page horloge
  &boutonenregistrerhorloge,
  // boutons page standby
  &pagestandby,
  // boutons page graph 24h
  &boutonversgraph7j,
  // boutons page orph&boutonoui,
  &boutoncalibrer,
  // boutons page sd
  &boutonhaut, &boutonbas, &boutonclear,
  // boutons page eclairage
  &boutonenregistrereclairage,
  // boutons page alarme
  &onoffbuzzeralarme, &boutonenregistreralarme, &onoffrac,
  NULL
};

/* Regarde si un bouton a été actionné, a mettre dans le loop */
void lectureBoutons() {
  nexLoop(nex_listen_list);
}

/* affiche la page demandé selon le bouton appuyer */
void versConfig(void *ptr) {
  affichePageConfig(); // affiche la page
}
void versInfo(void *ptr) {
  affichePageInfo(); // affiche la page
}
void versTel(void *ptr) {
  affichePageTel(); // affiche la page
}
void versEclairage(void *ptr) {
  affichePageEclairage(); // affiche la page
}
void versWifi(void *ptr) {
  affichePageWifi(); // affiche la page
}
void versMenu(void *ptr) {
  affichePageMenu(); // affiche la page
}
void versBrassage(void *ptr) {
  affichePageBrassage(); // affiche la page
}
void versHorloge(void *ptr) {
  affichePageHorloge(); // affiche la page
}
void versAlarme(void *ptr) {
  affichePageAlarme(); // affiche la page
}
void versGraph24H(void *ptr) {
  affichePageGraph24H(); // affiche la page
}
void versGraph7J(void *ptr) {
  affichePageGraph7J(); // affiche la page
}
void versStandby(void *ptr) {
  pagestandby.show();  // affiche la page standby
  pageActuelNextion = standby; // indique a l'arduino que la page actuel est standby
  DPRINTF(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Page Standby <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"); DPRINTLN(); // debug
}
void versEtalonnage(void *ptr) {
  affichePageEtalonnage(); // affiche la page
}
void versSd(void *ptr) {
  affichePageSd(); // affiche la page
}
void versPopup(void *ptr) {
  affichePagePopup(); // affiche la page
}

/* le bouton associé est appuyer */
void boutonOnOffRemonte(void *ptr) {
  if (ChangementDEauON == false) {
    variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
    if (number == VRemonteDelay) { // Si DELAY
#ifdef multiPrise
      digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#else
      digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#endif
      sauvegarde.remonteDelay = Delay; // remonté ON, coupure de la remonté lors du nourrissage
      DPRINTF("remonté delais");  DPRINTLN(); // debug
    }
    else if (number == VRemonteOn ) { // si ON
#ifdef multiPrise
      digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#else
      digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#endif
      sauvegarde.remonteDelay = Actif; // remonté ON, pas de coupure de la remonté lors du nourrissage
      DPRINTF("remonté ON");  DPRINTLN(); // debug //debug
    }
    else if (number == VRemonteOff) { // si OFF
#ifdef multiPrise
      digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#else
      digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#endif
      sauvegarde.remonteDelay = Arret; // remonté OFF
      DPRINTF("remonté OFF");  DPRINTLN(); // debug
    }
    else {
      DPRINTF("Probleme Valeur Variable remonté : "); DPRINT(number); DPRINTLN(); // debug
    }
    statutOnOffDelay();
  }
}

/* le bouton associé est appuyer */
void boutonNourrissageManuel(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
  if (number == VAlimMOn) { // si ON
    nourissageManuelEnCours = true;
    nourissageEnCours = true; // boolean pour ne pas avoir de nourrissage au demarrage
    tempsPrecedentNourrissage = millis(); // demarre le compteur de duree du nourissage
    if (sauvegarde.remonteDelay == Delay) { // si remonté en delay
#ifdef multiPrise
      digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#else
      digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#endif
      DPRINTF("pompe de remonté OFF");  DPRINTLN(); // debug //debug
    }
    DPRINTF("nourrissage manuel On");  DPRINTLN(); // debug //debug
  }
  else if (number == VAlimMOff) { //si OFF
    nourissageManuelEnCours = false;
    nourissageEnCours = false; // boolean pour ne pas avoir de nourrissage au demarrage
    tempsPrecedentNourrissage = sauvegarde.dureeNourrissageMillis + 1; // stop le compteur de duree nourissage
#ifdef multiPrise
    digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#else
    digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#endif
    DPRINTF("nourrissage manuel Off");  DPRINTLN(); // debug //debug
  }
  iconNourrissageEnCours(); // affiche icone nourissage
  rafraichirboutonalimentationmanuel();
  DPRINTF("Probleme Valeur Variable nourrissage manuel : "); DPRINT(number);  DPRINTLN(); // debug //debug
}

/* le bouton associé est appuyer */
void boutonOnOffAlim1(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
  if (number == VAlim1On ) { // si bouton ON
    sauvegarde.nourrissageON[numeroUn] = true; // indique ON nourissage 1
  }
  else if (number == VAlim1Off ) { // si bouton OFF
    sauvegarde.nourrissageON[numeroUn] = false; // indique OFF nourissage 1
  }
  statutOnOffAlim1();
}

/* le bouton associé est appuyer */
void boutonOnOffAlim2(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
  if (number == VAlim2On ) { // si bouton ON
    sauvegarde.nourrissageON[numeroDeux] = true; // indique ON nourissage 2
  }
  else if (number == VAlim2Off ) { // si bouton OFF
    sauvegarde.nourrissageON[numeroDeux] = false; // indique OFF nourissage 2
  }
  statutOnOffAlim2();
}

/* le bouton associé est appuyer */
void boutonCouleur1(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
  if (number == VCouleur1PWM) { // si pwm
    sauvegarde.eclairageOnOffPwm[Couleur1] = Pwm;
    DPRINTF("couleur1 PWM");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur1On) { // si ON
    sauvegarde.eclairageOnOffPwm[Couleur1] = Actif;
    DPRINTF("couleur1 ON");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur1Off) { // si OFF
    sauvegarde.eclairageOnOffPwm[Couleur1] = Arret;
    DPRINTF("couleur1 OFF");  DPRINTLN(); // debug //debug
  }
  statutCouleur1();
  eclairage();
  affichagepourcentcouleur1.setValue(variable.pwmEclairage[Couleur1]); // affiche % couleur1
}

/* le bouton associé est appuyer */
void boutonCouleur2(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
  if (number == VCouleur2PWM) { // si pwm
    sauvegarde.eclairageOnOffPwm[Couleur2] = Pwm;
    DPRINTF("couleur2 PWM");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur2On) { // si ON
    sauvegarde.eclairageOnOffPwm[Couleur2] = Actif;
    DPRINTF("couleur2 ON");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur2Off) { // si OFF
    sauvegarde.eclairageOnOffPwm[Couleur2] = Arret;
    DPRINTF("couleur2 OFF");  DPRINTLN(); // debug //debug
  }
  statutCouleur2();
  eclairage();
  affichagepourcentcouleur2.setValue(variable.pwmEclairage[Couleur2]); // affiche % couleur2
}

/* le bouton associé est appuyer */
void boutonCouleur3(void *ptr) {
  variableetatboutonsmenu.getValue(&number);  // lit le statut du bouton
  if (number == VCouleur3PWM) { // si pwm
    sauvegarde.eclairageOnOffPwm[Couleur3] = Pwm;
    DPRINTF("couleur3 PWM");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur3On) { // si On
    sauvegarde.eclairageOnOffPwm[Couleur3] = Actif;
    DPRINTF("couleur3 ON");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur3Off) { // si Off
    sauvegarde.eclairageOnOffPwm[Couleur3] = Arret;
    DPRINTF("couleur3 OFF");  DPRINTLN(); // debug //debug
  }
  statutCouleur3();
  eclairage();
  affichagepourcentcouleur3.setValue(variable.pwmEclairage[Couleur3]); // affiche % couleur3
}

/* le bouton associé est appuyer */
void boutonCouleur4(void *ptr) {
  variableetatboutonsmenu.getValue(&number);  // lit le statut du bouton
  if (number == VCouleur4PWM) { // si pwm
    sauvegarde.eclairageOnOffPwm[Couleur4] = Pwm;
    DPRINTF("couleur4 PWM");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur4On) { // si On
    sauvegarde.eclairageOnOffPwm[Couleur4] = Actif;
    DPRINTF("couleur4 ON");  DPRINTLN(); // debug //debug
  }
  else if (number == VCouleur4Off) { // si Off
    sauvegarde.eclairageOnOffPwm[Couleur4] = Arret;
    DPRINTF("couleur4 OFF");  DPRINTLN(); // debug //debug
  }
  statutCouleur4();
  eclairage();
  affichagepourcentcouleur4.setValue(variable.pwmEclairage[Couleur4]); // affiche % couleur4
}

/* le bouton associé est appuyer */
void boutonBrassage1(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
  if (number == VBrassage1PWM) { // si pwm
    sauvegarde.BrassageOnOffPwm[numeroUn] = Pwm;
    DPRINTF("Brassage 1 PWM");  DPRINTLN(); // debug //debug
  }
  else if (number == VBrassage1On) { // si On
    sauvegarde.BrassageOnOffPwm[numeroUn] = Actif;
    DPRINTF("Brassage 1 ON");  DPRINTLN(); // debug //debug
  }
  else if (number == VBrassage1Off) { // si Off
    sauvegarde.BrassageOnOffPwm[numeroUn] = Arret;
    DPRINTF("Brassage 1 OFF");  DPRINTLN(); // debug //debug
  }
  statutBrassage1();
  brassage();
  rafraichirBrassage1Nextion();
}

/* le bouton associé est appuyer */
void boutonBrassage2(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit le statut du bouton
  if (number == VBrassage2PWM) { // si pwm
    sauvegarde.BrassageOnOffPwm[numeroDeux] = Pwm;
    DPRINTF("Brassage 2 PWM");  DPRINTLN(); // debug //debug
  }
  else if (number == VBrassage2On) { // si On
    sauvegarde.BrassageOnOffPwm[numeroDeux] = Actif;
    DPRINTF("Brassage 2 ON");  DPRINTLN(); // debug //debug
  }
  else if (number == VBrassage2Off) { // si Off
    sauvegarde.BrassageOnOffPwm[numeroDeux] = Arret;
    DPRINTF("Brassage 2 OFF");  DPRINTLN(); // debug //debug
  }
  statutBrassage2();
  brassage();
  rafraichirBrassage2Nextion();
}

/* le bouton associé est appuyer */
void boutonBrassage3(void *ptr) {
  variableetatboutonsmenu.getValue(&number);  // lit le statut du bouton
  if (number == VBrassage3PWM) { // si pwm
    sauvegarde.BrassageOnOffPwm[numeroTrois] = Pwm;
    DPRINTF("Brassage 3 PWM");  DPRINTLN(); // debug //debug
  }
  else if (number == VBrassage3On) { // si On
    sauvegarde.BrassageOnOffPwm[numeroTrois] = Actif;
    DPRINTF("Brassage 3 ON");  DPRINTLN(); // debug //debug
  }
  else if (number == VBrassage3Off) { // si off
    sauvegarde.BrassageOnOffPwm[numeroTrois] = Arret;
    DPRINTF("Brassage 3 OFF");  DPRINTLN(); // debug //debug
  }
  statutBrassage3();
  brassage();
  rafraichirBrassage3Nextion();
}

/* le bouton associé est appuyer */
void boutonRefuge(void *ptr) {
  variableetatboutonsmenu.getValue(&number);  // lit le statut du bouton
  if (number == VRefugeTimer) { // si time
    sauvegarde.RefugeOnOffTimer = Timer;
    DPRINTF("refuge timer");  DPRINTLN(); // debug //debug
  }
  else if (number == VRefugeOn) { // si On
    sauvegarde.RefugeOnOffTimer = Actif;
    DPRINTF("refuge on");  DPRINTLN(); // debug //debug
  }
  else if (number == VRefugeOff) { // si off
    sauvegarde.RefugeOnOffTimer = Arret;
    DPRINTF("refuge off");  DPRINTLN(); // debug //debug
  }
  else {
    DPRINTF("Probleme Valeur refuge");  DPRINTLN(); // debug //debug
  }
  statutRefuge();
  refuge();
}

/* déclenche une tempete manuelement */
void boutonTempeteManuel(void *ptr) {
  if (nourissageEnCours == false) {
    variableetatboutonsmenu.getValue(&number); // lit la valeur du bouton
    if (number == VTempeteMOn) { // si ON
      tempsPrecedentTempete = millis(); // demarre le compteur
      tempeteManuelEnCours = true;
      roulementPrecedent1 = millis(); // reinitialise le compteur
      DPRINTF("tempete manuel déclanché");  DPRINTLN(); // debug //debug
    }
    else if (number == VTempeteMOff) { // si OFF
      tempeteManuelEnCours = false; // tempete OFF
      DPRINTF("tempete arrété manuelement");  DPRINTLN(); // debug //debug
      tempeteManuelEnCours = false;
      progressionPuissanceBrassage = false;
      tempeteAleatoireEnCours = false; // tempette aleatoir OFF si etait en ON
    }
    statutTempete();
    brassage();  // lance la fonction pour affecter les nouveau parametres
    rafraichirBrassage1Nextion(); // MAJ de l'ecran
    rafraichirBrassage2Nextion(); // MAJ de l'ecran
    rafraichirBrassage3Nextion(); // MAJ de l'ecran
  }
}

/* bouton active la tempete Aleatoire */
void boutonTempeteAleatoire(void *ptr) {
  variableetatboutonsmenu.getValue(&number); // lit la valeur du bouton
  if (number == VTempeteAOn) { // si ON
    sauvegarde.tempeteAleatoireOn = true; // tempete aleatoir ON
    definiTempeteAleatoire(); // defini horraire et duree de la tempette aleatoir
    DPRINTF("tempete aléatoir ON");  DPRINTLN(); // debug
  }
  else if (number == VTempeteAOff) { // si OFF
    sauvegarde.tempeteAleatoireOn = false; // tempete aleatoir OFF
    dureeTempeteAleatoire = 0; // remise a zero de la duree de tempete aleatoir
    tempeteAleatoireEnCours = false; // tempete aleatoir en cours OFF
    brassage(); // lance la fonction pour affecter les nouveau parametres
    DPRINTF("tempete aléatoir OFF");  DPRINTLN(); // debug
  }
  statutTempeteAleatoire();
}

/* lit la valeur du bouton et modifie la puissance de la pompe a cette valeur */
void boutonminBrassage1(void *ptr) {
  valeurminBrassage1.getValue(&number); // recupere valeur
  analogWrite(pinOutBrassage[numeroUn], number); // pompe a la puissance recuperé
  DPRINTF("valeurminBrassage1 : "); DPRINT(number);  DPRINTLN(); // debug
}

/* lit la valeur du bouton et modifie la puissance de la pompe a cette valeur */
void boutonminBrassage2(void *ptr) {
  valeurminBrassage2.getValue(&number); // recupere valeur
  analogWrite(pinOutBrassage[numeroDeux], number); // pompe a la puissance recuperé
  DPRINTF("valeurminBrassage2 : "); DPRINT(number);  DPRINTLN(); // debug
}

/* lit la valeur du bouton et modifie la puissance de la pompe a cette valeur */
void boutonminBrassage3(void *ptr) {
  valeurminBrassage3.getValue(&number); // recupere valeur
  analogWrite(pinOutBrassage[numeroTrois], number); // pompe a la puissance recuperé
  DPRINTF("valeurminBrassage3 : "); DPRINT(number);  DPRINTLN(); // debug
}

/* lit la valeur du bouton et l'affiche a l'ecran puis deplace l'oscillateur a cette valeur */
void boutonoscillo1min(void *ptr) {
  valeuroscillo1min.getValue(&number); // recupere valeur
  boutonoscillo1(number); // deplace l'oscillateur
}

/* lit la valeur du bouton et l'affiche a l'ecran puis deplace l'oscillateur a cette valeur */
void boutonoscillo1max(void *ptr) {
  valeuroscillo1max.getValue(&number); // recupere valeur
  boutonoscillo1(number); // deplace l'oscillateur
}

/* lit la valeur du bouton et l'affiche a l'ecran puis deplace l'oscillateur a cette valeur */
void boutonoscillo2min(void *ptr) {
  valeuroscillo2min.getValue(&number); // recupere valeur
  boutonoscillo2(number); // deplace l'oscillateur
}

/* lit la valeur du bouton et l'affiche a l'ecran puis deplace l'oscillateur a cette valeur */
void boutonoscillo2max(void *ptr) {
  valeuroscillo2max.getValue(&number); // recupere valeur
  boutonoscillo2(number); // deplace l'oscillateur
}

/* lit la valeur du bouton et l'affiche a l'ecran puis deplace l'oscillateur a cette valeur */
void boutonoscillo3min(void *ptr) {
  valeuroscillo3min.getValue(&number); // recupere valeur
  boutonoscillo3(number); // deplace l'oscillateur
}

/* lit la valeur du bouton et l'affiche a l'ecran puis deplace l'oscillateur a cette valeur */
void boutonoscillo3max(void *ptr) {
  valeuroscillo3max.getValue(&number); // recupere valeur
  boutonoscillo3(number); // deplace l'oscillateur
}

/* arret de la pompe de remonté + ecumeur + thermometre + osmolation*/
void boutonChangementDEau(void *ptr) {
  if (ChangementDEauON == false) {
    ChangementDEauON = true; // changement d'eau On
    digitalWrite(pinOutRelaisOsmolation, LOW); // on coupe l'osmolation
#ifdef multiPrise
    digitalWrite(pinOutRelaisEcumeur, LOW);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté OFF
    digitalWrite(pinOutRelaisChauffage, LOW);// pompe remonté off
#else
    digitalWrite(pinOutRelaisEcumeur, HIGH);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté OFF
    digitalWrite(pinOutRelaisChauffage, HIGH);// pompe remonté off
#endif
    DPRINTF("changement d'eau On"); DPRINTLN(); // debug
  }
  else if (ChangementDEauON) { // changement d'eau OFF
    ChangementDEauON = false; // changement d'eau Off
#ifdef multiPrise
    digitalWrite(pinOutRelaisEcumeur, HIGH);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté OFF
    digitalWrite(pinOutRelaisChauffage, HIGH);// pompe remonté off
#else
    digitalWrite(pinOutRelaisEcumeur, LOW);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté OFF
    digitalWrite(pinOutRelaisChauffage, LOW);// pompe remonté off
#endif
    DPRINTF("changement d'eau Off"); DPRINTLN(); // debug
  }
  affichePageMenu();
}

/* active desactive le buzzer */
void boutonBuzzerMenu (void *ptr) {
  if (sauvegarde.BuzzerOn == false) {
    sauvegarde.BuzzerOn = true;
    soundBuzzerOn();
  }
  else if (sauvegarde.BuzzerOn) {
    sauvegarde.BuzzerOn = false;
  }
  affichePageMenu();
}

/* active desactive le buzzer */
void boutonBuzzerAlarme (void *ptr) {
  onoffbuzzeralarme.Get_background_image_pic(&number); // recupere valeur
  DPRINTF("onoffbuzzeralarme : "); DPRINT(number); DPRINTLN(); // debug
  if (number == VBuzzerOn) { // si ON
    sauvegarde.BuzzerOn = true;
    DPRINTF(" sauvegarde.BuzzerOn = true "); DPRINTLN(); // debug
  }
  else {
    sauvegarde.BuzzerOn = false;
    DPRINTF(" sauvegarde.BuzzerOn = false "); DPRINTLN(); // debug
  }
  statutBuzzerOnOffAlarme();
  soundBuzzerOn();
}

/* fait sonner ou non le buzzer */
void soundBuzzerOn() {
  if (sauvegarde.BuzzerOn) {
    TimerFreeTone(pinOutbuzzer, 2000, 300); // envoie les notes
  }
  else if (sauvegarde.BuzzerOn == false) {
  }
}

/* active desactive la gestion RAC */
void boutonPhRAC(void *ptr) {
  onoffrac.Get_background_image_pic(&number); // recupere valeur
  if (number == Ioff) { // si ON
    sauvegarde.PhRAC = true;
    DPRINTF(" sauvegarde.PhRAC = true "); DPRINTLN(); // debug
  }
  else {
    sauvegarde.PhRAC = false;
    DPRINTF(" sauvegarde.PhRAC = false "); DPRINTLN(); // debug
  }
  statutOnOffRAC();
}

/* lit la valeur du bouton pour savoir quelle sonde calibrer et sauvegarde*/
void sondeACalibrer(void *ptr) {
  DPRINTF("etalonnage en cours"); DPRINTLN(); // debug
  uint16_t bufnumber = 0;
  float bufnumberfloat = 0;
  uint8_t bufquelleSonde;
  uint8_t etape = 1;
  const uint8_t totalEtape = 5;
  const uint8_t delaisprograissbar = 150;
  enum { ERREUR = 0, PHSET = 1, ORPSET = 2, ECSET = 3, PHLOW = 11, PHMIDDLE = 12, PHHIGH = 13 } ; // valeur dans nextion
  statutprogressbar(etape, totalEtape, orph);
  etape++;
  variableetalonnage.getValue(&number); // recupere valeur
  uint8_t quelleSonde = number;
  DPRINTF("quelleSonde : "); DPRINT(quelleSonde); DPRINTLN(); // debug
  delay(delaisprograissbar);// delay entre les deux demandes + delay pour la progress bar
  statutprogressbar(etape, totalEtape, orph);
  etape++;
  if (quelleSonde > ECSET) {
    bufquelleSonde = PHSET;
  }
  else {
    bufquelleSonde = quelleSonde;
  }
  statutprogressbar(etape, totalEtape, orph);
  etape++;
  delay(delaisprograissbar);// delay pour la progress bar
  switch (bufquelleSonde) {
    case ERREUR: // si la variable n'ai pas recu correctement
      DPRINTF("variable de quelleSonde non recu"); DPRINTLN(); // debug
      break;
    case PHSET: // etalonnage PH
      valeuretalonnagePh.getValue(&number);// recupere la valeur dans nextion
      DPRINTF("number: "); DPRINT(number); DPRINTLN(); // debug
      bufnumberfloat = (float)number / cent;
      DPRINTF("la sonde Ph a ete calibrer a : "); DPRINT(bufnumberfloat); DPRINTLN(); // debug
      break;
    case ORPSET: // etalonnage ORP
      valeuretalonnageRedox.getValue(&number);// recupere la valeur dans nextion
      DPRINTF("number: "); DPRINT(number); DPRINTLN(); // debug
      bufnumber = number;
      DPRINTF("la sonde ORP a ete calibrer a : "); DPRINT(bufnumber); DPRINTLN(); // debug
      break;
    case ECSET: // etalonnage EC
      valeuretalonnageSalinite.getValue(&number);// recupere la valeur dans nextion
      DPRINTF("number: "); DPRINT(number); DPRINTLN(); // debug
      bufnumber = number;
      DPRINTF("la sonde EC a ete calibrer a : "); DPRINT(bufnumber); DPRINTLN(); // debug
      break;
  }
  statutprogressbar(etape, totalEtape, orph);
  etape++;
  delay(delaisprograissbar);// delay pour la progress bar
  etalonnageDesSondes(quelleSonde, bufnumber, bufnumberfloat); // sonde a calibrer + valeur
  delay(delaisprograissbar);// delay pour la progress bar
  statutprogressbar(etape, totalEtape, orph);

}

/* deplace l'oscillateur a l'angle parametré */
void boutonoscillo1(uint32_t valeur) {
  positionOscillo[numeroUn] = valeur;
  oscillo[numeroUn].write(positionOscillo[numeroUn]);
  DPRINTF("positionOscillo1 : "); DPRINT(number);  DPRINTLN(); // debug
}

/* deplace l'oscillateur a l'angle parametré */
void boutonoscillo2(uint32_t valeur) {
  positionOscillo[numeroDeux] = valeur;
  oscillo[numeroDeux].write(positionOscillo[numeroDeux]);
  DPRINTF("positionOscillo2 : "); DPRINT(number);  DPRINTLN(); // debug
}

/* deplace l'oscillateur a l'angle parametré */
void boutonoscillo3(uint32_t valeur) {
  positionOscillo[numeroTrois] = valeur;
  oscillo[numeroTrois].write(positionOscillo[numeroTrois]);
  DPRINTF("positionOscillo3 : "); DPRINT(number);  DPRINTLN(); // debug
}

/* bouton pour redemarrer l'aquabouns */
void redemarrage(void *ptr) {
  wdt_enable(WDTO_1S);
  DPRINTF("on va redemarrer ... "); DPRINT(minuteEnTexteOut); DPRINTLN(); // debug
  horodatageEtEcritSurSD (texteSurSDRedemarrage, "");
  delay(deuxMille);
}

void restaure(void *ptr) {
  DPRINTF("restaure les paramettre eeprom"); DPRINTLN(); // debug
  demandeRestaure = true;
  etablirValeursParDefaut();
  affichePageConfig();
}

/* bouton bas evenements carte sd */
void versBasSd(void *ptr) {
  const uint8_t nombre2Ligne = 5; // nombre de ligne dans nextion
  effaceLesEvenementsSurLecranSd();
  for (uint8_t i = 0; i < nombre2Ligne; i++) {
    numero2Ligne = i;
    lirePage(PAGE_PRECEDENTE);
  }
  SDPrecedementVersLeBas = true;
  numero2Ligne = 0;
}

/* bouton haut evenements carte sd */
void versHautSd(void *ptr) {
  const uint8_t nombre2Ligne = 5; // nombre de ligne dans nextion
  effaceLesEvenementsSurLecranSd();
  for (int8_t i = nombre2Ligne - 1; i >= 0; i--) {
    numero2Ligne = i;
    lirePage(PAGE_SUIVANTE);
  }
  SDPrecedementVersLeBas = false;
  numero2Ligne = 0;
}

/* vide tout les champs texte du nextion de la page sd*/
void effaceLesEvenementsSurLecranSd() {
  valeursd00.setText("");
  valeursd01.setText("");
  valeursd02.setText("");
  valeursd03.setText("");
  valeursd04.setText("");
}

/* bouton efface carte sd */
void effaceSd(void *ptr) {
  videFichierErreur();
  NotifOn = false;
  affichePageMenu(); // affiche la page
}

void changementAdresseSonde(void *ptr) {
  DPRINTF("avant variable.temperatureBac: "); DPRINT(variable.temperatureBac); DPRINTLN(); // debug
  DPRINTF("avant variable.temperatureRampe: "); DPRINT(variable.temperatureRampe); DPRINTLN(); // debug
  if (inverseSonde) {
    inverseSonde = false;
    boutonadressesonde.Set_background_image_pic(Isonde2);
    sauvegarde.adresseSondeBac = 1;
    sauvegarde.adresseSondeRampe = 0;
    DPRINTF("inverseSonde Isonde2: "); DPRINT(inverseSonde); DPRINTLN(); // debug
  }
  else {
    inverseSonde = true;
    boutonadressesonde.Set_background_image_pic(Isonde1);
    sauvegarde.adresseSondeBac = 0;
    sauvegarde.adresseSondeRampe = 1;
    DPRINTF("inverseSonde Isonde1: "); DPRINT(inverseSonde); DPRINTLN(); // debug
  }
  sensors.requestTemperatures(); // on lance une demande aux sondes pour prochain relevé de temperature
  delay(500);
  variable.temperatureBac = sensors.getTempCByIndex(sauvegarde.adresseSondeBac); // affecte la mesure
  variable.temperatureRampe = sensors.getTempCByIndex(sauvegarde.adresseSondeRampe); // affecte la mesure
  temperature();
  DPRINTF("apres variable.temperatureBac: "); DPRINT(variable.temperatureBac); DPRINTLN(); // debug
  DPRINTF("apres variable.temperatureRampe: "); DPRINT(variable.temperatureRampe); DPRINTLN(); // debug
}

/*sauvegarde les parametres de la page config */
void enregistrerConfig(void *ptr) {
  DPRINTF("*************************************** sauvegarde Config ******************************************"); DPRINTLN(); // debug
  const uint8_t totalEtape = 8;
  uint8_t etape = 1;
  const uint32_t minuteEnMillis = 60000ul; // correspond a 1 minute en millisseconde
  uint32_t number2;
  valeurtemperatureaquarium.getValue(&number);// recupere la valeur dans nextion
  DPRINTF("valeurtemperatureaquarium : "); DPRINT(number); DPRINTLN(); // debug
  sauvegarde.temperatureaquarium = number;// attribut la valeur nextion dans l'arduino
  sauvegarde.temperatureaquarium = sauvegarde.temperatureaquarium / dix;
  statutprogressbar(etape, totalEtape, parametre);
  etape++;
  valeurventilationaquarium.getValue(&number);// recupere la valeur dans nextion
  DPRINTF("valeurventilationaquarium : "); DPRINT(number); DPRINTLN(); // debug
  sauvegarde.ventilationaquarium = number;// attribut la valeur nextion dans l'arduino
  sauvegarde.ventilationaquarium = sauvegarde.ventilationaquarium / dix;
  statutprogressbar(etape, totalEtape, parametre);
  etape++;
  valeurventilationrampe.getValue(&number);// recupere la valeur dans nextion
  DPRINTF("valeurventilationrampe : "); DPRINT(number); DPRINTLN(); // debug
  sauvegarde.ventilationrampe = number;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, parametre);
  etape++;
  valeurheureNourrissage1h.getValue(&number);// recupere la valeur dans nextion
  DPRINTF("valeurheureNourrissage1h : "); DPRINT(number); DPRINTLN(); // debug
  statutprogressbar(etape, totalEtape, parametre);
  etape++;
  valeurheureNourrissage1m.getValue(&number2); // recupere la valeur dans nextion
  DPRINTF("valeurheureNourrissage1m : "); DPRINT(number2); DPRINTLN(); // debug
  horraireEnSeconde(number, number2);// calcule h*3600+m*60
  DPRINTF("horraireEnSeconde : "); DPRINT(recupHorraireTemporraire); DPRINTLN(); // debug
  sauvegarde.heureNourrissage[numeroUn] = recupHorraireTemporraire;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, parametre);
  etape++;
  valeurheureNourrissage2h.getValue(&number);// recupere la valeur dans nextion
  valeurheureNourrissage2m.getValue(&number2); // recupere la valeur dans nextion
  horraireEnSeconde(number, number2);// calcule h*3600+m*60
  sauvegarde.heureNourrissage[numeroDeux] = recupHorraireTemporraire;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, parametre);
  etape++;
  valeurdureeNourrissageMillis.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.dureeNourrissageMillis = number;// attribut la valeur nextion dans l'arduino
  sauvegarde.dureeNourrissageMillis = sauvegarde.dureeNourrissageMillis * minuteEnMillis;
  statutprogressbar(etape, totalEtape, parametre);
  etape++;
  EEPROM.put(adresseDeSauvegarde, sauvegarde);  // sauvegarde tout les parametres
  statutprogressbar(etape, totalEtape, parametre);
  temperature();
}

/*sauvegarde les parametres de la page wifi */
void enregistrerWifi(void *ptr) {
  const uint8_t totalEtape = 3;
  uint8_t etape = 1;
  DPRINTF("*************************************** sauvegarde Wifi ******************************************"); DPRINTLN(); // debug
  effaceBufferTexte();
  valeurssid.getText(bufferTexte, maxbufferTexte); // recupere la valeur dans nextion
  strncpy(identifiantWifi.ssid, bufferTexte, maxSsid); // copie le char
  ecritSurSd(ssidSurSD, identifiantWifi.ssid); // copie sur la SD
  statutprogressbar(etape, totalEtape, sd);
  etape++;
  effaceBufferTexte();
  valeurmdp.getText(bufferTexte, maxbufferTexte); // recupere la valeur dans nextion
  strncpy(identifiantWifi.mdp, bufferTexte, maxMdp); // copie le char
  ecritSurSd(mdpSurSD, identifiantWifi.mdp); // copie sur la SD
  statutprogressbar(etape, totalEtape, sd);
  etape++;
  executer(ENVOYERidentifiantWifi, true); // envoie les nouveau identifiants de connection
  statutprogressbar(etape, totalEtape, sd);
}

/*sauvegarde les parametres de la page tel */
void enregistrerTel(void *ptr) {
  const uint8_t totalEtape = 7;
  uint8_t etape = 1;
  DPRINTF("*************************************** sauvegarde Tel ******************************************"); DPRINTLN(); // debug
  statutprogressbar(etape, totalEtape, tel);
  etape++;
  effaceBufferTexte();
  valeurtel.getText(bufferTexte, maxbufferTexte); // recupere la valeur dans nextion
  statutprogressbar(etape, totalEtape, tel);
  etape++;
  strncpy(numero2tel, bufferTexte, maxnumero2tel); // copie le char
  numero2tel[maxnumero2tel] = '\0'; // ajoute le caractere de fin
  statutprogressbar(etape, totalEtape, tel);
  etape++;
  ecritSurSd(numero2TelSurSD, numero2tel); // copie sur la SD
  DPRINTF("numero de tel enregistré"); DPRINT(numero2tel); DPRINTLN(); // debug
  etape++;
  effaceBufferTexte();
  valeurpin.getText(bufferTexte, maxbufferTexte); // recupere la valeur dans nextion
  statutprogressbar(etape, totalEtape, tel);
  etape++;
  strncpy(codePin, bufferTexte, maxPinSize); // copie le char
  codePin[maxPinSize] = '\0'; // ajoute le caractere de fin
  statutprogressbar(etape, totalEtape, tel);
  etape++;
  ecritSurSd(pinSurSD, codePin); // copie sur la SD
  DPRINTF("pin enregistré : "); DPRINT(codePin); DPRINTLN(); // debug
  statutprogressbar(etape, totalEtape, tel);
}

/* sauvegarde les parametres de la page Brassage */
void enregistrerBrassage(void *ptr) {
  const uint8_t totalEtape = 20;
  // const float pourCentEnPwm = 2.55;
  const uint32_t minuteEnMillis = 60000ul; // correspond a 1 minute en millisseconde
  uint8_t etape = 1;
  DPRINTF("*************************************** sauvegarde Brassage ******************************************"); DPRINTLN(); // debug
  valeurminBrassage1.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.puissanceDemarageBrassage[numeroUn] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurminBrassage2.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.puissanceDemarageBrassage[numeroDeux] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurminBrassage3.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.puissanceDemarageBrassage[numeroTrois] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurBrassage1.getValue(&number); // recupere la valeur dans nextion
  variable.pwmBrassage[numeroUn] = sauvegarde.puissanceMaxBrassage[numeroUn] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurBrassage2.getValue(&number); // recupere la valeur dans nextion
  variable.pwmBrassage[numeroDeux] = sauvegarde.puissanceMaxBrassage[numeroDeux] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurBrassage3.getValue(&number); // recupere la valeur dans nextion
  variable.pwmBrassage[numeroTrois] = sauvegarde.puissanceMaxBrassage[numeroTrois] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuroscillo1min.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.angle1Oscillo[numeroUn] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuroscillo2min.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.angle1Oscillo[numeroDeux] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuroscillo3min.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.angle1Oscillo[numeroTrois] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuroscillo1max.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.angle2Oscillo[numeroUn] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuroscillo2max.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.angle2Oscillo[numeroDeux] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuroscillo3max.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.angle2Oscillo[numeroTrois] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurmouvementoscillo1.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.delaisMouvementOscilloMillis[numeroUn] = (number * mille); // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurmouvementoscillo2.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.delaisMouvementOscilloMillis[numeroDeux] = (number * mille); // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurmouvementoscillo3.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.delaisMouvementOscilloMillis[numeroTrois] = (number * mille); // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurdureetempete.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.dureeTempeteMillis = (number * minuteEnMillis); // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeurtempete.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.puissanceTempete = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuraccalemienocturne.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.accalmieNocturne = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  valeuraccalemiealimentation.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.accalmieAlimentation = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, brass);
  etape++;
  EEPROM.put(adresseDeSauvegarde, sauvegarde); // sauvegarde tout les parametres
  //majValeursParDefaut(); // recalcule les nouvelles valeurs
  statutprogressbar(etape, totalEtape, brass);
  brassage(); // lance la fonction pour affecter les nouveau parametres
}

/* sauvegarde les parametres de la page horloge */
void enregistrerHorloge(void *ptr) {
  DPRINTF("*************************************** sauvegarde Horloge ******************************************"); DPRINTLN(); // debug
  uint8_t adjustHeure, adjustminut, adjustjour, adjustmois;
  uint16_t adjustannee;
  const uint8_t totalEtape = 6;
  uint8_t etape = 1;
  affichageH.getValue(&number); // recupere la valeur dans l'ecran
  DPRINTF("affichageH.getValue(number) :");  DPRINTLN(number); // debug //debug
  adjustHeure = number;
  DPRINTF("adjustHeure :");  DPRINTLN(adjustHeure); // debug //debug
  statutprogressbar(etape, totalEtape, horloge);
  etape++;
  affichageM.getValue(&number); // recupere la valeur dans l'ecran;
  adjustminut = number;
  DPRINTF("adjustminut :");  DPRINTLN(adjustminut); // debug //debug
  statutprogressbar(etape, totalEtape, horloge);
  etape++;
  affichagejour.getValue(&number); // recupere la valeur dans l'ecran
  adjustjour = number;
  statutprogressbar(etape, totalEtape, horloge);
  etape++;
  affichagemois.getValue(&number); // recupere la valeur dans l'ecran
  adjustmois = number;
  statutprogressbar(etape, totalEtape, horloge);
  etape++;
  affichageannee.getValue(&number); // recupere la valeur dans l'ecran
  adjustannee = number + deuxMille; // ajoute deuxMille
  rtc.adjust(DateTime(adjustannee, adjustmois, adjustjour, adjustHeure, adjustminut, cinquante)); // met a l'heure l'horloge
  statutprogressbar(etape, totalEtape, horloge);
  etape++;
  horaire();// lance la fonction
  brassage(); // lance la fonction pour affecter les nouveau parametres
  eclairage(); // lance la fonction
  statutprogressbar(etape, totalEtape, horloge);
}

void enregistrerEclairage(void *ptr) {
  uint32_t number2;
  uint8_t etape = 1;
  const uint8_t totalEtape = 10;
  const uint8_t minuteEnSeconde = 60;
  DPRINTF("*************************************** sauvegarde Eclairage ******************************************"); DPRINTLN(); // debug
  valeurcouleur1.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.puissanceMaxEclairage[Couleur1] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurcouleur2.getValue(&number); // recupere la valeur dans nextion
  sauvegarde.puissanceMaxEclairage[Couleur2] = number; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurcouleur3.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.puissanceMaxEclairage[Couleur3] = number;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurcouleur4.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.puissanceMaxEclairage[Couleur4] = number;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurleverSoleilh.getValue(&number);// recupere la valeur dans nextion
  valeurleverSoleilm.getValue(&number2);// recupere la valeur dans nextion
  DPRINTF("valeurleverSoleilh.getValue(&number): ");  DPRINTLN(number); // debug
  DPRINTF("valeurleverSoleilm.getValue(&number2): ");  DPRINTLN(number2); // debug
  horraireEnSeconde(number, number2);// calcule h*3600+m*60
  sauvegarde.leverSoleil = recupHorraireTemporraire;// attribut la valeur nextion dans l'arduino
  //valeurleverSoleilm.setText(recupHorraireTemporraire); // minute
  DPRINTF("sauvegarde.leverSoleil = recupHorraireTemporraire : ");  DPRINTLN(recupHorraireTemporraire); // debug
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurcoucherSoleilh.getValue(&number);// recupere la valeur dans
  valeurcoucherSoleilm.getValue(&number2);// recupere la valeur dans nextion
  horraireEnSeconde(number, number2);// calcule h*3600+m*60
  sauvegarde.coucherSoleil = recupHorraireTemporraire;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurdureelevercoucher.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.dureelevercoucher = number * minuteEnSeconde; // attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurdebutrefugeh.getValue(&number);// recupere la valeur dans
  valeurdebutrefugem.getValue(&number2);// recupere la valeur dans nextion
  horraireEnSeconde(number, number2);// calcule h*3600+m*60
  sauvegarde.debutRefuge = recupHorraireTemporraire;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  valeurfinrefugeh.getValue(&number);// recupere la valeur dans
  valeurfinrefugem.getValue(&number2);// recupere la valeur dans nextion
  horraireEnSeconde(number, number2);// calcule h*3600+m*60
  sauvegarde.finRefuge = recupHorraireTemporraire;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, light);
  etape++;
  EEPROM.put(adresseDeSauvegarde, sauvegarde); // sauvegarde tout les parametres
  majValeursParDefaut(); // recalcule les nouvelles valeurs
  eclairage();
  refuge();
  statutprogressbar(etape, totalEtape, light);
}

void enregistrerAlarme(void *ptr) {
  const uint8_t totalEtape = 9;
  const uint8_t minuteEnSeconde = 60;
  uint8_t etape = 1;
  valeuralertetemperaturebasse.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.alertetemperaturebasse = number;// attribut la valeur nextion dans l'arduino
  sauvegarde.alertetemperaturebasse = sauvegarde.alertetemperaturebasse / dix;
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  valeuralertetemperaturehaute.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.alertetemperaturehaute = number;// attribut la valeur nextion dans l'arduino
  sauvegarde.alertetemperaturehaute = sauvegarde.alertetemperaturehaute / dix;
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  valeurdureeosmolation.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.dureeOsmolationMillis = number;// attribut la valeur nextion dans l'arduino
  sauvegarde.dureeOsmolationMillis = sauvegarde.dureeOsmolationMillis * mille;
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  valeurdureerenplissage.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.dureeremplissage = number; // attribut la valeur nextion dans l'arduino
  sauvegarde.dureeremplissage = sauvegarde.dureeremplissage * minuteEnSeconde;
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  valeuralertephbacbas.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.alertephbacbas = number;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  valeuralertephbachaut.getValue(&number);// recupere la valeur dans nextion
  sauvegarde.alertephbachaut = number;// attribut la valeur nextion dans l'arduino
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  affichageHdebutalerte.getValue(&number); // recupere la valeur dans l'ecran
  horraireEnSeconde(number, 0); // converti en seconde
  sauvegarde.heureDebutAlerte = recupHorraireTemporraire;
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  affichageHfinalerte.getValue(&number); // recupere la valeur dans l'ecran
  horraireEnSeconde(number, 0); // converti en secoinde
  sauvegarde.heureFinAlerte = recupHorraireTemporraire;
  statutprogressbar(etape, totalEtape, alarme);
  etape++;
  EEPROM.put(adresseDeSauvegarde, sauvegarde);  // sauvegarde tout les parametres
  ph();
  statutprogressbar(etape, totalEtape, alarme);
}

void statutprogressbar(uint8_t etape, uint8_t totalEtape, uint8_t quelBar) {
  if (quelBar == light) {
    progressbareclairage.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbareclairage.setValue(zero);
    }
  }
  else if (quelBar == brass) {
    progressbarbrassage.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbarbrassage.setValue(zero);
    }
  }
  else if (quelBar == horloge) {
    progressbarhorloge.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbarhorloge.setValue(zero);
    }
  }
  else if (quelBar == wifi) {
    progressbarwifi.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbarwifi.setValue(zero);
    }
  }
  else if (quelBar == tel) {
    progressbartel.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbartel.setValue(zero);
    }
  }
  else if (quelBar == orph) {
    progressbarorph.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbarorph.setValue(zero);
    }
  }
  else if (quelBar == parametre) {
    progressbarconfig.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbarconfig.setValue(zero);
    }
  }
  else if (quelBar == alarme) {
    progressbarconfig.setValue((cent / totalEtape)*etape);
    if (etape == totalEtape) {
      delay(500);
      progressbarconfig.setValue(zero);
    }
  }
  delay(50);
}

/* affiche heure et duree de la tempete Aleatoire */
void tempeteNextion() {
  TIMEenTEXTE(heureTempeteAleatoire); // convertie numerique en char
  affichageheuretempete.setText(textime); // affichage de l'heure de la tempete Aleatoire
  TIMEenTEXTE(dureeTempeteAleatoire / mille); // convertie la duree de la tempete en char
  affichagedureetempete.setText(textime); // affichage de la durée de la tempete
}

/* affiche l'heure sur l'ecran */
void affichageHeureNextion() {
  TIMEenTEXTE(Time); // convertie numerique en char
  affichageheure.setText(textime); // affiche l'heure
}

/* initialise la chaine à vide */
void effaceBufferTexte() {
  memset(bufferTexte, '\0', maxbufferTexte); // effacer buffer
}

/* initialise la chaine à vide */
void effaceminuteEnTexteIn() {
  memset(minuteEnTexteIn, '\0', maxminuteEnTexte); // effacer buffer
}

/* affichage icone alimentation */
void iconNourrissageEnCours() {
  if (nourissageEnCours) {
    iconealimentationencours.setPic(InourissageOn);
  }
  else {
    iconealimentationencours.setPic(InourissageOff);
  }
  brassage();
  rafraichirBrassage1Nextion();
  rafraichirBrassage2Nextion();
  rafraichirBrassage3Nextion();
}

void texteProgmemAuNextion(NexText champTexte, const char texteEnProgmem[] PROGMEM, const uint16_t couleurTexte) {
  effaceBufferTexte();
  if (couleurTexte > pas2Changement2Couleur) {
    champTexte.Set_font_color_pco(couleurTexte); // text couleur
  }
  strncpy_P (bufferTexte, texteEnProgmem, maxbufferTexte); // recupere "char" en memoire flash et le copie
  bufferTexte[maxbufferTexte] = '\0'; // ajoute le caractere de fin
  champTexte.setText(bufferTexte);// envoi le texte a l'ecran nextion// envoi le texte a l'ecran nextion
  DPRINT(bufferTexte); DPRINTLN(); // debug
}

void texteAuNextion(NexText champTexte, char* texte, const uint16_t couleurTexte) {
  if (couleurTexte > pas2Changement2Couleur) {
    champTexte.Set_font_color_pco(couleurTexte); // text couleur
  }
  champTexte.setText(texte);// envoi le texte a l'ecran nextion// envoi le texte a l'ecran nextion
  DPRINT(texte); DPRINTLN(); // debug
}

void graphique24H(uint16_t T, uint16_t bufT, uint16_t Ph, uint16_t bufPh, uint16_t redox, uint16_t bufRedox, float sali, float bufSali) {
  const uint8_t basGraph = 0;
  const uint8_t milieuGraph = 72;
  const uint8_t hautGraph1 = 144;
  const uint8_t hautGraph2 = 120;
  const uint8_t pixelUneLigne = 24;
  const uint16_t minT = 2400;
  const uint16_t maxT = 2800;
  const uint16_t minPh = 750;
  const uint16_t maxPh = 900;
  const uint8_t minRedox = 250;
  const uint16_t maxRedox = 550;
  const uint16_t minSali = 3400;
  const uint16_t maxSali = 3700;
  const uint8_t nbr2Pixel = 7;
  float stockDifT = ((float)T - bufT) / nbr2Pixel;
  float stockDifPh = ((float)Ph - bufPh) / nbr2Pixel;
  float stockDifRedox = ((float)redox - bufRedox) / nbr2Pixel;
  float stockDifSali = ((float)sali - bufSali) / nbr2Pixel;
  for (uint8_t i = 1; i <= nbr2Pixel; i++) {
    float calculT = ((float)T - (stockDifT * i));
    float calculPh = ((float)Ph - (stockDifPh * i));
    float calculRedox = ((float)redox - (stockDifRedox * i));
    float calculSali = ((float)sali - (stockDifSali * i));
    calculSali = calculSali * cent;
    if (calculT < minT) {
      calculT = minT;
    }
    else if (calculT > maxT) {
      calculT = maxT;
    }
    if (calculPh < minPh) {
      calculPh = minPh;
    }
    else if (calculPh > maxPh) {
      calculPh = maxPh;
    }
    if (calculRedox < minRedox) {
      calculRedox = minRedox;
    }
    else if (calculRedox > maxRedox) {
      calculRedox = maxRedox;
    }
    if (calculSali < minSali) {
      calculSali = minSali;
    }
    else if (calculSali > maxSali) {
      calculSali = maxSali;
    }
    graph24H2.addValue(0, map(calculT, minT, maxT, (milieuGraph - pixelUneLigne), hautGraph1)); // envoie 1 pixels
    graph24H2.addValue(1, map(calculPh, minPh, maxPh, basGraph , milieuGraph)); // envoie 1 pixels
    graph24H1.addValue(0, map(calculRedox, minRedox, maxRedox, basGraph, (milieuGraph + pixelUneLigne))); // envoie 1 pixels
    graph24H1.addValue(1, map(calculSali, minSali, maxSali, milieuGraph, hautGraph2)); // envoie 1 pixels
  }
}

void graphique7J(uint16_t T, uint16_t Ph, uint16_t redox, float sali) {
  const uint8_t basGraph = 0;
  const uint8_t milieuGraph = 72;
  const uint8_t hautGraph1 = 144;
  const uint8_t hautGraph2 = 120;
  const uint8_t pixelUneLigne = 24;
  const uint16_t minT = 2400;
  const uint16_t maxT = 2800;
  const uint16_t minPh = 750;
  const uint16_t maxPh = 900;
  const uint8_t minRedox = 250;
  const uint16_t maxRedox = 550;
  const uint16_t minSali = 3400;
  const uint16_t maxSali = 3700;
  sali = sali * cent;
  if (T < minT) {
    T = minT;
  }
  else if (T > maxT) {
    T = maxT;
  }
  if (Ph < minPh) {
    Ph = minPh;
  }
  else if (Ph > maxPh) {
    Ph = maxPh;
  }
  if (redox < minRedox) {
    redox = minRedox;
  }
  else if (redox > maxRedox) {
    redox = maxRedox;
  }
  if (sali < minSali) {
    sali = minSali;
  }
  else if (sali > maxSali) {
    sali = maxSali;
  }
  graph7J2.addValue(0, map(T, minT, maxT, (milieuGraph - pixelUneLigne), hautGraph1)); // envoie 1 pixels
  graph7J2.addValue(1, map(Ph, minPh, maxPh, basGraph , milieuGraph)); // envoie 1 pixels
  graph7J1.addValue(0, map(redox, minRedox, maxRedox, basGraph, (milieuGraph + pixelUneLigne))); // envoie 1 pixels
  graph7J1.addValue(1, map(sali, minSali, maxSali, milieuGraph, hautGraph2)); // envoie 1 pixels
}
