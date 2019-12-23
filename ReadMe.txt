This project is based on Opengl and SimpleGLUT (https://icg.gwu.edu/sites/g/files/zaxdzs1481/f/downloads/SimpleGLUT.zip). 

What did I do:
Source/Quanternion.h: Quanternion class with quanternions calculation.
Source/Interpolation.h:  Tnterpolating translation of key frames by Catmul-Rom and B-splines, and interpolating rotation by fixed angles and quanternion. Keyframes and speed setting.
SimpleGLUT.cpp:	choose 2 rotations and 2 translations and update t in update() function.

Compile command:	"g++ SimpleGLUT.cpp -o SimpleGLUT -lopengl32 -lglew32 -lfreeglut -lglu32"