/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEDemandElement.h
/// @author  Pablo Alvarez Lopez
/// @date    Dec 2018
/// @version $Id$
///
// A abstract class for demand elements
/****************************************************************************/
#ifndef GNEDemandElement_h
#define GNEDemandElement_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <netedit/GNEHierarchicalElementParents.h>
#include <netedit/GNEHierarchicalElementChildren.h>
#include <netedit/GNEGeometry.h>
#include <utils/common/Parameterised.h>
#include <utils/geom/PositionVector.h>
#include <utils/gui/globjects/GUIGlObject.h>
#include <utils/router/SUMOAbstractRouter.h>
#include <netbuild/NBVehicle.h>
#include <netbuild/NBEdge.h>

// ===========================================================================
// class declarations
// ===========================================================================

class GNEViewNet;
class GNEAdditional;
class GNEDemandElement;
class GNENetElement;
class GNEEdge;
class GNELane;
class GNEJunction;

// ===========================================================================
// class definitions
// ===========================================================================

/**
 * @class GNEDemandElement
 * @brief An Element which don't belongs to GNENet but has influency in the simulation
 */
class GNEDemandElement : public GUIGlObject, public GNEAttributeCarrier, public GNEHierarchicalElementParents, public GNEHierarchicalElementChildren {

public:
    /// @brief struct for pack all variables related with Demand Element moving
    struct DemandElementMove {
        /// @brief boundary used during moving of elements (to avoid insertion in RTREE)
        Boundary movingGeometryBoundary;

        /// @brief value for saving first original position over lane before moving
        Position originalViewPosition;

        /// @brief value for saving first original position over lane before moving
        std::string firstOriginalLanePosition;

        /// @brief value for saving second original position over lane before moving
        std::string secondOriginalPosition;
    };

    /// @brief class used to calculate routes in nets
    class RouteCalculator {

    public:
        /// @brief constructor
        RouteCalculator(GNENet* net);

        /// @brief destructor
        ~RouteCalculator();

        /// @brief update DijkstraRoute (called when SuperMode Demand is selected)
        void updateDijkstraRouter();

        /// @brief calculate Dijkstra route between a list of partial edges
        std::vector<GNEEdge*> calculateDijkstraRoute(SUMOVehicleClass vClass, const std::vector<GNEEdge*>& partialEdges) const;

        /// @brief calculate Dijkstra route between a list of partial edges (in string format)
        std::vector<GNEEdge*> calculateDijkstraRoute(GNENet* net, SUMOVehicleClass vClass, const std::vector<std::string>& partialEdgesStr) const;

        /// @brief check if exist a route between the two given consecutives edges
        bool areEdgesConsecutives(SUMOVehicleClass vClass, GNEEdge* from, GNEEdge* to) const;

    private:
        /// @brief pointer to net
        GNENet* myNet;

        /// @brief SUMO Abstract DijkstraRouter
        SUMOAbstractRouter<NBRouterEdge, NBVehicle>* myDijkstraRouter;
    };

    /**@brief Constructor
     * @param[in] id Gl-id of the demand element element (Must be unique)
     * @param[in] viewNet pointer to GNEViewNet of this demand element element belongs
     * @param[in] type GUIGlObjectType of demand element
     * @param[in] tag Type of xml tag that define the demand element element (SUMO_TAG_ROUTE, SUMO_TAG_VEHICLE, etc...)
     * @param[in] edgeParents vector of edge parents
     * @param[in] laneParents vector of lane parents
     * @param[in] shapeParents vector of shape parents
     * @param[in] additionalParents vector of additional parents
     * @param[in] demandElementChildren vector of demandElement parents
     * @param[in] edgeChildren vector of edge children
     * @param[in] laneChildren vector of lane children
     * @param[in] shapeChildren vector of shape children
     * @param[in] additionalChildren vector of additional children
     * @param[in] demandElementChildren vector of demandElement children
     */
    GNEDemandElement(const std::string& id, GNEViewNet* viewNet, GUIGlObjectType type, SumoXMLTag tag,
                     const std::vector<GNEEdge*>& edgeParents,
                     const std::vector<GNELane*>& laneParents,
                     const std::vector<GNEShape*>& shapeParents,
                     const std::vector<GNEAdditional*>& additionalParents,
                     const std::vector<GNEDemandElement*>& demandElementParents,
                     const std::vector<GNEEdge*>& edgeChildren,
                     const std::vector<GNELane*>& laneChildren,
                     const std::vector<GNEShape*>& shapeChildren,
                     const std::vector<GNEAdditional*>& additionalChildren,
                     const std::vector<GNEDemandElement*>& demandElementChildren);

    /**@brief Constructor
     * @param[in] demandElementParent pointer to demand element parent pointer (used to generate an ID)
     * @param[in] viewNet pointer to GNEViewNet of this demand element element belongs
     * @param[in] type GUIGlObjectType of demand element
     * @param[in] tag Type of xml tag that define the demand element element (SUMO_TAG_ROUTE, SUMO_TAG_VEHICLE, etc...)
     * @param[in] edgeParents vector of edge parents
     * @param[in] laneParents vector of lane parents
     * @param[in] shapeParents vector of shape parents
     * @param[in] additionalParents vector of additional parents
     * @param[in] demandElementChildren vector of demandElement parents
     * @param[in] edgeChildren vector of edge children
     * @param[in] laneChildren vector of lane children
     * @param[in] shapeChildren vector of shape children
     * @param[in] additionalChildren vector of additional children
     * @param[in] demandElementChildren vector of demandElement children
     */
    GNEDemandElement(GNEDemandElement* demandElementParent, GNEViewNet* viewNet, GUIGlObjectType type, SumoXMLTag tag,
                     const std::vector<GNEEdge*>& edgeParents,
                     const std::vector<GNELane*>& laneParents,
                     const std::vector<GNEShape*>& shapeParents,
                     const std::vector<GNEAdditional*>& additionalParents,
                     const std::vector<GNEDemandElement*>& demandElementParents,
                     const std::vector<GNEEdge*>& edgeChildren,
                     const std::vector<GNELane*>& laneChildren,
                     const std::vector<GNEShape*>& shapeChildren,
                     const std::vector<GNEAdditional*>& additionalChildren,
                     const std::vector<GNEDemandElement*>& demandElementChildren);

    /// @brief Destructor
    ~GNEDemandElement();

    /// @brief get demand element geometry
    const GNEGeometry::Geometry& getDemandElementGeometry() const;

    /// @brief get demand element segment geometry
    const GNEGeometry::SegmentGeometry& getDemandElementSegmentGeometry() const;

    /// @brief gererate a new ID for an element child
    std::string generateChildID(SumoXMLTag childTag);

    /// @name members and functions relative to elements common to all demand elements
    /// @{
    /// @brief obtain from edge of this demand element
    virtual GNEEdge* getFromEdge() const = 0;

    /// @brief obtain to edge of this demand element
    virtual GNEEdge* getToEdge() const = 0;

    /// @brief obtain VClass related with this demand element
    virtual SUMOVehicleClass getVClass() const = 0;

    /// @brief get color
    virtual const RGBColor& getColor() const = 0;
    /// @}

    /// @name members and functions relative to write demand elements into XML
    /// @{
    /**@brief writte demand element element into a xml file
     * @param[in] device device in which write parameters of demand element element
     */
    virtual void writeDemandElement(OutputDevice& device) const = 0;

    /// @brief check if current demand element is valid to be writed into XML (by default true, can be reimplemented in children)
    virtual bool isDemandElementValid() const;

    /// @brief return a string with the current demand element problem (by default empty, can be reimplemented in children)
    virtual std::string getDemandElementProblem() const;

    /// @brief fix demand element problem (by default throw an exception, has to be reimplemented in children)
    virtual void fixDemandElementProblem();
    /// @}

    /**@brief open DemandElement Dialog
     * @note: if demand element needs an demand element dialog, this function has to be implemented in childrens (see GNERerouter and GNEVariableSpeedSign)
     * @throw invalid argument if demand element doesn't have an demand element Dialog
     */
    virtual void openDemandElementDialog();

    /**@brief get begin time of demand element
     * @note: used by demand elements of type "Vehicle", and it has to be implemented as children
     * @throw invalid argument if demand element doesn't has a begin time
     */
    virtual std::string getBegin() const;

    /// @name Functions related with geometry of element
    /// @{
    /// @brief begin geometry movement
    virtual void startGeometryMoving() = 0;

    /// @brief end geometry movement
    virtual void endGeometryMoving() = 0;

    /**@brief change the position of the element geometry without saving in undoList
     * @param[in] offset Position used for calculate new position of geometry without updating RTree
     */
    virtual void moveGeometry(const Position& offset) = 0;

    /**@brief commit geometry changes in the attributes of an element after use of moveGeometry(...)
     * @param[in] undoList The undoList on which to register changes
     */
    virtual void commitGeometryMoving(GNEUndoList* undoList) = 0;

    /// @brief update pre-computed geometry information
    virtual void updateGeometry() = 0;

    /// @brief partial update pre-computed geometry information
    virtual void updatePartialGeometry(const GNEEdge *edge) = 0;

    /// @brief Returns position of demand element in view
    virtual Position getPositionInView() const = 0;

    /// @brief split geometry
    virtual void splitEdgeGeometry(const double splitPosition, const GNENetElement* originalElement, const GNENetElement* newElement, GNEUndoList* undoList) = 0;
    /// @}

    /// @brief Returns a pointer to GNEViewNet in which demand element element is located
    GNEViewNet* getViewNet() const;

    /// @name members and functions relative to RouteCalculator isntance
    /// @{

    /// @brief create instance of RouteCalculator
    static void createRouteCalculatorInstance(GNENet* net);

    /// @brief delete instance of RouteCalculator
    static void deleteRouteCalculatorInstance();

    /// @brief obtain instance of RouteCalculator
    static RouteCalculator* getRouteCalculatorInstance();

    /// @}

    /// @name inherited from GUIGlObject
    /// @{

    /**@brief Returns an own popup-menu
     *
     * @param[in] app The application needed to build the popup-menu
     * @param[in] parent The parent window needed to build the popup-menu
     * @return The built popup-menu
     * @see GUIGlObject::getPopUpMenu
     */
    virtual GUIGLObjectPopupMenu* getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent);

    /**@brief Returns an own parameter window
     *
     * @param[in] app The application needed to build the parameter window
     * @param[in] parent The parent window needed to build the parameter window
     * @return The built parameter window
     * @see GUIGlObject::getParameterWindow
     */
    GUIParameterTableWindow* getParameterWindow(GUIMainWindow& app, GUISUMOAbstractView& parent);

    /**@brief Returns the boundary to which the view shall be centered in order to show the object
     * @return The boundary the object is within
     */
    virtual Boundary getCenteringBoundary() const = 0;

    /**@brief Draws the object
     * @param[in] s The settings for the current view (may influence drawing)
     * @see GUIGlObject::drawGL
     */
    virtual void drawGL(const GUIVisualizationSettings& s) const = 0;
    /// @}

    /// @name inherited from GNEAttributeCarrier
    /// @{
    /// @brief select attribute carrier using GUIGlobalSelection
    virtual void selectAttributeCarrier(bool changeFlag = true) = 0;

    /// @brief unselect attribute carrier using GUIGlobalSelection
    virtual void unselectAttributeCarrier(bool changeFlag = true) = 0;

    /// @brief check if attribute carrier is selected
    bool isAttributeCarrierSelected() const;

    /// @brief check if attribute carrier must be drawn using selecting color.
    bool drawUsingSelectColor() const;

    /* @brief method for getting the Attribute of an XML key
     * @param[in] key The attribute key
     * @return string with the value associated to key
     */
    virtual std::string getAttribute(SumoXMLAttr key) const = 0;

    /* @brief method for getting the Attribute of an XML key in double format (to avoid unnecessary parse<double>(...) for certain attributes)
     * @param[in] key The attribute key
     * @return double with the value associated to key
     */
    virtual double getAttributeDouble(SumoXMLAttr key) const = 0;

    /**@brief method for setting the attribute and letting the object perform demand element changes
     * @param[in] key The attribute key
     * @param[in] value The new value
     * @param[in] undoList The undoList on which to register changes
     */
    virtual void setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) = 0;

    /**@brief method for checking if the key and their conrrespond attribute are valids
     * @param[in] key The attribute key
     * @param[in] value The value asociated to key key
     * @return true if the value is valid, false in other case
     */
    virtual bool isValid(SumoXMLAttr key, const std::string& value) = 0;

    /* @brief method for enable attribute
     * @param[in] key The attribute key
     * @param[in] undoList The undoList on which to register changes
     * @note certain attributes can be only enabled, and can produce the disabling of other attributes
     */
    virtual void enableAttribute(SumoXMLAttr key, GNEUndoList* undoList) = 0;

    /* @brief method for disable attribute
     * @param[in] key The attribute key
     * @param[in] undoList The undoList on which to register changes
     * @note certain attributes can be only enabled, and can produce the disabling of other attributes
     */
    virtual void disableAttribute(SumoXMLAttr key, GNEUndoList* undoList) = 0;

    /* @brief method for check if the value for certain attribute is set
     * @param[in] key The attribute key
     */
    virtual bool isAttributeEnabled(SumoXMLAttr key) const = 0;

    /// @brief get PopPup ID (Used in AC Hierarchy)
    virtual std::string getPopUpID() const = 0;

    /// @brief get Hierarchy Name (Used in AC Hierarchy)
    virtual std::string getHierarchyName() const = 0;
    /// @}

    /** @brief check if a route is valid
     * @param[in] edges vector with the route's edges
     * @param[in] report enable or disable writting warnings if route isn't valid
     */
    static bool isRouteValid(const std::vector<GNEEdge*>& edges, bool report);

protected:
    /// @brief The GNEViewNet this demand element element belongs
    GNEViewNet* myViewNet;

    /// @brief demand element geometry
    GNEGeometry::Geometry myDemandElementGeometry;

    /// @brief demand element segment geometry
    GNEGeometry::SegmentGeometry myDemandElementSegmentGeometry;

    /// @name Functions relative to change values in setAttribute(...)
    /// @{

    /// @brief returns DemandElement ID
    const std::string& getDemandElementID() const;

    /// @brief check if a new demand element ID is valid
    bool isValidDemandElementID(const std::string& newID) const;

    /**@brief change ID of demand element
     * @throw exception if exist already an demand element whith the same ID
     * @throw exception if ID isn't valid
     */
    void changeDemandElementID(const std::string& newID);

    /// @}

    /// @brief calculate personPlan start and end positions over lanes
    void calculatePersonPlanLaneStartEndPos(double &startPos, double &endPos) const;

    /// @brief calculate personPlan start and end positions
    void calculatePersonPlanPositionStartEndPos(Position &startPos, Position &endPos) const;

    /// @brief get first allowed vehicle lane
    GNELane* getFirstAllowedVehicleLane() const;

    /// @brief get first allowed vehicle lane
    GNELane* getLastAllowedVehicleLane() const;

private:
    /**@brief check restriction with the number of children
     * @throw ProcessError if itis called without be reimplemented in child class
     */
    virtual bool checkDemandElementChildRestriction() const;

    /// @brief method for setting the attribute and nothing else (used in GNEChange_Attribute)
    virtual void setAttribute(SumoXMLAttr key, const std::string& value) = 0;

    /// @brief RouteCalculator instance
    static RouteCalculator* myRouteCalculatorInstance;

    /// @brief Invalidated copy constructor.
    GNEDemandElement(const GNEDemandElement&) = delete;

    /// @brief Invalidated assignment operator.
    GNEDemandElement& operator=(const GNEDemandElement&) = delete;
};


#endif

/****************************************************************************/

