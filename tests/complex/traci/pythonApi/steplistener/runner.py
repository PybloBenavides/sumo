#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2008-2019 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
# SPDX-License-Identifier: EPL-2.0

# @file    runner.py
# @author  Michael Behrisch
# @author  Daniel Krajzewicz
# @author  Jakob Erdmann
# @date    2011-03-04
# @version $Id$


from __future__ import print_function
from __future__ import absolute_import
import os
import sys

SUMO_HOME = os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..")
sys.path.append(os.path.join(os.environ.get("SUMO_HOME", SUMO_HOME), "tools"))
import traci  # noqa
import sumolib  # noqa


class ExampleListener(traci.StepListener):
    def step(self, t=0):
        # do something at every simulaton step
        print("ExampleListener called at time %s ms." % t)
        # indicate that the step listener should stay active in the next step
        return True


traci.start([sumolib.checkBinary('sumo'), "-c", "sumo.sumocfg", "--ignore-route-errors"])
listener = ExampleListener()
traci.addStepListener(listener)
for i in range(10):
    traci.simulationStep(i)
traci.close()
