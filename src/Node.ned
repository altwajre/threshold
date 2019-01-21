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

package threshold;

module Node
{
    parameters:
        int address;
    gates:
        inout port[];
    submodules:
        broadcaster: Broadcaster {
            gates:
                in[2];
                out[sizeof(port)];
        }
        generator: Generator {
            parameters:
                address = address;
        }

        processor: Processor {
            gates:
                in[sizeof(port)];
        }

    connections:
        processor.out --> broadcaster.in[0];
        generator.out --> broadcaster.in[1];

        for i=0..sizeof(port)-1 {
            processor.in[i] <-- port$i[i];
            broadcaster.out[i] --> port$o[i];
        }
}
