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
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot, ['--gui-testing-debug-gl'])

# go to shape mode
netedit.shapeMode()

# go to shape mode
netedit.changeElement("poiLane")

# create poi
netedit.leftClick(referencePosition, 150, 215)

# enable ID
netedit.changeDefaultBoolValue(2)

# create poi
netedit.leftClick(referencePosition, 170, 215)

# set invalid ID
netedit.changeDefaultValue(3, ";;;;;;")

# try to create poi
netedit.leftClick(referencePosition, 190, 215)

# set invalid ID
netedit.changeDefaultValue(3, "POI_0")

# try to create poi
netedit.leftClick(referencePosition, 220, 215)

# set invalid ID
netedit.changeDefaultValue(3, "customID")

# create POI
netedit.leftClick(referencePosition, 240, 215)

# Check undo redo
netedit.undo(referencePosition, 4)
netedit.redo(referencePosition, 4)

# save shapes
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
