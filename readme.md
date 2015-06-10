SwarmSimulator
==================

Example of running with simplest possible settings
```
# Run with the values 0.35 and 0.45
./bin/SwarmSimulator 0.35 0.45 --NoEvo

# Run with random values (different for each agent)
# But still don't evolve beyond that as there is no environment set
./bin/SwarmSimulator X X --NoEvo
```

Command line options
---------------
```
--NoDisplay
    Don't show a display

--NoEvo
    Stops the individuals from attempting to evolve (ie. their parameter values are fixed)
    This MUST be applied if no environment has been set otherwise it will cause instability
    in the simulated behaviors

-R --EnableRecord
    Enable recording (Note: The directory to save the images in must also be specified)
    Requires the display to be enabled

-D <dir>
    Specifies the output directory for the recording images
    Requires the display to be enabled

-E <ENV_NAME>
    Specifies the environment to run the simulation in
    Default is none
    Available environments:
        - SPREAD_MIN
            Minimises the distance of each agent from the center of mass
        - SPREAD_MAX
            Maximises the distance of each agent from the center of mass
        - DISP_MIN
            Minimises the displacement of the center of mass of the flock
            by penalising the agents that cause it
        - DISP_MAX
            Maximises the displacement of the center of mass of the flock
            by penalising the agents that cause it

        - FOOD
            Food is spread out throughout the environment and can be consumed by
            agents
        - SCAN
            Scans across and penalises agents on the outside of the swarm
        - INTER
            Agents are encouraged to share as much of their line of sight with
            other agents as possible
        - VORTEX
            a.k.a. Hydrodynamic efficiency, each individual attempts to minimise
            the energy they use to move about
        - CONF
            Individuals attempt to confuse predators by grouping together
        - COPE
            Simulation of feeding on copepod plankton

        - M_DESC
            Measures the 'describe' parameters (spread & speed)
            
--RunTime <FRAMES>
    Specifies the number of frames to run the simulation for

--RoundN <NUMBER_OF_ROUNDS>
    Specified the run time in terms of the number of evolution rounds to use
    Takes precedence over the '--RunTime' argument

--jsonDir <DIR>
    Specifies the directory where the json output should be saved to (the output
    of the GA)
```

