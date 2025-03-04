/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEPersonTrip.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Jun 2019
/// @version $Id$
///
// A class for visualizing person trips in Netedit
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <utils/gui/windows/GUIAppEnum.h>
#include <netedit/additionals/GNEAdditional.h>
#include <netedit/changes/GNEChange_Attribute.h>
#include <netedit/GNENet.h>
#include <netedit/GNEUndoList.h>
#include <netedit/GNEViewNet.h>
#include <netedit/GNEViewParent.h>
#include <netedit/netelements/GNELane.h>
#include <netedit/netelements/GNEEdge.h>
#include <netedit/frames/GNESelectorFrame.h>
#include <utils/common/StringTokenizer.h>
#include <utils/gui/div/GUIGlobalSelection.h>
#include <utils/vehicle/SUMOVehicleParameter.h>

#include "GNEPersonTrip.h"


// ===========================================================================
// method definitions
// ===========================================================================

GNEPersonTrip::GNEPersonTrip(GNEViewNet* viewNet, GNEDemandElement* personParent, GNEEdge* fromEdge, GNEEdge* toEdge, double arrivalPosition, 
    const std::vector<std::string>& types, const std::vector<std::string>& modes) :
    GNEDemandElement(viewNet->getNet()->generateDemandElementID("", SUMO_TAG_PERSONTRIP_FROMTO), viewNet, GLO_PERSONTRIP, SUMO_TAG_PERSONTRIP_FROMTO, 
        {}, {}, {}, {}, {personParent}, {}, {}, {}, {}, {}),
    Parameterised(),
    myFromEdge(fromEdge),
    myToEdge(toEdge),
    myArrivalPosition(arrivalPosition),
    myVTypes(types),
    myModes(modes) {
    // compute person trip without referencing edges
    computeWithoutReferences();
}


GNEPersonTrip::GNEPersonTrip(GNEViewNet* viewNet, GNEDemandElement* personParent, GNEEdge* fromEdge, GNEAdditional* busStop, 
    const std::vector<std::string>& types, const std::vector<std::string>& modes) :
    GNEDemandElement(viewNet->getNet()->generateDemandElementID("", SUMO_TAG_PERSONTRIP_BUSSTOP), viewNet, GLO_PERSONTRIP, SUMO_TAG_PERSONTRIP_BUSSTOP, 
        {}, {}, {}, {busStop}, {personParent}, {}, {}, {}, {}, {}),
    Parameterised(),
    myFromEdge(fromEdge),
    myToEdge(nullptr),
    myArrivalPosition(-1),
    myVTypes(types),
    myModes(modes) {
    // compute person trip without referencing edges
    computeWithoutReferences();
}


GNEPersonTrip::~GNEPersonTrip() {}


GUIGLObjectPopupMenu*
GNEPersonTrip::getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent) {
    GUIGLObjectPopupMenu* ret = new GUIGLObjectPopupMenu(app, parent, *this);
    // build header
    buildPopupHeader(ret, app);
    // build menu command for center button and copy cursor position to clipboard
    buildCenterPopupEntry(ret);
    buildPositionCopyEntry(ret, false);
    // buld menu commands for names
    new FXMenuCommand(ret, ("Copy " + getTagStr() + " name to clipboard").c_str(), nullptr, ret, MID_COPY_NAME);
    new FXMenuCommand(ret, ("Copy " + getTagStr() + " typed name to clipboard").c_str(), nullptr, ret, MID_COPY_TYPED_NAME);
    new FXMenuSeparator(ret);
    // build selection and show parameters menu
    myViewNet->buildSelectionACPopupEntry(ret, this);
    buildShowParamsPopupEntry(ret);
    // show option to open demand element dialog
    if (myTagProperty.hasDialog()) {
        new FXMenuCommand(ret, ("Open " + getTagStr() + " Dialog").c_str(), getIcon(), &parent, MID_OPEN_ADDITIONAL_DIALOG);
        new FXMenuSeparator(ret);
    }
    new FXMenuCommand(ret, ("Cursor position in view: " + toString(getPositionInView().x()) + "," + toString(getPositionInView().y())).c_str(), nullptr, nullptr, 0);
    return ret;
}


void
GNEPersonTrip::writeDemandElement(OutputDevice& device) const {
    // open tag
    device.openTag(SUMO_TAG_PERSONTRIP);
    // only write From attribute if this is the first Person Plan
    if (getDemandElementParents().front()->getDemandElementChildren().front() == this) {
        device.writeAttr(SUMO_ATTR_FROM, getEdgeParents().front()->getID());
    }
    // check if write busStop or edge to
    if (getAdditionalParents().size() > 0) {
        device.writeAttr(SUMO_ATTR_BUS_STOP, getAdditionalParents().front()->getID());
    } else {
        device.writeAttr(SUMO_ATTR_TO, getEdgeParents().back()->getID());
    }
    // write modes
    if (myModes.size() > 0) {
        device.writeAttr(SUMO_ATTR_MODES, myModes);
    }
    // write vTypes
    if (myVTypes.size() > 0) {
        device.writeAttr(SUMO_ATTR_VTYPES, myVTypes);
    }
    // only write arrivalPos if is different of -1
    if (myArrivalPosition != -1) {
        device.writeAttr(SUMO_ATTR_ARRIVALPOS, myArrivalPosition);
    }
    // write parameters
    writeParams(device);
    // close tag
    device.closeTag();
}


bool
GNEPersonTrip::isDemandElementValid() const {
    if (getEdgeParents().size() == 0) {
        return false;
    } else if (getEdgeParents().size() == 1) {
        return true;
    } else {
        // check if exist at least a connection between every edge
        for (int i = 1; i < (int)getEdgeParents().size(); i++) {
            if (getRouteCalculatorInstance()->areEdgesConsecutives(getDemandElementParents().front()->getVClass(), getEdgeParents().at((int)i - 1), getEdgeParents().at(i)) == false) {
                return false;
            }
        }
        // there is connections bewteen all edges, then return true
        return true;
    }
}


std::string
GNEPersonTrip::getDemandElementProblem() const {
    if (getEdgeParents().size() == 0) {
        return ("A person trip need at least one edge");
    } else {
        // check if exist at least a connection between every edge
        for (int i = 1; i < (int)getEdgeParents().size(); i++) {
            if (getRouteCalculatorInstance()->areEdgesConsecutives(getDemandElementParents().front()->getVClass(), getEdgeParents().at((int)i - 1), getEdgeParents().at(i)) == false) {
                return ("Edge '" + getEdgeParents().at((int)i - 1)->getID() + "' and edge '" + getEdgeParents().at(i)->getID() + "' aren't consecutives");
            }
        }
        // there is connections bewteen all edges, then all ok
        return "";
    }
}


void
GNEPersonTrip::fixDemandElementProblem() {
    // currently the only solution is removing PersonTrip
}


GNEEdge*
GNEPersonTrip::getFromEdge() const {
    if (getDemandElementParents().size() == 2) {
        // obtain position and rotation of first edge route
        return getDemandElementParents().at(1)->getFromEdge();
    } else {
        return getEdgeParents().front();
    }
}


GNEEdge*
GNEPersonTrip::getToEdge() const {
    if (getDemandElementParents().size() == 2) {
        // obtain position and rotation of first edge route
        return getDemandElementParents().at(1)->getToEdge();
    } else {
        return getEdgeParents().back();
    }
}


SUMOVehicleClass
GNEPersonTrip::getVClass() const {
    return getDemandElementParents().front()->getVClass();
}


const RGBColor&
GNEPersonTrip::getColor() const {
    return getDemandElementParents().front()->getColor();
}


void
GNEPersonTrip::startGeometryMoving() {
    // only start geometry moving if arrival position isn't -1
    if (myArrivalPosition != -1) {
        // always save original position over view
        myPersonTripMove.originalViewPosition = getPositionInView();
        // save arrival position
        myPersonTripMove.firstOriginalLanePosition = getAttribute(SUMO_ATTR_ARRIVALPOS);
        // save current centering boundary
        myPersonTripMove.movingGeometryBoundary = getCenteringBoundary();
    }
}


void
GNEPersonTrip::endGeometryMoving() {
    // check that myArrivalPosition isn't -1 and endGeometryMoving was called only once
    if ((myArrivalPosition != -1) && myPersonTripMove.movingGeometryBoundary.isInitialised()) {
        // reset myMovingGeometryBoundary
        myPersonTripMove.movingGeometryBoundary.reset();
    }
}


void
GNEPersonTrip::moveGeometry(const Position& offset) {
    // only move if myArrivalPosition isn't -1
    if (myArrivalPosition != -1) {
        // Calculate new position using old position
        Position newPosition = myPersonTripMove.originalViewPosition;
        newPosition.add(offset);
        // filtern position using snap to active grid
        newPosition = myViewNet->snapToActiveGrid(newPosition);
        // obtain lane shape (to improve code legibility)
        const PositionVector& laneShape = getEdgeParents().back()->getLanes().front()->getLaneShape();
        // calculate offset lane
        double offsetLane = laneShape.nearest_offset_to_point2D(newPosition, false) - laneShape.nearest_offset_to_point2D(myPersonTripMove.originalViewPosition, false);
        std::cout << offsetLane << std::endl;
        // Update arrival Position
        myArrivalPosition = parse<double>(myPersonTripMove.firstOriginalLanePosition) + offsetLane;
        // Update geometry
        updateGeometry();
    }
}


void
GNEPersonTrip::commitGeometryMoving(GNEUndoList* undoList) {
    // only commit geometry moving if myArrivalPosition isn't -1
    if (myArrivalPosition != -1) {
        undoList->p_begin("arrivalPos of " + getTagStr());
        undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), SUMO_ATTR_ARRIVALPOS, toString(myArrivalPosition), true, myPersonTripMove.firstOriginalLanePosition));
        undoList->p_end();
    }
}


void
GNEPersonTrip::updateGeometry() {
    // declare depart and arrival pos lane
    double departPosLane = -1;
    double arrivalPosLane = -1;
    // declare start and end positions
    Position startPos = Position::INVALID;
    Position endPos = Position::INVALID;
    // calculate person plan start and end lanepositions
    calculatePersonPlanLaneStartEndPos(departPosLane, arrivalPosLane);
    // calculate person plan start and end positions
    calculatePersonPlanPositionStartEndPos(startPos, endPos);
    // calculate geometry path
    GNEGeometry::calculateEdgeGeometricPath(this, myDemandElementSegmentGeometry, getEdgeParents(), getVClass(), 
        getFirstAllowedVehicleLane(), getLastAllowedVehicleLane(), departPosLane, arrivalPosLane, startPos, endPos);
    // update demand element childrens
    for (const auto& i : getDemandElementChildren()) {
        i->updateGeometry();
    }
}


void 
GNEPersonTrip::updatePartialGeometry(const GNEEdge* edge) {
    // declare depart and arrival pos lane
    double departPosLane = -1;
    double arrivalPosLane = -1;
    // declare start and end positions
    Position startPos = Position::INVALID;
    Position endPos = Position::INVALID;
    // calculate person plan start and end lanepositions
    calculatePersonPlanLaneStartEndPos(departPosLane, arrivalPosLane);
    // calculate person plan start and end positions
    calculatePersonPlanPositionStartEndPos(startPos, endPos);
    // calculate geometry path
    GNEGeometry::updateGeometricPath(myDemandElementSegmentGeometry, edge, departPosLane, arrivalPosLane, startPos, endPos);
    // update demand element childrens
    for (const auto& i : getDemandElementChildren()) {
        i->updatePartialGeometry(edge);
    }
}


Position
GNEPersonTrip::getPositionInView() const {
    return Position();
}


std::string
GNEPersonTrip::getParentName() const {
    return myViewNet->getNet()->getMicrosimID();
}


Boundary
GNEPersonTrip::getCenteringBoundary() const {
    Boundary personTripBoundary;
    // return the combination of all edge parents's boundaries
    for (const auto& i : getEdgeParents()) {
        personTripBoundary.add(i->getCenteringBoundary());
    }
    // check if is valid
    if (personTripBoundary.isInitialised()) {
        return personTripBoundary;
    } else {
        return Boundary(-0.1, -0.1, 0.1, 0.1);
    }
}


void 
GNEPersonTrip::splitEdgeGeometry(const double /*splitPosition*/, const GNENetElement* /*originalElement*/, const GNENetElement* /*newElement*/, GNEUndoList* /*undoList*/) {
    // geometry of this element cannot be splitted
}


void
GNEPersonTrip::drawGL(const GUIVisualizationSettings& /*s*/) const {
    // PersonTrips are drawn in GNEEdges
}


void
GNEPersonTrip::selectAttributeCarrier(bool changeFlag) {
    if (!myViewNet) {
        throw ProcessError("ViewNet cannot be nullptr");
    } else {
        gSelected.select(getGlID());
        // add object of list into selected objects
        myViewNet->getViewParent()->getSelectorFrame()->getLockGLObjectTypes()->addedLockedObject(getType());
        if (changeFlag) {
            mySelected = true;
        }
    }
}


void
GNEPersonTrip::unselectAttributeCarrier(bool changeFlag) {
    if (!myViewNet) {
        throw ProcessError("ViewNet cannot be nullptr");
    } else {
        gSelected.deselect(getGlID());
        // remove object of list of selected objects
        myViewNet->getViewParent()->getSelectorFrame()->getLockGLObjectTypes()->removeLockedObject(getType());
        if (changeFlag) {
            mySelected = false;

        }
    }
}


std::string
GNEPersonTrip::getAttribute(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ID:
            return getDemandElementID();
        case SUMO_ATTR_FROM:
            return getEdgeParents().front()->getID();
        case SUMO_ATTR_TO:
            return getEdgeParents().back()->getID();
        case SUMO_ATTR_VIA:
            return toString(myVia);
        case SUMO_ATTR_EDGES:
            return parseIDs(getEdgeParents());
        case SUMO_ATTR_BUS_STOP:
            return getAdditionalParents().front()->getID();
        case SUMO_ATTR_MODES:
            return joinToString(myModes, " ");
        case SUMO_ATTR_VTYPES:
            return joinToString(myVTypes, " ");
        case SUMO_ATTR_ARRIVALPOS:
            return toString(myArrivalPosition);
        case GNE_ATTR_SELECTED:
            return toString(isAttributeCarrierSelected());
        case GNE_ATTR_PARAMETERS:
            return getParametersStr();
        case GNE_ATTR_PARENT:
            return getDemandElementParents().front()->getID();
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


double
GNEPersonTrip::getAttributeDouble(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ARRIVALPOS:
            return myArrivalPosition;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNEPersonTrip::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
        case SUMO_ATTR_FROM:
        case SUMO_ATTR_TO:
        case SUMO_ATTR_VIA:
        case SUMO_ATTR_EDGES:
        case SUMO_ATTR_BUS_STOP:
        case SUMO_ATTR_MODES:
        case SUMO_ATTR_VTYPES:
        case SUMO_ATTR_ARRIVALPOS:
        case GNE_ATTR_SELECTED:
        case GNE_ATTR_PARAMETERS:
            undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), key, value));
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool
GNEPersonTrip::isValid(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_FROM:
        case SUMO_ATTR_TO:
            return SUMOXMLDefinitions::isValidNetID(value) && (myViewNet->getNet()->retrieveEdge(value, false) != nullptr);
        case SUMO_ATTR_VIA:
            if (value.empty()) {
                return true;
            } else {
                return canParse<std::vector<GNEEdge*> >(myViewNet->getNet(), value, false);
            }
        case SUMO_ATTR_EDGES:
            if (canParse<std::vector<GNEEdge*> >(myViewNet->getNet(), value, false)) {
                // all edges exist, then check if compounds a valid route
                return GNEDemandElement::isRouteValid(parse<std::vector<GNEEdge*> >(myViewNet->getNet(), value), false);
            } else {
                return false;
            }
        case SUMO_ATTR_BUS_STOP:
            return (myViewNet->getNet()->retrieveAdditional(SUMO_TAG_BUS_STOP, value, false) != nullptr);
        case SUMO_ATTR_MODES: {
            SVCPermissions dummyModeSet;
            std::string dummyError;
            return SUMOVehicleParameter::parsePersonModes(value, myTagProperty.getTagStr(), getID(), dummyModeSet, dummyError);
        }
        case SUMO_ATTR_VTYPES:
            return canParse<std::vector<std::string> >(value);
        case SUMO_ATTR_ARRIVALPOS:
            if (canParse<double>(value)) {
                double parsedValue = canParse<double>(value);
                // a arrival pos with value -1 means that it will be ignored
                if (parsedValue == -1) {
                    return true;
                } else {
                    return parsedValue >= 0;
                }
            } else {
                return false;
            }
        case GNE_ATTR_SELECTED:
            return canParse<bool>(value);
        case GNE_ATTR_PARAMETERS:
            return Parameterised::areParametersValid(value);
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNEPersonTrip::enableAttribute(SumoXMLAttr /*key*/, GNEUndoList* /*undoList*/) {
    //
}


void
GNEPersonTrip::disableAttribute(SumoXMLAttr /*key*/, GNEUndoList* /*undoList*/) {
    //
}


bool
GNEPersonTrip::isAttributeEnabled(SumoXMLAttr /*key*/) const {
    return true;
}


std::string
GNEPersonTrip::getPopUpID() const {
    return getTagStr();
}


std::string
GNEPersonTrip::getHierarchyName() const {
    if (myTagProperty.getTag() == SUMO_TAG_PERSONTRIP_FROMTO) {
        return "personTrip: " + getEdgeParents().front()->getID() + " -> " + getEdgeParents().back()->getID();
    } else {
        return "personTrip: " + getEdgeParents().front()->getID() + " -> " + getAdditionalParents().front()->getID();
    }
}

// ===========================================================================
// private
// ===========================================================================

void
GNEPersonTrip::setAttribute(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        // Specific of Trips and flow
        case SUMO_ATTR_FROM: {
            // update myFrom edge
            myFromEdge = myViewNet->getNet()->retrieveEdge(value);
            // compute path
            updateGeometry();
            break;
        }
        case SUMO_ATTR_TO: {
            // update myToEdge edge
            myToEdge = myViewNet->getNet()->retrieveEdge(value);
            // compute path
            updateGeometry();
            break;
        }
        case SUMO_ATTR_VIA: {
            if (!value.empty()) {
                // set new via edges
                myVia = parse< std::vector<std::string> >(value);
            } else {
                // clear via
                myVia.clear();
            }
            // compute path
            updateGeometry();
            break;
        }
        case SUMO_ATTR_EDGES:
            changeEdgeParents(this, value, true);
            updateGeometry();
            break;
        case SUMO_ATTR_BUS_STOP:
            changeAdditionalParent(this, value, 0);
            updateGeometry();
            break;
        case SUMO_ATTR_MODES:
            myModes = GNEAttributeCarrier::parse<std::vector<std::string> >(value);
            break;
        case SUMO_ATTR_VTYPES:
            myVTypes = GNEAttributeCarrier::parse<std::vector<std::string> >(value);
            break;
        case SUMO_ATTR_ARRIVALPOS:
            myArrivalPosition = parse<double>(value);
            updateGeometry();
            break;
        case GNE_ATTR_SELECTED:
            if (parse<bool>(value)) {
                selectAttributeCarrier();
            } else {
                unselectAttributeCarrier();
            }
            break;
        case GNE_ATTR_PARAMETERS:
            setParametersStr(value);
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNEPersonTrip::setEnabledAttribute(const int /*enabledAttributes*/) {
    //
}


void 
GNEPersonTrip::computeWithoutReferences() {
    if ((myTagProperty.getTag() == SUMO_TAG_PERSONTRIP_FROMTO) && myFromEdge && myToEdge) {
        // declare a from-via-to edges vector
        std::vector<std::string> FromViaToEdges;
        // add from edge
        FromViaToEdges.push_back(myFromEdge->getID());
        // add via edges
        FromViaToEdges.insert(FromViaToEdges.end(), myVia.begin(), myVia.end());
        // add to edge
        FromViaToEdges.push_back(myToEdge->getID());
        // calculate route
        std::vector<GNEEdge*> route = getRouteCalculatorInstance()->calculateDijkstraRoute(myViewNet->getNet(), getDemandElementParents().at(0)->getVClass(), FromViaToEdges);
        // check if rute is valid
        if (route.size() > 0) {
            changeEdgeParents(this, route, false);
        } else if (getEdgeParents().size() > 0) {
            changeEdgeParents(this, getEdgeParents().front()->getID() + " " + toString(myVia) + " " + getEdgeParents().back()->getID(), false);
        }
    } else if ((myTagProperty.getTag() == SUMO_TAG_PERSONTRIP_BUSSTOP) && myFromEdge && (getAdditionalParents().size() > 0)) {
        // declare a from-via-busStop edges vector
        std::vector<std::string> FromViaBusStopEdges;
        // add from edge
        FromViaBusStopEdges.push_back(myFromEdge->getID());
        // add via edges
        FromViaBusStopEdges.insert(FromViaBusStopEdges.end(), myVia.begin(), myVia.end());
        // add busStop edge
        FromViaBusStopEdges.push_back(getAdditionalParents().front()->getLaneParents().front()->getParentEdge().getID());
        // calculate route
        std::vector<GNEEdge*> route = getRouteCalculatorInstance()->calculateDijkstraRoute(myViewNet->getNet(), getDemandElementParents().at(0)->getVClass(), FromViaBusStopEdges);
        // check if rute is valid
        if (route.size() > 0) {
            changeEdgeParents(this, route, false);
        } else if (getEdgeParents().size() > 0) {
            changeEdgeParents(this, getEdgeParents().front()->getID() + " " + toString(myVia) + " " + getEdgeParents().back()->getID(), false);
        }
    }
    // update geometry
    updateGeometry();
}

/****************************************************************************/
