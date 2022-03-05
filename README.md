# Aquabouns-V3.0

#### l'Aquabouns, le programme entièrement GRATUIT d'automatisation d'aquarium récifal.

 *-*-*-*-*
 
 *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-*
 
 *-*-*-*-*
 
###### l’Aquabouns en questions:  
> l'Aquabouns c'est quoi ? ----------------------- l'Aquabouns est un programme pour automatiser votre aquarium récifal. 
> 
> Comment ça fonctionne ?------------------------- le programme Aquabouns est prévu pour etre instalé sur une cartes arduino Mega. l'arduino communique ensuite avec différents périphériques (écran, module wifi et module GSM) et interprète l'état des différents capteurs (flotteur, sonde de température, horloge) et en fonction du paramétrage, intervient sur le matériel du bac (pompe remonté, brassage, ventilation, écumeur ...). 
> 
> Comment obtenir le programme ? à quel prix ? --- l'Aquabouns est mis a disposition GRATUITEMENT, il a été déposé sous License libre (GPL 3.0) afin que tout le monde puisse en profiter. 
> 
> Comment faire le montage ? --------------------- Tout se trouve dans les notices que vous pouvez télécharger sur www.aquabouns.fr et si le montage n'est pas à votre porté, vous pouvez vous faire aider d'un PCBouns, www.pcbouns.fr. 

 *-*-*-*-*
 
 *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-*
 
 *-*-*-*-*
 
#### Soutenez le projet pour qu'il continue d’évoluer et d’être mis à disposition gratuitement en faisant un DON


 [![Donate](https://www.paypalobjects.com/fr_FR/FR/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate/?hosted_button_id=HWQ5X3M39VEP4)
 

 
 *-*-*-*-*
 
 *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-*
 
 *-*-*-*-*

### Remerciements
-----------------

Cela a été possible grâce à quelques rares personnes qui m’ont aidé ou guidé quand j’en avais besoin.
Un GRAND merci à JML du forum Arduino pour son aide et tous ses conseils reçus, qui ont permis à ce projet d’être aussi abouti.
Merci à tous le forum Arduino pour son aide.
Merci à Commodore et maxime d'avoir participer a la phase de test.

Et un énorme merci a celui qui a redesigner, entre autre, toute la partie graphique de cette Aquabouns V3.0.

!!!!!!!!!! Loïc 'Solyum' D. !!!!!!!!!!

Pour voir son travail ou le contacter:
https://www.behance.net/solyum
contact: loic_dekeyser@outlook.com


 *-*-*-*-*
 
 *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-* *-*-*-*-*
 
 *-*-*-*-*
 
 
### Changements apportés dans la V3.0
-------------------------------------

#### Correction de BUG
- Correction de certaines valeures qui n'etait pas mis a jour en temps reel sur l'ecran

#### Améliorations
- Re design total de la partie graphique
- Amelioration de l'utilisation de la memoire -4% soit 275 octets
- 13 ecrans Nextion compatible au lieu de 5 (serie standard, enhanced et inteligent) en resolution 800*480
- Ajout de la gestion de la temperature du bac (au lieu de juste la securiser)
- Passage de 2 (bleu, blanc) a 4 (couleur 1, couleur 2, couleur3 3, couleur 4) canaux independant d'éclairage
- Ajout d'un bouton "changement d'eau" qui coupe la pompe de remontée, l'osmolation, l'ecumeur et le chauffage. cela permet de vidanger sa decante et faire son changement d'eau
- Ajout d'une page dédié au parametrage de l'eclairage
- Ajout d'une page dédié au alarme
- Réglage de la luminosité de l'ecran
- Gestion de l'eclairage du refuge
- Séparation entre acalmie nocturne et acalmie nourissage pour avoir si besoin deux valeur differente
- Supression des messages d'erreur sur l'ecran principale, remplacer par un icone de notification dirigeant vers la page de visualisation des erreurs (carte SD)
- Ajout d'un bouton d'activation / desactivation du buzzer
- Choix dans la fonction du flotteur de securité, securité du niveau decante ou securité du niveau du bac (descente bouchée)
- Ajout d'un flotteur niveau haut pour la reserve d'eau
- Remplissage de la reserve d'eau osmosé avec un timer de securité (pour couper une elecrovanne)
- Choix pour l'utilisation d'une multiprise commandée (inverse le fonctionement de l'ecumeur, la pompe de remonté et le chauffage)
- Retour sur la page principale apres 1 minutes d'inactivité pour toutes les pages (sauf pour la page calibrage des sondes)
- Code pin pour deverouiller SIM
- 4 courbes graphique de mesure pour les ecrans standard (au lieu de deux precedement)
- Confirmation par pop up pour securiser certaine action sur l'ecran
- Modification des valeurs via clavier azerty ou numerique
- Possibilité de gerer un RAC via PH (en remplacement des mesure PH du bac. desactive les alarmes PH)
- Ajout d'un bouton "recovery" pour remmetre l'aquabouns au parametre d'origine
- Controlle des rampe KESSIL (A80 et A160)
- Possibiliter de configurer le nombre de clic sur le distributeur de nouriture

