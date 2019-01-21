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
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Packet_m.h"

using namespace omnetpp;

class NeighbourStat {
 public:
  int address;
  uint64_t count{0};
};

class Processor : public cSimpleModule {
 protected:
  void doThreshold();

 private:
  std::unordered_set<std::string> packetsSeen;
  std::unordered_map<int, NeighbourStat> neighbourStats;
  std::unordered_set<int> neighboursThresholded;

  simsignal_t sigPktDropSeen;
  simsignal_t sigPktIn;
  simsignal_t sigPktDropThresh;
  simsignal_t sigNeighbourThresholded;

  cMessage *thresholdMsg;
  double thresholdInterval;

 public:
  Processor();
  virtual ~Processor();

 protected:
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Processor);

Processor::Processor() {}
Processor::~Processor() { cancelAndDelete(thresholdMsg); }

void Processor::initialize() {
  sigPktDropSeen = registerSignal("pktDropSeen");
  sigPktIn = registerSignal("pktIn");
  sigPktDropThresh = registerSignal("pktDropThresh");
  sigNeighbourThresholded = registerSignal("neighbourThresholded");

  thresholdInterval = par("thresholdInterval");
  thresholdMsg = new cMessage("thresholdMsg");

  // Initialise neighbours
  int inBase = gateBaseId("in");
  int inCount = gateSize("in");
  for (int i = inBase; i < (inBase + inCount); i++) {
    cGate *g = gate(i);
    neighbourStats[i].address =
        static_cast<cSimpleModule *>(
            g->getPathStartGate()->getOwner()->getOwner())
            ->par("address")
            .intValue();
  }

  scheduleAt(thresholdInterval, thresholdMsg);
}

void Processor::doThreshold() {
  EV << "Performing thresholding: ";

  if (neighbourStats.size() <= 1) {
    EV << "Aborting as we don't have any neighbours. " << endl;
    return;
  }

  neighboursThresholded.clear();

  // Calculate sum of all messages received during last window.
  uint64_t sum = 0;
  for (auto const &el : neighbourStats) {
    sum += el.second.count;
  }

  for (auto &el : neighbourStats) {
    double ratio = ((double)el.second.count) / sum;
    if (ratio > 0.67) {
      neighboursThresholded.insert(el.first);
      emit(sigNeighbourThresholded, el.second.address);
    }
    el.second.count = 0;
  }

  for (auto const &el : neighboursThresholded) {
    EV << neighbourStats[el].address << ", ";
  }

  if (neighboursThresholded.size() == 0) {
    EV << " none";
  }
  EV << endl;
}

void Processor::handleMessage(cMessage *msg) {
  if (msg == thresholdMsg) {
    doThreshold();

    scheduleAt(simTime() + thresholdInterval, thresholdMsg);
    return;
  }

  Packet *pk = check_and_cast<Packet *>(msg);
  emit(sigPktIn, 1);

  // Attempt to insert packet id
  if (!packetsSeen.insert(pk->getName()).second) {
    // Processor has seen packet already. Ignore.
    emit(sigPktDropSeen, 1);
    delete pk;
    return;
  } else {
    // Count arrival independent of whether node is being thresholded
    // But only count *new* packets
    neighbourStats[pk->getArrivalGateId()].count++;
  }

  // Check if neighbour is being thresholded;
  if (neighboursThresholded.find(msg->getArrivalGateId()) !=
      neighboursThresholded.end()) {
    emit(sigPktDropThresh, 1);
    delete pk;
    return;
  }

  EV << "Broadcasting paket " << pk->getName() << endl;

  // Broadcast signal to neighbours via Broadcaster.
  send(msg, "out");
}
