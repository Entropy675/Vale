Entropy

This was tested on an Ubuntu Linux system. 
The code should be transferrable to any environment configured to include OpenFrameworks libraries.

On Linux, please change config.make to point to your OpenFrameworks installation, then run:
make
cd bin
./project


On Windows, please:
Open visual studio
Use openframeworks projectGenerator to generate an app
Replace the /src and /bin folders with mine
Compile and run





Future Plans


Technical:
- HUD system/GUI system
- Raytracing
- Level creator
- Particle system

Ideas:
- Generative AI Narrative System *** (I really like this idea) ***

Vale’s narrative system will eventually integrate a generative AI-driven engine that dynamically shapes player experiences. Each player’s journey will unfold through a personal "fairytailer"—a generative AI model running locally on their machine—which interprets their words and actions to drive individualized story progression. These fairytailers contribute to a server-side narrative engine, known as the “void,” which determines overarching story elements, distinguishing true and false aspects of the game’s lore.

While the server-side narrative engine establishes the broad framework, each player’s local game engine brings their world to life, allowing individual story elements to act independently. These elements follow the physical laws within Vale and act according to their own “will” based on their unique interpretation of the player’s interactions and thier own personal story (provided by the void). The result is a cohesive yet personalized narrative experience, where each player’s world reflects a distinct, perspective-driven story within a shared mythos.

- Generative AI based procedural generation
Kind of straight forward (but hard), an implementation of the 3d voxel denoising paper

- Normal procedural generation
For maps and skys, maybe throw some random moons/suns in there. Random color pallete worlds.

