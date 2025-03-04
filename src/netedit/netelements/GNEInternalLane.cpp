/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEInternalLane.cpp
/// @author  Jakob Erdmann
/// @date    June 2011
/// @version $Id$
///
// A class for visualizing Inner Lanes (used when editing traffic lights)
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <utils/geom/GeomHelper.h>
#include <utils/gui/div/GUIParameterTableWindow.h>
#include <utils/gui/globjects/GUIGLObjectPopupMenu.h>
#include <utils/gui/div/GLHelper.h>
#include <netedit/frames/GNETLSEditorFrame.h>
#include <netedit/GNEViewNet.h>
#include <utils/gui/globjects/GLIncludes.h>

#include "GNEInternalLane.h"

// ===========================================================================
// FOX callback mapping
// ===========================================================================
/// @note: msvc10 does not approve of allocating empty arrays
/*
FXDEFMAP(GNEInternalLane) GNEInternalLaneMap[]= {
    //FXMAPFUNC(SEL_COMMAND,  MID_GNE_TLSFRAME_PHASE_DURATION,     GNETLSEditorFrame::onDefault),
};
*/

// Object implementation
//FXIMPLEMENT(GNEInternalLane, FXDelegator, GNEInternalLaneMap, ARRAYNUMBER(GNEInternalLaneMap))
FXIMPLEMENT(GNEInternalLane, FXDelegator, 0, 0)

// ===========================================================================
// static member definitions
// ===========================================================================

StringBijection<FXuint>::Entry GNEInternalLane::linkStateNamesValues[] = {
    { "Green-Major", LINKSTATE_TL_GREEN_MAJOR },
    { "Green-Minor", LINKSTATE_TL_GREEN_MINOR },
    { "Yellow-Major", LINKSTATE_TL_YELLOW_MAJOR },
    { "Yellow-Minor", LINKSTATE_TL_YELLOW_MINOR },
    { "Red", LINKSTATE_TL_RED },
    { "Red-Yellow", LINKSTATE_TL_REDYELLOW },
    { "Stop", LINKSTATE_STOP },
    { "Off", LINKSTATE_TL_OFF_NOSIGNAL },
    { "Off-Blinking", LINKSTATE_TL_OFF_BLINKING },
};

const StringBijection<FXuint> GNEInternalLane::LinkStateNames(
    GNEInternalLane::linkStateNamesValues, LINKSTATE_TL_OFF_BLINKING);

// ===========================================================================
// method definitions
// ===========================================================================
GNEInternalLane::GNEInternalLane(GNETLSEditorFrame* editor, const std::string& id, const PositionVector& shape, int tlIndex, LinkState state) :
    GUIGlObject(editor == nullptr ? GLO_JUNCTION : GLO_TLLOGIC, id),
    myState(state),
    myStateTarget(myState),
    myEditor(editor),
    myTlIndex(tlIndex),
    myPopup(nullptr) {
    // calculate internal lane geometry
    myInternalLaneGeometry.updateGeometryShape(shape);
}


GNEInternalLane::GNEInternalLane() :
    GUIGlObject(GLO_TLLOGIC, "dummyInternalLane") {
    assert(false);
}


GNEInternalLane::~GNEInternalLane() {}


long
GNEInternalLane::onDefault(FXObject* obj, FXSelector sel, void* data) {
    if (myEditor != nullptr) {
        FXuint before = myState;
        myStateTarget.handle(obj, sel, data);
        if (myState != before) {
            myEditor->handleChange(this);
        }
        // let GUISUMOAbstractView know about clicks so that the popup is properly destroyed
        if (FXSELTYPE(sel) == SEL_COMMAND) {
            if (myPopup != nullptr) {
                myPopup->getParentView()->destroyPopup();
                myPopup = nullptr;
            }
        }
    }
    return 1;
}


void
GNEInternalLane::drawGL(const GUIVisualizationSettings& s) const {
    glPushMatrix();
    glPushName(getGlID());
    glTranslated(0, 0, GLO_JUNCTION + 0.1); // must draw on top of junction
    GLHelper::setColor(colorForLinksState(myState));
    // draw lane
    // check whether it is not too small
    if (s.scale < 1.) {
        GLHelper::drawLine(myInternalLaneGeometry.getShape());
    } else {
        GNEGeometry::drawGeometry(myEditor->getViewNet(), myInternalLaneGeometry, 0.2);
    }
    glPopName();
    glPopMatrix();
}


void
GNEInternalLane::setLinkState(LinkState state) {
    myState = state;
    myOrigState = state;
}


LinkState
GNEInternalLane::getLinkState() const {
    return (LinkState)myState;
}


int
GNEInternalLane::getTLIndex() const {
    return myTlIndex;
}


GUIGLObjectPopupMenu*
GNEInternalLane::getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent) {
    myPopup = new GUIGLObjectPopupMenu(app, parent, *this);
    buildPopupHeader(myPopup, app);
    if ((myEditor != nullptr) && (myEditor->getViewNet()->getEditModes().currentSupermode == GNE_SUPERMODE_NETWORK)) {
        const std::vector<std::string> names = LinkStateNames.getStrings();
        for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); it++) {
            FXuint state = LinkStateNames.get(*it);
            std::string origHint = ((LinkState)state == myOrigState ? " (original)" : "");
            FXMenuRadio* mc = new FXMenuRadio(myPopup, (*it + origHint).c_str(), this, FXDataTarget::ID_OPTION + state);
            mc->setSelBackColor(MFXUtils::getFXColor(colorForLinksState(state)));
            mc->setBackColor(MFXUtils::getFXColor(colorForLinksState(state)));
        }
    }
    return myPopup;
}


GUIParameterTableWindow*
GNEInternalLane::getParameterWindow(GUIMainWindow& app, GUISUMOAbstractView&) {
    // internal lanes don't have attributes
    GUIParameterTableWindow* ret = new GUIParameterTableWindow(app, *this, 2);
    // close building
    ret->closeBuilding();
    return ret;
}


Boundary
GNEInternalLane::getCenteringBoundary() const {
    Boundary b = myInternalLaneGeometry.getShape().getBoxBoundary();
    b.grow(10);
    return b;
}


RGBColor
GNEInternalLane::colorForLinksState(FXuint state) {
    if (state == LINKSTATE_TL_YELLOW_MINOR) {
        // special case (default gui does not distinguish between yellow major/minor
        return RGBColor(179, 179, 0, 255);
    } else {
        try {
            return GUIVisualizationSettings::getLinkColor((LinkState)state);
        } catch (ProcessError&) {
            WRITE_WARNING("invalid link state='" + toString(state) + "'");
            return RGBColor::BLACK;
        }
    }
}

/****************************************************************************/
