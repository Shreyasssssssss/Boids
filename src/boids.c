/*Naive Boids Algorithm: simulates boid flock using separation, alignment, and cohesion*/
#include "boids.h"
#include <stdlib.h>

//functionality: sets initial positions and velocities
//parameters: boids[] - array to store boids, count - number of boids to be initialized
void initBoids(boid boids[], int count){
    //creates count boids at random
    for (int i = 0; i < count; i++){
        for (int j = 0; j < 3; j++){

            //sets random position for boid within the box
            boids[i].position[j] = (float)rand()/(float)RAND_MAX * BOX_SIZE;

            //sets random velocity between -1 and 1
            boids[i].velocity[j] = (float)rand()/(float)RAND_MAX * 2.0f - 1.0f;
        }
    }
}

//functionality: limits vector's magnitude without changing direction
//parameters: v - vector to modify, max - max allowed magnitude
void limitVector (vec3 v, float max){
    //calculates magnitude of v
    float magnitude = glm_vec3_norm(v);
    
    //modifies v to be within max
    if (magnitude > max){
        //translates v to unit vector, then scales by max
        glm_vec3_normalize(v);
        glm_vec3_scale(v, max, v);
    }
}

//functionality: updates each boid's position
/*parameters: boids[] - array of current boids, next[] - array of updated boids, 
count - number of updates/boids, dt - time taken for each simulation step */
void updateBoids(boid boids[], boid next[], int count, float dt){
    for (int i = 0; i < count; i++){
       
        vec3 separation = {0.0f, 0.0f, 0.0f};
        vec3 alignment = {0.0f, 0.0f, 0.0f};
        vec3 cohesion = {0.0f, 0.0f, 0.0f};
        int neighbors = 0;

        //compare current boid with every other boid
        for (int j = 0; j < count; j++){
            //continue if equal because it's not a neighbor of itself
            if (i==j){continue;}

            float distance = glm_vec3_distance(boids[i].position, boids[j].position);

            //counts number of neighbors the current boid has
            if (distance < DETECTION_RADIUS){
                neighbors++;

                //adds all neighboring birds' velocities
                glm_vec3_add(alignment, boids[j].velocity, alignment);

                //
                glm_vec3_add(cohesion, boids[j].position, cohesion);

                //avoids birds that are too close
                if (distance < SEPARATION_RADIUS && distance > 0.0f){
                    vec3 difference;
                    //calculates opposite direction from neighbor
                    glm_vec3_sub(boids[i].position, boids[j].position, difference);
                    //scales so repulsion is greater for closer birds
                    glm_vec3_scale(difference, 1.0f/(distance*distance), difference);
                    //adds each neighbor's repulsion to total separation vector
                    glm_vec3_add(separation, difference, separation);
                }
            }
        }

        if (neighbors > 0){
            //calculate avg neighbor velocity
            glm_vec3_scale(alignment, 1.0f/neighbors, alignment);
            //calculate velocity change needed to match neighbors
            glm_vec3_sub(alignment, boids[i].velocity, alignment);

            //calculates avg neighbor position
            glm_vec3_scale(cohesion, 1.0f/neighbors, cohesion);
            //finds direction that goes towards avg center of neighbors
            glm_vec3_sub(cohesion, boids[i].position, cohesion);
        }

        //apply weight to each flocking rule
        glm_vec3_scale(cohesion, COHESION_WEIGHT, cohesion);
        glm_vec3_scale(alignment, ALIGNMENT_WEIGHT, alignment);
        glm_vec3_scale(separation, SEPARATION_WEIGHT, separation);

        //combine the 3 steering vectors
        vec3 acceleration;
        glm_vec3_add(separation, alignment, acceleration);
        glm_vec3_add(acceleration, cohesion, acceleration);
        limitVector(acceleration, MAX_FORCE);

        //update velocity using acceleration
        glm_vec3_scale(acceleration, dt, acceleration);
        glm_vec3_add(boids[i].velocity, acceleration, next[i].velocity);
        limitVector(next[i].velocity, MAX_SPEED);

        //update position using new velocity
        vec3 movement;
        glm_vec3_scale(next[i].velocity, dt, movement);
        glm_vec3_add(boids[i].position, movement, next[i].position);

        //keep boids inside the simulation box
        for (int k = 0; k < 3; k++){
            if (next[i].position[k] < 0.0f){
                next[i].position[k] = 0.0f;
                next[i].velocity[k] *= -1;
            }
            else if (next[i].position[k] > BOX_SIZE){
                next[i].position[k] = BOX_SIZE;
                next[i].velocity[k] *= -1;
            }
        }
    }
}