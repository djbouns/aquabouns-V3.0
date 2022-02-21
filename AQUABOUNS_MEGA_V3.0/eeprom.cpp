
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

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fichiers $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include "eeprom.h"
#include "global.h"
#include "debug.h"
#include "affichage.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
paramS_t sauvegarde;
bool demandeRestaure = false;

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fonctions $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/* verifie si il y a des parametre dans l'eeprom, si non, chargement des parametre par default */
void etablirValeursParDefaut() {
  uint32_t lectureMotClef;
  EEPROM.get(adresseMotClef, lectureMotClef); // verifie si il y a des parametres dans l'eeprom
  if (lectureMotClef == motClef && demandeRestaure == false) { // il y a des parametres dans l'eeprom, on les charges
    EEPROM.get(adresseDeSauvegarde, sauvegarde);
    texteProgmemAuNextion(texte1, texteNextionImportationEepromOK, vert); // champ, texte, couleur
  }
  else { // pas de parametre dans l'epprom, chargement des parametre pas default
    // page menu
    sauvegarde.tempeteAleatoireOn = true; // off tempete aleatoire
    sauvegarde.nourrissageON[numeroUn] = true; // on nourrissage 1
    sauvegarde.nourrissageON[numeroDeux] = false; // off nourrissage 2
    sauvegarde.remonteDelay = Actif; // on remonté
    sauvegarde.eclairageOnOffPwm[Couleur1] = Pwm; // mode pwm
    sauvegarde.eclairageOnOffPwm[Couleur2] = Pwm; // mode pwm
    sauvegarde.eclairageOnOffPwm[Couleur3] = Pwm; // mode pwm
    sauvegarde.eclairageOnOffPwm[Couleur4] = Pwm; // mode pwm
    sauvegarde.BrassageOnOffPwm[numeroUn] = Pwm; // mode pwm
    sauvegarde.BrassageOnOffPwm[numeroDeux] = Pwm; // mode pwm
    sauvegarde.BrassageOnOffPwm[numeroTrois] = Pwm; // mode pwm
    sauvegarde.RefugeOnOffTimer = Timer; // mode timer
    sauvegarde.BuzzerOn = true; // buzzer actif
    // page config
    sauvegarde.temperatureaquarium = 25.5; // temperature du bac
    sauvegarde.ventilationaquarium = 26.5; // temperature de declanchement de la ventilation du bac
    sauvegarde.ventilationrampe = 34; //  temperature de declanchement de la ventilation de la rampe
    sauvegarde.adresseSondeBac = 1; // adresse de la sonde de temperature du bac
    sauvegarde.adresseSondeRampe = 0; // adresse de la sonde de temperature de la rampe
    sauvegarde.heureNourrissage[numeroUn] = 46800ul; // heure du nourrissage auto 1 en seconde
    sauvegarde.heureNourrissage[numeroDeux] = 68400ul; // heure du nourrissage auto 2 en seconde
    sauvegarde.dureeNourrissageMillis = 900000ul; // durée du Nourrissage en millis
    // page Brassage
    sauvegarde.angle1Oscillo[numeroUn] = 40 ; // angle oscillo entre 0 et 180
    sauvegarde.angle2Oscillo[numeroDeux] = 100 ; // angle oscillo entre 0 et 180
    sauvegarde.angle1Oscillo[numeroTrois] = 80 ; // angle oscillo entre 0 et 180
    sauvegarde.angle2Oscillo[numeroUn] = 150 ; // angle oscillo entre 0 et 180
    sauvegarde.angle1Oscillo[numeroDeux] = 50 ; // angle oscillo entre 0 et 180
    sauvegarde.angle2Oscillo[numeroTrois] = 140 ; // angle oscillo entre 0 et 180
    sauvegarde.delaisMouvementOscilloMillis[numeroUn] = 11000u ; // delais mouvement oscillo en millis
    sauvegarde.delaisMouvementOscilloMillis[numeroDeux] = 13000u ; // delais mouvement oscillo en millis
    sauvegarde.delaisMouvementOscilloMillis[numeroTrois] = 15000u ; // delais mouvement oscillo en millis
    sauvegarde.puissanceDemarageBrassage[numeroUn] = 192; // puissance sur 255
    sauvegarde.puissanceDemarageBrassage[numeroDeux] = 187; // puissance sur 255
    sauvegarde.puissanceDemarageBrassage[numeroTrois] = 159; // puissance sur 255
    sauvegarde.puissanceMaxBrassage[numeroUn] = 75 ; // puissance  en %
    sauvegarde.puissanceMaxBrassage[numeroDeux] = 60 ; // puissance  en %
    sauvegarde.puissanceMaxBrassage[numeroTrois] = 60 ; // puissance  en %
    sauvegarde.puissanceTempete = 100; // puissance de toutes les pompe jebao en mode tempête en %
    sauvegarde.dureeTempeteMillis = 1800000ul; // durée tempête millis
    sauvegarde.accalmieAlimentation = 5; // pourcentage du brassage pendant le nourissage
    sauvegarde.accalmieNocturne = 35; // pourcentage du Brassage la nuit
    //eclaiarage
    sauvegarde.puissanceMaxEclairage[Couleur1] = 100 ; // puissance  en %
    sauvegarde.puissanceMaxEclairage[Couleur2] = 100 ; // puissance  en %
    sauvegarde.puissanceMaxEclairage[Couleur3] = 35 ; // puissance  en %
    sauvegarde.puissanceMaxEclairage[Couleur4] = 0 ; // puissance  en %
    sauvegarde.dureelevercoucher = 7200u; // temps de levé Soleil en seconde
    sauvegarde.leverSoleil = 37800ul; // heure debut lever Soleil en seconde
    sauvegarde.coucherSoleil = 73800ul; // heure fin couché Soleil en seconde
    sauvegarde.debutRefuge = 82800ul; // heure debut refuge
    sauvegarde.finRefuge = 36000ul; // heure fin refuge
    //alarme
    sauvegarde.alertetemperaturebasse = 24.5; // temperature de declanchement de la ventilation du bac
    sauvegarde.alertetemperaturehaute = 27.5; // temperature de declanchement de la ventilation du bac
    sauvegarde.dureeOsmolationMillis = 30000u; //durée de l'osmolation en millis
    sauvegarde.dureeremplissage = 36600u; // nombre de declanchement de l'osmolation avant alarme
    sauvegarde.alertephbacbas = 770; // limite PH bas du bac pour alerte sans decimal
    sauvegarde.alertephbachaut = 850;; // limite ph haut du bac pour alerte sans decimal
    sauvegarde.heureDebutAlerte = 36000ul; // heure debut alerte 10h en seconde
    sauvegarde.heureFinAlerte = 72000ul; // heure fin alerte 20h en seconde
    // etat edf
    sauvegarde.bootApresCoupureEDF = false;
    EEPROM.put(adresseDeSauvegarde, sauvegarde); // sauvegarde les parametre par default dans l'eeprom
    EEPROM.put(adresseMotClef, motClef); // ecrit le mots cle pour savoir que l'eeprom contient les données.
    texteProgmemAuNextion(texte1, texteNextionImportationEepromDefault, orange); // champ, texte, couleur
  }
  majValeursParDefaut(); // mettre a jour toute les variables ayant pour base un parametre chargé
}

/* met a jour toute les variables ayant pour base un parametre chargé
  a lancer apres chaque demarage ou sauvegarde */
void majValeursParDefaut() {
  const uint8_t nbrEtapes = 5; // pour avoir un chevauchement des couleurs dans la progression
  DPRINTF("sauvegarde.dureelevercoucher : "); DPRINTLN(sauvegarde.dureelevercoucher); // debug
  const uint16_t progression = (sauvegarde.dureelevercoucher / nbrEtapes); // duree du lever/coucher diviser par nbrEtapes pour avoir un chevauchement des couleurs dans la progression
  DPRINTF("progression : "); DPRINTLN(progression); // debug

  /* debut lever Soleil */
  debutleverSoleil[Couleur1] = sauvegarde.leverSoleil; // 0
  debutleverSoleil[Couleur2] = (debutleverSoleil[Couleur1] + progression); // 24
  debutleverSoleil[Couleur3] = (debutleverSoleil[Couleur2] + progression); // 48
  debutleverSoleil[Couleur4] = (debutleverSoleil[Couleur3] + progression); // 72
  /* fin lever Soleil */
  finleverSoleil[Couleur4] = (sauvegarde.leverSoleil + sauvegarde.dureelevercoucher); // 120
  finleverSoleil[Couleur3] = (finleverSoleil[Couleur4] - progression); // 96
  finleverSoleil[Couleur2] = (finleverSoleil[Couleur3] - progression); // 72
  finleverSoleil[Couleur1] = (finleverSoleil[Couleur2] - progression); // 48
  /* debut coucher Soleil */
  debutcoucherSoleil[Couleur4] = (sauvegarde.coucherSoleil - sauvegarde.dureelevercoucher); // 0
  debutcoucherSoleil[Couleur3] = (debutcoucherSoleil[Couleur4] + progression); // 24
  debutcoucherSoleil[Couleur2] = (debutcoucherSoleil[Couleur3] + progression); // 48
  debutcoucherSoleil[Couleur1] = (debutcoucherSoleil[Couleur2] + progression); // 72
  /* fin coucher Soleil */
  fincoucherSoleil[Couleur1] = sauvegarde.coucherSoleil; // 120
  fincoucherSoleil[Couleur2] = (fincoucherSoleil[Couleur1] - progression); // 96
  fincoucherSoleil[Couleur3] = (fincoucherSoleil[Couleur2] - progression); // 72
  fincoucherSoleil[Couleur4] = (fincoucherSoleil[Couleur3] - progression); // 48
  /* si utilisation de rampe kessil */
#ifdef kessil
  debutleverSoleil[Couleur2] = debutleverSoleil[Couleur3];
  finleverSoleil[Couleur1] = finleverSoleil[Couleur2];
  finleverSoleil[Couleur2] = finleverSoleil[Couleur4];
  debutcoucherSoleil[Couleur1] = debutcoucherSoleil[Couleur2];
  debutcoucherSoleil[Couleur2] = debutcoucherSoleil[Couleur4];
  fincoucherSoleil[Couleur2] = fincoucherSoleil[Couleur3];
#endif
}
