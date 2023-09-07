## Drawed Trees
The goal of this project is to create an algorithm that can generate trees with genome preservation and the possibility of crossing.

The project is inspired by [this video](https://youtu.be/HHcx0pyic3I?si=ZSXOmZuqSxfaQiwk). Many ideas are taken from there.


### Brief description of the genome:
 - Genome length: 196 bytes.
 - The tree is generated using the [L-system](https://en.wikipedia.org/wiki/L-system). The genome defines this L-system.
 - The genome sets the angles of the branches, the color and thickness of the trunk.
 - The leaves are imitated by swirling lines, which are also set by the genome.


At the moment, the algorithm is able to generate both a plausible earth tree and an alien tree.

Earth tree             |  Alien tree
:-------------------------:|:-------------------------:
![e8d434e4-17e2-4b92-aa64-5f30f740027a](https://github.com/VladislavWaza/DrawedTrees/assets/73028197/4eb3c79f-e9aa-4189-b9e2-43cec356bab7)  |  ![Безымянный](https://github.com/VladislavWaza/DrawedTrees/assets/73028197/5add7627-bf77-4bf5-bcb0-a4494573c6fb)



