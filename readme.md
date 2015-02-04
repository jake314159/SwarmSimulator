SwarmSimulator
==================

Example of running with simplest possible settings
```
# Run with the values 0.35 and 0.45
./bin/SwarmSimulator 0.35 0.45

# Run with random values (different for each agent)
./bin/SwarmSimulator X X
```

Command line options
---------------
```
--NoDisplay
    Don't show a display

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
        - FOOD
            Food is spread out throughout the environment and can be consumed by
            agents
            
--RunTime <FRAMES>
    Specifies the number of frames to run the simulation for

--RoundN <NUMBER_OF_ROUNDS>
    Specified the run time in terms of the number of evolution rounds to use
    Takes precedence over the '--RunTime' argument
```

