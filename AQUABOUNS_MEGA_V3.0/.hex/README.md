# Affectation des pins pour les versions simplifiées

Les instalations simplifiées de l'Aquabouns ont été configurées en fonctions de differentes affectations des pins en fonctions de votre montage :

#### > Affectation version PCBouns 1.5, 1.6 et 2.0 faite conformement au PCBouns utilisé.
-----
#### > Affectation version utilisateur : 

// entrées/sorties analogique
* hardResetWIFI = A5
* hardResetGSM = A6
* pinInCoupureCourant = A7
* pinInBatterie = A8
* pinOutRelaisChauffage = A10
* pinOutRelaisEcumeur = A11
* pinOutRelaisRemontee = A13

// entrées/sorties digital
* pinOutBrassage = 2, 3, 4
* pinOUTOscillo1 = 5
* pinOUTOscillo2 = 6
* pinOUTOscillo3 = 7
* pinOutRelaisVentilateurRampe = 8
* pinOutEclairage = 11, 12, 9, 10
* pinInSondeDs18b20 = 13
* nextion = Serial1
* gsm = Serial2
* d1mini = Serial3
* sda > ds3231 + 3x MODUL ATLAS = 20
* scl > ds3231 + 3x MODUL ATLAS = 21
* pinInFlotteurGodetEcumeur = 27
* pinOutRelaisRefuge = 28
* pinInFlotteurReserveBas = 29
* pinOutRelaisRemplissageReserve = 30
* pinInFlotteurReserveHaut = 31
* pinOutRAC = 32
* pinInFlotteurOsmolation = 33
* pinInFlotteurNiveauEcumeur = 35
* pinOutRelaisDistributeurNouriture = 37
* pinInFlotteurSecurite = 39
* pinOutBatterie = 41
* pinOutRelaisVentilateurBac = 47
* pinOutbuzzer = 48
* pinOutRelaisOsmolation = 49;
* miso > carte sd = 50
* mosi > carte sd = 51
* sck > carte sd = 52
* cs > carte sd = 53
-------
#### > Affectation version PCBouns MINI 1.0 : 

// entrées/sorties analogique
* hardResetWIFI = A0
* hardResetGSM = A1
* pinOutRelaisChauffage = A2
* pinOutRelaisEcumeur = A3
* pinOutRelaisRemontee = A4
* pinOutRelaisVentilateurRampe = A5
* pinOutRelaisDistributeurNouriture = A6
* pinOutRelaisVentilateurBac = A7
* pinOutRelaisOsmolation = A8
* pinOutBatterie = A9
* pinInBatterie = A10
* pinOutRelaisRefuge = A11
* pinOutRelaisRemplissageReserve = A12
* pinOutRAC = A13

// entrées/sorties digital
* pinOutBrassage = 2, 3, 4
* pinOUTOscillo1 = 5
* pinOUTOscillo2 = 6
* pinOUTOscillo3 = 7
* pinOutEclairage = 8, 9, 10 , 11
* pinInSondeDs18b20 13
* nextion Serial1
* d1mini Serial2
* gsm Serial3
* sda > ds3231 + 3x MODUL ATLAS = 20
* scl > ds3231 + 3x MODUL ATLAS = 21
* pinInCoupureCourant = 22
* pinInFlotteurGodetEcumeur = 24
* pinInFlotteurReserveBas = 25
* pinInFlotteurOsmolation = 26
* pinInFlotteurNiveauEcumeur = 27
* pinInFlotteurSecurite = 28
* pinInFlotteurReserveHaut = 29
* pinOutbuzzer = 49
* miso > carte sd = 50
* mosi > carte sd = 51
* sck > carte sd = 52
* sc > carte sd = 53
-------
## Le reste de la configuration a été faite tel que :

// Choix du mode inversé pour l'eclairage
* PWMinverse = OFF

// Utilisation d'une rampe Kessil
* kessil = OFF

// nombre de clic manuel sur le distributeur de nouriture
* nombre2ClicDistributeur = 1

// Utilisation d'une multiprise commandée
* multiPrise = OFF

// Choix du mode du flotteur securité
* secuDec = ON
* secuBac = OFF
-------
## Pour toute autre configuration :

Vous devrez configurer le fichier "AQUABOUNS_MEGA_V3.0.ino" selon vos besoins puis utiliser la methode classique pour instaler le programme dans l'arduino.
En cas de grosse difficulté, contacter moi.






