# Ray Tracer
 Ray Tracing Engine 

The program implements a Ray Tracing Engine that supports
    - Anti Aliasing through Super sampling
    - Diffuse Shading
    - Specular Highlights
    - Refraction, Reflection and Fresnel
    - Point, Directional and Spot Light
    - Shadow Casting 
    - Texture Mapping 
    - Env Sky Map
    - Distributed Ray Tracing (Motion Blur, Out of focus)
    - Triangluar Mesh Support
    - Custom OBJ loader
    
    
PROGRAM STRUCTURE

The ray tracer is constructed in modules such that Each shape to be ray traced is in a seperate module with all the intersection detection functions of that shape collected together into that module. The Light module has an abstract class Light which is inherited by Point Light, directional light and spot light subclass. This provides the functionality to add more lights in an organised way. The four material sub classes lambertian, metal, transparent, fresnelmat implements the different shading effects. Each material has a texture component attached to it (a seperate module of its own). The Ray tracer currently supports a constant_texture, checker_texture, image_texture with planar mapping, image_texture with cylindrical map. The Camera and the ray it shoots are seperate modules. So each shape with a material, that can be hit by a ray, can be added to a hitable list. The Color function in the source.cpp takes in th hitable list, lights, the ray and calculates the color of that pixel. The for loop in

HOW THE PROGRAM WORKS

   - In Source.cpp, create a material instance with a texture instance
   - If the texture instance contains a image, then load the image into program using stbi_load
   - Create a shape instance initialising it with the material
   - Add the shapes to the list array. This is of type hitable.
   - The list of shapes are then added to a hitable_list instance 
   - Then each ray is evaluated against the hitable list to find the first object the ray can see
   - The color of that pixel is calculated by taking the properties of the object the ray is hitting
   - If the ray doesn't hit any object, an spherical env map if present is taken into account

LIMITATION AND WORK IN PROGRESS

    - As of now the ray tracer can handle only one light in the scene. But adding a module that takes in multiple lights and sending its reference instead of the pointer to a single light should sufficient to handle multiple lights
    - CPU based ray tracer. Multithreading can improve speed
    - Area Lights and Image Based Illumination are the features currently in progress
    - UI to handle the ray tracer would be really nice
    
