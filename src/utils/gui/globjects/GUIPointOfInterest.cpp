/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GUIPointOfInterest.cpp
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @date    June 2006
/// @version $Id$
///
// The GUI-version of a point of interest
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <utils/gui/div/GUIParameterTableWindow.h>
#include <utils/gui/globjects/GUIGLObjectPopupMenu.h>
#include <utils/gui/div/GUIGlobalSelection.h>
#include <utils/gui/windows/GUIMainWindow.h>
#include <utils/gui/images/GUIIconSubSys.h>
#include <utils/gui/images/GUITexturesHelper.h>
#include <utils/gui/windows/GUIAppEnum.h>
#include <utils/gui/settings/GUIVisualizationSettings.h>
#include <utils/gui/div/GLHelper.h>
#include <utils/gui/globjects/GLIncludes.h>
#include "GUIPointOfInterest.h"


// ===========================================================================
// static members
// ===========================================================================

std::vector<Position> GUIPointOfInterest::myPOIVertices;


// ===========================================================================
// method definitions
// ===========================================================================

GUIPointOfInterest::GUIPointOfInterest(const std::string& id, const std::string& type,
                                       const RGBColor& color, const Position& pos, bool geo,
                                       const std::string& lane, double posOverLane, double posLat,
                                       double layer, double angle, const std::string& imgFile,
                                       bool relativePath, double width, double height) :
    PointOfInterest(id, type, color, pos, geo, lane, posOverLane, posLat, layer, angle, imgFile, relativePath, width, height),
    GUIGlObject_AbstractAdd(GLO_POI, id) {
}


GUIPointOfInterest::~GUIPointOfInterest() {}


GUIGLObjectPopupMenu*
GUIPointOfInterest::getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent) {
    GUIGLObjectPopupMenu* ret = new GUIGLObjectPopupMenu(app, parent, *this);
    // build shape header
    buildShapePopupOptions(app, ret, getShapeType());
    return ret;
}


GUIParameterTableWindow*
GUIPointOfInterest::getParameterWindow(GUIMainWindow& app, GUISUMOAbstractView&) {
    GUIParameterTableWindow* ret = new GUIParameterTableWindow(app, *this, 3 + (int)getParametersMap().size());
    // add items
    ret->mkItem("type", false, getShapeType());
    ret->mkItem("layer", false, getShapeLayer());
    ret->closeBuilding(this);
    return ret;
}


Boundary
GUIPointOfInterest::getCenteringBoundary() const {
    Boundary b;
    b.add(x(), y());
    if (getShapeImgFile() != DEFAULT_IMG_FILE) {
        b.growWidth(myHalfImgWidth);
        b.growHeight(myHalfImgHeight);
    } else {
        b.grow(3);
    }
    return b;
}


void
GUIPointOfInterest::drawGL(const GUIVisualizationSettings& s) const {
    // first clear vertices
    myPOIVertices.clear();
    // check if POI can be drawn
    if (checkDraw(s)) {
        // push name (needed for getGUIGlObjectsUnderCursor(...)
        glPushName(getGlID());
        // draw inner polygon
        drawInnerPOI(s, false);
        // pop name
        glPopName();
    }
}


void
GUIPointOfInterest::setColor(const GUIVisualizationSettings& s, bool disableSelectionColor) const {
    const GUIColorer& c = s.poiColorer;
    const int active = c.getActive();
    if (s.netedit && active != 1 && gSelected.isSelected(GLO_POI, getGlID()) && disableSelectionColor) {
        // override with special colors (unless the color scheme is based on selection)
        GLHelper::setColor(RGBColor(0, 0, 204));
    } else if (active == 0) {
        GLHelper::setColor(getShapeColor());
    } else if (active == 1) {
        GLHelper::setColor(c.getScheme().getColor(gSelected.isSelected(GLO_POI, getGlID())));
    } else {
        GLHelper::setColor(c.getScheme().getColor(0));
    }
}


bool
GUIPointOfInterest::checkDraw(const GUIVisualizationSettings& s) const {
    // only continue if scale is valid
    if (s.scale * (1.3 / 3.0) *s.poiSize.getExaggeration(s, this) < s.poiSize.minSize) {
        return false;
    }
    return true;
}


void
GUIPointOfInterest::drawInnerPOI(const GUIVisualizationSettings& s, bool disableSelectionColor) const {
    const double exaggeration = s.poiSize.getExaggeration(s, this);
    glPushMatrix();
    setColor(s, disableSelectionColor);
    glTranslated(x(), y(), getShapeLayer());
    glRotated(-getShapeNaviDegree(), 0, 0, 1);
    // check if has to be drawn as a circle or with an image
    if (getShapeImgFile() != DEFAULT_IMG_FILE) {
        int textureID = GUITexturesHelper::getTextureID(getShapeImgFile());
        if (textureID > 0) {
            GUITexturesHelper::drawTexturedBox(textureID,
                                               -myHalfImgWidth * exaggeration, -myHalfImgHeight * exaggeration,
                                               myHalfImgWidth * exaggeration,  myHalfImgHeight * exaggeration);
        }
    } else {
        // fallback if no image is defined
        if (s.drawForRectangleSelection) {
            GLHelper::drawFilledCircle((double) 1.3 * exaggeration, 8);
        } else {
            // draw filled circle saving vertices
            myPOIVertices = GLHelper::drawFilledCircleReturnVertices((double) 1.3 * exaggeration, 16);
        }
    }
    glPopMatrix();
    if (!s.drawForRectangleSelection) {
        const Position namePos = *this;
        drawName(namePos, s.scale, s.poiName, s.angle);
        if (s.poiType.show) {
            const Position p = namePos + Position(0, -0.6 * s.poiType.size / s.scale);
            GLHelper::drawTextSettings(s.poiType, getShapeType(), p, s.scale, s.angle);
        }
    }
}

/****************************************************************************/

