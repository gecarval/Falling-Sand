<h2 align="center">
	REPOSITORY STATS
</h2>

<p align="center">
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/gecarval/Falling-Sand?color=lightblue" />
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/gecarval/Falling-Sand?color=yellow" />
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/gecarval/Falling-Sand?color=blue" />
	<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/gecarval/Falling-Sand?color=green" />
</p>

# Cellular Automaton
> A cellular automaton (pl. cellular automata, abbrev. CA) is a discrete model of computation studied in automata theory. Cellular automata are also called cellular spaces, tessellation automata, homogeneous structures, cellular structures, tessellation structures, and iterative arrays. Cellular automata have found application in various areas, including physics, theoretical biology and microstructure modeling.
> 
> *-On Wikipedia.*

The cellular automaton is a simulation that got populazired throught Conway's game of life where it consists of regular grids of cells or matrix where it has a finite amount of states and each cell as a defined amount of rules based on it's neighbors relative to the cell's position and with such simple rules complex and emergent behaviors could occur.

# Conway's Game of Life
> The Game of Life, also known as Conway's Game of Life or simply Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970. It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves. It is Turing complete and can simulate a universal constructor or any other Turing machine.
>
> *-On Wikipedia.*


<div align="left">
  <img src="https://upload.wikimedia.org/wikipedia/commons/e/e5/Gospers_glider_gun.gif">
</div>

The game of life has simple rule set has descrived:

- If a cell neighbourhood has less than two live cells, all cells die;
- If the alive cell has two or three live neighbours, the cells remain;
- If the cell neighbourhood has more than three cells, the singular cell dies;
- If a dead cell has exactly three alive cells in the neighbourhood, it becomes a live.

# Falling Sand Game

> A falling-sand game is a genre of video game and a sub-genre of sandbox games which typically utilize a two-dimensional particle or cellular automaton based game engine to simulate various materials interacting in a sandbox environment.
> In falling-sand games, the user can interact with (e.g. place and remove) particles on a canvas which can interact with other particles in various ways, which can lead to complex emergent behaviour. As sandbox games, they generally have an emphasis on free-form gameplay, relaxed rules, and minimal goals.
> 
> *-On Wikipedia.*

The falling sand game is a type os cellular automaton where as the game of life each set of cell as a defined behavior when in contact with diferent cells.

## Behavior

The main objective of this simulation is to recreate the behaviors of different material types within a grid-based environment. A simple action, like setting fire to water, can trigger cascading effects across large quantities of materials. These interactions create a dynamic and engaging experience, where players observe emergent physical phenomena, such as evaporation, flow, and chemical reactions.

### Sand

In this game, sand behaves like a loose granular material. When the cell below a sand particle is empty, the particle "falls" by swapping places. If the space below is occupied, the sand checks neighboring cells to move diagonally, mimicking gravitational behavior.

![image](https://github.com/user-attachments/assets/2aff4d92-dcd1-4c7f-a88c-612091dca13c)


### Water

Water behaves similarly to sand in its falling motion, but when it hits the ground, it attempts to flow left or right. The water particles randomly move into empty horizontal spaces, simulating fluidity.


![image](https://github.com/user-attachments/assets/d1d8a22f-40bb-4a6f-8853-cc9f3660d620)


### Chemical Reactions and Physics

Other complex interactions include:

- Chemical reactions such as soap bubbles and acid reactions.
- Heat-based interactions like water evaporating or lava being extinguished.
- Inertia and momentum, where water flows to fill available space in a container, and gases spread or liquids cascade based on physical properties.

## Footage

There are diverse footage took from the simulation., feel free to get a look into it:

![sand](https://github.com/user-attachments/assets/dbb0091a-3391-47b9-8831-a57871db9423)

> [!Note]
> In the sand, there is even the behavior of inercia and collision.

- Soap and acid reactions:

![soap](https://github.com/user-attachments/assets/5bcad04d-ebe3-4d50-9017-b780d0c8fb08)

![acid ract](https://github.com/user-attachments/assets/edbc1bf7-be8f-482a-a0bf-fb26216c7d23)

- Lava, fire, and water interactions:

![fire and lava](https://github.com/user-attachments/assets/5ca7a21d-e8c3-47d8-8aa4-1f06ca5a0125)

![oil burn](https://github.com/user-attachments/assets/429a0d43-9452-404f-9b31-2875e8b33fb8)

![gas and wood](https://github.com/user-attachments/assets/0937e65b-a67a-4124-b62a-0438197e09e6)


- Sand in motion, reacting to inertia and collisions:

![cinetics](https://github.com/user-attachments/assets/179f0d3a-2bfe-4e82-bd1a-5b61e9aba41f)


- Water physics, with inertia and momentum-based behavior as water fills available space:

![water physics](https://github.com/user-attachments/assets/480a9ab4-676f-4819-b778-977124b7544d)

# Installation
The project usees the [MiniLibX](https://harm-smits.github.io/42docs/libs/minilibx) using the X11 Window system in the Linux Operating System, after the instalation of all dependencies you should do as required:

- Clone the repository;
``` sh
git clone https://github.com/gecarval/Falling-Sand.git
```
- Move to the folder;
``` sh
cd Falling-Sand
```
- Compile the project;
``` sh
make
```
- execute the program.
``` sh
./render
```
## Controls
|BUTTON|ACTION|
|---|---|
|`Q`| Empty|
|`W`| Sand|
|`E`| WetSand|
|`R`| Stone|
|`T`| GunPowder|
|`Y`| Soap|
|`U`| Wood|
|`I`| Iron|
|`O`| Rust|
|`P`| Glass|
|`A`| Water|
|`S`| Lava|
|`D`| Oil|
|`F`| Acid|
|`G`| Fly|
|`Z`| Fire|
|`X`| Oxygen|
|`C`| Hidrogen|
|`V`| Propane|
|`B`| Steam|
|`N`| Missil|
|`,`| decrease brush size|
|`.`| increase brush size|
|`LMB`| activate brush|
|`RMB`| deactivate brush|
|`Esc`| Exit|
|`[`| increase speed|
|`]`| decrease speed|
|`1`| on/off 3Dcube|
|`2`| on/off falling sand|
|`Space`| clear screen|
