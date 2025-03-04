/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2003-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    NGEdge.cpp
/// @author  Markus Hartinger
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Mar, 2003
/// @version $Id$
///
// A netgen-representation of an edge
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <algorithm>
#include <utils/common/RandHelper.h>
#include <netbuild/NBNode.h>
#include <netbuild/NBNodeCont.h>
#include <netbuild/NBEdge.h>
#include <netbuild/NBOwnTLDef.h>
#include <netbuild/NBTypeCont.h>
#include <netbuild/NBTrafficLightLogicCont.h>
#include <netbuild/NBNetBuilder.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/ToString.h>
#include <utils/geom/GeoConvHelper.h>
#include <utils/options/OptionsCont.h>
#include <utils/options/Option.h>
#include "NGEdge.h"
#include "NGNode.h"


// ===========================================================================
// method definitions
// ===========================================================================
// ---------------------------------------------------------------------------
// NGEdge-definitions
// ---------------------------------------------------------------------------
NGEdge::NGEdge(const std::string& id, NGNode* startNode, NGNode* endNode)
    : Named(id), myStartNode(startNode), myEndNode(endNode) {
    myStartNode->addLink(this);
    myEndNode->addLink(this);
}


NGEdge::~NGEdge() {
    myStartNode->removeLink(this);
    myEndNode->removeLink(this);
}


NBEdge*
NGEdge::buildNBEdge(NBNetBuilder& nb, const std::string& type) const {
    int priority = nb.getTypeCont().getPriority(type);
    if (priority > 1 && OptionsCont::getOptions().getBool("rand.random-priority")) {
        priority = RandHelper::rand(priority) + 1;
    }
    int lanenumber = nb.getTypeCont().getNumLanes(type);
    if (lanenumber > 1 && OptionsCont::getOptions().getBool("rand.random-lanenumber")) {
        lanenumber = RandHelper::rand(lanenumber) + 1;
    }

    SVCPermissions permissions = nb.getTypeCont().getPermissions(type);
    LaneSpreadFunction lsf = LANESPREAD_RIGHT;
    if (isRailway(permissions) &&  nb.getTypeCont().getIsOneWay(type)) {
        lsf = LANESPREAD_CENTER;
    }
    NBEdge* result = new NBEdge(
            myID,
            nb.getNodeCont().retrieve(myStartNode->getID()), // from
            nb.getNodeCont().retrieve(myEndNode->getID()), // to
            type, nb.getTypeCont().getSpeed(type), lanenumber,
            priority, nb.getTypeCont().getWidth(type), NBEdge::UNSPECIFIED_OFFSET,
            "", lsf);
    result->setPermissions(permissions);
    return result;
}


/****************************************************************************/

