/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNERouteProbReroute.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Jan 2017
/// @version $Id$
///
//
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <netedit/changes/GNEChange_Attribute.h>
#include <netedit/dialogs/GNERerouterIntervalDialog.h>
#include <netedit/GNEUndoList.h>
#include <netedit/GNEViewNet.h>
#include <netedit/GNENet.h>

#include "GNERouteProbReroute.h"

// ===========================================================================
// member method definitions
// ===========================================================================

GNERouteProbReroute::GNERouteProbReroute(GNERerouterIntervalDialog* rerouterIntervalDialog) :
    GNEAdditional(rerouterIntervalDialog->getEditedAdditional(), rerouterIntervalDialog->getEditedAdditional()->getViewNet(), GLO_REROUTER, SUMO_TAG_ROUTE_PROB_REROUTE, "", false,
    {}, {}, {}, {rerouterIntervalDialog->getEditedAdditional()}, {}, {}, {}, {}, {}, {}) {
    // if exist a reroute, set newRoute ID
    if (rerouterIntervalDialog->getEditedAdditional()->getViewNet()->getNet()->getAttributeCarriers().demandElements.at(SUMO_TAG_ROUTE).size() > 0) {
        myNewRouteId = rerouterIntervalDialog->getEditedAdditional()->getViewNet()->getNet()->getAttributeCarriers().demandElements.at(SUMO_TAG_ROUTE).begin()->first;
    }
    // fill route prob reroute interval with default values
    setDefaultValues();
}


GNERouteProbReroute::GNERouteProbReroute(GNEAdditional* rerouterIntervalParent, const std::string& newRouteId, double probability) :
    GNEAdditional(rerouterIntervalParent, rerouterIntervalParent->getViewNet(), GLO_REROUTER, SUMO_TAG_ROUTE_PROB_REROUTE, "", false,
    {}, {}, {}, {rerouterIntervalParent}, {}, {}, {}, {}, {}, {}),
    myNewRouteId(newRouteId),
    myProbability(probability) {
}


GNERouteProbReroute::~GNERouteProbReroute() {}


void
GNERouteProbReroute::moveGeometry(const Position&) {
    // This additional cannot be moved
}


void
GNERouteProbReroute::commitGeometryMoving(GNEUndoList*) {
    // This additional cannot be moved
}


void
GNERouteProbReroute::updateGeometry() {
    // Currently this additional doesn't own a Geometry
}


Position
GNERouteProbReroute::getPositionInView() const {
    return getAdditionalParents().at(0)->getPositionInView();
}


Boundary
GNERouteProbReroute::getCenteringBoundary() const {
    return getAdditionalParents().at(0)->getCenteringBoundary();
}


void 
GNERouteProbReroute::splitEdgeGeometry(const double /*splitPosition*/, const GNENetElement* /*originalElement*/, const GNENetElement* /*newElement*/, GNEUndoList* /*undoList*/) {
    // geometry of this element cannot be splitted
}


std::string
GNERouteProbReroute::getParentName() const {
    return getAdditionalParents().at(0)->getID();
}


void
GNERouteProbReroute::drawGL(const GUIVisualizationSettings&) const {
    // Currently This additional isn't drawn
}


std::string
GNERouteProbReroute::getAttribute(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ID:
            return getAdditionalID();
        case SUMO_ATTR_ROUTE:
            return myNewRouteId;
        case SUMO_ATTR_PROB:
            return toString(myProbability);
        case GNE_ATTR_PARENT:
            return getAdditionalParents().at(0)->getID();
        case GNE_ATTR_PARAMETERS:
            return getParametersStr();
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


double 
GNERouteProbReroute::getAttributeDouble(SumoXMLAttr key) const {
    throw InvalidArgument(getTagStr() + " doesn't have a double attribute of type '" + toString(key) + "'");
}


void
GNERouteProbReroute::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
        case SUMO_ATTR_ID:
        case SUMO_ATTR_ROUTE:
        case SUMO_ATTR_PROB:
        case GNE_ATTR_PARAMETERS:
            undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), key, value));
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool
GNERouteProbReroute::isValid(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            return isValidAdditionalID(value);
        case SUMO_ATTR_ROUTE:
            return SUMOXMLDefinitions::isValidVehicleID(value);
        case SUMO_ATTR_PROB:
            return canParse<double>(value);
        case GNE_ATTR_PARAMETERS:
            return Parameterised::areParametersValid(value);
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool 
GNERouteProbReroute::isAttributeEnabled(SumoXMLAttr /* key */) const {
    return true;
}


std::string
GNERouteProbReroute::getPopUpID() const {
    return getTagStr();
}


std::string
GNERouteProbReroute::getHierarchyName() const {
    return getTagStr() + ": " + myNewRouteId;
}

// ===========================================================================
// private
// ===========================================================================

void
GNERouteProbReroute::setAttribute(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            changeAdditionalID(value);
            break;
        case SUMO_ATTR_ROUTE:
            myNewRouteId = value;
            break;
        case SUMO_ATTR_PROB:
            myProbability = parse<double>(value);
            break;
        case GNE_ATTR_PARAMETERS:
            setParametersStr(value);
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}

/****************************************************************************/
