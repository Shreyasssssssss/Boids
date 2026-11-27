#ifndef BOIDS_H
#define BOIDS_H

#include <cglm/cglm.h>

//simulation paramaters
#define NUM_BOIDS 100
#define BOX_SIZE 20.0f

//nearby boids detection distanes
#define DETECTION_RADIUS 5.0f
#define SEPARATION_RADIUS 2.0f

//weightage of each flocking rule
#define SEPARATION_WEIGHT 1.5f
#define ALIGNMENT_WEIGHT 1.0f
#define COHESION_WEIGHT 1.0f

//movement limits
#define MAX_SPEED 4.0f
#define MAX_FORCE 2.0f

//creating structure for boid
typedef struct {
    vec3 position;
    vec3 velocity;
} boid;

void initBoids(boid boids[], int count);
void updateBoids(boid boids[], boid next[], int count, float dt);

#endif