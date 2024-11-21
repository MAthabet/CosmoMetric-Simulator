#pragma once

// to maitain golden ratio :D
#define WINDOW_H 927
#define WINDOW_W 1500

#define PI 22/7

//sun radius 696265000 m
//our length scale 1:1e7
const float StarMass = 1.9891e30;
const float StarRadius = 69.6265;

// * 1.e24
const float MAX_MASS = 1898;
const float MIN_MASS = 0.0130;

//e28 to avoid too small radius
const float AVG_PLANET_DENS = 2987.89e28;

const float TIME_STEP = 1.0f / 60.0f;
// Gscaled
const float G = 8.5457e-31 /TIME_STEP/ TIME_STEP;
const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;
