Simulate N particles of mass m = 1 that are confined to move within a cubic box of length L (with periodic
boundary conditions). The particles interact with each other through a Lennard-Jones (LJ) potential
where σ =1 and ϵ =1. Choose L = 10σ and ρ = N/V = 0.2σ^−3
. Integrate the equations of motion with the
Velocity Verlet algorithm. 
- N particles (see below)
- m=1
- L = 10σ = 10 = box length 
- ρ = N/V = 0.2σ^−3, N = V*ρ = (10σ)^3*0.2σ^−3 = 1000*0.2 = 200 particles
- Periodic BC (use P_Img)
- LJ potential (Use ex.9 potential)
- σ =1
- ϵ =1
- T*=1 (reduced parameter)
- Integrate equations of motion (with Velocit verlet from ex. 10), meaning update positions with this technique, and use LJ forces


Generate a suitable, well equilibrated initial condition. Choose one of the two strategies introduced
in one of the previous exercise sessions. Draw the initial velocity distribution from the equilibrium
(Maxwell-Boltzmann) distribution at T∗ = 1, enforcing the total momentum to zero
- Equilibriated system (run for steps = N_particles of times)
- Draw initial velocities from Maxwell Boltzmann distr.
- T*=1 (reduced parameter)
- tot momentum p=0 (equal -v as v)



Check the effect of the cut-off radius: perform different simulations, increasing the cut-off from rc =2^(1/6)*σ to rc = 4σ with steps ∆rc = 0.2σ (round the numbers to the first decimal place after the first one). After equilibration (if needed), compute the radial distribution function to compare the different cases.
- Check the effect of the cut-off radius
  - rc: from 2^(1/6)*σ to 4σ with step size ∆rc = 0.2σ
  - (round the numbers to the first decimal place after the first one)
- Equilibration?
- Compute radial distribution (read up on readial distribution)
- Plot results/print results in ipynb and compare (plot radial distr.(r_c))


Extra notes:
- Use F = -d/dr V_LJ between particles, calculate all forces on each particle to calculate new movement (remember to avoid double counting for total forces/energy)
- Use cut off potential (force = 0 beyond cut offfor now)
- Dont care about collisions? LJ pot. should make sure of this

Running:
Can be doen with ex 9 code:------------------------------------------------
1. Fix all IC 
2. Place all particles in box randomly (should be easy with enough density)
3. Equilibrate system, run for N steps (Skip LJ for this, just use as ex 9)
---------------------------------------------------------------------------
1. Draw velocities for particles (max. bolz. distr.)
2. Save position, velocity, forces
3. When working, run for differnet cut off radiuses

Analyzing:
- Compute radial distribution (read up on readial distribution)
- Plot results/print results in ipynb and compare (plot radial distr.(r_c))