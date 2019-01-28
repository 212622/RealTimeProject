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
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/MotionrulesB.png)

## Tasks description
- CAMERA
1. Image scanning and thresholding: read the pixel color in a given window and discard the pixel with black color.
2. Centroid computation: compute the centroid of pixels found in the previous point.
3. Camera control: control the camera axes to move to the calculated centroid.
4. When a fixed number of detections is done, the enemys trajectory is calculated.
5. Using the trajectory information obtained from the previous point, a Patriots defense missile is launched.

- DRAW
1. Draws every image in the updated position.
2. Detects possiles crashes between enemies and allies.
3. Write statistics in the status window and title and instructions in the menu area.

- ALLY
1. Using camera's informations, each Patriots defense missile moves to the target position.

- ENEMY:
1. Generates a random trajectory.
2. Moves the plane using a fixed speed to the target position.

- COMMAND:
1. Takes users command and activates enemy task or exit the program or visualizes camera and trajectory depending on the pressed key.

## Istruzioni per Git

Per caricare le modifiche

	git add .
	git commit -m 'messaggio'
	git push origin master

Per scaricare le modifiche

	git pull
	
Per vedere stato delle modifiche

	git status
	
