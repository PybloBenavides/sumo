/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    CarEdge.h
/// @author  Michael Behrisch
/// @date    Mon, 03 March 2014
/// @version $Id$
///
// The CarEdge is a special intermodal edge representing the SUMO network edge
/****************************************************************************/
#ifndef CarEdge_h
#define CarEdge_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#ifdef HAVE_FOX
#include <fx.h>
#endif
#include "IntermodalEdge.h"


// ===========================================================================
// class definitions
// ===========================================================================
/// @brief the car edge type that is given to the internal router (SUMOAbstractRouter)
template<class E, class L, class N, class V>
class CarEdge : public IntermodalEdge<E, L, N, V> {
private:
    typedef IntermodalEdge<E, L, N, V> _IntermodalEdge;

public:
    CarEdge(int numericalID, const E* edge, const double pos = -1.) :
        _IntermodalEdge(edge->getID() + "_car" + toString(pos), numericalID, edge, "!car"),
        myStartPos(pos >= 0 ? pos : 0.) { }

    bool includeInRoute(bool /* allEdges */) const {
        return true;
    }

    const std::vector<_IntermodalEdge*>& getSuccessors(SUMOVehicleClass vClass = SVC_IGNORING) const {
        if (vClass == SVC_IGNORING) {
            return this->myFollowingEdges;
        }
#ifdef HAVE_FOX
        FXMutexLock locker(myLock);
#endif
        typename std::map<SUMOVehicleClass, std::vector<_IntermodalEdge*> >::const_iterator i = myClassesSuccessorMap.find(vClass);
        if (i != myClassesSuccessorMap.end()) {
            // can use cached value
            return i->second;
        } else {
            // this vClass is requested for the first time. rebuild all successors
            const std::set<const E*> classedCarFollowers = std::set<const E*>(this->getEdge()->getSuccessors(vClass).begin(), this->getEdge()->getSuccessors(vClass).end());
            for (_IntermodalEdge* const e : this->myFollowingEdges) {
                if (!e->includeInRoute(false) || e->getEdge() == this->getEdge() || classedCarFollowers.count(e->getEdge()) > 0) {
                    myClassesSuccessorMap[vClass].push_back(e);
                }
            }
            return myClassesSuccessorMap[vClass];
        }
    }

    virtual const std::vector<std::pair<const _IntermodalEdge*, const _IntermodalEdge*> >& getViaSuccessors(SUMOVehicleClass vClass = SVC_IGNORING) const {
        if (vClass == SVC_IGNORING) {
            return this->myFollowingViaEdges;
        }
#ifdef HAVE_FOX
        FXMutexLock locker(myLock);
#endif
        typename std::map<SUMOVehicleClass, std::vector<std::pair<const _IntermodalEdge*, const _IntermodalEdge*> > >::const_iterator i = myClassesViaSuccessorMap.find(vClass);
        if (i != myClassesViaSuccessorMap.end()) {
            // can use cached value
            return i->second;
        } else {
            // this vClass is requested for the first time. rebuild all successors
            std::set<const E*> classedCarFollowers;
            for (const auto& pair : this->getEdge()->getViaSuccessors(vClass)) {
                classedCarFollowers.insert(pair.first);
            }
            for (const std::pair<const _IntermodalEdge*, const _IntermodalEdge*>& e : this->myFollowingViaEdges) {
                if (!e.first->includeInRoute(false) || e.first->getEdge() == this->getEdge() || classedCarFollowers.count(e.first->getEdge()) > 0) {
                    myClassesViaSuccessorMap[vClass].push_back(e);
                }
            }
            return myClassesViaSuccessorMap[vClass];
        }
    }

    bool prohibits(const IntermodalTrip<E, N, V>* const trip) const {
        return trip->vehicle == 0 || this->getEdge()->prohibits(trip->vehicle);
    }

    double getTravelTime(const IntermodalTrip<E, N, V>* const trip, double time) const {
        const double travelTime = E::getTravelTimeStatic(this->getEdge(), trip->vehicle, time);
        double distTravelled = this->getLength();
        // checking arrivalPos first to have it correct for identical depart and arrival edge
        if (this->getEdge() == trip->to) {
            distTravelled = trip->arrivalPos - myStartPos;
        }
        if (this->getEdge() == trip->from) {
            distTravelled -= trip->departPos - myStartPos;
        }
        return travelTime * distTravelled / this->getEdge()->getLength();
    }

    double getStartPos() const {
        return myStartPos;
    }

    double getEndPos() const {
        return myStartPos + this->getLength();
    }

private:
    /// @brief the starting position for split edges
    const double myStartPos;

    /// @brief The successors available for a given vClass
    mutable std::map<SUMOVehicleClass, std::vector<_IntermodalEdge*> > myClassesSuccessorMap;

    /// @brief The successors available for a given vClass
    mutable std::map<SUMOVehicleClass, std::vector<std::pair<const _IntermodalEdge*, const _IntermodalEdge*> > > myClassesViaSuccessorMap;

#ifdef HAVE_FOX
    /// The mutex used to avoid concurrent updates of myClassesSuccessorMap
    mutable FXMutex myLock;
#endif
};


#endif

/****************************************************************************/
