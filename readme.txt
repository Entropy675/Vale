By Entropy67 (Sibte Kazmi)
This was tested on an Ubuntu Linux system.
The code should be transferrable to any environment configured to include OpenFrameworks libraries.

Getting involved
This project is licensed under Creative Commons Non-Commercial (CC BY NC) - however I am not opposed to it being used for commercial purposes. Please reach out to me if you plan on doing so. I will support you! The license remains because I mean to maintain my intellectual property rights (I'm not rich).

Forking/Branching/PRs
Feel free to fork and use my code, as long as you maintain the license and contribute back to the community.
I will thoroughly review any PRs submitted, and give a personal analysis.
If sufficient standards are met the code will be integrated. Please try to match the style of the codebase.

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
- Raytracing (only very efficient implementations (approximations) for me!)
- Material system
- Finishing the tag-based physics system
- Performance overhaul | simd wrapper?
                       | research optimizations in cache memory access & how memory is moved to gpu
                       | generally find hot code and optimize 
- Level/model creator (and interplay with other formats)
- Procedural animation system
- Particle system
- Shaders (difference of gaussians for edge detection? anisotropic kuwahara filter for an anime look?)
- Procedural generation (maps and skys, throw some random moons/suns in. Random aesthetic color pallete worlds - can we generate worlds?)

Creative:

- Generative AI Narrative System *** (I really like this idea) ***

Vale’s narrative system will eventually integrate a generative AI-driven engine that dynamically shapes player experiences. Each player’s journey will unfold through a personal "fairytailer"—a generative AI model running locally on their machine—which interprets their words and actions to drive individualized story progression. These fairytailers contribute to a server-side narrative engine, known as the “void,” which determines overarching story elements, distinguishing true and false aspects of the game’s lore.

While the server-side narrative engine establishes the broad framework, each player’s local narrative engine (local LLM) brings their world to life, allowing individual story elements to act independently. These elements follow the physical laws within Vale and act according to their own “will” based on their unique interpretation of the player’s interactions and thier own personal story (provided by the server side void, which is the core source of truth when it comes to story). A local LLM will interpret the stories of NPC's provided by the server, and interact with the games API to manipulate the NPC avatars according to their personality. The result is a cohesive yet personalized narrative experience, where each player’s world reflects a distinct, perspective-driven story within a shared mythos.

Key idea for efficient world simulation:
The story of an entity only updates when its needed for the local runtime. Each NPC retains its own interpretation of past events, meaning that while historical events remain consistent, individual characters may remember or react to them differently. Understand the following; a story of a location could remain stagnant for many years, and the moment a local model representing the player interacts with it, suddenly the void generates the story missing between when it was last interacted with till now, same applies to individual characters, their lives are recorded as trajectories and histories of internal thought, and when time skips need to occur, the gap is generated from the given information and the story of the world from that moment till the player interaction (or upon loading the entity so that it can be done in the background moments before the player interacts with the NPC). That way if the player massively changes the world between their interactions, the back-generated world of all NPCs will maintain the impact of this disruption and remain internally consistant, all while not needing to simulate all of reality at all times. The goal is to allow absolute player autonomy given an arbitrarily generated world.

- Other AI components
Some potentially integrated open source models:
https://github.com/Tencent/Hunyuan3D-2                          | 3D model gen
https://aruichen.github.io/Dora/                                | for 3D model gen
https://tangoflux.github.io/                                    | for audio gen
https://huggingface.co/Tencent-Hunyuan/HYDiT-LoRA               | for base 2d image gen
https://m-usamasaleem.github.io/publication/GenHMR/GenHMR.html  | for ingame AI vision
Looking at the https://github.com/deepseek-ai/DeepSeek-V3 for the primary local/remote tale generation system. 
If these are integrated, the license may be changed.
List subject to change as this is an evolving field, and I do not want to keep it updated... I'll update it whenever I feel like it.

