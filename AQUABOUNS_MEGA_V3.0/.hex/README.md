# Affectation des pins pour les versions simplifiées
----------------

Les instalations simplifiées de l'Aquabouns ont étées configurées en fonctions de differentes affectations des pins en fonctions du montage.

- Pour les versions PCBouns 1.5, 1.6 et 2.0, l'affectations a été faite en concordance.
----------------
- Pour la version utilisateur voici l'affectation : 

// entrées/sorties analogique
hardResetWIFI = A5; 
hardResetGSM = A6;
pinInCoupureCourant = A7;
pinInBatterie = A8;
pinOutRelaisChauffage = A10;
pinOutRelaisEcumeur = A11;
pinOutRelaisRemontee = A13;

//entrées/sorties digital
pinOutBrassage[3] {2, 3, 4};
pinOUTOscillo1 = 5;
pinOUTOscillo2 = 6;
pinOUTOscillo3 = 7;
pinOutRelaisVentilateurRampe = 8;
pinOutEclairage[4] {11, 12, 9, 10};
pinInSondeDs18b20 13
nextion Serial1
gsm Serial2
d1mini Serial3
20 sda > ds3231 + 3x MODUL ATLAS
21 scl > ds3231 + 3x MODUL ATLAS
pinInFlotteurGodetEcumeur = 27;
pinOutRelaisRefuge = 28;
pinInFlotteurReserveBas = 29;
pinOutRelaisRemplissageReserve = 30;
pinInFlotteurReserveHaut = 31;
pinOutRAC = 32;
pinInFlotteurOsmolation = 33;
pinInFlotteurNiveauEcumeur = 35;
pinOutRelaisDistributeurNouriture = 37;
pinInFlotteurSecurite = 39;
pinOutBatterie = 41;
pinOutRelaisVentilateurBac = 47;
pinOutbuzzer = 48;
pinOutRelaisOsmolation = 49;
50 miso > carte sd
51 mosi > carte sd
52 sck > carte sd
SD_CS_PIN
----------------













































Le reste de la configuration a été faite t'elle que :

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



