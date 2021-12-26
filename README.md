# Autonomous-Car

## French/Français

Voiture autonome contrôlée à l'aide de deux Arduinos Nanos qui recoivent des commandes à l'aide d'une télécommande IR. 

La majorité des fonctions de la voiture sont effectuées à travers d'un des Arduinos ('master'), qui fonctionne avec le code dans 'main' et les headers associés. Le deuxième arduino 'slave' sert juste à effectuer la détection d'obstacles en mode autonome lorsque il reçoit un signal de 'master'. 

Voici un resumé de chacun des fichiers, plus de détails se retrouvent dans le code sous forme de commentaires. 
### Master Arduino 
Le code est executé à partir du fichier 'main.ino', qui se refère à plusieurs headers, chacun servant à faire fonctionner une partie spécifique de la voiture. Voici une courte explication des différents fichiers (aussi présente en commentaire dans chacun d'eux). 

*"main.ino"*: Contrôle l'état général de la voiture en processant les commandes reçues par la télécommande et en exécutant les fonctions associées (avec la fonction _process_command()_). En utilisant la variable 'command' (qui est une chaîne de caractères), il est plus facile de comprendre le *flow* du code que si on traitait directement les commandes sous forme de hex codes. La valeur de la variable command est obtenue à l'aide de la fonction _get_command()_, fonction qui est définie dans "remote.h". 

*"remote.h"*: Défini la fonction _get_command()_, qui met à jour la variable 'command' avant qu'elle soit utilisée dans "main.ino". C'est ici qu'on fait le traitement brut des hex codes envoyés par la télécommande. 

*"motors.h"*: Défini toutes les fonctions générales de la voiture excepté celles qui servent à se déplacer. Par exemple, on y retrouve les fonctions qui servent à tourner, à changer de vitesse et à changer de direction. 

*"moving_orders.h"*: Défini les fonctions nécéssaires à que la voiture se déplace. On peut soit faire avancer la voiture pour un temps spécifique (ex: 5secondes), ou la faire avancer indéfiniment jusqu'à qu'on l'arrête. 

*"auto_mode.h"*: Défini la fonction _detect_obstacles()_, qui envoie un signal au Arduino 'slave', lui indiquant d'analyser les environs et déterminer un chemin sans obstacles, attend sa réponse et détermine quel est le chemin à suivre. 

*"setup.h"*: Défini les variables nécéssaires au fonctionnement du code et exécute _void setup()_. 

### Slave Arduino 
Tout le code se retrouve dans le fichier "slave_code.ino" qui sert uniquement à détecter les obstacles en mode de déplacement autonome et à retourner un chemin à suivre au 'master' Arduino. L'utilisation de la librarie _NewPing_ permet d'augmenter grandement la vitesse du processus. Avant j'utilisais une fonction que j'avais écrite moi-même pour prendre 44 mesures afin de déterminer une distance plus précise, mais la légère augmentation de précision ne valait pas la peine avec le plus long temps d'exécution. 

## En / English 

Autonomous car controlled by two Arduino Nano's that receive commands from an IR remote.  

The majority of the car's functions are performed through one of the Arduinos ('master'), which works with the code in the 'main' directory (.ino file and the supporting headers). The second ('slave') arduino  is just used to perform the obstacle detection in autonomous mode when it receives a signal from 'master'. 

Here is a summary of each of the files, more details can be found in the code in comment form. 

### Arduino Master 
The code is executed from the 'main.ino' file, which refers to several headers, each one being used to operate a specific functionality of the car. Here is a short explanation of the different files. 

*main.ino "*: Controls the general state of the car by processing the commands received by the remote control and by calling the associated functions (with the _process_command()_ function). By using the 'command' variable (which is a string), the flow of the code is easier to understand than if we directly handled the hex codes sent by the remote. The value of the command variable is obtained using the _get_command()_ function, which is defined in "remote.h". 

*remote.h "*: Defines the _get_command()_ function, which updates the 'command' variable before it is used in "main.ino". This is where we do the raw processing of the hex codes sent by the remote control. 

*motors.h "*: Defines all the general functions of the car except those used to move. For example, the functions that are used to turn, to change speed and to change direction are defined in this header. 

*"moving_orders.h "*: Defines the functions needed for the car to move. It is possible to either make the car move for a specific time (ex: 5seconds), or make it move indefinitely until you stop it. 

*auto_mode.h "*: Defines the _detect_obstacles()_ function, which sends a signal to the Arduino 'slave', telling it to analyze the surroundings and find a path without obstacles, wait for its response, and determine which path to follow. 

*setup.h "*: Defines the variables needed to run the code and executes _void setup()_. 

### Slave Arduino
All of the code is in the file "slave_code.ino" which is only used to detect obstacles in autonomous mode and return a path to follow to the 'master' Arduino. The use of the _NewPing_ library allows to greatly increase the speed of the process. I used to use a function I wrote myself to take 44 measurements to determine a more accurate distance, but the slight increase in accuracy was not worth it with the longer execution time. 
