# CSCI-561

## HW1

### Description

This is a time travelling problem. There are many possible worlds along with the timeline indexed by the years. Each world is a grid of space with (x, y) locations in which your agent can use some elementary actions to move to one of the eight neighboring grid locations.

Within a possible grid world, there are some special bi-directional time-travelling channels located at certain locations that will allow your agent to travel in time forward or backward in the history.

At a special location where there is a time-travel channel, your agent may use an action called “jaunt“ to time-travel to a different year in the history at the other end of the channel.

Given three inputs: (1) a time-travelling configuration, such as the one in Figure 1; (2) an initial year-location, and (3) a goal or target year-location, your program must search in the configuration and find the optimal shortest path from the initial year-location to the target year-location, using the elementary move actions and jaunt.

