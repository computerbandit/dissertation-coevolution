# CoEvolution-Project

Game Design Applications and Implementation of Evolutionary Learning of an Agent in a 2D Platforming Environment 
To break the problem down into parts:
•	Game Framework design implementation and testing (2D Platformer)
•	Evolving a population of agents to complete a set of procedurally generated levels (or single levels)
•	Co-Evolve the agent and the level to incorporate a goal and to challenge the player
Design progress and stages 
Game Framework
Design Requirements:
1.	State Machine (Main Game Loop)
2.	Rendering 
3.	Input Handling (Keyboard and Mouse)
4.	Asset Management (Textures, sheets and animation)
5.	GUI System (Buttons and sliders info/text display)
6.	Environment space for traversable 2D Platforming Levels
a.	Made up of tiles that can be loaded from an external file
b.	Levels require start and finish
c.	Levels are made up of a number of smaller levels – stitched together
d.	Sections of levels need to be mutable 
e.	Collision detection
f.	Optimisation of collision with tiles and rendering
g.	Levels are made up of a tile map and an overlaying entity map
7.	Entity System
a.	Update components and render sprites attached to the entity
8.	Animation Controller
a.	Control the animation of an entity throughout its lifetime
9.	Player Controlled Character
a.	Lives 
b.	Level timer
c.	Specific Control of the player (Linked to the procedural design of levels) 
i.	Simple move set, jump, jump hold, left, right, stop
d.	Explicit Tile collision events
i.	Death, checkpoint and finish flag tiles
e.	Points/score system (time to complete level + coins collected)
 
Stage two Evolving agents to learn persuadably generated levels 
Design Requirements: 
1.	Neural Network 
a.	Generate a fully connected network with a given topology
b.	Convert to and from a Chromosome (string of weights which govern the networks connections)
c.	Simple feed forward layer architecture, sigmoid activation nodes.
d.	Input and output parsed to and from the Game Framework
e.	Neural network Controlled Agent
i.	Connect the input of the network to quantized view of the level around the player (in a radius, tile values for solid and death tiles)
2.	Genetic Algorithm 
a.	Evaluate fitness of a network-controlled agent
i.	Calculated with time, points, percent of level complete and finished or not, coins collected
b.	Given the fitness, calculate everyone’s fitness ratio based on the current population. 
c.	Select 2 networks based on the ratio for the current population
