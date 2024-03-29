# OpenGL Bedroom - Final Project Submission
Tristan English\
Computer Graphics (CSCI 4229) Fall 2022

## Screenshot

<img width="595" alt="bedroom" src="https://user-images.githubusercontent.com/71680462/230977126-93a92f38-7d58-4bca-bea2-6badbbcfd371.png">

## Description

For this project, I modeled, lit, and textured a fully furnished bedroom in OpenGL 2.0!

Extra notes / things I'm especially proud of:
- The walls, door, and window disappear appropriately depending on view angle! This allows you to see the room at all angles without obstructions.
- I spent a lot of time on some more complex, non-cuboid objects! The lamps on the dresser and the luxo lamp on the desk proved to be a challenge, as I had to define individual vertices at every point in a cylinder.
- The penguins were created in my initial OpenGL assignments, and I couldn't bear to see them go! They were written in a function that allows you to specify the color of the penguin without effect on the beak, eye, and belly color.
- Make sure to press c to see the individual objects separately!

## Run Instructions

To run, just run `make` and `./final`.

Key bindings\
  c &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Change scene\
  l &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Toggles lighting\
  F3 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Toggle light distance (1/5)\
  m &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Toggles light movement\
  <> &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Move light (when stopped)\
  [] &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Lower/rise light\
  t &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; enable/disable axes\
  +/- &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; changes field of view for perspective\
  PgDn/PgUp &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Zoom in and out\
  arrows &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;change view angle\
  0 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Reset view angle\
  ESC &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; Exit
  
## References
Texture images sourced from textures.com, CSCIx229 library sourced from Willem A. (Vlakkies) Schreuder.
