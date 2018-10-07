# ProjectRadon
A Voxel Game Engine that will hopefully one day turn into an actual game, called Radon.

However, we are far, far, far from being finished. So, here are a few things that we should work on that are HIGHLY Important:


- Making our already pretty fast rendering system even faster by using DOD (Data Oriented Design)
- Creating a Scene system that will store all of the required drawing and transform data required to draw an entire scene, this is to simplify our main game loop
- Commenting some of our code, because so far I am the only one who knows what any of it means

ALREADY DONE:
- Offloading noise generation and planet generation to the GPU using OpenCL (For huge speedups) 

Some really nice things to have but are not required at this point or would require a rewrite:

- DUAL CONTOURING -- this should probably be moved to the HIGHLY Important list, but I dont understand it well enough to even attempt to do it
- OCTREE SYSTEM -- Goes with the Dual Contouring, would be extremely nice to have some sort of generic class that we can call to to use for textures, DC, and n-body physics optimization. 
