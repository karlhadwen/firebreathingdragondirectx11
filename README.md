# Fire breathing dragon
Fire breathing dragon that can burn villages. The program includes a terrain and a skybox, and lastly an object loader.

# What I used to build this particular project
This game (I use that term lightly) was built using Visual Studio 2012 and DirectX SDK (June 2012).

# Playing instructions
When you start the game you will be presented with a follow cam that follows the Dragon around. The keys to control the dragon are as follows:
•	Forward: W
•	Backward: S
•	Strafe Left: A
•	Strafe Right: D
•	Space Bar: Fire the Dragon’s Breath
In addition to using the keys to move the Dragon you can also use the mouse to move the view of the Dragon. There’s a terrain .obj that’s incorporated within the program as well, although no Physics has been applied to the terrain; therefore the Dragon can fly through the terrain. There is collision detection between the Dragon’s breath and the villages (5 villages on spawn). I did try and try to implement Bullet but it seems that my coordinates for my sphere was backwards therefore it would have taken me a lot longer to implement the API. I did actually implement triangle to triangle Physics on the terrain but at 64*64 triangles it made the game really slow, therefore I removed it and just kept collision detection using a sphere based around my village .obj’s.
When the Dragon is flying the wings have sound attached as they’re separate .obj files (the wings)—the sound of the wings are very Dragon like to say the least.

# Other
This particular game uses multiple design patterns: IRenderable, IUpdatable and IResource. This was my first game that I've ever created and it was without a doubt a bigger task than I initially thought.
