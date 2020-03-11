# Graphics_Engine
This is a repository that contains the code for a hobby project I did to learn graphics programming.
The code needs the Resources folder that I have locally to compile, I didn't think this code will be usable for other people so I didn't upload it.

## Previous work :
Now it's time to showcase some of the things I made with it in no particular order.

### 1- Uncomplete project : 3D minesweeper
This is the latest thing I tried to make with my engine, It's incomplete but I got the important mecanisms to work (It doesn't have a UI yet) like raycasting for detecting which tile was pressed and making the camera rotate around the displayed globe.
For now it's uses triangular tiles but I can use other tiling methods because of the way I create these tiles (I made an isosphere and subdivided each of it's triangle surfaces into 3 other triangles which is the same mecanism I can use for subdividing iteratively untill it forms a sphere, I hope that makes sense I guess).
I will try to complete this if I get the time.
![](https://i.imgur.com/H3BtkGh.gif)

### 2- Getting model loading and multi-textured models to work
That was one of the best moment I experienced working with opengl.
I got to integrate Asset Import Library (aka assip) to propoerlty load the model vertices, normals and the textures for the nanosuit model (from crysis).
I made a simple .obj files loader before but it wasn't as cool because the .obj format doesn't do as much as what assimp offer.
![image](https://i.imgur.com/I43pEOv.jpg)

### 3- Simple water rendering, Perlin noise terrain generation and batch rendering :
In this I did a lot of things following Thinmatrix awesome tutorial videos which helped me a lot.
The screenshot was from the time I was participating at google hashcode 2019 contest.
The code contains some glitches like the white area you see at the bottom left of the image, but I think I fixed it after I took the screenshot (sorry I don't have many screenshots of my previous work, I sometimes don't even commit when I have to).
The trees are batch randered using the same model.
The text rendering is made using a .obj file for each caracter.
![image](https://i.imgur.com/minkEkn.jpg)

### 4- Physics Based Rendering :
Here I tried to learn physics based rendering and how to use BRDF equation to make realistic materials, It wasn't good but an acceptable try I think (Some may even say that it's not physics based but I did my best to use the BRDF properly lol)
![image](https://i.imgur.com/Hevsebn.jpg)

### 5- Raymarching and simple geometry rendering:
I used signed distance functions to make a sphere and a torus and the compine them using the smoothing union operator and got reflections to work
![image](https://i.imgur.com/wlKYbhG.jpg)

### 5- Mandelbox rendering :
Here I tried using some raymarching techniques to render 3D version of the most know fractal "Mandelbrot", The first one is named mandelbox and the second one is mandelbulb
![image](https://i.imgur.com/uw8gAiT.jpg)
![image](https://i.imgur.com/7LbsTRA.jpg)
