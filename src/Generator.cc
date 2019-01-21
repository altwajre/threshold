//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <omnetpp.h>
#include "Packet_m.h"

using namespace omnetpp;

class Generator: public cSimpleModule {
private:
    double chanceGeneratePkt;
    double chancePktIsSpam;

    // state
    cPar* generationInterval;
    cMessage *generatePacket;
    int myAddress;
    long pktCounter;

public:
    Generator();
    virtual ~Generator();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Generator);

Generator::Generator() {
    generatePacket = nullptr;

    pktCounter = 0;
}

Generator::~Generator() {
    cancelAndDelete(generatePacket);

}

void Generator::initialize() {
    myAddress = par("address");
    generationInterval = &par("generationInterval");
    chanceGeneratePkt = par("chanceGeneratePkt");
    chancePktIsSpam = par("chancePktIsSpam");

    generatePacket = new cMessage("generatePacket");
    scheduleAt(generationInterval->doubleValue(), generatePacket);
}

void Generator::handleMessage(cMessage *msg) {
    if (msg == generatePacket) {

        if (uniform(0, 1) >= chanceGeneratePkt) {
            return;
        }

        char pktID[42];
        sprintf(pktID, "pk-%d-#%ld", myAddress, pktCounter++);

        Packet *pk = new Packet(pktID);
        pk->setSrcAddr(myAddress);

        // Broadcast signal to neighbours.
        send(pk, "out");

        scheduleAt(simTime() + generationInterval->doubleValue(),
                generatePacket);
        if (hasGUI()) {
            getParentModule()->bubble("Generating packet...");
        }
    }

}
