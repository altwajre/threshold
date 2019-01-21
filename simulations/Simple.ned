package threshold.simulations;

import threshold.Node;

network Simple
{
    submodules:
        rte[5]: Node {
            address = index;
        }
    connections:
        rte[1].port++ <--> rte[0].port++;
        rte[0].port++ <--> rte[2].port++;
        rte[4].port++ <--> rte[3].port++;
        rte[3].port++ <--> rte[1].port++;
        rte[3].port++ <--> rte[2].port++;
        rte[2].port++ <--> rte[4].port++;
        rte[1].port++ <--> rte[2].port++;
}
