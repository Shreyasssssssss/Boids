For CS4900 Project
Create Boids in 3d Space to then optimize

Naive Boids Algorithm:
Simulates Boid flocking behavior using:
1. Cohesion: Boids move toward average position of neighbors
2. Alignment: Boids match average velocity of neighbors (moving in same general direction as well)
3. Separation: Boids repulse each other when they are too close (to avoid overcrowding)
There is a certain detection radius that determines which boids are considered "neighbors" aka the boids that influence behavior. The 3 vectors from the 3 aforementioned parts of the algorithm are combined by weightage to find acceleration. Then, the boid's position and velocity are updated.
