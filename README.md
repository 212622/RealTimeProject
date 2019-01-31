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

## Graphical layout
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/Graphicallayout.png)

## User interface
The user can interact with the application with the following keys:
- SPACE : create a new enemy with random direction;
- Q : visualize the camera and the predicted trajectory;
- ESC : exit the program.

## Tasks description
- CAMERA
1. Image scanning and thresholding: reads the pixel color in a given window and discard the pixel with black color.
2. Centroid computation: computes the centroid of pixels found in the previous point.
3. Camera control: controls the camera axes to move to the calculated centroid.
4. When a fixed number of detections is done, the enemies trajectory is calculated.
5. Using the trajectory information obtained from the previous point, a Patriots defense missile is launched.

- DRAW
1. Draws every image in the updated position.
2. Detects possiles crashes between enemies and allies.
3. Writes statistics in the status window and title and instructions in the menu area.

- ALLY
1. Calculates trajectory and speed using camera's informations.
2. Moves the Patriots defense missile to the target position.

- ENEMY
1. Generates a random trajectory.
2. Moves the plane with a fixed speed to the target position.

- COMMAND
1. Takes user commands.
2. Activates enemy task or visualizes camera and trajectory depending on the pressed key.

## Application Structure
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/Applicationstructure.png)

## Motion rules
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/Motionrules.png)
![alt text](https://github.com/212622/RealTimeProject/blob/master/README/Motionrules2.png)
