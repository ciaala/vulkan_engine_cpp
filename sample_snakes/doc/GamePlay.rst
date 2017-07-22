Snakes: a clone of Nibbler
==========================

Sections

#. Flow of idea
#. Snake V1

1- Flow of Idea
===============

Player control a small snake that has to move around and eat some fruit and insect.
By eating the snake becomes bigger

The snake move around by turning left or right.

Fruits grow and fall from the trees and insects come out of the rocks and move around.

To move to the next level in the game he has to reach a specified length.

Each level there's a different layout of food and obstacles.

The obstacles are static layout elements.
We can make water and bridge that opens at specific time as time-dynamic space-static obstacles.
Exploding fruit can also be a game play element.
And some spitting around insect-enemy are slowly moving around their rock on the map.
The snake can also shake its tail and gain extra speed for a while.

Snake V1
========

World Elements
Snake:

- move around with arrows
- eat fruits and insects and becomes longer
- die if touches border, obstacles and it is hit by enemy-insect

Fruits

- different kind, for now just different color of the snake segments
- static on the ground

Tree

- are obstacles
- drops fruits in a radius around them of 2

Rocks

- are obstacles
- spawn insects

Insects

- different kind ( food, enemy )
- they eat fruits around them and become tastier up to 3 food slots
- they move around its rock 3x3

Enemy Insects

- one single kind
- move around its rock 3x3
- they eat fruits around them and become tastier up to 3 food slots
- and shoots and hit at a distance of 2 to the snake

Water

- is an obstacle

Bridge

- is an obstacle if closed
- is a passage if open
