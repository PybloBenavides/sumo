/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNERide.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Jun 2019
/// @version $Id$
///
// A class for visualizing rides in Netedit
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

#include "GNERide.h"


// ===========================================================================
// method definitions
// ===========================================================================

GNERide::GNERide(GNEViewNet* viewNet, GNEDemandElement* personParent, GNEEdge* fromEdge, GNEEdge* toEdge, double arrivalPosition, const std::vector<std::string>& lines) :
    GNEDemandElement(viewNet->getNet()->generateDemandElementID("", SUMO_TAG_RIDE_FROMTO), viewNet, GLO_RIDE, SUMO_TAG_RIDE_FROMTO, 
        {}, {}, {}, {}, {personParent}, {}, {}, {}, {}, {}),
    Parameterised(),
    myFromEdge(fromEdge),
    myToEdge(toEdge),
    myArrivalPosition(arrivalPosition),
    myLines(lines) {
    // compute ride without referencing edges
    computeWithoutReferences();
}


GNERide::GNERide(GNEViewNet* viewNet, GNEDemandElement* personParent, GNEEdge* fromEdge, GNEAdditional* busStop, const std::vector<std::string>& lines) :
    GNEDemandElement(viewNet->getNet()->generateDemandElementID("", SUMO_TAG_RIDE_BUSSTOP), viewNet, GLO_RIDE, SUMO_TAG_RIDE_BUSSTOP, 
        {}, {}, {}, {busStop}, {personParent}, {}, {}, {}, {}, {}),
    Parameterised(),
    myFromEdge(fromEdge),
    myToEdge(nullptr),
    myArrivalPosition(-1),
    myLines(lines) {
    // compute ride without referencing edges
    computeWithoutReferences();
}


GNERide::~GNERide() {}


GUIGLObjectPopupMenu*
GNERide::getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent) {
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
GNERide::writeDemandElement(OutputDevice& device) const {
    // open tag
    device.openTag(SUMO_TAG_RIDE);
    // write attributes depending  of ride type
    if (getDemandElementParents().front()->getDemandElementChildren().front() == this) {
        device.writeAttr(SUMO_ATTR_FROM, getEdgeParents().front()->getID());
    }
    // check if write busStop or edge to
    if (getAdditionalParents().size() > 0) {
        device.writeAttr(SUMO_ATTR_BUS_STOP, getAdditionalParents().front()->getID());
    } else {
        device.writeAttr(SUMO_ATTR_TO, getEdgeParents().back()->getID());
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
GNERide::isDemandElementValid() const {
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
GNERide::getDemandElementProblem() const {
    if (getEdgeParents().size() == 0) {
        return ("A ride need at least one edge");
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
GNERide::fixDemandElementProblem() {
    // currently the only solution is removing Ride
}


GNEEdge*
GNERide::getFromEdge() const {
    if (getDemandElementParents().size() == 2) {
        // obtain position and rotation of first edge route
        return getDemandElementParents().at(1)->getFromEdge();
    } else {
        return getEdgeParents().front();
    }
}


GNEEdge*
GNERide::getToEdge() const {
    if (getDemandElementParents().size() == 2) {
        // obtain position and rotation of first edge route
        return getDemandElementParents().at(1)->getToEdge();
    } else {
        return getEdgeParents().back();
    }
}


SUMOVehicleClass
GNERide::getVClass() const {
    return getDemandElementParents().front()->getVClass();
}


const RGBColor&
GNERide::getColor() const {
    return getDemandElementParents().front()->getColor();
}


void
GNERide::startGeometryMoving() {
    // only start geometry moving if arrival position isn't -1
    if (myArrivalPosition != -1) {
        // always save original position over view
        myRideMove.originalViewPosition = getPositionInView();
        // save arrival position
        myRideMove.firstOriginalLanePosition = getAttribute(SUMO_ATTR_ARRIVALPOS);
        // save current centering boundary
        myRideMove.movingGeometryBoundary = getCenteringBoundary();
    }
}


void
GNERide::endGeometryMoving() {
    // check that myArrivalPosition isn't -1 and endGeometryMoving was called only once
    if ((myArrivalPosition != -1) && myRideMove.movingGeometryBoundary.isInitialised()) {
        // reset myMovingGeometryBoundary
        myRideMove.movingGeometryBoundary.reset();
    }
}


void
GNERide::moveGeometry(const Position& offset) {
    // only move if myArrivalPosition isn't -1
    if (myArrivalPosition != -1) {
        // Calculate new position using old position
        Position newPosition = myRideMove.originalViewPosition;
        newPosition.add(offset);
        // filtern position using snap to active grid
        newPosition = myViewNet->snapToActiveGrid(newPosition);
        // obtain lane shape (to improve code legibility)
        const PositionVector& laneShape = getEdgeParents().back()->getLanes().front()->getLaneShape();
        // calculate offset lane
        double offsetLane = laneShape.nearest_offset_to_point2D(newPosition, false) - laneShape.nearest_offset_to_point2D(myRideMove.originalViewPosition, false);
        std::cout << offsetLane << std::endl;
        // Update arrival Position
        myArrivalPosition = parse<double>(myRideMove.firstOriginalLanePosition) + offsetLane;
        // Update geometry
        updateGeometry();
    }
}


void
GNERide::commitGeometryMoving(GNEUndoList* undoList) {
    // only commit geometry moving if myArrivalPosition isn't -1
    if (myArrivalPosition != -1) {
        undoList->p_begin("arrivalPos of " + getTagStr());
        undoList->p_add(new GNEChange_Attribute(this, myViewNet->getNet(), SUMO_ATTR_ARRIVALPOS, toString(myArrivalPosition), true, myRideMove.firstOriginalLanePosition));
        undoList->p_end();
    }
}


void
GNERide::updateGeometry() {
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
    GNEGeometry::calculateEdgeGeometricPath(this, myDemandElementSegmentGeometry, getEdgeParents(), 
        getVClass(), getFirstAllowedVehicleLane(), getLastAllowedVehicleLane(), departPosLane, arrivalPosLane, startPos, endPos);
    // update demand element childrens
    for (const auto& i : getDemandElementChildren()) {
        i->updateGeometry();
    }
}


void 
GNERide::updatePartialGeometry(const GNEEdge* edge) {
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
GNERide::getPositionInView() const {
    return Position();
}


std::string
GNERide::getParentName() const {
    return myViewNet->getNet()->getMicrosimID();
}


Boundary
GNERide::getCenteringBoundary() const {
    Boundary rideBoundary;
    // return the combination of all edge parents's boundaries
    for (const auto& i : getEdgeParents()) {
        rideBoundary.add(i->getCenteringBoundary());
    }
    // check if is valid
    if (rideBoundary.isInitialised()) {
        return rideBoundary;
    } else {
        return Boundary(-0.1, -0.1, 0.1, 0.1);
    }
}


void 
GNERide::splitEdgeGeometry(const double /*splitPosition*/, const GNENetElement* /*originalElement*/, const GNENetElement* /*newElement*/, GNEUndoList* /*undoList*/) {
    // geometry of this element cannot be splitted
}


void
GNERide::drawGL(const GUIVisualizationSettings& /*s*/) const {
    // Rides are drawn in GNEEdges
}


void
GNERide::selectAttributeCarrier(bool changeFlag) {
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
GNERide::unselectAttributeCarrier(bool changeFlag) {
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
GNERide::getAttribute(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ID:
            return getDemandElementID();
        case SUMO_ATTR_FROM:
            return getEdgeParents().front()->getID();
        case SUMO_ATTR_TO:
            return getEdgeParents().back()->getID();
        case SUMO_ATTR_VIA:
            return toString(myVia);
        case SUMO_ATTR_BUS_STOP:
            return getAdditionalParents().front()->getID();
        case SUMO_ATTR_LINES:
            return joinToString(myLines, " ");
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
GNERide::getAttributeDouble(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ARRIVALPOS:
            return myArrivalPosition;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNERide::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
        case SUMO_ATTR_FROM:
        case SUMO_ATTR_TO:
        case SUMO_ATTR_VIA:
        case SUMO_ATTR_BUS_STOP:
        case SUMO_ATTR_LINES:
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
GNERide::isValid(SumoXMLAttr key, const std::string& value) {
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
        case SUMO_ATTR_BUS_STOP:
            return (myViewNet->getNet()->retrieveAdditional(SUMO_TAG_BUS_STOP, value, false) != nullptr);
        case SUMO_ATTR_LINES:
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
GNERide::enableAttribute(SumoXMLAttr /*key*/, GNEUndoList* /*undoList*/) {
    //
}


void
GNERide::disableAttribute(SumoXMLAttr /*key*/, GNEUndoList* /*undoList*/) {
    //
}


bool
GNERide::isAttributeEnabled(SumoXMLAttr /*key*/) const {
    return true;
}


std::string
GNERide::getPopUpID() const {
    return getTagStr();
}


std::string
GNERide::getHierarchyName() const {
    if (myTagProperty.getTag() == SUMO_TAG_RIDE_FROMTO) {
        return "ride: " + getEdgeParents().front()->getID() + " -> " + getEdgeParents().back()->getID();
    } else {
        return "ride: " + getEdgeParents().front()->getID() + " -> " + getAdditionalParents().front()->getID();
    }
}

// ===========================================================================
// private
// ===========================================================================

void
GNERide::setAttribute(SumoXMLAttr key, const std::string& value) {
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
        case SUMO_ATTR_BUS_STOP:
            changeAdditionalParent(this, value, 0);
            updateGeometry();
            break;
        case SUMO_ATTR_LINES:
            myLines = GNEAttributeCarrier::parse<std::vector<std::string> >(value);
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
GNERide::setEnabledAttribute(const int /*enabledAttributes*/) {
    //
}


void 
GNERide::computeWithoutReferences() {
    if ((myTagProperty.getTag() == SUMO_TAG_RIDE_FROMTO) && myFromEdge && myToEdge) {
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
    } else if ((myTagProperty.getTag() == SUMO_TAG_RIDE_BUSSTOP) && myFromEdge && (getAdditionalParents().size() > 0)) {
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
