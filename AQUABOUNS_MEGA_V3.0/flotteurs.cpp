
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


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fichiers $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include "config.h"
#include "flotteurs.h"
#include "global.h"
#include "debug.h"
#include "affichage.h"
#include "gsm.h"
#include "autres.h"
#include "wifi.h"
#include "carteSD.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
boolean osmolationOn = true;
uint32_t tempsPrecedentOsmolation;
uint32_t tempsAttenteOsmolation;
uint32_t compteurPinInFlotteurReserveBas = 1800000ul; // doit etre egal a const uint32_t delayFlottement (reserve pleine) pour que l'osmolation soit active des le demarrage
uint32_t compteurPinInFlotteurReserveBasUp;
uint32_t compteurDureeremplissage;
boolean smsAlerteOsmolation = true;
boolean smsAlerteGodet = true;
boolean smsAlerteSecurite = true;
boolean smsAlerteReserve = true;
boolean smsAlerteRemplissage = true;
boolean remplissageReserve = false;

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fonction $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/* lit l'etat du flotteurs securite decante*/
void flotteurs() {
  if (digitalRead (pinInFlotteurSecurite) == HIGH) { // flotteur securite HAUT = tout couper
    digitalWrite(pinOutRelaisOsmolation, LOW); // relais OFF = osmolateur OFF
#ifdef secuBac
#ifdef multiPrise
    digitalWrite(pinOutRelaisEcumeur, LOW);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté OFF
#else
    digitalWrite(pinOutRelaisEcumeur, HIGH);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté OFF
#endif
#else // secuDec
#ifdef multiPrise
    digitalWrite(pinOutRelaisEcumeur, LOW);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, HIGH);// pompe remonté on
#else
    digitalWrite(pinOutRelaisEcumeur, HIGH);// ecumeur OFF
    digitalWrite(pinOutRelaisRemontee, LOW);// pompe remonté on
#endif
#endif
    if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
      delaisEntre2SMSOK = true;
      if (smsAlerteSecurite ) { // si boolean alerte true envoie alerte + alarme
        completerMessageAlerte(texteNextionAlerte, smsAlerteSecuriteDecante); // lance la fonction
        smsAlerteSecurite = false; // boolean pour ne pas avoir de message a repetition
        declenchementAlarme(); // lance la fonction
        DPRINTF("alarme flotteur securite haut "); DPRINTLN() // debug
      }
    }
  }
  else {// flotteur securite BAS = voir autres flotteurs
    if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
      delaisEntre2SMSOK = true;
      if (smsAlerteSecurite == false) { // si fin d'alerte
        completerMessageAlerte(texteNextionFinAlerte, smsSecuriteDecanteOk); // lance la fonction
        smsAlerteSecurite = true; // boolean pour ne pas avoir de message a repetition
        alarmeOnOff = true; // remet l'alarme sonore On
        DPRINTF("flotteur securite bas "); DPRINTLN() // debug
      }
    }
    // DPRINTF("securite decante OK");  DPRINTLN(); // debug
    if (ChangementDEauON == false) {
      flotteurNiveauEcumeur(); // lance la fonction qui lit l'etat d'autres flotteurs
      flotteurReserve(); // lance la fonction qui lit l'etat d'autres flotteurs
    }
  }
}

/* lit l'etat du flotteur niveau reserve d'eau osmosé */
void flotteurReserve() {
  if (digitalRead (pinInFlotteurReserveBas) == HIGH) { // reserve d'eau osmose OK
    const uint32_t delayFlottement = 1800000ul; // 30 minutes en millis pour ne pas avoir d'alerte/redemarage a repetition lors du remplissage de la reserve
    compteurPinInFlotteurReserveBasUp = millis();
    //DPRINTF("(pinInFlotteurReserveBas) == HIGH"); DPRINTLN() // debug
    if ( millis() - compteurPinInFlotteurReserveBas > delayFlottement) {
      flotteurNiveauConstant(); // lance la fonction qui lit l'etat d'autres flotteurs
      if (smsAlerteReserve == false) { // boolean pour ne pas avoir de message a repetition
        smsAlerteReserve = true; // remet SMS en On
        alarmeOnOff = true; // remet l'alarme sonore On
        DPRINTF("flotteur reserve bas OK");  DPRINTLN(); // debug
      }
    }
    else {
      DPRINTF("delay flottement flotteur reserve bas : ");  DPRINTLN(millis() - compteurPinInFlotteurReserveBas); // debug
    }
  }
  else { // si reserve VIDE
    const uint16_t delayFlottement = 5000u; // 5 secondes en millis pour ne pas avoir d'alerte/redemarage a repetition lors de l'arret de la pompe
    if ( millis() - compteurPinInFlotteurReserveBasUp > delayFlottement ) {
      digitalWrite(pinOutRelaisOsmolation, LOW); // relais osmolateur OFF
      compteurPinInFlotteurReserveBas = millis();
      if ( smsAlerteReserve ) { // si boolean true
        if (sauvegarde.dureeremplissage == zero) { // si on a desactiver le remplissage de la reserve automatique = alarme
          if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
            delaisEntre2SMSOK = true;
            smsAlerteReserve = false; // boolean pour ne pas avoir de message a repetition
            completerMessageAlerte(texteNextionAlerte, smsReserveVide); // lance la fonction
            declenchementAlarme(); // lance la fonction
            DPRINTF(" !!! reserve d'eau osmose vide");  DPRINTLN(); // debug
          }
        }
        else { // si remplissage auto activé
          smsAlerteReserve = false; // boolean pour ne pas avoir de message a repetition
          remplissageReserve = true; // remplissage auto ON
          compteurDureeremplissage = millis();
          DPRINTF(" reserve d'eau osmose vide, on commence le remplissage");  DPRINTLN(); // debug
        }
        DPRINTF("alarme reserve bas "); DPRINTLN() // debug
      }
    }
  }
  if (remplissageReserve) {
    remplissageReserveEnCours();
  }
}

/* lit l'etat du flotteur niveau constant de la decante */
void flotteurNiveauConstant() {
  if (sauvegarde.dureeOsmolationMillis != zero) { // si la durée d'osmolation n'est pas OFF
    if ((digitalRead (pinInFlotteurOsmolation) == LOW) && (digitalRead (pinInFlotteurNiveauEcumeur) == LOW)) { // si flotteurniveau constant + flotteur niveau ecumeur bas
      if (osmolationOn) { // si boolean true
        tempsPrecedentOsmolation = millis(); // activation de la duree d'osmolation
        osmolationOn = false; // boolean pour ne pas avoir de remise a zero du compteur a repetition
      }

      if (millis() - tempsPrecedentOsmolation < sauvegarde.dureeOsmolationMillis) { // osmolation si duree d'osmolation non depassé
        digitalWrite(pinOutRelaisOsmolation, HIGH); // relais osmolateur ON
        if ( smsAlerteOsmolation == false) { // si il y avait une alerte
          smsAlerteOsmolation = true;
        }
        DPRINTF("osmolation en cours");  DPRINTLN(); // debug
      }
      else {
        digitalWrite(pinOutRelaisOsmolation, LOW); // on coupe l'osmolation
        if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
          delaisEntre2SMSOK = true;
          if (smsAlerteOsmolation ) { // si true
            smsAlerteOsmolation = false; // boolean pour ne pas avoir de message a repetition
            completerMessageAlerte(texteNextionAlerte, smsTropOsmolation); // lance la fonction
            declenchementAlarme(); // lance la fonction
            DPRINTF("alarme trop osmolation "); DPRINTLN() // debug
          }
        }
      }
    }
    else { // quand flotteur osmolation en position haute
      digitalWrite(pinOutRelaisOsmolation, LOW); // relais osmolateur OFF
      if (osmolationOn == false) { // si false
        osmolationOn = true; // boolean pour remetre l'osmolation ON
      }
      if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
        delaisEntre2SMSOK = true;
        if ( smsAlerteOsmolation == false) { // si false
          completerMessageAlerte(texteNextionFinAlerte, smsOsmolationOk ); // envoie un SMS de fin d'alerte
          smsAlerteOsmolation = true; // boolean pour ne pas avoir de message a repetition
          alarmeOnOff = true; // remet l'alarme sonore On
          DPRINTF("niveau constant OK"); DPRINTF("/// "); DPRINT(variable.Heure); DPRINTF("h"); DPRINT(variable.minut); DPRINTLN(); // debug
        }
      }
    }
  }
}

/* lit l'etat du flotteur niveau ecumeur + godet ecumeur */
void flotteurNiveauEcumeur() { //
  if ((digitalRead (pinInFlotteurNiveauEcumeur) == LOW) && (digitalRead (pinInFlotteurGodetEcumeur) == LOW)) { // flotteur niveau ecumeur + godet en bas
#ifdef multiPrise
    digitalWrite(pinOutRelaisEcumeur, HIGH);// pompe remonté on
#else
    digitalWrite(pinOutRelaisEcumeur, LOW);// pompe remonté on
#endif
    //DPRINTF("ecumeur ok");  DPRINTLN(); // debug
    if (smsAlerteGodet == false) { // fin d'alerte godet plein
      smsAlerteGodet = true;  // boolean pour ne pas avoir de message a repetition
      alarmeOnOff = true; // remet l'alarme sonore On
      DPRINTF("godet ecumeur OK");  DPRINTLN(); // debug
    }
  }
  else {
#ifdef multiPrise
    digitalWrite(pinOutRelaisEcumeur, LOW);//
#else
    digitalWrite(pinOutRelaisEcumeur, HIGH);//
#endif
    if (digitalRead (pinInFlotteurNiveauEcumeur) == HIGH) {// flotteur niveau ecumeur
      digitalWrite(pinOutRelaisOsmolation, LOW); // relais osmolateur OFF
      //DPRINTF("flotteurs haut ecumeur Off");  DPRINTLN(); // debug
    }
    else if (digitalRead (pinInFlotteurGodetEcumeur) == HIGH) {// flotteur godet ecumeur haut
      //DPRINTF("godet plein, ecumeur Off");  DPRINTLN(); // debug
      if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
        delaisEntre2SMSOK = true;
        if (smsAlerteGodet ) { // alerte godet plein
          completerMessageAlerte(texteNextionAlerte, smsGodetPlein);
          declenchementAlarme(); // lance la fonction
          smsAlerteGodet = false; // boolean pour ne pas avoir de message a repetition
          DPRINTF("SMS godet plein, ecumeur Off");  DPRINTLN(); // debug
        }
      }
    }
    else {
      if (smsAlerteGodet == false) {
        smsAlerteGodet = true;  // boolean pour ne pas avoir de message a repetition
        alarmeOnOff = true; // remet l'alarme sonore On
      }
    }
  }
}

/* lit l'etat du flotteur niveau haut de la reserve */
void remplissageReserveEnCours() {
  if (digitalRead (pinInFlotteurReserveHaut) == LOW) {
    DPRINTF("pinInFlotteurReserveHaut DOWN");  DPRINTLN(); // debug
    if (millis() - compteurDureeremplissage < (sauvegarde.dureeremplissage * mille)) {
      smsAlerteRemplissage = true;
      DPRINTF("duree remplissage non atteinte, on remplis ...");  DPRINTLN(); // debug
      digitalWrite(pinOutRelaisRemplissageReserve, HIGH); // on remplis la reserve
    }
    else {
      if ((millis() - compteurEntre2SMS) > delaisEntre2SMS) {
        delaisEntre2SMSOK = true;
        if (smsAlerteRemplissage) {
          digitalWrite(pinOutRelaisRemplissageReserve, LOW); // on stop le remplissage de la reserve
          DPRINTF("duree remplissage atteinte");  DPRINTLN(); // debug
          completerMessageAlerte(texteNextionAlerte, smsTropRemplissage); // lance la fonction
          declenchementAlarme(); // lance la fonction
          smsAlerteRemplissage = false; // pour ne pas avoir de repetition
          remplissageReserve = false;
          DPRINTF("alarme temps de remplissage depassé "); DPRINTLN() // debug
        }
      }
    }
  }
  else { // la reserve est remplie
    DPRINTF("pinInFlotteurReserveHaut UP");  DPRINTLN(); // debug
    if (remplissageReserve) {
      const uint32_t delayFlottement = 1800000ul; // 30 minutes en millis pour ne pas avoir d'alerte/redemarage a repetition lors du remplissage de la reserve
      digitalWrite(pinOutRelaisRemplissageReserve, LOW); // on stop le remplissage de la reserve
      compteurPinInFlotteurReserveBas = millis();
      compteurPinInFlotteurReserveBas = compteurPinInFlotteurReserveBas - delayFlottement; // simule le depassement du compteur pour reactiver l'osmolation si le remplissage a été plus court que 30min (delayFlottement)
      remplissageReserve = false;
      smsAlerteRemplissage = true;
      DPRINTF("la reserve est remplie");  DPRINTLN(); // debug
    }
  }
}
