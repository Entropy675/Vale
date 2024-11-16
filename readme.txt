By Entropy (Sibte Kazmi)
This was tested on an Ubuntu Linux system. 
The code should be transferrable to any environment configured to include OpenFrameworks libraries.

Getting involved
This project is licensed under Creative Commons Non-Commercial (CC BY NC) - however I am not opposed to it being used for commercial purposes. Please reach out to me if you plan on doing so. I will support you! The license is what it is because I mean to maintain my intellectual property rights. If you wish to use my engine, I will ask for hardly anything in return (depends on the amount of work you make me do lol).

Forking/Branching/PRs
Feel free to fork and use my code, as long as you maintain the license and contribute back to the community.
I will thoroughly review any PRs submitted, and give a personal analysis. 
If sufficient standards are met the code will be integrated.

--- BUILD ---

On Linux, please change config.make to point to your OpenFrameworks installation, then run:
make
cd bin
./project


On Windows, please:
Open visual studio
Use openframeworks projectGenerator to generate an app
Replace the /src and /bin folders with mine
Compile and run

--- PLANS ---


Technical:
- HUD system/GUI system
- Raytracing
- Performance overhaul | lots of different ways, ex: 
                       | z-buffer for occlusion
                       | switching to matricies for all calculations, simd wrapper?
                       | research optimizations in cache memory access & how memory is moved to gpu
- Level/model creator (and interplay with other formats)
- Procedural animation system
- Particle system
- Shaders (difference of gaussians for edge detection? anisotropic kuwahara filter for an anime look?)
- Procedural generation (maps and skys, throw some random moons/suns in. Random aesthetic color pallete worlds)

Creative:

- Generative AI Narrative System *** (I really like this idea) ***

Vale’s narrative system will eventually integrate a generative AI-driven engine that dynamically shapes player experiences. Each player’s journey will unfold through a personal "fairytailer"—a generative AI model running locally on their machine—which interprets their words and actions to drive individualized story progression. These fairytailers contribute to a server-side narrative engine, known as the “void,” which determines overarching story elements, distinguishing true and false aspects of the game’s lore.

While the server-side narrative engine establishes the broad framework, each player’s local narrative engine (local LLM) brings their world to life, allowing individual story elements to act independently. These elements follow the physical laws within Vale and act according to their own “will” based on their unique interpretation of the player’s interactions and thier own personal story (provided by the server side void, which is the core source of truth when it comes to story). A local LLM will interpret the stories of NPC's provided by the server, and interact with the games API to manipulate the NPC avatars according to their personality. The result is a cohesive yet personalized narrative experience, where each player’s world reflects a distinct, perspective-driven story within a shared mythos.

- Generative AI based procedural generation
Kind of straight forward (but very hard), an implementation for creating 3d objects based off the 3d voxel denoising paper (https://arxiv.org/abs/2306.07473 or https://github.com/genentech/voxmol)

