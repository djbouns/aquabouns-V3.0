
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
#include "eclairage.h"
#include "global.h"
#include "debug.h"
#include "wifi.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
uint32_t debutleverSoleil[quatre];
uint32_t finleverSoleil[quatre];
uint32_t debutcoucherSoleil[quatre];
uint32_t fincoucherSoleil[quatre];

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Fonction $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/* defini la puissance de l'eclairage */
void eclairage() {
  const float pourCentEnPwm = 2.55;
  for (uint8_t numeroEclairage = Couleur1; numeroEclairage <= Couleur4; numeroEclairage++) { // lance la fonction une fois pour chaque canal
    if (sauvegarde.eclairageOnOffPwm [numeroEclairage] == Arret) { // si OFF
      variable.pwmEclairage[numeroEclairage] = zero; // puissance zero
    }
    else if (sauvegarde.eclairageOnOffPwm [numeroEclairage] == Actif) { // si ON
      variable.pwmEclairage[numeroEclairage] = sauvegarde.puissanceMaxEclairage[numeroEclairage]; // puissance max definie
    }
    else if (sauvegarde.eclairageOnOffPwm [numeroEclairage] == Pwm) { // si PWM
      if ((Time >= debutleverSoleil[numeroEclairage]) && (Time < finleverSoleil[numeroEclairage])) { // lever de soleil
        variable.pwmEclairage[numeroEclairage] = map(Time, debutleverSoleil[numeroEclairage], finleverSoleil[numeroEclairage], zero, sauvegarde.puissanceMaxEclairage[numeroEclairage]); // progression
      }
      else if ((Time >= debutcoucherSoleil[numeroEclairage]) && (Time < fincoucherSoleil[numeroEclairage])) { // coucher de soleil
        variable.pwmEclairage[numeroEclairage] = map(Time, debutcoucherSoleil[numeroEclairage], fincoucherSoleil[numeroEclairage], sauvegarde.puissanceMaxEclairage[numeroEclairage], zero); // progression
      }
      else if ((Time >= finleverSoleil[numeroEclairage] ) && (Time < debutcoucherSoleil[numeroEclairage])) { // zenith
        variable.pwmEclairage[numeroEclairage] = sauvegarde.puissanceMaxEclairage[numeroEclairage]; // puissance max definie
      }
      else { // nuit
        variable.pwmEclairage[numeroEclairage] = zero;
      }
    }
    DPRINTF("variable.pwmEclairage "); DPRINT(numeroEclairage); DPRINTF(" = "); DPRINTLN(variable.pwmEclairage[numeroEclairage]); // debug
#ifdef kessil
    if (numeroEclairage == Couleur4) {
      const uint8_t MilieuTemperatureCouleur = 50;
      uint8_t temperatureCouleur;
      uint8_t puissanceKessil;
      /* on calcule la temperature de couleur */
      if (variable.pwmEclairage[Couleur1] == zero) {
        DPRINTF("1"); DPRINTLN(); // debug
        if (variable.pwmEclairage[Couleur2] > zero) {
          temperatureCouleur =  cent; // pwm a cent pour cent = couleur blanc
          DPRINTF("kessil 100% blanc "); DPRINTLN() // debug
        }
      }
      else if (variable.pwmEclairage[Couleur2] == zero) {
        DPRINTF("2"); DPRINTLN(); // debug
        if (variable.pwmEclairage[Couleur1] > zero) {
          temperatureCouleur =  zero; // pwm a zero pour cent = couleur bleu
          DPRINTF("kessil 100% bleu"); DPRINTLN(); // debug
        }
      }
      else {
        temperatureCouleur = MilieuTemperatureCouleur - (variable.pwmEclairage[Couleur1] / 2) + (variable.pwmEclairage[Couleur2] / 2); // pour le temperature de couleur
        DPRINTF("kessil temperatureCouleur : "); DPRINT(temperatureCouleur); DPRINTF(" (0 = bleu / 100 = blanc)"); DPRINTLN();// debug
      }
      analogWrite (pinOutEclairage[un], temperatureCouleur * pourCentEnPwm); // envoie sur le pin la puissance calculé
      /* on calcule la puissance d'eclairage */
      if (variable.pwmEclairage[Couleur1] > variable.pwmEclairage[Couleur2]) {
        puissanceKessil = variable.pwmEclairage[Couleur1];
        DPRINTF("kessil puissance couleur 1 : "); DPRINTLN(puissanceKessil); // debug
      }
      else if (variable.pwmEclairage[Couleur2] > variable.pwmEclairage[Couleur1]) {
        puissanceKessil = variable.pwmEclairage[Couleur2];
        DPRINTF("kessil puissance couleur 2 : "); DPRINTLN(puissanceKessil); // debug
      }
      else if (variable.pwmEclairage[Couleur1] == variable.pwmEclairage[Couleur2]) {
        puissanceKessil = variable.pwmEclairage[Couleur1];
        DPRINTF("kessil puissance couleur 1 et 2 : "); DPRINTLN(puissanceKessil); // debug
      }
      analogWrite (pinOutEclairage[zero], puissanceKessil * pourCentEnPwm); // envoie sur le pin la puissance calculé
      DPRINTF("eclairaige mode kessil"); DPRINTLN(); // debug
    }
#else
#ifdef PWMinverse // Si éclairage invessé (PWM 0 = 100% et PWM 255 = 0%)
    uint8_t pwmEclairageInverse = (variable.pwmEclairage[numeroEclairage] + cent) - (variable.pwmEclairage[numeroEclairage] * deux);
    analogWrite (pinOutEclairage[numeroEclairage], pwmEclairageInverse * pourCentEnPwm); // envoie sur le pin la puissance calculé
    DPRINTF("eclairaige mode inverse"); DPRINTLN(); // debug
#else // Si éclairage standard
    analogWrite (pinOutEclairage[numeroEclairage], variable.pwmEclairage[numeroEclairage] * pourCentEnPwm); // envoie sur le pin la puissance calculé
    DPRINTF("eclairaige mode normal"); DPRINTLN(); // debug
#endif
#endif
  }
}

void refuge() {
  if (sauvegarde.RefugeOnOffTimer == Arret) { // si OFF
    digitalWrite (pinOutRelaisRefuge, LOW);
    // DPRINTF("digitalWrite (pinOutRelaisRefuge, LOW); ARRET "); DPRINTLN(); // debug
  }
  else if (sauvegarde.RefugeOnOffTimer == Actif) { // si ON
    digitalWrite (pinOutRelaisRefuge, HIGH);
    //DPRINTF("digitalWrite (pinOutRelaisRefuge, HIGH); ON "); DPRINTLN(); // debug
  }
  else if (sauvegarde.RefugeOnOffTimer == Timer) { // si PWM
    if (sauvegarde.debutRefuge < sauvegarde.finRefuge) {
      if ((Time >= sauvegarde.debutRefuge) && (Time < sauvegarde.finRefuge)) {
        digitalWrite (pinOutRelaisRefuge, HIGH);
        //DPRINTF("digitalWrite (pinOutRelaisRefuge, HIGH); TIMER1 "); DPRINTLN(); // debug
      }
      else {
        digitalWrite (pinOutRelaisRefuge, LOW);
        //DPRINTF("digitalWrite (pinOutRelaisRefuge, LOW); TIMER1 "); DPRINTLN(); // debug
      }
    }
    else if (sauvegarde.debutRefuge > sauvegarde.finRefuge) {
      if ((Time >= sauvegarde.finRefuge) && (Time < sauvegarde.debutRefuge)) {
        digitalWrite (pinOutRelaisRefuge, LOW);
        //DPRINTF("digitalWrite (pinOutRelaisRefuge, LOW); TIMER2 "); DPRINTLN(); // debug
      }
      else {
        digitalWrite (pinOutRelaisRefuge, HIGH);
        // DPRINTF("digitalWrite (pinOutRelaisRefuge, HIGH); TIMER2 "); DPRINTLN(); // debug
      }
    }
  }
}
