# effluvium
A Fluid Dynamics Simulator in Unreal 

#### The code for this project is located at this Url below and only uses Fluid_Array.cpp and Fluid_Array.h. The other classes are test classes and do not contian working code. [Link to code](https://github.com/robertjdoig/effluvium/tree/master/Effluvium_UE4/Source/Effluvium_UE4)

## Brief 

This is my project for Physics and animation. I focused on adapting and understanding Jos Stam's stable fluid code from GGC 2003. This code was ported to C++ by Andy Thomason. I used both of these projects and changed and merged them in some places to work with UE4. Initially the idea was to create a 3D volume of smoke or gas which the player could manipulate thought vents or filters. This idea changed when technically problems with Unreals dynamic arrays (TArrays) had a bug. This will be explained in more detail in further sections. The goal was changed to understand and get a working version in 2D as time was limited. 


## Implementation 

I decided to use Unreal Engine to start this project as you can write standard C++. This proved to be the right move as even the original papers code, written in C, was able to run within the engine with mimimal editting. This made debugging and rewriting some of the variables more manable. Linking with Unreal's build in structures and implementation of variables was the biggest let down. The std::array which was used in Andy Thomason's version of the code was not compatiable. This meant each functions was implemented and converted over serperately to make sure that it fit with Unreal's implementation. Currently I am not sure why however I narrowed it down to the SWAP function as this was creating NULL pointer references and crashed Unreal before an error message could be generated. This was a fairly large problem that took up the most of my time, however it made me look though and really understand both the physics behind the fluid's simulation and also the code. 

The functions followed the same pattern as the original code where there are three main phases that happen, user input, velocity and density. These three stages break up the problem into manageable sections to explore. The user input stage controls how much each time should be added to the simulation. For instance each time the simulation is called how much and where to add the density and velocity. The next stage takes all of the velocitys of each vector fields and calculates the movement around the grid. This is then used to move the density's which are represented as colour, into neighbouring cells. This is where most simulations of fluid fail and it is due to the pressure of the fluid compounding and creating a Not a Number error (NaN). This solver uses the Guass Siedel relaxation method.    


# Technical Problems

## Rendering 

I explored a number of different way of rendering or displaying the smoke. This mainly looked at instancing, pixel shaders and dynamic textures. In the end none of these ways worked so was forced to use the ineffecient plane mesh grid which is currently being used. 

The problem I encounted with instancing is that Unreal was more complex to setup than initially thought. 



A simple colour shader was also not easy to apply. The reason for this is that normal HLSL code can not be written straight out of the box and a different method which involves adapting the way that the experiement shader node is called. After reading the tutorial from Fredrik Lindh on the Unreal discussion page (https://wiki.unrealengine.com/HLSL_Shaders), I would have to use his plugin to include a pixel shader. This is still believe is the best method for the 2D version of the fluids. It would cut out a lot of the rendering cycles currently slowing down the project. It would also allow for a great level of detail as it can be down to 1 pixel wide. 

However, I had explored another possibly which was creating a texture every fluid call which would be applied to a single plane mesh. This was scraped when I tested out this method with perlin noise. It would run slowly then the current solution due to having to change the texture. This means loading and swapping a lot of data which the gpu can not handle aswell as leting the cpu change each plane when the calucation is done. 
