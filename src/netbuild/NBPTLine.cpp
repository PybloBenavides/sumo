/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    NBPTLine.cpp
/// @author  Gregor Laemmel
/// @author  Nikita Cherednychek
/// @date    Tue, 20 Mar 2017
/// @version $Id$
///
// The representation of one direction of a single pt line
/****************************************************************************/
#include <utils/iodevices/OutputDevice.h>

#include <utility>
#include <utils/common/ToString.h>
#include <utils/common/StringUtils.h>
#include <utils/common/MsgHandler.h>
#include "NBEdgeCont.h"
#include "NBPTLine.h"
#include "NBPTStop.h"

NBPTLine::NBPTLine(const std::string& id, const std::string& name, const std::string& type, const std::string& ref, int interval, const std::string& nightService,
        SUMOVehicleClass vClass) :
    myName(name),
    myType(type),
    myPTLineId(id),
    myRef(ref != "" ? ref : name),
    myInterval(interval),
    myNightService(nightService),
    myVClass(vClass)
{ }

void NBPTLine::addPTStop(NBPTStop* pStop) {
    myPTStops.push_back(pStop);

}

std::vector<NBPTStop*> NBPTLine::getStops() {
    return myPTStops;
}

void NBPTLine::write(OutputDevice& device, NBEdgeCont& ec) {
    device.openTag(SUMO_TAG_PT_LINE);
    device.writeAttr(SUMO_ATTR_ID, myPTLineId);
    if (!myName.empty()) {
        device.writeAttr(SUMO_ATTR_NAME, StringUtils::escapeXML(myName));
    }

    device.writeAttr(SUMO_ATTR_LINE, StringUtils::escapeXML(myRef));
    device.writeAttr(SUMO_ATTR_TYPE, myType);
    device.writeAttr(SUMO_ATTR_VCLASS, toString(myVClass));
    if (myInterval > 0) {
        // write seconds
        device.writeAttr(SUMO_ATTR_PERIOD, 60 * myInterval);
    }
    if (myNightService != "") {
        device.writeAttr("nightService", myNightService);
    }
    device.writeAttr("completeness", toString((double)myPTStops.size() / (double)myNumOfStops));

    std::vector<std::string> validEdgeIDs;
    // filter out edges that have been removed due to joining junctions
    // (the rest of the route is valid)
    for (NBEdge* e : myRoute) {
        if (ec.retrieve(e->getID())) {
            validEdgeIDs.push_back(e->getID());
        }
    }
    if (!myRoute.empty()) {
        device.openTag(SUMO_TAG_ROUTE);
        device.writeAttr(SUMO_ATTR_EDGES, validEdgeIDs);
        device.closeTag();
    }

    for (auto& myPTStop : myPTStops) {
        device.openTag(SUMO_TAG_BUS_STOP);
        device.writeAttr(SUMO_ATTR_ID, myPTStop->getID());
        device.writeAttr(SUMO_ATTR_NAME, StringUtils::escapeXML(myPTStop->getName()));
        device.closeTag();
    }
    device.closeTag();

}

void NBPTLine::addWayNode(long long int way, long long int node) {
    std::string wayStr = toString(way);
    if (wayStr != myCurrentWay) {
        myCurrentWay = wayStr;
        myWays.push_back(wayStr);
    }
    myWaysNodes[wayStr].push_back(node);

}
const std::vector<std::string>& NBPTLine::getMyWays() const {
    return myWays;
}
std::vector<long long int>* NBPTLine::getWaysNodes(std::string wayId) {
    if (myWaysNodes.find(wayId) != myWaysNodes.end()) {
        return &myWaysNodes[wayId];
    }
    return nullptr;
}

void 
NBPTLine::setEdges(const std::vector<NBEdge*>& edges) {
    myRoute = edges;
    // ensure permissions
    for (NBEdge* e: edges) {
        SVCPermissions permissions = e->getPermissions(); 
        if ((permissions & myVClass) != myVClass) {
            SVCPermissions nVuln = ~(SVC_PEDESTRIAN | SVC_BICYCLE);
            if (permissions != 0 && (permissions & nVuln) == 0) {
                // this is a footpath or sidewalk. Add another lane
                e->addRestrictedLane(SUMO_const_laneWidth, myVClass);
            } else {
                // add permissions to the rightmost lane that is not exclusively used for pedestrians / bicycles
                for (int i = 0; i < (int)e->getNumLanes(); i++) {
                    if ((e->getPermissions(i) & nVuln) != 0) {
                        e->allowVehicleClass(i, myVClass);
                        break;
                    }
                }
            }
        }
    }
}

void NBPTLine::setMyNumOfStops(int numStops) {
    myNumOfStops = numStops;
}
const std::vector<NBEdge*>& NBPTLine::getRoute() const {
    return myRoute;
}

std::vector<NBEdge*> 
NBPTLine::getStopEdges(const NBEdgeCont& ec) const {
    std::vector<NBEdge*> result;
    for (NBPTStop* stop : myPTStops) {
        NBEdge* e = ec.retrieve(stop->getEdgeId());
        if (e != nullptr) {
            result.push_back(e);
        }
    }
    return result;
}

NBEdge* 
NBPTLine::getRouteStart(const NBEdgeCont& ec) const {
    std::vector<NBEdge*> validEdges;
    // filter out edges that have been removed due to joining junctions
    for (NBEdge* e : myRoute) {
        if (ec.retrieve(e->getID())) {
            validEdges.push_back(e);
        }
    }
    if (validEdges.size() == 0) {
        return nullptr;
    }
    // filter out edges after the first stop
    if (myPTStops.size() > 0) {
        NBEdge* firstStopEdge = ec.retrieve(myPTStops.front()->getEdgeId());
        if (firstStopEdge == nullptr) {
            WRITE_WARNING("Could not retrieve edge '" + myPTStops.front()->getEdgeId() + "' for first stop of line '" + myPTLineId + "'");
            return nullptr;

        }
        auto it = std::find(validEdges.begin(), validEdges.end(), firstStopEdge);
        if (it == validEdges.end()) {
            WRITE_WARNING("First stop edge '" + firstStopEdge->getID() + "' is not part of the route of line '" + myPTLineId + "'");
            return nullptr;
        }
    }
    return validEdges.front();
}

NBEdge* 
NBPTLine::getRouteEnd(const NBEdgeCont& ec) const {
    std::vector<NBEdge*> validEdges;
    // filter out edges that have been removed due to joining junctions
    for (NBEdge* e : myRoute) {
        if (ec.retrieve(e->getID())) {
            validEdges.push_back(e);
        }
    }
    if (validEdges.size() == 0) {
        return nullptr;
    }
    // filter out edges after the last stop
    if (myPTStops.size() > 0) {
        NBEdge* lastStopEdge = ec.retrieve(myPTStops.back()->getEdgeId());
        if (lastStopEdge == nullptr) {
            WRITE_WARNING("Could not retrieve edge '" + myPTStops.back()->getEdgeId() + "' for last stop of line '" + myPTLineId + "'");
            return nullptr;

        }
        auto it = std::find(validEdges.begin(), validEdges.end(), lastStopEdge);
        if (it == validEdges.end()) {
            WRITE_WARNING("Last stop edge '" + lastStopEdge->getID() + "' is not part of the route of line '" + myPTLineId + "'");
            return nullptr;
        }
    }
    return validEdges.back();
}

void
NBPTLine::replaceStop(NBPTStop* oldStop, NBPTStop* newStop) {
    for (int i = 0; i < (int)myPTStops.size(); i++) {
        if (myPTStops[i] == oldStop) {
            myPTStops[i] = newStop;
        }
    }
}
