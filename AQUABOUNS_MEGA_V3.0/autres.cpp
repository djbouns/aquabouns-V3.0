
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
#include "TimerFreeTone.h"
#include <EEPROM.h>

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fichiers $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include "config.h"
#include "autres.h"
#include "arduino.h"
#include "global.h"
#include "affichage.h"
#include "debug.h"
#include "gsm.h"
#include "wifi.h"
#include "carteSD.h"


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
boolean texteNourrissage = true;
boolean alerteTemperature = true;
boolean alarmeOnOff = true;
boolean distributeurOn = false;
//uint8_t Hprecedent = 0;
boolean smsAlerteEdf = true;
boolean rebootAlarmeNoLoop = true;

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fonctions $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/* nourrissage  automatique */
void nourrissage() {
  const uint16_t oneMinuteEnMillis = 60000ul;
  for (uint8_t numeroNourrissage = numeroUn; numeroNourrissage <= numeroDeux; numeroNourrissage++) { // lance la fonction pour chaque nourrissage
    if (sauvegarde.nourrissageON[numeroNourrissage] ) { // si ON
      if (Time == sauvegarde.heureNourrissage[numeroNourrissage]) { // si heure de nourrissage ateint
        if (sauvegarde.remonteDelay == Delay) { // si en mode delay
#ifdef multiPrise
          digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#else
          digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#endif
          DPRINTF("pompe de remonté OFF");  DPRINTLN(); // debug
        }
        tempsPrecedentNourrissage = millis(); // lance le compteur
        nourissageEnCours = true; // nourrissage ON
        distributeurOn = true; // active la distribution
        iconNourrissageEnCours(); // affiche icon a l'ecran
        DPRINTF("pause nourissage n°")DPRINT(numeroNourrissage + 1); DPRINTF(" en cours");  DPRINTLN(); // debug
      }
    }
  }
  if (texteNourrissage ) { // Pour MAJ apres distribution
    if (millis() - tempsPrecedentNourrissage > sauvegarde.dureeNourrissageMillis) { // fin distribution
      if (sauvegarde.remonteDelay == Delay) { // remet en ON la remonte apres alimentation si en mode delay
#ifdef multiPrise
        digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#else
        digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#endif
        DPRINTF("pompe remonté ON");  DPRINTLN(); // debug
      }
      DPRINTF("fin Nourrissage");  DPRINTLN(); // debug
      texteNourrissage = false; // boolean pour ne pas avoir de MAJ a repetition
      iconNourrissageEnCours(); // affiche icon a l'ecran
    }
  } // la fonction distribution manuel est avec le bouton dans l'affichage
  if (distributeurOn) {
    if (millis() - tempsPrecedentNourrissage > oneMinuteEnMillis) { // 1 minute apres l'heure de nourissage pour que l'eau soit plus calme.
      for (uint8_t i = 1; i <= nombre2ClicDistributeur ; i++) {
        digitalWrite(pinOutRelaisDistributeurNouriture, HIGH); // declanche le distributeur
        delay (750); // pause
        digitalWrite(pinOutRelaisDistributeurNouriture, LOW); // arrete le distributeur
        delay (500); // pause
        DPRINTF("nombre de clic sur le distributeur : ");  DPRINTLN(i); // debug
      }
      distributeurOn = false; // desactive la distribution
      iconNourrissageEnCours(); // affiche icon a l'ecran
      DPRINTF("distribution en cours");  DPRINTLN(); // debug
    }
  }
}

/* alerte en cas de coupure EDF */
void coupureEdf() {
  const int16_t Val12VMin = 450; // quand 12V OK = ~573 sur l'echele de 1024 avec pont diviseur on prend une valeur inferieur pour laisser de la marge
#ifdef PinPCBounsMini10
  int16_t Val12V = digitalRead(pinInCoupureCourant);
  if (Val12V == HIGH) { // si presence courant
    Val12V = Val12VMin + 1; // on atribut une valeur analogique superieur au minimum (Val12VMin)
  }
#else
  int16_t Val12V = analogRead(pinInCoupureCourant);
#endif
  DPRINTF("Val12V : "); DPRINTLN(Val12V); // debug
  if (sauvegarde.bootApresCoupureEDF) { // si l'arduino se rallume apres une coupure edf + batterie
    DPRINTF("sauvegarde.bootApresCoupureEDF = true"); DPRINTLN(); // debug
    if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
      delaisEntre2SMSOK = true;
      DPRINTF("SMS apres reboot "); DPRINTLN(); // debug
      completerMessageAlerteTemperature(texteAlerteActuellement); // envoie un SMS
      sauvegarde.bootApresCoupureEDF = false; // inverse le boolean
      EEPROM.put(adresseDeSauvegarde, sauvegarde);  // sauvegarde tout les parametres
    }
  }
  else if ( Val12V < Val12VMin) { // si coupure EDF
    if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
      delaisEntre2SMSOK = true;
      if (smsAlerteEdf) { // si premiere alerte
        completerMessageAlerte(texteNextionAlerte, smsCoupureEdf); // envoie un SMS
        sauvegarde.bootApresCoupureEDF = true; // pour envoyer le SMS temperature apres
        smsAlerteEdf = false; // boolean pour ne pas avoir de SMS a repetition
        declenchementAlarme();
        DPRINTF("alarme coupure edf : "); DPRINTLN(); // debug
      }
    }
  }
  else {
    if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
      delaisEntre2SMSOK = true;
      if (smsAlerteEdf == false) { // Si juste apres coupure EDF
        completerMessageAlerte(texteNextionFinAlerte, smsEdfOk); // envoie un SMS
        sauvegarde.bootApresCoupureEDF = true; // pour envoyer le SMS temperature apres
        smsAlerteEdf = true; // boolean pour ne pas avoir de SMS a repetition²
        delay(deuxMille); // attente de 2 seconde le temps que l'ecran se rallume
        affichePageMenu(); // on affiche la page menu sur l'ecran
        DPRINTF("alarme edf ok "); DPRINTLN() // debug
      }
    }
  }
}

void coupureBatterie() { // coupe la batterie afin d'eviter le dechargement total et le scitillement de l'ecran lorsque la tension devient trop faible
  uint16_t mesureNiveauBatterie = analogRead(pinInBatterie); // prend la mesure
  DPRINTF("mesureNiveauBatterie : "); DPRINTLN(mesureNiveauBatterie) // debug
  if ((mesureNiveauBatterie < batterie0) && (mesureNiveauBatterie != zero) && (sauvegarde.bootApresCoupureEDF == false)) { // si mesure est inferieur a la tension minimum necessaire.
    DPRINTLN("coupure batterie vide");
    sauvegarde.bootApresCoupureEDF = true; //
    EEPROM.put(adresseDeSauvegarde, sauvegarde);  // sauvegarde tout les parametres
    delay(mille);
    digitalWrite(pinOutBatterie , LOW); //  coupe la batterie
  }
  else {
    DPRINTF("Batterie On OU pas de batterie"); DPRINTLN() // debug
  }
}


/* relance les alerte toute les heure entre l'intervale configurer */
void rebootAlarme() {
  if (((Time >= sauvegarde.heureDebutAlerte) && (M == 0) && (variable.seconde == 0)) && ((Time <= sauvegarde.heureFinAlerte) && (M == 0) && (variable.seconde == 0))) { // reboot toute les heures
    if (rebootAlarmeNoLoop) { // boolean pour ne pas avoir de repetition
      if (alerteTemperature == false) {
        alerteTemperature = true;
        DPRINTF("reboot alarme temperature");  DPRINTLN(); // debug
      }
      if (smsAlerteOsmolation == false) {
        smsAlerteOsmolation = true;
        DPRINTF("reboot alarme osmolateur");  DPRINTLN(); // debug
      }
      if (alertePhBas == false) {
        alertePhBas = true;
        DPRINTF("reboot alarme ph bas");  DPRINTLN(); // debug
      }
      if (alertePhHaut == false) {
        alertePhHaut = true;
        DPRINTF("reboot alarme ph haut");  DPRINTLN(); // debug
      }
      if (smsAlerteGodet == false) {
        smsAlerteGodet = true;
        DPRINTF("reboot alarme godet ecumeur");  DPRINTLN(); // debug
      }
      if (smsAlerteEdf == false) {
        smsAlerteEdf = true;
        DPRINTF("reboot alarme EDF");  DPRINTLN(); // debug
      }
      if (smsAlerteSecurite == false) {
        smsAlerteSecurite = true;
        DPRINTF("reboot alarme securité");  DPRINTLN(); // debug
      }
      if (smsAlerteReserve == false) {
        smsAlerteReserve = true;
        DPRINTF("reboot alarme reserve");  DPRINTLN(); // debug
      }
      if (alarmeOnOff == false) {
        alarmeOnOff = true;
        DPRINTF("reboot alarme sonore");  DPRINTLN(); // debug
      }
      if (smsAlerteRemplissage == false) {
        smsAlerteRemplissage = true;
        DPRINTF("reboot alarme remplissage");  DPRINTLN(); // debug
      }
      rebootAlarmeNoLoop = false; // boolean pour ne pas avoir de repetition
    }
    etatGSM(); // verifie la presence du module et reboot si necessaire
    etatWifi(); // verifie la presence du module et reboot si necessaire
  }
  else {
    rebootAlarmeNoLoop = true;// boolean pour ne pas avoir de repetition
  }
}

/* alarme sonnore */
void declenchementAlarme() {
  if (sauvegarde.BuzzerOn) { // si buzzer On
    if ((Time >= sauvegarde.heureDebutAlerte) && (Time <= sauvegarde.heureFinAlerte)) {
      if (alarmeOnOff) {
        uint8_t i = 0;
        uint8_t i2 = 0;
        uint16_t note = 800u;
        const uint8_t repetition = 3;
        const uint8_t variationNote = 25;
        const uint8_t duree = 4;
        const uint8_t Nbr2Loop = 125;
        while (i2 < repetition) { // joue 3 fois la "melodie"
          while (i < Nbr2Loop) {
            TimerFreeTone(pinOutbuzzer, note, duree); // envoie les notes
            note = note + variationNote; // change la note
            i++;
          }
          delay (250); // pause
          while (i > 1) {
            if (i2 < 2) {
              TimerFreeTone(pinOutbuzzer, note, duree); // envoie les notes
              note = note - variationNote; // change la note
            }
            i--;
          }
          i2++;
        }
        alarmeOnOff = false;
      }
      else {
        DPRINTF("Buzzer a deja sonné");  DPRINTLN(); // debug
      }
    }
  }
}

void reboot(const uint8_t pinHardReset) { // pour faire hard reboot
  digitalWrite(pinHardReset, LOW); // met pin en OFF
  delay (100); // attente
  digitalWrite(pinHardReset, HIGH); // met pin en On
}
