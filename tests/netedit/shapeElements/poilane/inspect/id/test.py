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

# go to shape mode
netedit.shapeMode()

# select POILane in list of shapes
netedit.changeElement("poiLane")

# create POILane
netedit.leftClick(referencePosition, 140, 215)

# create POILane
netedit.leftClick(referencePosition, 200, 215)

# go to inspect mode
netedit.inspectMode()

# inspect first POILane
netedit.leftClick(referencePosition, 140, 215)

# Change parameter ID with a non valid value (Duplicated ID)
netedit.modifyAttribute(0, "POI_1", True)

# Change parameter ID with a non valid value (empty)
netedit.modifyAttribute(0, "", True)

# Change parameter ID with a non valid value (invalid)
netedit.modifyAttribute(0, "ID with spaces", True)

# Change parameter ID with a valid value
netedit.modifyAttribute(0, "newID", True)

# Check undos and redos
netedit.undo(referencePosition, 3)
netedit.redo(referencePosition, 3)

# save shapes
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
