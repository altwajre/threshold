//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package threshold.simulations;

import threshold.Node;

network RandomGraph
{
    parameters:
        int n @prompt("Number of nodes") = default(50);
    submodules:
        node[n]: Node {
            parameters:
                address = index;
        }
    connections allowunconnected:
        for i=0..n-2, for j=i+1..n-1 {
            node[i].port++ <--> node[j].port++ if uniform(0,1)<0.33;
        }
}


