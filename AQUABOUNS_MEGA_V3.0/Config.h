#ifndef  _config_h_
#define _config_h_
#include "arduino.h"

/* Cette page vous permet de selectionner le materiel que vous utilisez afin que le programme soit adapté.
  Il vous suffit d'ajouter ou de retirer // devant les #define corespondant a votre configuration.
  Si vous utiliser le type de materiel proposé retirer // (la ligne commence par: #define, le #define est vert et le texte est noir)
  Si vous n'utiliser pas le type de materiel proposé ajouter // (La ligne commence par: //#define, le texte est gris claire) */

//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ faite votre configuration ici ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

// Choix de la configuration des PIN de l'arduino
/*choisir le type de configuration des pin correspondant a votre montage, devant votre configuration retirer //          */ // ne choisir qu'une configuration
/* !!!!!!!!!! Un seul choix possible !!!!!!!!!! */
#define PinUtilisateur // Affectation des PIN defini par l'utilisateur
//#define PinPCBounsMini10 // pour Shield PCBouns Mini V1.*
//#define PinPCBouns15 // pour Shield PCBouns V1.5
//#define PinPCBouns16 // pour Shield PCBouns V1.6
//#define PinPCBouns20 // pour Shield PCBouns V2.*
//#define PinPCBounsTest // pour Shield PCBouns Test

// Choix du mode inversé pour l'eclairage
/*si votre éclairage fonctionne en invessé (PWM 0 = 100% et PWM 255 = 0%). retirer //           */
//#define PWMinverse

// Utilisation d'une rampe Kessil
/*si vous utilisez une rampe kessil. retirer // devant multiPrise         */
//#define kessil

// nombre de clic manuel sur le distributeur de nouriture
/* doit rester a 1 pour la plus part des distributeur de nouriture, sauf par exemple pour distributeur JBL Autofood ou le nombre de clic determine la quantité de nouriture */
const uint8_t nombre2ClicDistributeur = 1; // Minimum 1, Maximum 5 sinon depassement du watchdog = reboot

// Utilisation d'une multiprise commandée
/* Si vous utilisez une prise commandée (fonctionement inverse des relais ecumeur + pompe remontée + temperature, les appareils ne fonctionne que lorsque l'Aquabouns est allumé). retirer // devant multiPrise         */
//#define multiPrise

// Choix du mode du flotteur securité
/* Si vous voulez utiliser le flotteur de securité comme securité d'hauteur d'eau dans votre decante, pour etre prevenu en cas de niveau trop haut / risque de debordement. coupe osmolation et ecumeur. retirer // devant secuDec        */
/* Si vous voulez utiliser le flotteur de securité comme securité d'hauteur d'eau de votre bac, pour etre prevenu en cas de niveau trop haut / risque de debordement. coupe osmolation, ecumeur et pompe de remonté. retirer // devant secuBac        */
/* !!!!!!!!!! Un seul choix possible !!!!!!!!!! */
#define secuDec
//#define secuBac


//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ faite votre configuration ici ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
/*





  888b    888 8888888888      8888888b.  888     888     888  .d8888b.       88888888888 .d88888b.  888     888  .d8888b.  888    888 8888888888 8888888b.
  8888b   888 888             888   Y88b 888     888     888 d88P  Y88b          888    d88P" "Y88b 888     888 d88P  Y88b 888    888 888        888   Y88b
  88888b  888 888             888    888 888     888     888 Y88b.               888    888     888 888     888 888    888 888    888 888        888    888
  888Y88b 888 8888888         888   d88P 888     888     888  "Y888b.            888    888     888 888     888 888        8888888888 8888888    888   d88P
  888 Y88b888 888             8888888P"  888     888     888     "Y88b.          888    888     888 888     888 888        888    888 888        8888888P"
  888  Y88888 888             888        888     888     888       "888          888    888     888 888     888 888    888 888    888 888        888 T88b
  888   Y8888 888             888        888     Y88b. .d88P Y88b  d88P          888    Y88b. .d88P Y88b. .d88P Y88b  d88P 888    888 888        888  T88b
  888    Y888 8888888888      888        88888888 "Y88888P"   "Y8888P"           888     "Y88888P"   "Y88888P"   "Y8888P"  888    888 8888888888 888   T88b

                   d8888      8888888b.     d8888 8888888b. 88888888888 8888888 8888888b.       8888888b.  d8b 8888888 .d8888b. 8888888
                  d88888      888   Y88b   d88888 888   Y88b    888       888   888   Y88b      888  "Y88b 88    888  d88P  Y88b  888
                 d88P888      888    888  d88P888 888    888    888       888   888    888      888    888       888  888    888  888
                d88P 888      888   d88P d88P 888 888   d88P    888       888   888   d88P      888    888       888  888         888
               d88P  888      8888888P" d88P  888 8888888P"     888       888   8888888P"       888    888       888  888         888
              d88P   888      888      d88P   888 888 T88b      888       888   888 T88b        888    888       888  888    888  888
             d8888888888      888     d8888888888 888  T88b     888       888   888  T88b       888  .d88P       888  Y88b  d88P  888
            d88P     888      888    d88P     888 888   T88b    888     8888888 888   T88b      8888888P"      8888888 "Y8888P" 8888888






*///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Messages d'erreur $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// erreur de type carte utilisé
#if defined(__AVR_ATmega2560__) // la carte MEGA a bien été selectionner pour la compilation, le televersement peu avoir lieu ...
#else // une autre carte a été selectionner pour la compilation, erreur de compilation !
#error "Vous n'avez pas selectionné la bonne carte ! aller dans outils/type de carte et selectionner arduino/genuino Mega or Mega 2560"
#endif

// erreur de configuration des pin
static_assert(1 >= 0
#ifdef PinUtilisateur
              + 1
#endif
#ifdef PinPCBouns15
              + 1
#endif
#ifdef PinPCBouns16
              + 1
#endif
#ifdef PinPCBouns20
              + 1
#endif
#ifdef PinPCBounsTest
              + 1
#endif
#ifdef PinPCBounsMini10
              + 1
#endif
              , "Vous avez selectionnez plusieurs configuration de pin meme temps !!! ajouter // devant des lignes de configuration des pin afin de n'en garder qu'une seul");
#if !defined (PinUtilisateur) && !defined (PinPCBouns15) && !defined (PinPCBouns16)&& !defined (PinPCBouns20)&& !defined (PinPCBounsTest)&& !defined (PinPCBounsMini10)
#error "Vous n'avez selectionnez aucune configuration de pin !!! retirer // devant une lignes de configuration des pin"
#endif
#ifdef PinUtilisateur
#include "PinUtilisateur.h"
#endif
#ifdef PinPCBouns15
#include "PinPCBouns15.h"
#endif
#ifdef PinPCBouns16
#include "PinPCBouns16.h"
#endif
#ifdef PinPCBouns20
#include "PinPCBouns20.h"
#endif
#ifdef PinPCBounsTest
#include "PinPCBounsTest.h"
#endif
#ifdef PinPCBounsMini10
#include "PinPCBounsMini10.h"
#endif


// erreur de configuration du flotteur de securité
static_assert(1 >= 0
#ifdef secuDec
              + 1
#endif
#ifdef secuBac
              + 1
#endif
              , "Vous avez selectionnez plusieurs configuration du flotteur de securite  !!! ajouter // devant des lignes de configuration du flotteur de securite afin de n'en garder qu'une seul");
#if !defined (secuDec) && !defined (secuBac)
#error "Vous avez selectionnez aucune configuration du flotteur de securite !!! retirer // devant une lignes de configuration"
#endif

#endif
