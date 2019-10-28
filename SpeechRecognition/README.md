# Reconnaisance de voix Demo
C'est la partie de projet Reconnaissance de voix

Introduction 
===========
Le projet est inspiree par @Sirajology on [Youtube](https://youtu.be/u9FPqkuoEJ8).  
La version courante est v1.4. 

PRE-REQUIS
===========

tflearn (http://tflearn.org/)  
tensorflow (https://www.tensorflow.org/versions/r0.12/get_started/os_setup.html)  
future  
librosa  
numpy  
  
Utiliser pip pour installer facilement!  
Liste de tous les packages au fin est disponible dans le fichier <em>requirments.txt</em>  

Bases de donnes
===========
Si vous voulez entrainer le model, vous allez besion de la bases de donnees speech commands  
qui est disponible sur le lien:  
  
https://www.kaggle.com/c/tensorflow-speech-recognition-challenge/data?fbclid=IwAR1TPQ16OH-XG62eocOPMRQ-W_apEX8ziA3_OLcUjECuK5XbdBs0rB3ZCAA
  
ou lien direct ici:  
  
http://download.tensorflow.org/data/speech_commands_v0.01.tar.gz.
  
PREPARATION
=======
Choisir 10 dossiers (10 mots) et les mettre dans un dossier <em>speech</em>.  
<speech> doit etre dans la meme repertoire qui le script <em>make_label.py</em>  
Pour preparer nos donnees utiliser notre script <em>make_label.py</em> avec la commande:  
  
python make_label.py
  
v1.0 - OUTDATED
========
Reconnaissance des chiffres 0-9 fournis comme fichier audio.  
La version initiale qui utilise le script creee par @Sirajology.  
Avec un temp pour entrainement tres long et des prediction mauvaise on a laisse ce script a cote.  
Utilisation de mfcc pour la creation des batchs  
'python demo.py' - pour l'entrainment et sauvgarde de model  
'python test.py' - pour fournir des autre fichiers et test   
  
version v1.2 - OUTDATED
================
Deuxieme version qui entraine tres vite avec une precision moyenn.  
Utilisation de waves pour la creation des batchs  
'python class.py <em>cheminfichiertest</em>' - pour l'entrainement et classification d'un clip audio  

version v1.4 - VERSION COURANT
==============
Verison actuelle qui reconnait 10 mots.   
Pour entrainer le model*: python train4.py   
Pour tester : python test.py <em>cheminfichiertest</em>  
  
Le resultat de la prediction est donnee par la derniere ligne:"Le mot de fichier est <em>fichier</em>"  
Les fichier test sont disponibles dans la repertoire TestFiles!  
  
*Si vous entrainer le model, le model precedent sera écrasé!  
!!!!!IMPORTANT!!!! Si vous voulez tester le script avec un modèle déja entrainé, effectuez d'abord les tests avant de lancer un entrainement (qui effacera notre modèle correctement entrainé)  
Ce probème sera réglé bientôt!