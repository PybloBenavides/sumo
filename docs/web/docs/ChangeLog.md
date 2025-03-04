---
title: ChangeLog
permalink: /ChangeLog/
---

## Git Master

### Bugfixes

- Simulation
  - Fixed crash when setting a negative value for option **--device.rerouting.pre-period** Issue #6012
  - Avoiding unnecessary rerouting while trips and flows are delayed from insertion. Issue #6013
  - Lanechange reason is now written in lanechange-output when using the [simple continuous lanechange model](Simulation/SublaneModel.md#simple_continous_lane-change_model). Issue #6014
  - Fixed invalid mode change from car during intermodal routing. Issue #6070
  - Fixed invalid cooperative speed adjustment in sublane model. Issues #6266, #6267
  - Fixed crash related to parkingAreaReroute. Issue #6283

- NETEDIT
  - Fixed crash when defining walks between disconnected locations. Issue #5369
  - Fixed crash transforming trips to flows. Issue #6050
  - Now blocked shapes can be moved again. Issue #6053
  - Fixed invalid snap to grid when trying to move junction. Issue #6067
  - Coloring edges by generic parameter is now working. Issue #6062
  - "Open in SUMO-GUI" option now works correctly when there are spaces in the path. Issue #6227
  - Fixed bug where multiple junctions could mistakenly be created in the same place. Issue #6228
  - Fixed invalid lane shapes while moving edge geometry in a lefthand network. Issue #6234
  - Fixed invalid edge shape when using 'replace junction' ahead of junction with custom shape. Issue #5777
  - Connection mode function 'select dead starts' now correctly selects lanes without predecessor (regression in 1.0.0). Issue #6278
  - Fixed slowdown clicking over edges with high concentration of routes. Issue #6276

- NETCONVERT
  - Fixed bugs in classification of roundabouts. Issue #6032
  - Preventing invalid double tracks when using typemap [osmNetconvertRailUsage.typ.xml]({{Source}}data/typemap/osmNetconvertRailUsage.typ.xml). Issue #6056
  - Disabling invalid handling of railway track count in OSM input. Issue #6057
  - Missing track numbers in bidirectional railway edges. Issue #6058
  - Corrected handling of user-specified edge length when adding bidirectional railway edges. Issue #6280
  - Fixed invalid junction and connection shapes in plain-xml-output when using option **--proj.plain-geo** Issue #6066
  - Fixed invalid geometry when using option **--geometry.max-segment-length** Issue #6074
  - Fixed invalid network when using option **--geometry.split** Issue #6075
  - Fixed invalid handling of custom crossing shape when network coordinates are shifted. Issue #4621
  - Fixed invalid error message when loading .tll-file with tlLogid-programID other than '0'. Issue #6160
  
- Meso
  - device.emissions now collects data. Issue #6086
  - Fixed crash when using device.btsender or device.btreceiver. Issue #6087
  - Fixed crash when loading network with internal lanes that have neighbor ('neigh') data. Issue #6205

- SUMO-GUI
  - Sublane-borders are no longer drawn for railways and sidewalks (where the sublane model does not apply).
  - Fixed crash when using vehicle popup-menu after the vehicle has left the simulation. Issue #6107
  - Fixed crash after removing view with active vehicle visualization once the vehicle leaves the simulation. Issue #6112
  - "Open in Netedit" option now works correctly when there are spaces in the path. Issue #6227

- DUAROUTER
  - Fixed invalid mode change from car during intermodal routing. Issue #6070
  - Fixed invalid car routes during intermodal routing. Issue #6077

- TraCI
  - TraaS functions *Trafficlight.getControlledJunctions*, *Edge.setEffort*, *Edge.getEffort*, *Edge.adaptTraveltime*", *Edge.getAdaptedTraveltime* are now working. Issue #6019  
  - Vehicles being controlled by moveToXY are no longer ignored by cross-traffic while on an intersection. Issue #6140
  - Traffic light programs of type 'actuated' and 'delay_based' can now be created. Issue #4324


### Enhancements
- All Applications
  - Environment variables can now be referenced in configuration files using a shell-like syntax: `<net-file value="${NETFILENAME}.net.xml"/>`. Issue #1901
- Simulation
  - Simulation of parking vehicles now optionally takes into account [maneuvering times](Simulation/ParkingArea.md#modelling_maneuvering_times_when_entering_and_leaving_the_parking_space). Issue #5956
  - Vehicle stop elements now support attribute 'speed'. This can be used to force a vehicle to drive with a fixed speed on a particular stretch of lane. Issue #1020
  - When defining a person ride, setting attribute `lines="ANY"` can now be used to take any vehicle that stops at the destination busStop or destination edge. Issue #6039
  - when defining vehicle stops, the new attribute 'extension' can be used to limit delays when too many people are boarding or when expected persons do not show up. Issue #6072 #3635
  - Improved smoothness of acceleration / deceleration for carFollowModel 'IDM' when speed limits change and there is no leader vehicle. Issue #6080
  - Persons can now start their plan in a driving vehicle by setting `depart="triggered"`. Issue #4042
  - Traffic light type 'actuated' now supports skipping phases by defining a list of alternative successor phases in phase attribute 'next'. Issue #5042

- Meso
  - Vehicles that reach maximum impatience can now pass junctions from an unprioritized road regardless of foe traffic. The old behavior can be restored by disabling impatience growth via **--time-to-impatience 0**. Issue #5830
- NETCONVERT
  - Railway network input with sharp angles along an edge is now corrected by default. (The previous behavior can be enabled with option **--geometry.min-angle.fix.railways false**). Issue #6921
  - Pedestrian paths are no longer checked for minimum turning angles (greatly reducing superfluous warnings). Issue #6022
  - The option **--opposites.guess.fix-length** is no longer required when using networks with default lengths (no user-set values). The resulting networks will automatically adapt their edge lengths when changing geometry. Issue #6028
  - When importing public transport lines (**--ptline-output**), edge permissions are now adapted to allow the respective public transport vehicle. Issue #3437
  - When importing public transport lines from OSM, stops that are not part of the road network are now mapped to the nearest line edge (previously, these stops were discarded). Issue #6114

- Netconvert
  - Regional road names (A9, I-95) are now exported as edge params. They are also exported as Regional Name ID in dlr-navteq-output. Issue #6256
  - Added option **--offset.z** to apply a constant z-offset to a simulation network. Issue #6274
  - Added option **--tls.ignore-internal-junction-jam** to force building traffic light right-of-way rules without mutual response flags. Issue #6269

- Netedit
  - Now additionals and routes can optionally be loaded using option "Open in SUMO-GUI"). Issue #6049
  - Added shortcuts (Shift F1,F2,F3) for edge templates. Issue #6033
  - VTypes can be edited in multiple selected vehicles. Issue #6051
  - Improved support of GNEStops. Issue #5367
  
- SUMO-GUI
  - Added 'select reachable' function to the lane context-menu to select all lanes that can be reached by a given vehicle class from the origin lane. After such an operation, the coloring scheme 'by reachability' can be used to render an isochrone travel time map. Issue #3650
  - The currently loaded options can now be saved to a configuration via the file menu. Issue #5707  
  - The object locator dialog now allows filtering the object list by matching all ids/names that include a given text string.
  - Added option for rendering the (relative) route index for each edge when drawing a vehicles route. Issue #6199
  - Traffic light parameter dialog now includes more values describing the current phase (phase, phase name, duration, minDur, maxDur, running duration)
  
- TraCI
  - Added function 'traci.trafficlight.getServedPersonCount' to return the number of pedestrians that would be served by all the crossings for a given phase index. Issue #6088
  - Can now access vehicle device parameters 'device.ssm.minTTC', 'device.ssm.maxDRAC', 'device.ssm.minPET'. Issue #4983
  - Added function 'traci.simulation.writeMessage' to append a custom message to the log file (and sumo-gui message window). Issue #6249
  
- Tools
  - When importing public transport traffic with osmWebWizard, vehicles from incomplete lines now start and end their routes at the network border instead of starting at the first stop. Issue #6025
  - Added new tool [analyzePersonPlans.py](Tools/Routes.md) to count the different types of plans in a route file. Issue #6083
  - Added new tool [matsim_importPlans.py](Tools/Import/MATSim.md) to import intermodal traffic plans from MATSim
  - Added new tool [generateContinuousRerouters.py](Tools/Miscm.md#generatecontinuousrerouterspy) to easily create a simulation where vehicles circulate according to configured turning ratios without ever arriving. #6310
  
### Other

- SUMO-GUI, NETEDIT
  - Zooming now centers on the mouse-position by default instead of on the screen center. As before, the behavior can be toggled using the icon in the view menu bar and persists across application restarts. Issue #5992
- NETCONVERT
  - The default number of lanes when importing 'highway.secondary' from OSM was changed from 2 to 1. Issue #5690
- Documentation moved from MediaWiki to MkDocs

## Version 1.3.1 (27.08.2019)

### Bugfixes

- NETEDIT
  - Fixed crash when defining vehicle stops. Issue #5963 (regression in 1.3.0)
  - Fixed crashing and invalid errors when editing vehicle type attributes. Issue #5957 (regression in 1.3.0)
  - Fixed crash after defining flow on unusable edges. Issue #5958
  - Fixed crash after loading flow with invalid route. Issue #5967
  - Fixed bug where new vehicles could not be created if trips with a specific id already existed. Issue #5969
  - Fixed invalid application focus after activating checkboxes Issue #5966 (regression in 1.2.0)
  - Attribute *trainType* for carFollowModel *Rail* can now be set. Issue #5959

- Simulation
  - Fixed crash when loading rail signals with long gaps in between. Issue #5983
  - Fixed invalid restrictions on vType parameters. Issue #5991 (regression in 1.3.0)

- NETCONVERT
  - Fixed high running time when using edge-removal options with long edge lists. Issue #5954
  - Fixed creation of invalid network after defining invalid splits. Issue #5980

- DUAROUTER
  - Fixed crash on empty vType in input

- TraCI / libsumo
  - Function *vehicle.changeLaneRelative* is now working with a negative lane offset. Issue #5990
  - several functions were adopted to have the same interface in libsumo and the python client, see Issue #5523

- Tools
  - flowrouter.py can now route vehicles on networks with isolated edges and pedestrian structures. Issue #5951

### Enhancements

- NETEDIT
  - Added menu option for recomputing routes of trips and flows in demand mode (F5) Issue #5978

- TraCI / libsumo
  - You can now trigger the usage of libsumo instead of TraCI in Python by setting the environment variable LIBSUMO_AS_TRACI to any value.

- Tools,
  - [traceExporter.py](Tools/TraceExporter.md) now supports conversion of fcd-output to KML. Issue #1989

### Other

- Miscellaneous
  - Updated SUMOLibraries structure. To build on windows, the libraries must be updated as well.
  - removed doxygen docu from the distribution

## Version 1.3.0 (20.08.2019)

### Bugfixes

- Simulation
  - Fixed invalid default parameters for the electrical vehicle model that were causing too high energy consumption. Issue #5505
  - Fixed deadlock on controlled intersection with pedestrian crossing. Issue #5487 (regression in 1.1.0)
  - Fixed invalid emergency braking in jammed scenario. Issue #5632
  - Fixed inappropriate lane choice when approaching a multi-lane roundabout in dense traffic. Issue #2576, Issue #2634
  - Rail signals ahead of uncontrolled switches now properly take the vehicle route into account (this was causing invalid red states previously). Issue #5137
  - Options **--tripinfo-output.write-unfinished, --vehroute-output.write-unfinished** now also apply to persons. Issue #3939
  - Fixed bug that could corrupt pedestrian routes when walking across very short lanes. Issue #5661
  - Fixed bug that could cause sumo to freeze when using opposite-direction driving.
  - Fixed crash when using option **--device.rerouting.pre-period 0** with a **--scale** lower than 1. Issue #5704
  - Fixed emergency braking after lane changing when using `carFollowMode="Wiedemann"`. Issue #5711
  - Fixed exaggerated braking to avoid overtaking on the right. Issue #5713
  - Fixed invalid junction rules when using sublane simulation on lefthand-networks Issue #5738
  - Fixed invalid edgeData and tripinfo output in the [mesoscopic model](Simulation/Meso.md) when vehicles are completely jammed. Issue #5722
  - Fixed crash when using SSM-Device with opposite-direction-driving Issue #5231, #5839
  - FullOutput now returns all speeds as m/s. Issue #5787
  - Fixed invalid error when using calibrators with a fixed departLane. Issue #5794
  - Fixed invalid jamming at occupied parkingAreas close to the start of a lane. Issue #5864
  - Fixed detector placement at actuated traffic light (avoiding warnings of the form *minDur X is too short for detector for a detector gap of Y*) Issue #5119
  - Access edges to stops from lanes which do not allow pedestrians are now ignored. Issue #5890
  - Fixed crash on using ToC device with an unknown vehicle type. Issue #5761
  - Route length in tripinfo output is now correct also when rerouting happens. Issue #5755
  - When combining `<closingReroute>` and `<destProbReroute>`, all vehicles that cannot reach their target due to the closing are rerouted. Issue #5904
  - Fixed bug where vehicles could enter a fully occupied parkingArea. Issue #5905
  - Fixed bug where vehicles would fail to exit from a parkingArea. Issue #5907
  - Fixed unusable person plan when loading a personTrip with a non-zero departPos. Issue #5927

- SUMO-GUI
  - Fixed crash when rendering short vehicles as *simple shapes* with guiShape *truck/trailer* and *truck/semitrailer*. Issue #5510 (regression in 1.2.0)
  - Pedestrian crossings and walkingareas are no longer listed in the edge locator dialog by default (they can still be enabled by checking *Show internal structures*. Issue #5613
  - Fixed invalid image position and size when rendering vehicles as raster images. Issue #5688 (regression in 1.2.0)
  - Fixed invalid vehicle coloring in mesoscopic mode. (regression in 0.32.0).
  - Fixed invalid vehicle angle when using the sublane model in lefthand-networks. Issue #5737
  - Fixed 3D-View rotation when holding middle-mouse button.

- NETCONVERT
  - Fixed handling of custom shapes in lefthand networks (for connections, crossings, walkingareas). Issue #5507
  - Fixed missing connections when importing OpenDRIVE networks with short laneSections in connecting roads. Issue #5504
  - Fixed invalid link direction at intermodal intersection and at intersections with sharp angles. Issue #5511, Issue #5512
  - Fixed invalid large traffic light clusters when using options **--tls.guess --tls.join**. Issue #5524
  - Fixed invalid custom edge lengths when using option **--geometry.remove** Issue #5612
  - Zipper junctions with multiple incoming edges are now supported. Issue #5657
  - Fixed error when guessing ramps which are close to each other. Issue #5681
  - Option **--tls.guess** now works correctly at junctions with 2 incoming edges. Issue #5684
  - Fixed invalid junction shape at geometry-like nodes where only the lane-width changes. Issue #5749
  - Fixed invalid lane geometry when defining `<split>` for lefthand network. Issue #5856
  - VISSIM networks can now also have multiple lanes connected to one. Issue #5560
  - Option **--junctions.scurve-stretch** is now working for junctions with non-default radius. Issue #5881
  - Fixed exaggerated lane widths when importing briefly-orthogonal border lanes from OpenDRIVE. Issue #5888

- NETEDIT
  - Crossing tls indices are now properly reset when deleting traffic light. Issue #5549
  - Controlled pedestrian crossings at a *rail_crossing* nodes are no longer lost when saving the network with netedit. Issue #5559 (regression in 1.0.0)
  - Snap-to-grid is now working in networks with elevation data. Issue #5878
  - Fixed bug where neigh-edges could get lost upon loading and saving a network. Issue #5916
  - Demand mode issues Issue #5576
    - Creating routes from non-consecutive edges is now working
    - Flow attribute route is now correctly saved
    - Fixed bug where some demand attributes could not be edited

- MESO
  - Fixed invalid *departPos* in tripinfo-output when loading saved state. Issue #5819

- DUAROUTER
  - Option **--write-trips** is now working for persons. Issue #5563

- JTRROUTER
  - Vehicle types with PHEMlight emission class can now be handled. Issue #5669

- TraCI / Libsumo
  - TraCI does **not** execute another step when close() is called. Please recheck your scripts, they might finish one step earlier than before.
  - Fixed TraaS method *Simulation.convertRoad*. Issue #5478
  - Fixed bug where vehicle is frozen after removing stop at stopping place via TraCI. Issue #5561
  - Fixed invalid distance to far-away TLS returned by *vehicle.getNextTLS* Issue #5568
  - Fixed crash due to invalid person list at busStop. Issue #5620
  - Persons are no longer listed before their departure in *traci.person.getIDList()* Issue #5674 (regression in 1.1.0)
  - Fixed crash when using parallel rerouting with TraCI. Issue #5553
  - Function *traci.vehicle.setStop* now correctly updates an existing stop when setting a new *until*-value. Issue #5712
  - TraaS function *SumoTraciConection.close()* now cleanly disconnects from the SUMO server. Issue #4962
  - Fixed several issues with position conversions when the edge has a length different from the geometrical distance. Issue #5780
  - Libsumo now supports getAllSubscriptionResults
  - Function *traci.vehicle.moveTo* can now move vehicles onto internal lanes. Issue #5932
  - Traffic lights for junction types rail_signal and rail_crossing can now be switched off with *trafficlight.setProgram("off")* and remote-controlled with *trafficlight.setRedYellowGreenState*. Issue #5946

- General
  - The build version number is updated correctly after all git updates. Issue #3963

### Enhancements

- Simulation
  - Added junction model parameter *jmDriveAfterYellowTime* to configure driving at yellow behavior. Issue #5474
  - calibrators now accept attribute *vTypes* to restrict their application (insertion/removal) to selected vehicle types. Issue #3082
  - Vehicle [`<stops>`s](Definition_of_Vehicles,_Vehicle_Types,_and_Routes.md#stops) now support the optional attributes tripId and line to track the current state when serving a cyclical public transport route. Issue #5280, Issue #5514
  - Added vehicle class *rail_fast* to model [High-Speed-Rail](https://en.wikipedia.org/wiki/High-speed_rail) Issue #5525
  - Netstate-output now includes attribute *speedLat* when using a [model for lateral dynamics](Simulation/SublaneModel.md). Issue #5636
  - Pedestrians now switch to *jam resolving* behavior earlier when jammed on a pedestrian crossing. The time threshold can be configured with the new option **--pedestrian.striping.jamtime.crossing** {{DT_TIME}} (default 10s) Issue #5662
  - Extended [ToC Model](ToC_Device.md#configuration) by various functionalities (4 new parameters: dynamicToCThreshold, dynamicMRMProbability, mrmKeepRight, maxPreparationAccel). Issue #5484
  - Detectors for actuated traffic lights can now be placed on upstream lane to achieve the desired distance to the stop line. Issue #5628
  - Added lane-change model parameter *lcOvertakeRight* to model violation of rules against overtaking on the right side. Issue #5633
  - Lanechange-output now includes the speeds of surrounding vehicles at the time of lane-change. Issue #5744
  - Added new departSpeed values desired (departure with speedLimit * speedFactor) and speedLimit (departure with speedLimit). Issue #2024
  - Added new carFollowModel *W99* which is a 10-Parameter version of the Wiedemann model. Issue #5765
  - Saved states now include lane ids and only the lanes with traffic on them. This reduces state file size and also permits loading state files with a modified network. Issue #5872
  - Initial "pseudo routes" (consisting of start and end edge only) are only written optionally. Issue #5639
  - Traffic lights with type delay_based now use a default detection range of 100m (instead of using the whole lane length). This greatly improves efficiency by avoiding interference from far-away upstream junctions. The default range can be set using the new option **--tls.delay_based.detector-range** {{DT_FLOAT}}. Issue #5897
  - Added battery model parameter "recuperationEfficencyByDecel" to vary the recuperation efficiency based on the deceleration level. Thanks to Sagaama Insaf for the contribution. Issue #5675

- NETCONVERT
  - Now importing High-Speed-Rail tracks from OSM Issue #5525
  - Now importing attribute railway:bidirectional from OSM Issue #5531
  - Now importing track number (track_ref) parameter from OSM Issue #5533
  - Railway usage information (main, branch, industrial, ...) can now be imported from OSM by loading the new typemap [osmNetconvertRailUsage.typ.xml](Networks/Import/OpenStreetMap.md#recommended_typemaps). Issue #5529
  - Vehicle class *rail_electric* is now correctly assigned depending on railway electrification. Issue #5528
  - Now distinguishing more junction types in verbose summary. Issue #5541
  - Added experimental multi-language support for VISUM import using the new option **--visum.language-file** and language mapping files in [{{SUMO}}/data/lang]({{Source}}data/lang). Issue #1767
  - Added option **--junctions.right-before-left.speed-threshold** {{DT_FLOAT}} to influence the heuristic for guessing junction type (priority/right-before-left) from edge speeds. Issue #5799
  - Added option **--join-lanes** {{DT_BOOL}} to allow merging of adjacent non-vehicle lanes (sidewalk, green verge). Issue #5880
  - Added option **--geometry.remove.width-tolerance** {{DT_FLOAT}} to allow removal of geometry-like nodes when lane widths of merged edges differ no more than the given threshold. Issue #5883
  - Option **--junctions.scurve-stretch** is no longer applied for non-vehicular connections. Issue #5887
  - Added options for generating bicycle lanes: **--bikelanes.guess --bikelanes.guess.max-speed --bikelanes.guess.min-speed --bikelanes.guess.from-permissions --bikelanes.guess.exclude --default.bikelane-width** [These options work the same as the corresponding sidewalk options](Simulation/Bicycles.md#generating_a_network_with_bike_lanes) and can also be used with [NETGENERATE](NETGENERATE.md). Issue #5924

- NETEDIT
  - Inverting selection now takes selection locks into account. Issue #5615
  - When setting tltype to *actuated*, suitable values for *minDur* and *maxDur* will be added to the phases. Issue #5637
  - Added support for Vehicles, Flows and Trips. Issue #5585 Issue #5086 Issue #5050
  - Partial support of GNEPersons Issue #5369

- SUMO-GUI
  - Added new visualization presets *selection* (coloring everything according to selection status) and *rail* (analyze rail networks by showing allowed driving directions for all tracks and highlighting rail signals when zoomed out). Issue #5660
  - Added context menu option to show/hide detectors for actuated traffic lights Issue #5627
  - Added visualization option for drawing an edge color legend. Issue #5682
  - Vehicles and persons can now be removed via right-click menu (*remove*). Issue #5649
  - All Vehicle and person parameters that change during the simulation are now updated in any open parameter window. (previously string parameters were always static). Issue #4210
  - The parkingAreas parameter dialog now includes the number of alternative parkingAreas due to loaded *parkingAreaReroute* definitions.
  - The *Recalibrate Rainbow* functionality now generates a distinct color for every permission code when coloring *by permissions*. Issue #5852

- POLYCONVERT
  - Importing railway entities from OSM is now supported and [a new typemap](Networks/Import/OpenStreetMap.md#railway-specific_objects) was added for this. Issue #5572
  - Shapefile import now supports typemaps and also image files in type maps. Issue #5891

- TraCI
  - Added function *simulation.getBusStopWaitingIDList* to retrieve
    waiting persons. Issue #5493
  - Added function *getPersonCapacity* to vehicle and vehicletype
    domains. Issue #5519
  - Added function *addDynamics* to polygon domain. Issue #5396
  - Added function *highlight* to poi and vehicle domain. Issue #5396
  - TraaS function *Vehicle.setStop* now supports the arguments
    *startPos* and *until* in line with the other clients. Issue #5710
  - Added subscription filtering to C++ client. Issue #5772
  - Added function *person.appendStage* which allows adding a stage object directly to a person plan. #5498
  - Added function *person.replaceStage* which allows replacing an upcoming stage with a stage object. #5797
  - Added function *vehicle.getLateralSpeed* to the python and C++ client. #5010
  - StepListeners in the python client can now be added to a single connection (also allows using them with Flow). Issue #5863
  - The python client now tries again to start sumo and connect with a different port if the port chosen was in use. Issue #5802
  - Function *vehicle.moveTo* can now be used to move the vehicle to any lane on its route including those that were already passed.

- Tools
  - Added new toolbox [{{SUMO}}/tools/contributed/saga]({{Source}}tools/contributed/saga) (SUMO Activity Generation) to create intermodal scenarios from OSM data. This includes building a virtual populatin and generating mobility plans for a while day.
  - [Public transport import from
    OSM](Tutorials/PT_from_OpenStreetMap.md) (also used by
    [osmWebWizard](Tools/Import/OSM.md#osmwebwizardpy) now
    filters out lines that only run at night. If the option **--night** is set,
    only night-service lines are exported. Issue #5548
  - Improving UTF8 support for xml2csv.py and csv2xml.py. Issue #5538, Issue #5588
  - Added a new tool
    [generateTurnRatios.py](Tools/Misc.md#generateturnratiospy)
    for generating turning ratios according to a given route file. Issue #1666
  - The option cost modifier in
    [duaIterate.py](Tools/Assign.md#dua-iteratepy) is
    removed, since it is used for specific projects. Ticket #5594 is open
    to check the respective content for publication and to extend
    the cost modifier function.
  - Added [randomTrips.py](Tools/Trip.md#edge_probabilities)
    options **--angle-factor** {{DT_FLOAT}} and **--angle** {{DT_FLOAT}} to modify trip probabilities by direction of
    travel. Issue #5642
  - checkStyle.py now also checks Java files and can be used for single files. Issue #5653, Issue #5652
  - extractTest.py can now download tests directly from sumo.dlr.de. Issue #1092
- General
  - All applications can now read and write gzipped files (except for configurations). Issue #5448
  - The Windows build now automatically installs the needed DLLs in the bin dir (if SUMOLibraries are present). Issue #4887

### Other

- TraCI
  - Function *person.getStage* now returns a TraCIStage object instead of an integer denoting the stage type. #5495
  - Functions *person.appendWalkingStage*,
    *person.appendWaitingStage* now send duration value as a double
    in line with all other time values. #5708
  - Fixed inconsistency in client data model for routing results
    (TraCIStage). In the python client, the stage member which
    describes the stage type is now called *type* instead of
    *stageType* in line with the other clients. #5502
  - Function *vehicle.getNextStops* now includes the current stop if
    the vehicle is stopped. This case can be distinguished by the
    first bit ('reached').

- NETCONVERT
  - Network version is now 1.3
  - In the previous version, setting options **--tls.guess --tls.join** activated a heuristic
    that could create joint controllers for clusters of uncontrolled
    nodes (which would not be guessed as controled individually).
    Now this heuristic must be activated explicitly using option **--tls.guess.joining**.

- NETEDIT
  - Changed button in allow/disallow-dialog from *allow only
    non-road vehicles* to *allow only road vehicles* vClasses. Issue #5526

- NETGENERATE
  - Option **--rand.min-angle** {{DT_FLOAT}} now expects and argument in degrees instead of radians. Issue #5521

## Version 1.2.0 (16.04.2019)

### Bugfixes

- Simulation
  - Fixed too high insertion speeds for the ACC car following model leading to emergency braking events. Issue #4861
  - Fixed emergency deceleration after insertion when using IDM model. Issue #5040
  - Reduced emergency braking for the Wiedemann carFollowModel. Issue #3946
  - Vehicles approaching an occupied parkingArea where other vehicles wish to exit, now cooperate with the exiting vehicle instead of rerouting to another parkingArea. Issue #5128
  - Fixed error when loading a `<flow>` from saved simulation state. Issue #5131
  - Fixed crash when loading a `<flow>` from saved simulation state and vehroute-output is active Issue #5132
  - ParkingAreaReroute now takes subsequent stops into account when computing a new route. Issue #3659
  - ParkingAreaReroute now adapts subsequent person stages if the person returns to the same vehicle later. Issue #5164
  - Fixed a program crash potentially occurring when different car-following models were used in the same situation. Issue #5185
  - Fixed invalid route when adding trip with cyclical stops on the same edge. Issue #5206
  - Fixed invalid route and crashing when re-routing trip with cyclical stops. Issue #5205
  - Fixed freeze when setting `<vType>`-attribute *lcOpposite="0"* to disable opposite direction driving. Issue #5238
  - Person plans that start with a `<stop busStop="..."/>` and continue with a `<personTrip>` are now working. Issue #5298
  - Fixed invalid public transport routing when a `<personTrip>` is not the first item in the plan. Issue #5329
  - Simulation now termines if triggered vehicles are not inserted due to **--max-depart-delay** Issue #5420
  - Intersection behavior
    - Fixed routing bug where the cost of some left-turns was underestimated. Issue #4968
    - Fixed deadlock on intersection related to symmetrical left turns on multi-modal intersections. Issue #5004
    - Fixed invalid right-of-way behavior on multi-modal intersections Issue #5014
    - Fixed bug where vehicles would sometimes drive onto the intersection despite downstream jamming.
    - Fixed bugs where actuated traffic light would switch too early or too late. Issue #5161, Issue #5187 Issue #5119
    - Fixed deadlock at multi-lane roundabout Issue #5055
    - Fixed emergency deceleration when approaching a zipper node Issue #5080
    - Fixed collisions between vehicles and persons on walkingareas. Issue #3138, Issue #5423
  - Railway simulation
    - Update of averaged edge speeds within *device.rerouting* is now working correctly for bidirectional tracks. Issue #5060
    - Fixed behavior at *railSignal* when using *endOffset* for signal placement. Issue #5068
    - Fixed invalid behavior at *railSignal* when using bidirectional tracks (deadlocks / collisions). Issue #5069, Issue #5073, Issue #5062
    - Fixed detection of railway collisions. Issue #3686, Issue #5312
    - Railway insertion on bidirectional tracks now checks for presence of oncoming vehicles. Issue #5074

- SUMO-GUI
  - Fixed crash when using the 3D-View. Issue #4944
  - Pedestrian crossing traffic-light indices can be drawn again (regression in 1.1.0). Issue #4960
  - Loading breakpoints from a gui settings file using the settings dialog is now working. Issue #5015
  - Fix visual glitch when drawing rail carriages with exaggerated length. Issue #3907
  - Fixed invalid *pos* value in lane popup for lanes with a strong slope. Issue #5272
  - Stopped vehicles no longer have active brake signals.

- NETEDIT
  - *split junction* is now working reliably in intermodal networks. Issue #4999
  - Fixed crash when [copying edge template](NETEDIT.md#edge_template) with lane-specific attributes. Issue #5005
  - Fixed index of created lanes when adding restricted lanes with context menu. This is partly a regression fix and partly an improvement over the earlier behavior. Issue #5006
  - Inspection contour now works correctly for spread bidirectional rail edges. Issue #5064
  - Now showing correct edge length when using *endOffset*. Issue #5066
  - Defining e1Detector with negative position (counting backwards from the lane end) is now working Issue #4924
  - Setting connection attribute *uncontrolled* now takes effect. Issue #2599
  - Fixed crossing geometries after editing left-hand network. Issue #5265

- NETCONVERT
  - Custom node and crossing shapes are now correctly shifted when using options **--offset.x, offset.y**. Issue #4621
  - Fixed invalid right of way rules at traffic light intersection with uncontrolled connections. Issue #5047, Issue #5048
  - Connection attribute *uncontrolled* is no longer lost when saving as *plain-xml*. Issue #2956
  - Normal right-of-way rules are never used for unsignalized railway switches where all edges have the same *priority* value. (all links will have linkstate *M*). Issue #5061
  - Fixed bug where connections were not imported from OpenDRIVE networks if junction internal edges have lane sections with different lane numbers. Issue #4812
  - Fixed bugs where superfluous traffic light phase were generated. Issue #5175, Issue #5174
  - Fixed bug where generated traffic light phases had unnecessary red lights. Issue #5177
  - Fixed missing connection at roundabout when importing *dlr-navteq networks*. Issue #5169
  - Fixed bugs in connection to option **--tls.guess-signals** Issue #5179, Issue #5117
  - Nodes that were joined due to a `<join>` declaration are no longer joined with further nodes when option **junctions.join** is set.
  - Fixed invalid street names in dlr-navteq output. Issue #5204
  - Fixed invalid edge geometry when importing VISSIM networks Issue #5218
  - Fixed bug where joined traffic lights could get lost when importing a .net.xml file with option **--tls.discard-simple**. Issue #5229
  - Fixed bug where traffic lights could get lost when importing a .net.xml file containing joined traffic lights with option **--tls.join**. Issue #5229
  - Fixed low-radius connection shapes for left turns at large intersections Issue #5254
  - Joining junctions now preserves the prior connection topology. Issue #874, Issue #1126
  - Loading patch files with `<crossing>` elements is now working. Issue #5317
  - Fixed invalid turnaround-lane in multimodal networks Issue #5193
  - Patching attributes of existing connections is now working. Issue #2174
  - The default edge priorities for link-roads imported from OSM (e.g. on- and offRamps) have been changed to avoid invalid right-of-way rules at intersections. Issue #5382

- DUAROUTER
  - Fixed routing bug where the cost of some left-turns was underestimated. Issue #4968
  - vType attribute `speedDev="0"` is no longer lost in the output. Issue #5167
  - Person plans that start with a `<stop busStop="..."/>` and continue with a `<personTrip>` are now working. Issue #5298
  - Fixed invalid public transport routing when a `<personTrip>` is not the first item in the plan. Issue #5329

- DFROUTER
  - Fixed duplicate vehicle ids in generated output. Issue #5381

- TraCI
  - Fixed crash when calling moveToXY for a vehicle that is driving on the opposite direction lane. Issue #5029
  - Fixed *getParameter* and added *setParameter* calls to TraaS client library. Issue #5099
  - Fixed *vehicle.setStop* in TraaS client library. Issue #5105
  - Fixed crash when calling *simulation.getDistance* to compute the driving distance between unconnected parts of the network Issue #5114
  - Fixed invalid driving distance result when calling *simulation.getDistanceRoad* and one of the edges is an internal edges after an internal junction. Issue #5114
  - TraaS functions *Trafficlight.setPhaseDuraton, Trafficlight.getPhaseDuration, Trafficlight.getNextSwitch, Trafficlight.setCompleteRedYellowGreenDefinition, Vehicle.setAdaptedTraveltime, Vehicle.getAdaptedTraveltime, Lanearea.getLastStepHaltingNumber* and *Lanearea.getLastStepVehicleIDs* are now working (regression due to protocol change in 1.0.0).
  - Fixed C++ client function *vehicle.getStopState* (regression due to protocol change in 1.0.0)
  - Fixed estimation of speed and acceleration after using moveToXY. Issue #5250
  - *traci.vehicle.getLaneChangeState* now returns correct information while controlling the vehicle with moveToXY in sublane simulation. Issue #5255
  - Fixed invalid vehicle position after mapping with *vehicle.moveToXY* onto a lane with strong slope. Issue #5272
  - Fixed invalid lane change when moving to a new edge with different lane number after calling *traci.vehicle.changeLane*. Issue #5309

- Tools
  - restored python3 support for [osmWebWizard.py](Tools/Import/OSM.md#osmwebwizardpy) (regression in 1.1.0) Issue #5007
  - Fixed error when using sumolib functions *net.getNeighboringEdges* and *net.getNeighboringLanes* with the same net object. Issue #5246
  - [netdiff.py](Tools/Net.md#netdiffpy) now handles `<param>` and `<crossing>`-elements.Issue #5156, Issue #5317

### Enhancements

- Simulation
  - added new input element `<personFlow>` Issue #1515
  - lanechange-output now includes *maneuverDistance* when using the sublane model. Issue #4940
  - Traffic light phases now support the optional attribute *name*. This attribute can be set and retrieved via TraCI and makes it easier to establish the correspondence between phase indexing in SUMO and phase descriptions used by other traffic engineering methods Issue #4788
  - Time losses due to passing intersections from a non-priority road are now anticipated when routing even when no vehicle has driven there previously. The anticipated loss can be configured using the new option **--weights.minor-penalty** {{DT_FLOAT}} (new default 1.5, previous behavior when set to 0). Issue #2202.
  - Vehicles approaching a stop with *parking=true* now signal their intention by activating the blinker. When exiting from such a stop in dense traffic they signal their intention by activating the blinker. Issue #5126
  - Vehicles approaching a signalized junction now either stop at the given stop position (stopOffset) at red or they drive on to cross the junction if they already passed the stop position. Issue #5162
  - Person [rides](Specification/Persons.md#rides) now allow the special value *ANY* for the *lines* attribute. When this is set, persons may enter any vehicle that will stop at the destination *busStop* of the ride.
  - Option **--weights.random-factor** {{DT_FLOAT}} now also applies when routing persons. Issue #1353
  - Added output of harmonic mean speed to induction loops, which approximates the space mean speed. Issue #4919
  - Added controlability of reaction time (actionStepLength) to DriverState Model. Issue #5355
  - `<edgeData>`-output now supports attribute `detectPersons="walk"` which can be used to record pedestrian traffic instead of vehicular traffic. Issue #5252
  - Railway simulation improvements:
    - When a railSignal is placed on a short edge at the incoming network fringe, the signal will regulate train insertion for all trains longer than the insertion edge. Issue #5135
    - Trains can now reverse on bidirectional edges without the definition of a `<stop>`. Issue #5213
    - RailSignal logic no longer requires all railway switch nodes to be of type *railSignal*. Instead they can be guarded by other railSignal nodes along the incoming tracks. Issue #5091
    - Added new output option **--railsignal-block-output** {{DT_FILE}} to support analysis of the [generated signal block structure](Simulation/Output.md#additional_debugging_outputs). Issue #5109

- SUMO-GUI
  - Major improvement in rendering speed
  - Can [now load edge-data](SUMO-GUI.md#newer_versions) ([edgeData-output](Simulation/Output/Lane-_or_Edge-based_Traffic_Measures.md), [MAROUTER-output](MAROUTER.md#macroscopic_outputs.md), [randomTrips-weights](Tools/Trip.md#customized_weights)) for visualization (time-varying edge colors). Issue #4916
  - The current phase index can now optionally be shown for traffic-light controlled junctions. If a name was set for the current phase it is shown as well. Issue #4927
  - The current lane-changing state is now included in the vehicle parameter dialog. Issue #5038
  - Reloading is now disabled while running as TraCI-server. Issue #5052
  - When *show-detectors* is set for actuated traffic lights, the detector outline will switch to green for detectors that are used to control the active phase. Issue #5168
  - Added option **--tls.actuated.show-detectors** {{DT_BOOL}} to set the default for actuated detector visibility.
  - Vehicles with guiShape *truck/trailer* and *truck/semitrailer* now bend when cornering. Issue #3100
  - Added vehicle route visualisation *Show Future Route* to the vehicle context menu. This only shows the remaining portion of the route.
  - Persons riding in a vehicle now have distinct seat position. Issue #1628
  - Persons waiting at a busStop can now wait in multiple rows according the specified stop length and personCapacity.
  - Added person drawing style *circles*
  - Added openGL gui settings option *FPS* to enable a frames-per-second display. Issue #5253
  - Can now locate objects by their name (streetname or generic parameter key="name"). Issue #5270
  - Railway simulation improvements:
    - Improved visibility of railSignal-state when zoomed out and junction-exaggeration is active. Issue #5058
    - railSignal state indicator is now drawn with an offset to indicate the applicable track direction. Issue #5070
    - Junction shapes are now longer drawn for railway switches at default GUI settings. Issue #1655
    - Drawing bidirectional railways in *spread* style is now supported. Edge IDs are also drawn at an offset to improve readability.
    - The visual length of railway carriages and locomotive [can now be configured](Simulation/Railways.md#visualisation). Issue #1233

- NETCONVERT
  - Improved junction joining heuristic to prevent superfluous joins. Issue #4987
  - [OpenDrive road objects](Networks/Import/OpenDRIVE.md#road_objects) can now be imported Issue #4646
  - Road objects can now be embedded when [generating OpenDRIVE output](Networks/Further_Outputs.md#embedding_road_objects).
  - Attribute *endOffset* can now be used to move the signal position for bidirectional rail edges. Issue #5063
  - Minimum phase duration for actuated traffic lights now takes road speed into account to improve traffic light efficiency. Issue #5127
  - all `<node>`-attributes are now also supported within a `<join>` element to affect the joined node. Issue #1982
  - Various improvements to the generation of traffic light plans. Issue #5191, Issue #5192, Issue #5194, Issue #5196
  - If a custom ID was assigned to a traffic light (different from the junction ID), it will no longer be joined with other traffic lights when option **--tls.join** is set. Issue #5198
  - Lane widths are now imported from VISSIM networks. Issue #5216
  - Changed option default for **--opendrive.advance-stopline** to 0 to improve generated geometries in most cases.
  - Roundabouts can now be disabled and removed by setting the node type to *right_before_left* Issue #2225

- NETEDIT
  - Major improvement in rendering speed
  - Junction context menu function *split junctions* now restores original node ids Issue #4992
  - The new function *split junctions* and *reconnect* now recreates edges heuristically. Issue #4998
  - can now edit tls phase attributes *next* and *name*. Issue #4788
  - can now load additionals and shapes using -a "file.xml" or --additionals "file.xml". Issue #5049
  - Connection attributes *dir* and *state* can now be inspected and used as selection filter. Issue #3858
  - Can now locate objects by their name (streetname or generic parameter key="name"). Issue #5270
  - Roundabouts can now be removed by setting the node type to *right_before_left* Issue #2225

- DUAROUTER
  - added new input element `<personFlow>` Issue #1515
  - Added option **--write-trips** {{DT_BOOL}} to output trips instead of routes. This is useful for validating trip input that shall be routed during simulation. Issue #4971
  - Added option **--write-trips.geo** {{DT_BOOL}} to write trips with attributes *fromLonLat, toLonLat* instead of *from* and *to*
  - Reading trips with attributes *fromLonLat, toLonLat, fromXY, toXY, viaLonLat, viaXY* is now supported. Issue #2182
  - Time losses due to passing intersections from a non-priority road are now anticipated. The anticipated loss can be configured using the new option **--weights.minor-penalty** {{DT_FLOAT}} (new default 1.5, previous behavior when set to 0). Issue #2202.
  - Added option **--weights.random-factor** {{DT_FLOAT}} to randomize routing results with bounded deviation from optimality. Issue #1353

- TraCI
  - Added multi-client support to [TraaS](TraCI/TraaS.md) (*setOrder*) Issue #4957
  - *traci.vehicle.getNextTLS* now returns upcoming traffic lights for the whole route of the vehicle (before, only traffic lights until the first required lane change were returned). Issue #4974
  - Added functions *trafficlight.getPhaseName* and *trafficlight.getPhaseName* to all clients. Issue #4788
  - Extended the function *traci.vehicle.openGap()* to include an optional parameter specifying a reference vehicle. Issue #5151
  - Added function *traci.vehicle.getNeighbors()* and convenience wrappers (getLeftFollowers(), etc) to retrieve neighboring, lanechange relevant vehicles. Issue #5170
  - Added function *person.getSlope* to all clients. Issue #5171
  - Function *vehicle.changeLaneRelative(vehID, 0)* can now be used to a pin a vehicle to the current lane. Issue #5178

- Tools
  - [Generic parameters](Simulation/GenericParameters.md) of edges, lanes, junctions and traffic lights are now supported in [sumolib](Tools/Sumolib.md). Issue #4972
  - [randomTrips edge probabilities](Tools/Trip.md#edge_probabilities) can now be scaled by [Generic edge parameters](Simulation/GenericParameters.md). Issue #4963
  - Added new tool [tripinfoByTAZ.py](Tools/Output.md#tripinfobytazpy) for aggregating tripinfo attributes by origin/destination pair.
  - [netdiff.py](Tools/Net.md#netdiffpy) now supports the new option **--patch-on-import** to generate patch files which can be applied during initial network import. Issue #5293

- All Applications
  - The option **--help** now supports filtering by topic (e.g. **--help input**. Issue #5303)

### Other

- Simulation
  - The default [algorithm for averaging road speed to estimate
    rerouting travel
    times](Demand/Automatic_Routing.md#edge_weights) was
    changed from *exponential average (0.5)* to *moving average
    (180s)*. This is better suited for averaging traffic light
    cycles in urban road networks. To restore the old behavior, the
    option **--device.rerouting.adaptation-weight 0.5** can be used. Issue #4975

- NETCONVERT
  - Turn-arounds are no longer built at geometry-like nodes by
    default (when there is a node but no intersection so there would
    only be straight connections and turn-arounds). The old behavior
    can be restored by setting option **--no-turnarounds.geometry false**. Issue #5121
  - The default priorities for different railway edge types imported
    from OSM haven been changed so that all types use a different
    priority value.
  - The assigned edge speed for unrestricted motorways was lowered
    to 142km/h (from 300km/h) to better reflect real world speed
    distributions (together with the default speed factor
    distribution). Also, the default motorway speed was lowered from
    160km/h to 142km/h for the same reason. Issue #5434

- TraCI
  - TraCI Version is now 20
  - Function *trafficlight.setCompleteRedYellowGreeenDefinition* now
    expectes the 5th component *next* to be a list of integers when
    defining phases. Function
    '*'trafficlight.getCompleteRedYellowGreeenDefinition* now
    returns a list of integers as the 5th component (next) when
    defining phases. Custom clients must be patched to handle this.
  - Function *trafficlight.setCompleteRedYellowGreeenDefinition* now
    accepts *name* as an optional 6th component when defining
    phases. Function
    '*'trafficlight.getCompleteRedYellowGreeenDefinition* now
    returns *name* as the 6th component when defining phases. Custom
    clients must be patched to handle this.
  - TraaS function *Simulation.findRoute* now returns SumoStage
    object (containing all data returned by the TraCI API) instead
    of SumoStringList with just the edge IDs
  - TraaS function *Simulation.findRoute* now returns
    LinkedList<SumoStage\> (containing all data returned by the TraCI
    API) instead of LinkedList<SumoStringList\>

- Documentation
  - Added Tutorial for [Manhattan
    Mobility](Tutorials/Manhattan.md).
  - The [TraaS](TraCI/TraaS.md) library is now documented
    within the wiki.

## Version 1.1.0 (18.12.2018)

### Bugfixes

- Simulation
  - Fixed Bug where vehicles would stop moving after changing to the opposite direction lane. Issue #4610
  - Fixed [parkingAreaReroute](Simulation/Rerouter.md#rerouting_to_an_alternative_parking_area) to a destination which is too close for stopping. Issue #4612
  - Fixed crash when using multiple vehicles with device.bluelight. Issue #4629
  - Fixed crash when using carFollowModel ACC on multi-lane roads. (regression in 1.0.1)
  - Fixed crash when loading signal plan with invalid characters. Issue #4673
  - Fixed invalid halting count in E3 detector output Issue #4756
  - Fixed crashes related to vehicles that lane-change multiple times on the E3-detector entry edge. Issue #4803, Issue #4845
  - Fixed bugs that were causing unnecessarily strong deceleration. Issue #2310, Issue #4806, Issue #4809, Issue #4851, Issue #4855, Issue #4462, Issue #4900, Issue #4928, Issue #4930
  - Fixed deadlock caused by long vehicles being unable to re-enter traffic after parking in a short parkingArea. Issue #4832
  - Fixed invalid error when defining a stop on an edge and its consecutive internal edge. Issue #4837
  - Fixed bug where vehicles from minor roads would drive onto the intersection to aggressively. Issue #4854
  - Fixed bug where pedestrians would ignore prioritized vehicles when walking onto a crossing. Issue #4865
  - Fixed invalid upper bound on density in [edgeData-output](Simulation/Output/Lane-_or_Edge-based_Traffic_Measures.md#generated_output) for multi-lane edges. Issue #4904
  - Fixed bug where trains would fail to reverse on bidirectional rail edges. Issue #4921

- SUMO-GUI
  - Closing and re-opening an edge or lane no longer causes invalid lane choice. Issue #4514
  - Errors due to invalid `<param>` values are now shown in the message window. Issue #4753
  - Fixed invalid vehicle shapes when drawing trains as *simple shapes* in mesoscopic mode. Issue #4773

- NETCONVERT
  - Fixed bug that was causing invalid road geometry when using high-resolution input data at dense junction clusters. Issue #4648
  - Fixed bug when building networks for opposite-direction overtaking that was causing collisions in the simulation. Issue #4628
  - Fixed invalid right-of-way rules in left-hand networks. Issue #4256
  - Duplicate edge ids in shapefile import are no longer silently ignored. Instead, a warning is issued and ids are made unique with a numerical suffix.
  - Fixed crash when loading height data from multiple geotif files. Issue #4786
  - Option **--tls.guess** no longer builds traffic lights at almost every intersection. The default lane-speed-sum threshold (Option **--tls.guess.threshold**) was changed from 150km/h to 250km/h. Issue #1688
  - Options **--check-lane-foes.all**, **--check-lane-foes.roundabout** are no longer lost when re-importing a .net.xml file. Issue #4813
  - Fixed unnecessary right-of-way restrictions in some cases where incoming edges target distinct lanes on the target edge. Issue #4815
  - Fixed bug where custom walkingarea shapes where sometimes ignored. Issue #4847
  - Fixed bugs that could cause loss of elevation information. Issue #4877, Issue #4878
  - Fixed permissions on internal lanes after internal junctions. Issue #4912

- NETEDIT
  - Loading joined traffic light programs via menu is now working. Issue #4622
  - Menu option *Load Foreign* can now be used to import OSM files directly. Issue #4634
  - Fixed bug where *right-of-way* mode would show invalid properties. Issue #4637
  - TLS locator now lists traffic lights even if their id differs from the junction id. Issue #4661
  - Fixed crash when clicking buttons that should be disabled. Issue #4662
  - Fixed creation of unyielding connections via shift-click. Issue #4785 (regression in 1.0.1)
  - Fixed modifying elevation in Move-mode (regression in 1.0.0). Issue #4841

- TraCI
  - Fixed bug that was causing exaggerated output for *traci.vehicle.getDrivingDistance* when vehicles were driving on a looped route. Issue #4642
  - *traci.vehicle.getLaneChangeState* now returns correct information while controlling the vehicle with *moveToXY*. Issue #4545
  - Fixed retrieval of shape data with more than 255 points (*polygon.getShape, lane.getShape, junction.getShape* Issue #4707
  - Fixed bug when setting polygon shape with the C++ client.
  - Vehicle.isStopped is now working in TraaS. Issue #4883
  - Fixed adding vehicles after a simulation time of 24.8 days (python client). Issue #4920
  - Fixed bug where lateral collisions from unsafe calls to vehicle.changeSublane are prevented despite disabling safeguards via setLaneChangeMode. Issue #4864

### Enhancements

- Simulation
  - The eagerness for opposite-direction overtaking can now be
    configured with the new vType attribute *lcOpposite* (range
    \[0,inf\[). Issue #4608
  - `<personinfo>` elements now include attribute *type*. Issue #4695
  - The [ToC Device](ToC_Device.md) now provides an option
    for generating output. Issue #4750
  - Added warning messages for vehicles that perform emergency
    braking. The warning threshold can be configured with the new
    option **--emergencydecel.warning-threshold** {{DT_FLOAT}}. The given threshold is compared against the severity of
    the event (braking with emergencyDecel corresponds to 1 and
    braking with decel corresponds to 0). By default the threshold
    is set to 1. Issue #4792
  - Trains can now [reverse
    direction](Simulation/Railways.md#reversing_direction)
    anywhere on a bidirectional track.
  - TAZ can now be used as *via* destinations by using
    *<TAZID\>-sink* as edge id. Issue #4882
  - ParkingAreas now support the attribute *onRoad*. If set to
    *true*, vehicles will park on the road lane, thereby reducing
    road capacity. Issue #4895

- SUMO-GUI
  - Added `<poly>` attribute *lineWidth* to determine the drawing width of
    unfilled polygons. Issue #4715
  - TAZ now support attribute *color* for rendering the associated
    shape. This color can also be used in the new edge coloring mode
    *color by TAZ*. Issue #3979
  - Can now color streets according to custom lane or edge
    [parameters](Simulation/GenericParameters.md). Issue #4276
  - The numerical value for coloring edges/lanes, vehicles and
    persons (e.g. speed limit, acceleration, waitingTime) can now
    optionally be shown. When this option is set, the value will
    also be printed in the object tool tip. Likewise, the numerical
    value for coloring vehicles (e.g. acceleration) can optionally
    be shown. Issue #4840
  - Breakpoints can now be set via option **--breakpoints** {{DT_TIME}}**,**{{DT_TIME}}**...**. Issue #1957
  - Added visualization option to apply size exaggeration only to
    selected objects. Issue #3422
  - The text background color for IDs and object descriptions can
    now be configured. Issue #4894

- NETEDIT
  - New editing mode for TAZ (Traffic Analysis Zones). Issue #3425
  - Overlapping objects can now be inspected via consecutive clicks
    on the same location. Issue #2385
  - Shift-click can now be used to inspect, select and delete lanes.
  - LaneAreaDetectors (E2) that span multiple consecutive lanes can
    now be created within NETEDIT. Issue #2909
  - POIs can now be created from geo-coordinates in the clipboard
    (e.g. from online map sites) with a single click. Issue #4496
  - Selection colors can now be customized. Issue #4736
  - Can now color streets according to custom lane or edge
    [parameters](Simulation/GenericParameters.md). Issue #4276

- NETGENERATE
  - Added options **--perturb-x, --perturb-y, --perturb-z** to add random perturbations to all node
    coordinates. Perturbations can be specified as *mean* or capped
    normal distribution *normc(a,b,c,d)*. Issue #4776

- NETCONVERT
  - Added option **--tls.layout** {{DT_STR}} for selecting between layouts that group opposite
    directions ('opposites') and layouts that have one phase per
    incoming edge ('incoming'). Issue #4033
  - Added option **--tls.guess.threshold** {{DT_FLOAT}} to control the heuristic for guessing traffic
    lights. A traffic light is guessed if the sum of speeds of all
    incoming lanes exceeds the threshold value. Issue #4681
  - Added new node attribute `rightOfWay` to configure the algorithm for
    computing right-of-way rules (allowed values are *default*,
    *edgePriority*). Also added new option **--default.right-of-way** to set this value for
    all nodes in a network. Issue #4843
  - Importing internal lane shapes from OpenDRIVE is now supported
    using option **--opendrive.internal-shapes**. Issue #4331

- Tools
  - [netcheck.py](Tools/Net.md#netcheckpy) checks
    lane-based reachability and handles pedestrian infrastructure. Issue #4619
  - [route2poly.py](Tools/Visualization.md#intersection_flow_diagram)
    can now be used to generate intersection flow diagrams. Issue #4657
  - [tripinfoDiff.py](Tools/Output.md#tripinfodiffpy) now
    supports comparing `<personinfo>`s by setting option **--persons**.

- TraCI
  - [Context Subscriptions can now be equipped with
    filters](TraCI/Interfacing_TraCI_from_Python.md#context_subscription_filters)
    acting on the server side. Issue #4343
  - TraCI now offers the method
    [openGap()](TraCI/Change_Vehicle_State.md#open_gap_0x16)
    for temporary changes of the vehicle's desired headway. Issue #4172
  - Added functions *person.getPosition3D, person.getAngle,
    person.getLanePosition, persion.getColor, person.getLength* to
    the C++ client.

### Other

- Simulation
  - Simulation now runs about 17% faster compared to the previous
    release due to improvements in data structures and less
    (redundant) collision checking Issue #4917.
  - The default speedDeviation of vehicle classes
    *private,vip,hov,custom1* and *custom2* was changed from 0 to
    0.1. The default speed deviation for vehicle class *taxi* was
    changed from 0 to 0.05.
- Documentation
  - Updated [documentation for railway
    simulation](Simulation/Railways.md)
  - Updated [MAROUTER](MAROUTER.md#usage_description)
    documentation
- Miscellaneous
  - Network version is now 1.1
  - TraCI Version is now 19

- [sumolib](Tools/Sumolib.md) now uses an object with members
  *state, duration, minDur, maxDur, next* instead of tuple *(state,
  duration)* to represent traffic light phases.

## Version 1.0.1 (18.09.2018)

### Bugfixes

- Simulation
  - Fixed deadlock on parallel routing (regression in 1.0.0). Issue #4518
  - default emergency deceleration did not depend on the vClass but
    was always 9
  - Fixed bugs related to opposite-direction overtaking that were
    causing collisions and other invalid behavior. Issue #4550, Issue #3718, Issue #4564, Issue #4570, Issue #4588, Issue #4589, Issue #4591, Issue #4592
  - Fixed crash when using sublane model with varying lane widths. Issue #4547
  - Fixed hard braking after lane-changing when using carFollowModel
    *IDM* Issue #4517
  - Fixed exaggerated gap between standing vehicles when using
    carFollowModel *IDM*. Issue #4527
  - Fixed crash when using *device.ssm*. Issue #4556
  - Fixed invalid cooperative lane-changing attempts. Issue #4566
  - Fixed invald speed adaptation for lane changing. Issue #4563
  - Public transport routing now properly uses stops that are
    defined as child elements of a vehicle. Issue #4575

- SUMO-GUI
  - Fixed freeze when activating 'Show lane direction' for lanes
    with width=0. Issue #4533

- NETEDIT
  - Fixed freeze when activating 'Show lane direction' for lanes
    with width=0. Issue #4533
  - Custom endpoints are now honored when reversing an edge or
    adding a reverse direction edge via context menu. Issue #4501

- NETCONVERT
  - Networks for opposite direction driving now allow overtaking
    past junctions on straight connections that are slightly curved. Issue #4585

- OD2Trips
  - Flows with non-positive probability are no longer written. Issue #4600

- MESO
  - Fixed deadlock in public transport simulation. Issue #4560

- TraCI
  - Fixed bug when calling TraaS method
    *Simulation.getDistanceRoad*. Issue #4603
  - Closing and re-opening a lane no longer causes invalid lane
    choice. Issue #4514

### Enhancements

- Simulation
  - The collision-detection threshold configured via option **--collision.mingap-factor** can now
    be customized separately for each vType using attribute
    *collisionMinGapFactor*. Issue #4529
  - Increased running speed of simulations with *device.rerouting*
    using few vehicles in a large network. Issue #4598

- NETEDIT
  - Added option *reset custom shape* to the right-click menu of
    junction- and lane-selections Issue #4481, Issue #4490
  - When editing junction shapes, vertices can now be removed by
    shift-click. Issue #4494

- TraCI
  - added method *traci.edge.getStreetName* Issue #4557
  - method *simulation.getParameter* can now be used to [retrieve
    stopping place
    names](TraCI/Simulation_Value_Retrieval.md#generic_parameter_retrieval_0x7e). Issue #4558

- Tools
  - [cutRoutes.py](Tools/Routes.md#cutroutespy) now
    supports route references and stop access. Issue #4554, Issue #4595

### Other

- Simulation
  - Collision for vehicles controlled by carFollowModel *IDM* are
    only registered when less then half of the minGap distance
    remains between vehicles. This is done to compensate for the
    fact that the model does not guarantee the minGap distance (most
    of the time at least \~90% of the minGap are kept). The
    threshold can be customized via global option and vType
    attributes.

## Version 1.0.0 (04.09.2018)

### Bugfixes

- Simulation
  - All car-following models now respect the vType-attribute
    *emergencyDecel* as an absolute bound on deceleration. Issue #3556
  - Fixed some issues when using [continous lane
    change](Simulation/SublaneModel.md#simple_continous_lane-change_model). Issue #3767, Issue #3769, Issue #3770, Issue #3771, Issue #4364
  - Fixed back-and-forth changing when using [continous lane
    change](Simulation/SublaneModel.md#simple_continous_lane-change_model). Issue #4010
  - Fixed loading of teleporting vehicles from simulation state in
    *.sbx* format. Issue #3787
  - Fixed invalid vehicle counts by E2-detector related to
    lane-changing. Issue #3791
  - Fixed invalid vehicle counts by E3-detector related to re-using
    vehicle pointers Issue #3108, Issue #4079
  - Fixed bug that was causing invalid slowdown while passing an
    intersection. Issue #3861
  - Fixed bug that was causing pedestrians on looped routes to block
    themselves. Issue #3888
  - Vehicle speedFactor is now included in saved state. Issue #3881
  - Fixed invalid collision events when lanes are to narrow for the
    vehicles. Issue #3056
  - Fixed collision between pedestrians and vehicles. Issue #3964
  - Fixed bug where option **--random-depart-offset** would trigger invalid warnings regarding
    unsorted route file. Issue #4076
  - Fixed invalid stopping position after collision when using
    option **--collision.stoptime** Issue #4106
  - Fixed right-of-way rules for vehicles standing next to each
    other on the same lane and driving towards different edges. Issue #4071
  - Fixed crash within intersection between vehicles coming from the
    same lane. Issue #4100
  - Fixed invalid *departDelay* for triggered vehicles. Issue #4199
  - Fixed hang-up when scheduling a stop on internal edges after
    internal junctions Issue #4254
  - ParkingAreaReroute no longer triggers if the destination is not
    among the set of alternatives. Issue #4243
  - Fixed (near) infinite loop when specifying invalid speed
    distribution. Issue #4282
  - Fixed invalid car-following behavior at changing lane widths
    when using the sublane model. Issue #4223
  - Fixed crash when simulating invalid pedestrian routes with
    option **--ignore-route-errors**. Issue #4306
  - Option **--carfollow.model** is now working.
  - Car-follow models *IDM* and *IDMM* no longer fail to reach an
    exact stop position. Issue #658
  - Strategic look-ahead is now working as expected in networks with
    many short edges. Issue #4349
  - Switching a traffic light to the "off" definition now sets the
    correct right-of-way rules. Issue #1484
  - Fixed invalid deceleration at yellow traffic light. Issue #4450

- SUMO-GUI
  - width of railway edges is now taken into account when drawing
    (interpreted as gauge). Issue #3748
  - window-size and position now remain unchanged when reloading the
    simulation. Issue #3641
  - Random vehicle and person coloring is now more random on
    windows. Issue #3740
  - Vehicles that were not inserted (i.e. due to option **--scale** or **--max-depart-delay**) no
    longer count as *arrived* in the simulation parameter dialog.
    Instead the are counted under the new item *discarded vehicles*. Issue #3724
  - Fixed crashing due to concurrent access to vehicle numbers. Issue #3804
  - Fixed issues related to drawing smooth corners at curving roads. Issue #3840
  - Fixed vehicle positions when using the sublane model in lefthand
    networks. Issue #3923
  - Fixed crashing and visualization problems when using the
    3D-view. Issue #4037, Issue #4039
  - Fixed glitch where persons would appear to jump around while
    riding in a vehicle across an intersection. Issue #3673
  - Tracking of riding persons now centers on the person rather than
    the front its vehicle. Issue #4209
  - ChargingStation are visible again (regression in 0.32.0) Issue #4183
  - Fixed invalid vehicle blinkers in lefthand simulation. Issue #4258
  - Traffic lights now remain accessible (right-click, tooltip)
    after calling *traci.trafficlight.setRedYellowGreenStaate*. Issue #4426
  - Fixed invalid vehicle angle on lane with zero-length geometry.

- MESO
  - Fixed deadlock at roundabouts when running with option **--meso-junction-control.limited** or **--meso-junction-control false**. Issue #4074

- NETEDIT
  - Splitting edges, deleting individual edges and lanes or adding
    lanes via the *duplicate* menu option no longer resets
    connections and traffic light plans. Issue #3742
  - The viewing area and zoom loaded via option **--gui-settings-file** is no longer
    ignored
  - Fixed bug where connections on large junctions did not register
    clicks or were not drawn. Issue #3726
  - Fixed crash when removing inspected object via *undo*. Issue #3781
  - Fixed various bugs that led to re-computation of signal plans
    when modifying connections or tls indices. Issue #3742, Issue #3832
  - Fixed bug that could lead to the creation of invalid networks
    when adding and removing connections. Issue #3824, Issue #3813
  - Custom connection shape is now longer lost after
    *delete*+*undo*. Issue #3822
  - Moving geometry no longer removes z-data. Issue #3723
  - Splitting edges no longer introduces unnecessary custom
    endpoints. Issue #3895
  - Fixed inverted interpretation of *lanePosLat* for POIs compared
    to SUMO-GUI. Issue #4002
  - Fixed crash when deleting one of multiple signal programs for
    the same junction. Issue #4132
  - Added support for BusStops Acces. Issue #4018
  - Crossings no longer become invalid when splitting an edge. Issue #4295

- NETCONVERT
  - Option **--ramp.guess** no longer builds ramps at traffic light controlled
    nodes. Issue #3848
  - Fixed bug that was causing invalid link states at intermodal
    junctions. Issue #2944 Issue #3851
  - Fixed bugs that were causing invalid link directions. Issue #3852 Issue #3853
  - Fixed invalid junction logic in lefthand networks. Issue #3854
  - Generated signal plans will no longer have a protected left-turn
    phase if there is no dedicated left-turn lane. Issue #4087
  - Various fixes in regard to classifying nodes as type
    *rail_crossing* and joining clusters of rail crossing nodes.
  - Option **--dismiss-vclasses** is now working when loading a *.net.xml* file. Issue #4230
  - Fixed invalid junction shapes when using option **--junction.corner-detail**. Issue #4292
  - Fixed invalid intermodal junction logic that could cause
    collisions and deadlocks. Issue #4198, Issue #4252

- DUAROUTER
  - Fixed crash on intermodal routing. Issue #3883

- POLYCONVERT
  - Fixed bug when importing OSM data with objects that are marked
    as *deleted*. Issue #3786

- TraCI
  - Fixed bug where persons would "jump" when replacing the current
    walking stage with a new one. Issue #3744
  - Fixed crash when trying to access empty subscription result list
    using the C++ client.
  - Vehicle *emergencyDecel* is now at least as high as *decel*
    after calling *traci.vehicle.setDecel*. Issue #3755
  - Fixed python client bug in *traci.polygon.setShape*. Issue #3762
  - Adding a route with an empty list of edges now results in an
    error. Issue #3845
  - Vehicles that drive outside the road network under the control
    of *moveToXY* now properly updated their speed and brake lights. Issue #3837
  - Function *vehicle.getLaneChangeMode* now returns correct values
    for the original lane change state when the state is affected by
    *vehicle.setLaneChangeMode*. Issue #3810
  - Car-following related vehicle type parameters (e.g. *accel*)
    that are changed via traci are now correctly saved when saving
    simulation state. Issue #3522
  - Functions *simulation.findRoute* and
    *simulation.findIntermodalRoute* no longer crash sumo when
    trying to route from a forbidden edge. Issue #4121
  - Fixed invalid results when calling
    *simulation.findIntermodalRoute* Issue #4145, Issue #4147, Issue #4148
  - Multi-Lane E2-Detectors now return the correct length. Issue #4356
  - Fixed bug where *vehicle.couldChangeLane* returned *True*
    immediately after a lane change even though the requested target
    lane did not exist. Issue #4381
  - Fixed collisions when combining *vehicle.slowDown* with
    ballistic update. Issue #4418
  - Fixed python client bug in *traci.lane.getLinks* which returned
    always true for isOpen, hasFoe and hasPrio.
  - traci.person.setColor was fixed (was a noop before)

- Tools
  - [randomTrips.py](Tools/Trip.md#randomtripspy) now uses
    vType attributes from option **--trip-attributes** when generating persons.
  - Removed singularities at the distribution boundaries generated
    by
    [createVehTypeDistributions.py](Tools/Misc.md#createvehtypedistributionspy) Issue #4193


### Enhancements

- All applications
  - All time values in options and xml inputs can now be specified
    in the format *h:m:s* and *d:h:m:s* (where the values for days,
    hours and minutes are all positive integers and seconds may also
    be a positive floating point number).

    !!! note
        This does not apply to the python tools.
    
  - Added option **--human-readable-time** (short **-H**) that causes all time values to be written
    in h:m:s (or d:h:m:s) format.

- Simulation
  - Tripinfo-output now includes the attribute *stopTime* which
    records the time spent with intentional stopping. Issue #3693
  - A pedestrian crossings can now have different signal states for
    both directions. Issue #3820
  - FCD-output can now be switched on selectively for a subset of
    vehicles and the reporting period can be configured. Issue #1910
  - FCD-output can now be restricted to a subset of network edges
    loaded from a file with option **--fcd-output.filter-edges.input-file** {{DT_FILE}}. The file format is that of an
    edge selection as saved by [NETEDIT](NETEDIT.md).
  - Intended departure times (attribute *depart*) and intended
    vehicle id (attribute *intended*) are now added to
    vehroute-output of public transport rides. Issue #3948
  - Stopping place names are now added as XML-comments in
    vehroute-output of public transport rides. Issue #3952
  - Lane-Change-Model parameter *lcTurnAlignmentDistance* added for
    the control of dynamic alignment in simulations using the
    sublane model, see [Lane-Changing
    Models](Definition_of_Vehicles,_Vehicle_Types,_and_Routes.md#lane-changing_models), Issue #4025
  - Implemented [SSM
    Device](Simulation/Output/SSM_Device.md), which allows
    output of saftey related quantities. Issue #2669, Issue #4119
  - 'Smoothed' emergency braking Issue #4116
  - Added an initial version of a [driver state
    device](Driver_State.md) for modelling imperfect
    driving.
  - Added an initial version of a [transition of control
    model](ToC_Device.md).
  - Option **--ignore-route-errors** now also allows insertion of vehicles with unsafe
    headways. Issue #4118
  - Added a new car-following model "ACC" based on \[Milanés et al.
    "Cooperative Adaptive Cruise Control in Real Traffic
    Situations." IEEE Trans. Intelligent Transportation Systems 15.1
    (2014): 296-305.\]
  - [Statistic
    output](Simulation/Output.md#aggregated_traffic_measures)
    now also includes total delay by vehicles which could not be
    inserted by the end of the simulation if options **--duration-log.statistics** and **--tripinfo-output.write-unfinished** are set. Issue #4174
  - The default lane-changing model *LC2013* now supports [parameter
    *lcAssertive*](Definition_of_Vehicles,_Vehicle_Types,_and_Routes.md#lane-changing_models). Issue #4194
  - [Vehroute-output](Simulation/Output/VehRoutes.md) now
    includes the reason for rerouting. Issue #4204
  - Added optional attribute *visible* to `<parkingAreaReroute` (default *false*). This
    controls whether occupancy is known before reaching the
    parkingArea edge and can be used to model incomplete knowledge
    in parking reroute choice. Issue #4244
  - Rerouters now support the attribute *vTypes* which restricts
    their effect to vehicles from the given list of vehicle type
    IDs. Issue #4031
  - Vehicles are forming a coridor for emergency vehicles Issue #1967
  - Added option **--default.speeddev** {{DT_FLOAT}} to control the default speed deviation of all
    vehicle types that do not define it. Issue #4421
  - Added lane/edge-attribute stopOffset for defining vClass
    specific stopping positions in front of traffic lights. Issue #3754

- SUMO-GUI
  - Added control for scaling traffic demand dynamically. Issue #1951
  - Added option to disable drawing of bicycle lane markings on
    intersections.
  - All laneChangeModel-related vType-parameters and all
    junction-model related vType-parameters are now shown in the
    vType-Parameter dialog.
  - The simulation view can now be rotated via the new gui-settings
    attribute *angle* in the `<viewport>` or via the viewport dialog. Issue #3841
  - When drawing junction shapes with exaggerated size and setting
    option *show lane-to-lane connections*, the connecting lines are
    now scaled up at traffic light junctions. Issue #3796
  - The simulation state can now be saved via the *Simulation* Menu. Issue #2513
  - Object tracking can now be aborted via double-click.
  - Person plans can now be inspected via the right-click menu. Issue #3886
  - Object name rendering size can now be toggled between constant
    pixel size (all visible when zoomed out) and constant network
    size (invisible when zoomed out). Issue #3931
  - The *Delay* value is now invariant with regard to the simulation
    step length and always denotes delay per simulated second. Issue #4176
  - Added GUI Shape for different types of emergency vehicles
    (ambulance, police and firebrigade) Issue #1967

- MESO
  - Simulation of persons and public transport is now supported. Issue #3816

- NETEDIT
  - Added *Prohibition*-mode for checking right-of-way at junctions
    (hotkey 'w'). Thanks to Mirko Barthauer for the contribution. Issue #3850
  - Added virtual attribute to identify [bidirectional rail edge
    pairs](Simulation/Railways.md#bidirectional_track_usage). Issue #3720
  - Added option to modify the visualisation of [bidirectional rail
    edge
    pairs](Simulation/Railways.md#bidirectional_track_usage)
    (*spread superposed*) Issue #3720
  - Added button *add states* to *traffic light*-mode to complement
    the functionality of *cleanup states*. Issue #3846
  - Netedit now flags connection targets with incompatible
    permissions as *conflict* and only creates them with
    *<ctrl\>+<click\>*. Issue #3823
  - Traffic light indices are now drawn for pedestrian crossings
    when enabled via gui settings. Issue #3814
  - Now ParkingAreas and ParkingSpaces can be created in netedit. Issue #3104
  - When adding a new phase to a traffic light, the new phase will
    now have a plausible successor state depending on the selected
    previous phase (rather the being a copy). Issue #4041
  - Added function 'split' to junction context menu. This can be
    used to disaggregated joined junctions. Issue #4046
  - When joining traffic lights (by editing junction attribute
    'tl'), link indices of the target traffic light are now
    preserved if signal groups are used (multiple connections with
    the same *linkIndex* value). Issue #4094

    !!! caution
        The the signal states for the edited junction must be checked by the user

  - Width, height and diagonal size of a rectangle selection are now
    shown in the status bar. This can be used to measure distances.
  - Now Generic Parameters can be loaded, saved and edited. Issue #3485
  - Re-organized options in the options-dialog (F10). Issue #4420
  - Option help is now shown in the status bar of the options
    dialog. Issue #2900
  - Custom edge geometry endpoints can now be manipulated in *Move
    mode*. Issue #3716

- NETCONVERT
  - Geo-referenced networks (i.e. from OSM) can now be merged by loading them together (**-s net1.net.xml,net2.net.xml**). Issue #4085
  - Element `<crossing>` now supports attribute *linkIndex2* to specify a custom signal index for the reverse direction. Issue #3820
  - When defining [double connections](Networks/PlainXML.md#multiple_connections_from_the_same_edge_to_the_same_target_lane), the right-of-way rules now take the road topology into account to differentiate between on-ramp and off-ramp situations. Issue #3834
  - Importing VISUM networks up to format version 10 is now support. Issue #3898
  - Improved heuristics for options **--junctions.join**. Issue #876
  - Improved computation of intermodal junctions imported from OSM. Issue #4003
  - Improved computation of junction shapes.
  - Added option **--proj.rotate** {{DT_FLOAT}} for rotating the network.
  - Added option **--prefix** {{DT_STR}} which can be used to add a prefix to the written junction and edge IDs.
  - Added options **--tls.scramble.time --tls.crossing-clearance.time --tls.crossing-min.time** to control the timing of pedestrian crossing phases. Issue #4078
  - Added option **--tls.minor-left.max-speed** {{DT_FLOAT}} to ensure that left turns through oncoming traffic are not build for high-speed roads. Issue #4091
  - Sidewalk information is now imported from OSM for road types that have a positive sidewalkWidth attribute (e.g. by using typemap [osmNetconvertPedestrians.typ.xml](Networks/Import/OpenStreetMap.md#recommended_typemaps)). Issue #4096
  - Added option **--osm.stop-output.footway-access-factor** {{DT_FLOAT}} for increasing the length of stop access edges above the airline distance. Issue #4143.
  - Added option **--junctions.limit-turn-speed** {{DT_FLOAT}} which sets an upper bound on speed while passing an intersection based on the turning radius. To account for imperfect road geometry, the option **--junctions.limit-turn-speed.min-angle** can be used to avoid speed limits for small turns. Warnings are issued when the difference between road speed and connection speed rises above a configurable threshold (**--junctions.limit-turn-speed.warn.straight, --junctions.limit-turn-speed.warn.turn**). These warnings often indicate exaggerated road speeds on urban roads as well as faulty geometry. Issue #1141
  - Geo-reference information is now imported from OpenDRIVE input. Issue #4414

- NETGENERATE
  - Simplified node and edge names
    - The alphanumerical junction naming scheme now supports
      arbitrary grid sizes (using ids like *XY23*)
    - The alphanumerical junction naming scheme also extends to
      spider networks
    - The alphanumerical junction naming scheme is active by
      default (the option for enabling the old scheme was renamed
      from **grid.alphanumerical-ids** to **--alphanumerical-ids**).
    - When using alphanumerical junction ids, the intermediate
      string *to* is omitted from edge names because the edge ID
      already allows unambiguous determination of its junctions
      without it
  - Added option **--rand.random-lanenumber** to randomize lane numbers in random networks
    between 1 and *default.lanenumber*
  - Added option **--rand.random-priority** to edge priorities in random networks between 1
    and *default.priority*
  - Added option **--rand.grid** to place generated junctions on a regular grid
  - Added option **--prefix** {{DT_STR}} which can be used to add a prefix to the generated
    junction and edge IDs.
  - Corridor networks can now be generated by using options such as **-g --grid.x-number 3 --grid.y-number 1 --grid.attach-length 100**
  - Added options **--turn-lanes** {{DT_INT}} and **--turn-lanes.length** {{DT_FLOAT}} to add left-turn lanes to generated
    networks. Issue #3892

- DUAROUTER
  - Intended departure times (attribute *depart*) and intended
    vehicle id (attribute *intended*) are now added to
    vehroute-output of public transport rides. Issue #3948
  - Stopping place names are now added as XML-comments in
    route-output of public transport rides. Issue #3952

- POLYCONVERT
  - Option **--osm.use-name** now also applies to POIs. Issue #4246

- TraCI
  - [Libsumo](Libsumo.md) can now be used in place of the
    TraCI client libraries to increase execution speed of
    TraCI-Simulations by directly linking against
    [SUMO](SUMO.md) for all
    [SWIG](https://en.wikipedia.org/wiki/SWIG)-supported languages.
    Graphical simulation with libsumo is not yet supported.
  - function *vehicle.add* now supports using the empty string ("")
    as a route id to insert the vehicle on an arbitrary valid edge.
    This makes it easier to remote-control vehicles using moveToXY
    without defining an initial route. Issue #3722
  - added functions *simulation.getCollidingVehiclesNumber* and
    *simulation.getCollidingVehiclesIDList* to track collisions. Issue #3099
  - added functions *simulation.getEmergencyStoppingVehiclesNumber*
    and *simulation.getEmergencyStoppingVehiclesIDList* to track
    emergency stops. Issue #4312
  - added function *edge.getLaneNumber* to retrieve the number of
    lanes of an edge. Issue #3630
  - added function *vehicle.getAcceleration* to retrieve the
    acceleration in the previous step. Issue #4112
  - TraCI now allows to update a vehicle's best lanes
    (vehicle.updateBestLanes). Issue #4146
  - added function *gui.hasView* to determine whether graphical
    capabilities exist. Issue #4014
  - added function *lane.getFoes* to [to determine right-of-way and
    conflict relationships between incoming
    lanes](TraCI/Lane_Value_Retrieval.md#extended_retrieval_messages).
  - function *simulation.getMinExpectedNumber()* now includes
    persons that are still active in the simulation. Issue #3707
  - added function *traci.getLabel* to the python client to help
    working with multiple connections.
  - When adding a vehicle with a disconnected 2-edge route, it will
    be treated like a `<trip>` and rerouted automatically. Issue #4307
  - added functions *vehicle.getRoutingMode* and
    *vehicle.setRoutingMode*. When setting routing mode to
    *traci.constants.ROUTING_MODE_AGGREGATED*, [smoothed travel
    times](Demand/Automatic_Routing.md#edge_weights) are
    used instead of current travel times. Issue #3383

- Tools
  - added script
    [plot_trajectories.py](Tools/Visualization.md#plot_trajectoriespy)
    which allows plotting various combinations of time, distance,
    speed and acceleration for all trajectories in a given **--fcd-output** file.
  - added script
    [averageRuns.py](Tools/Misc.md#averagerunspy) for
    running a scenario multiple times with different random seeds
    and computing trip statistics
  - webWizard now correctly builds regions with left-hand traffic Issue #3928
  - Additional options for
    [generateTLSE3Detectors.py](Tools/Output.md#generatetlse3detectorspy)
    that make it suitable for generating various kind of junction
    related output. Issue #1622
  - [createVehTypeDistributions.py](Tools/Misc.md#createvehtypedistributionspy)
    now also allows to generate vehicle parameter distributions. Issue #4171
  - Added function simpla.createGap() Issue #4172
  - sumolib now supports lane.getWidth(). Issue #3842

### Other

- Miscellaneous
  - The space character ' ' is no longer allowed in xml option
    values when separating file names. Instead the comma ',' must be
    used. Files with space in their path are now supported. Issue #3817

- Documentation
  - Added [documentation on joined traffic lights and defining
    signal groups](Simulation/Traffic_Lights.md).
  - Added [documentation on the simple continous lane-change
    model](Simulation/SublaneModel.md#simple_continous_lane-change_model).
  - Documented all supported [routing
    algorithms](Simulation/Routing.md#routing_algorithms).

- Simulation
  - `<vTypeProbe>` is now deprecated in favour of FCD-output filtering
  - `<vaporizer>` is now deprecated in favour of calibrators.
  - The default arrival position for person walks is now the middle
    of the destination edge. This reduces the assymetry from
    arriving in forward or backward direction. Issue #3843
  - tripinfo-output attribute *waitSteps* which counts the number of
    simulation steps in which the vehicle was below a threshold
    speed of 0.1m/s was replaced by attribute *waitingTime* which
    measures the same time in seconds. This gives results which are
    independent of the **--step-length** simulation parameter. Issue #3749
  - The default value for option **--device.rerouting.pre-period** was changed from *1* to *60* to
    speed up simulation. Issue #3865
  - The default speed deviation has been changed to *0.1*. When
    defining a new `<vType>` with attribute *vClass*, this also influences
    the default speed deviation:
    - *truck*, *trailer*, *coach*, *delivery*: 0.05
    - *tram*, *rail*, *rail_electric*, *rail_urban*,
      *emergency*: 0
    - everything else 0.1

!!! note
    The previous behavior can be restored by setting option **--default.speeddev 0**

- SUMO-GUI
  - Default color for persons is now 'blue' to better distinguish
    them from vehicles.

- NETCONVERT
  - There is no longer an offset of 0.1m between lanes. This means
    the total visual width of an edge is now the sum of all lane
    widths. This also fixes an inconsistency between visualization
    and simulation as the vehicles always ignored this offset
    anyway. Road markings are now drawn on top of the lanes rather
    than between them. This causes small visual gaps in old networks
    (fixable by calling *netconvert -s old.net.xml -o new.net.xml*). Issue #3972
  - The default value for option **--default.junctions.radius** was increased from 1.5 to 4 in
    order to improve realism of turning angles.
  - The default value for option **--default.junctions.corner-detail** was increased from 0 to 5 to
    improve the visual realism of larger intersections.
  - Option **--ramps.guess** is now enabled by default
  - Vehicle speed while turning at intersections is now limited via
    the new option default **--limit-turn-speed 5.5**. To obtain the old behavior, this value
    can be set to -1.

- TraCI
  - TraCI Version is now 18
  - Embeded python is now deprecated in favour of
    [Libsumo](Libsumo.md).
  - TraCI now consistently represents time in seconds as a double
    precision floating point. This affects all client
    implementations. For convenience the affected python calls are
    listed below. Modifying your scripts should be necessary only
    for the methods printed in **bold**:
    - **traci.simulationStep**
    - traci.edge.getTravelTime
    - traci.edge.getEffort
    - traci.edge.adaptTravelTime
    - traci.edge.setEffort
    - traci.person.add
    - traci.vehicle.getAdaptedTravelTime
    - traci.vehicle.getEffort
    - traci.vehicle.adaptTravelTime
    - traci.vehicle.setEffort
    - **traci.vehicle.slowDown**
    - **traci.vehicle.changeLane**
    - **traci.vehicle.setStop**
    - traci.simulation.getCurrentTime still returns ms as int but
      has been deprecated in favor of traci.simulation.getTime
    - traci.simulation.findRoute
    - traci.simulation.findIntermodalRoute
    - **traci.simulation.getDeltaT**
    - **traci.trafficlight.getNextSwitch**
    - **traci.trafficlight.getPhaseDuration**
    - traci.trafficlight.getCompleteRedYellowGreenDefinition
    - **traci.trafficlight.setPhaseDuration**
    - traci.trafficlight.setCompleteRedYellowGreenDefinition
  - The TraCI boundary type has been replaced by a position list of
    2 positions
  - The TraCI float type has been removed
  - All protocol functions returning single bytes now return
    integers (lane.getLinkNumber, polygon.getFilled,
    vehicle.isRouteValid, vehicle.getStopState)
  - In the C++ client the function vehicle.getEdges was renamed to
    getRoute to be consistent with other clients
  - The order of error checks in the TraCI server changed so that
    unknown object errors show up before unknown variable / command
    errors
  - The python client
    - the function vehicle.add was replaced by the function
      addFull. The old "add" is still present and can be
      reactivated by monkey patching the module
      (traci.vehicle.add=traci.vehicle.addLegacy)
    - constants which were previously available in the single
      domains have moved to traci.constants
    - now returns tuples instead of lists for all getIDList and
      several other calls
    - the function *getSubscriptionResults* now requires the ID of
      the subscribed object as argument. To retrieve all results
      in a map, the function *getAllSubscriptionResults* can be
      used

## Older Versions

- [Z/Changes from Version 0.31.0 to Version
  0.32.0](Z/Changes_from_Version_0.31.0_to_Version_0.32.0.md)
- [Z/Changes from Version 0.30.0 to Version
  0.31.0](Z/Changes_from_Version_0.30.0_to_Version_0.31.0.md)
- [Z/Changes from Version 0.29.0 to Version
  0.30.0](Z/Changes_from_Version_0.29.0_to_Version_0.30.0.md)
- [Z/Changes from Version 0.28.0 to Version
  0.29.0](Z/Changes_from_Version_0.28.0_to_Version_0.29.0.md)
- [Z/Changes from Version 0.27.1 to Version
  0.28.0](Z/Changes_from_Version_0.27.1_to_Version_0.28.0.md)
- [Z/Changes from Version 0.27.0 to Version
  0.27.1](Z/Changes_from_Version_0.27.0_to_Version_0.27.1.md)
- [Z/Changes from Version 0.26.0 to Version
  0.27.0](Z/Changes_from_Version_0.26.0_to_Version_0.27.0.md)
- [Z/Changes from Version 0.25.0 to Version
  0.26.0](Z/Changes_from_Version_0.25.0_to_Version_0.26.0.md)
- [Z/Changes from Version 0.24.0 to Version
  0.25.0](Z/Changes_from_Version_0.24.0_to_Version_0.25.0.md)
- [Z/Changes from Version 0.23.0 to Version
  0.24.0](Z/Changes_from_Version_0.23.0_to_Version_0.24.0.md)
- [Z/Changes from Version 0.22.0 to Version
  0.23.0](Z/Changes_from_Version_0.22.0_to_Version_0.23.0.md)
- [Z/Changes from Version 0.21.0 to Version
  0.22.0](Z/Changes_from_Version_0.21.0_to_Version_0.22.0.md)
- [Z/Changes from Version 0.20.0 to Version
  0.21.0](Z/Changes_from_Version_0.20.0_to_Version_0.21.0.md)
- [Z/Changes from Version 0.19.0 to Version
  0.20.0](Z/Changes_from_Version_0.19.0_to_Version_0.20.0.md)
- [Z/Changes from Version 0.18.0 to Version
  0.19.0](Z/Changes_from_Version_0.18.0_to_Version_0.19.0.md)
- [Z/Changes from Version 0.17.1 to Version
  0.18.0](Z/Changes_from_Version_0.17.1_to_Version_0.18.0.md)
- [Z/Changes from Version 0.17.0 to Version
  0.17.1](Z/Changes_from_Version_0.17.0_to_Version_0.17.1.md)
- [Z/Changes from Version 0.16.0 to Version
  0.17.0](Z/Changes_from_Version_0.16.0_to_Version_0.17.0.md)
- [Z/Changes from Version 0.15.0 to Version
  0.16.0](Z/Changes_from_Version_0.15.0_to_Version_0.16.0.md)
- [Z/Changes from Version 0.14.0 to Version
  0.15.0](Z/Changes_from_Version_0.14.0_to_Version_0.15.0.md)
- [Z/Changes from Version 0.13.1 to Version
  0.14.0](Z/Changes_from_Version_0.13.1_to_Version_0.14.0.md)
- [Z/Changes from Version 0.13.0 to Version
  0.13.1](Z/Changes_from_Version_0.13.0_to_Version_0.13.1.md)
- [Z/Changes from Version 0.12.3 to Version
  0.13.0](Z/Changes_from_Version_0.12.3_to_Version_0.13.0.md)
- [Z/Changes from Version 0.12.2 to Version
  0.12.3](Z/Changes_from_Version_0.12.2_to_Version_0.12.3.md)
- [Z/Changes from Version 0.12.1 to Version
  0.12.2](Z/Changes_from_Version_0.12.1_to_Version_0.12.2.md)
- [Z/Changes from Version 0.12.0 to Version
  0.12.1](Z/Changes_from_Version_0.12.0_to_Version_0.12.1.md)
- [Z/Changes from Version 0.11.1 to Version
  0.12.0](Z/Changes_from_Version_0.11.1_to_Version_0.12.0.md)
- [Z/Changes from Version 0.11.0 to Version
  0.11.1](Z/Changes_from_Version_0.11.0_to_Version_0.11.1.md)
- [Z/Changes from Version 0.10.3 to Version
  0.11.0](Z/Changes_from_Version_0.10.3_to_Version_0.11.0.md)
- [Z/Changes from Version 0.10.2 to Version
  0.10.3](Z/Changes_from_Version_0.10.2_to_Version_0.10.3.md)
- [Z/Changes from Version 0.10.1 to Version
  0.10.2](Z/Changes_from_Version_0.10.1_to_Version_0.10.2.md)
- [Z/Changes from Version 0.10.0 to Version
  0.10.1](Z/Changes_from_Version_0.10.0_to_Version_0.10.1.md)
- [Z/Changes from Version 0.9.10 to Version
  0.10.0](Z/Changes_from_Version_0.9.10_to_Version_0.10.0.md)
- [Z/Changes from version 0.9.9 to version
  0.9.10](Z/Changes_from_version_0.9.9_to_version_0.9.10.md)
- [Z/Changes from version 0.9.8 to version
  0.9.9](Z/Changes_from_version_0.9.8_to_version_0.9.9.md)
- [Z/Changes from version 0.9.7 to version
  0.9.8](Z/Changes_from_version_0.9.7_to_version_0.9.8.md)
- [Z/Changes from version 0.9.6 to version
  0.9.7](Z/Changes_from_version_0.9.6_to_version_0.9.7.md)
- [Z/Changes from version 0.9.5 to version
  0.9.6](Z/Changes_from_version_0.9.5_to_version_0.9.6.md)
- [Z/Changes from version 0.9.3 to version
  0.9.4](Z/Changes_from_version_0.9.3_to_version_0.9.4.md)
- [Z/Changes from version 0.9.2 to version
  0.9.3](Z/Changes_from_version_0.9.2_to_version_0.9.3.md)
- [Z/Changes from version 0.9.1 to version
  0.9.2](Z/Changes_from_version_0.9.1_to_version_0.9.2.md)
- [Z/Changes from version 0.9.0 to version
  0.9.1](Z/Changes_from_version_0.9.0_to_version_0.9.1.md)
- [Z/Changes from version 0.8.x to version
  0.9.0](Z/Changes_from_version_0.8.x_to_version_0.9.0.md)
- [Z/Changes from version 0.8.2.2 to version
  0.8.3](Z/Changes_from_version_0.8.2.2_to_version_0.8.3.md)
- [Z/Changes from version 0.8.2.1 to version
  0.8.2.2](Z/Changes_from_version_0.8.2.1_to_version_0.8.2.2.md)
- [Z/Changes from version 0.8.0.2 to version
  0.8.2.1](Z/Changes_from_version_0.8.0.2_to_version_0.8.2.1.md)
- [Z/Changes from version 0.8.0.1 to version
  0.8.0.2](Z/Changes_from_version_0.8.0.1_to_version_0.8.0.2.md)
- [Z/Changes from version 0.8.0 to version
  0.8.0.1](Z/Changes_from_version_0.8.0_to_version_0.8.0.1.md)
- [Z/Changes from version 0.7.0 to version
  pre0.8](Z/Changes_from_version_0.7.0_to_version_pre0.8.md)
