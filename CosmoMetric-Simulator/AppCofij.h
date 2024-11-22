#pragma once

#define PI 22/7

// to maitain golden ratio :D
const int WINDOW_W = 1500;
const int WINDOW_H = 927;

const float VIEW_W = WINDOW_W * 2;
const float VIEW_H = WINDOW_H * 2;
const float VIEW_delta = 25;

//TODO
const float SCALE = 1;
const float R_SCALE = 1; // only affect visualization

//sun radius 696265000 m
const float StarMass = 1.9891e30;
const float StarRadius = 69.6265 * R_SCALE;
const float StarGF = StarRadius +50; // star exerts more force in this field
// * 1.e24
const float MAX_MASS = 1898;
const float MIN_MASS = 0.0130;

const float AVG_PLANET_DENS = 2987.89e4;

const float TIME_STEP = 1.0f / 60.0f;
// Gscaled
const float G = 8.5457e-29 / TIME_STEP / TIME_STEP;
const int VELOCITY_ITERATIONS = 8;
const int POSITION_ITERATIONS = 3;
