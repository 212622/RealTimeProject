# Patriots
Real Time Project by Tomas Torricelli and Lorenzo Cuoghi

This project simulate a physical system or the behavior of active agents interacting with each other and with the environment.

Simulate a set of Patriot defense missiles that identify enemy targets, predict their trajectories and are launched to catch them.

## General description
1. Each enemy is animated by dedicated periodic task that updates its state every period.
2. Enemy and defense missile motion is indipendent on the others, but depends on its speed and direction.
3. When camera detectes a new enemy, a patriots defense missile is launch for catch it.
4. Enemies must exploed when they arrive at the city or if they collide with a Patriots defense missile.
5. The user can interact with the program and create new enemies.
6. The system provides information on: number of active enemies and Patriots defense missiles, deadline misses, current statistics.

## Application Structure
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/Applicationstructure.png)
## Graphical layout
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/Graphicallayout.png)
## User interface
The user can create a new enemy:
- SPACE : create a new enemy with random direction.
- Q : visualize the camera and the predicted trajectory.
- ESC : exit the program.
## Motion rules
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/Motionrules.png)

## Tasks description
## Istruzioni per Git

Per caricare le modifiche

	git add .
	git commit -m 'messaggio'
	git push origin master

Per scaricare le modifiche

	git pull
	
Per vedere stato delle modifiche

	git status
	
