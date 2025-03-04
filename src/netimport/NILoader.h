/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2019 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    NILoader.h
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @date    Tue, 20 Nov 2001
/// @version $Id$
///
// Perfoms network import
/****************************************************************************/
#ifndef NILoader_h
#define NILoader_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <string>
#include <vector>
#include <xercesc/sax2/SAX2XMLReader.hpp>


// ===========================================================================
// class declarations
// ===========================================================================
class OptionsCont;
class NBNetBuilder;
class Position;
class PositionVector;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class NILoader
 * @brief Perfoms network import
 *
 * A plain loader which encapsulates calls to the import modules.
 */
class NILoader {
public:
    /** @brief Constructor
     * @param[in] nb The network builder to fill with loaded data
     */
    NILoader(NBNetBuilder& nb);


    /// @brief Destructor
    ~NILoader();


    /** loads data from the files specified in the given option container */
    void load(OptionsCont& oc);


private:
    /** loads data from sumo-files */
    //void loadSUMO(OptionsCont &oc);

    /** loads data from XML-files */
    void loadXML(OptionsCont& oc);


private:
    /// @brief The network builder to fill with loaded data
    NBNetBuilder& myNetBuilder;


private:
    /// @brief Invalidated copy constructor.
    NILoader(const NILoader&);

    /// @brief Invalidated assignment operator.
    NILoader& operator=(const NILoader&);


};


#endif

/****************************************************************************/

