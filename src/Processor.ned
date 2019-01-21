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

simple Processor
{
    parameters:
        double thresholdInterval @unit(s) = default(uniform(4s,6s));
        
        @signal[neighbourThresholded](type="long");
        @statistic[neighbourThresholded](title="neighbour id that was thresholded"; record=count,vector;interpolationmode=none);

        @signal[pktIn](type="long");
        @statistic[pktIn](title="packet received"; record=stats,vector; interpolationmode=none);

        @signal[pktDropSeen](type="long");
        @statistic[pktDropSeen](title="packet discarded as seen already"; record=stats,vector; interpolationmode=none);

        @signal[pktDropThresh](type="long");
        @statistic[pktDropThresh](title="packet discarded due to threshold"; record=stats,vector; interpolationmode=none);

    gates:
        input in[];
        output out;
}
