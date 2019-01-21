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

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Broadcaster : public cSimpleModule {
 protected:
  virtual void initialize();
  virtual void handleMessage(cMessage *msg);
};

Define_Module(Broadcaster);

void Broadcaster::initialize() {}

void Broadcaster::handleMessage(cMessage *msg) {
  size_t outCount = gateSize("out");
  size_t outBase = gateBaseId("out");
  for (size_t i = 0; i < outCount; i++) {
    send(i == (outCount - 1) ? msg : msg->dup(), outBase + i);
  }
}
