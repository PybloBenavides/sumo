/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNERouteProbe.h
/// @author  Pablo Alvarez Lopez
/// @date    May 2016
/// @version $Id$
///
//
/****************************************************************************/
#ifndef GNERouteProbe_h
#define GNERouteProbe_h


// ===========================================================================
// included modules
// ===========================================================================

#include "GNEAdditional.h"


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GNERouteProbe
 * @brief Representation of a RouteProbe in netedit
 */
class GNERouteProbe : public GNEAdditional {

public:
    /**@brief Constructor
     * @param[in] id The storage of gl-ids to get the one for this lane representation from
     * @param[in] viewNet pointer to GNEViewNet of this additional element belongs
     * @param[in] edge edge in which this routeProbe is placed
     * @param[in] frequency The frequency in which to report the distribution
     * @oaran[in] name Route Probe Name
     * @param[in] filename The file for generated output
     * @param[in] begin The time at which to start generating output
     */
    GNERouteProbe(const std::string& id, GNEViewNet* viewNet, GNEEdge* edge, const std::string& frequency, const std::string& name, const std::string& filename, SUMOTime begin);

    /// @brief Destructor
    ~GNERouteProbe();

    /// @name Functions related with geometry of element
    /// @{
    /**@brief change the position of the element geometry without saving in undoList
     * @param[in] offset Position used for calculate new position of geometry without updating RTree
     */
    void moveGeometry(const Position& offset);

    /**@brief commit geometry changes in the attributes of an element after use of moveGeometry(...)
     * @param[in] undoList The undoList on which to register changes
     */
    void commitGeometryMoving(GNEUndoList* undoList);

    /// @brief update pre-computed geometry information
    void updateGeometry();

    /// @brief Returns position of additional in view
    Position getPositionInView() const;

    /// @brief Returns the boundary to which the view shall be centered in order to show the object
    Boundary getCenteringBoundary() const;

    /// @brief split geometry
    void splitEdgeGeometry(const double splitPosition, const GNENetElement* originalElement, const GNENetElement* newElement, GNEUndoList* undoList);
    /// @}

    /// @name inherited from GUIGlObject
    /// @{
    /// @brief Returns the name of the parent object
    /// @return This object's parent id
    std::string getParentName() const;

    /**@brief Draws the object
     * @param[in] s The settings for the current view (may influence drawing)
     * @see GUIGlObject::drawGL
     */
    void drawGL(const GUIVisualizationSettings& s) const;
    /// @}

    /// @name inherited from GNEAttributeCarrier
    /// @{
    /* @brief method for getting the Attribute of an XML key
     * @param[in] key The attribute key
     * @return string with the value associated to key
     */
    std::string getAttribute(SumoXMLAttr key) const;

    /* @brief method for getting the Attribute of an XML key in double format (to avoid unnecessary parse<double>(...) for certain attributes)
     * @param[in] key The attribute key
     * @return double with the value associated to key
     */
    double getAttributeDouble(SumoXMLAttr key) const;

    /* @brief method for setting the attribute and letting the object perform additional changes
     * @param[in] key The attribute key
     * @param[in] value The new value
     * @param[in] undoList The undoList on which to register changes
     */
    void setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList);

    /* @brief method for checking if the key and their correspond attribute are valids
     * @param[in] key The attribute key
     * @param[in] value The value asociated to key key
     * @return true if the value is valid, false in other case
     */
    bool isValid(SumoXMLAttr key, const std::string& value);

    /* @brief method for check if the value for certain attribute is set
     * @param[in] key The attribute key
     */
    bool isAttributeEnabled(SumoXMLAttr key) const;

    /// @brief get PopPup ID (Used in AC Hierarchy)
    std::string getPopUpID() const;

    /// @brief get Hierarchy Name (Used in AC Hierarchy)
    std::string getHierarchyName() const;
    /// @}

protected:
    /// @brief Frequency of RouteProbe
    std::string myFrequency;

    /// @brief filename of RouteProbe
    std::string myFilename;

    /// @brief begin of rerouter
    SUMOTime myBegin;

    /// @brief route probe logo offset
    Position myRouteProbeLogoOffset;

    /// @brief relative position regarding to other route probes
    int myRelativePositionY;

private:
    /// @brief set attribute after validation
    void setAttribute(SumoXMLAttr key, const std::string& value);

    /// @brief Invalidated copy constructor.
    GNERouteProbe(const GNERouteProbe&) = delete;

    /// @brief Invalidated assignment operator.
    GNERouteProbe& operator=(const GNERouteProbe&) = delete;
};

#endif

/****************************************************************************/
