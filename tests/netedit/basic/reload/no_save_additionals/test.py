#!/usr/bin/env python
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2009-2019 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
# SPDX-License-Identifier: EPL-2.0

# @file    test.py
# @author  Pablo Alvarez Lopez
# @date    2016-11-25
# @version $Id$

# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit  # noqa

# Open netedit
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot)

# Change to create mode
netedit.createEdgeMode()

# Create two nodes
netedit.leftClick(referencePosition, 0, 400)
netedit.leftClick(referencePosition, 500, 400)

# go to additional mode
netedit.additionalMode()

# select busStop
netedit.changeElement("busStop")

# create busStop in mode "reference left"
netedit.leftClick(referencePosition, 250, 250)

# go to shape mode
netedit.shapeMode()

# go to additional mode
netedit.changeElement("poly")

# create polygon
netedit.createSquaredPoly(referencePosition, 100, 50, 100, True)

# go to demand mode
netedit.supermodeDemand()

# go to route mode
netedit.routeMode()

# create route using three edges
netedit.leftClick(referencePosition, 160, 250)
netedit.leftClick(referencePosition, 160, 225)

# press enter to create route
netedit.typeEnter()

# save network
netedit.saveNetwork(referencePosition)

# save routes
netedit.saveRoutes(referencePosition)

# reload netedit without saving additionals
netedit.reload(neteditProcess, False, False, True, False, False, False)

# quit netedit
netedit.quit(neteditProcess, False, False, False, False, False, False)
