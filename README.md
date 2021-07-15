# JoSIM
### Superconductor Circuit Simulator

##### Testing: v2.5.2 - Status: [![Build Status](https://joeydelp.visualstudio.com/JoSIM/_apis/build/status/JoSIM-CI-Devel?branchName=testing)](https://joeydelp.visualstudio.com/JoSIM/_build/latest?definitionId=1&branchName=testing)

##### Stable: v2.5.2 - Status: [![Build Status](https://joeydelp.visualstudio.com/JoSIM/_apis/build/status/JoeyDelp.JoSIM?branchName=master)](https://joeydelp.visualstudio.com/JoSIM/_build/latest?definitionId=3&branchName=master)

---

## Full Documentation

Please see full documentation here:
[JoSIM Documentation](https://joeydelp.github.io/JoSIM)

## Overview

JoSIM was developed under IARPA contract SuperTools(via the U.S. Army Research Office grant W911NF-17-1-0120).

JoSIM is a SPICE syntax circuit simulator specifically created to handle superconducting elements such as the Josephson junction. It reads in a standard SPICE deck, creates an A matrix and solves the linear algebra problem *Ax=b*. The linear algebra package KLU by [Tim Davis](http://faculty.cse.tamu.edu/davis/suitesparse.html) is used to solve the system of equations.

A two stage distribution platform is implemented in JoSIM with the first being a command line interface (CLI) binary that is always built as standard during compilation. This is accompanied by a C++ library (*libjosim*).

JoSIM allows output of results in various formats such as comma seperated value (CSV) or raw SPICE output. JoSIM has inherent support for .PARAM commands that allow components to have variable values as well as expression parsing. JoSIM implements the RCSJ model of the Josephson junction and only supports transient analysis at present.

JoSIM takes a *.cir* file as input and produces a *.dat/.csv* file as output.

Prebuilt binaries can be found here:

https://github.com/JoeyDelp/JoSIM/releases

Referencing:

[JoSIM — Superconductor SPICE Simulator](https://ieeexplore.ieee.org/document/8633946/)

---

## Changelog
### v2.5.3
- Fixed an issue with the pulse command producing incorrect pulses
- Fixed an issue where timesteps smaller than 10fs were causing out-of-range issues
- Various other bug fixes

### v2.5.2
- Fixed a major issue with relative paths not being correctly evaluated causing files to not be found even though the correct path was specified.

### v2.5.1 - 18/05/2021
- Added .spread command. See documentation for more details.
- Added .file command. See documentation for more detauls.
- Added .iv command. See documentation for more details.
- Fixed a bug where .neb value was not being read properly
- Fixed a bug where pulse source type was outputting incorrect number of pulses.
- Fixed a bug where custom waveform was not allowing cubic interpolation method.
- Code cleanup.

### v2.5 - 05/03/2021
- Phase mode is now the standard analysis mode. Produces the exact same results as voltage.
- Trapezoidal method has been replaced by superior 2nd order Gear method.
- JoSIM now internally determines the convention. (-c option removed)
- New progress indicators added. Spawns a seperate non blocking thread.
- New verbose modes. 1: Circuit statistics. 2: 1 + Parameters and evaluated values. 3: 2 + Complete expanded netlist.
- New DC source type. I.e. V01 1 0 DC 5V
- JJ has new IC= parameters that supersedes AREA= but ultimately performs similar model parameter scaling.
- Fixed NOISE source type to have syntax NOISE(AMP TD TSTEP)
- New '.temp' and '.neb' commands for noise thermal noise analysis
- Transient simulation command now allows PRSTART and PRSTEP commands. .tran TSTEP TSTOP PRSTART PSTEP
- Various memory improvements and overall speed-up
- Reduced output file size.
- Rewrote documentation: [JoSIM Documentation](https://joeydelp.github.io/JoSIM)
- Various bug fixes

### v2.4.1 - 09/03/2020
- Fixed an issue where "GND" was not recognized as ground, creating a new node causing matrix singularity.
- Fixed an issue with multiport devices (TX, CCCS, CCVS, VCCS, VCVS) not adding the 3rd and 4th node to the node map.
- Fixed an issue with multiport devices not expanding properly if part of subcircuits.
- Various other bug fixes

### v2.4 - 07/02/2020
- Complete rewrite of core internals
- Modularization of each component allowing easier implementation of integration methods
- Error reporting major improvment. Less random exits with no warnings. Better exits.
- Focus on object orientation for improved library access
- Speed up of matrix creation algorithm
- Improvements to output format
- Inclusion of '.include' control card
- Capability to read from standard in using '-i' command
- Various bug fixes

### v2.3 - 10/05/2019
- Major changes to JoSIM internal engine
- Removal of plotting engines in favor of external tools such as Python
- Introduction of pyJoSIM and libjosim packages
- Immense speed-up of internals
- Various bug fixes

### v2.2 - 12/02/2019
- Major rewrite of most of JoSIM code to accommodate library creation for use in C++ and Python
- Implementation of a phase source for use in phase-mode simulation

### v2.1 - 16/01/2019
- Inclusion of the Haberkorn junction current which allows for user switching between ballistic and normal electron tunneling
- Various bug fixes

### v2.0 - 10/10/2018
- New analysis method that allows Modified Nodal Phase Analysis (see ReadMe)
- Major memory improvement by not creating intermediate A matrix
- Various bug fixes

### v1.3 - 28/07/2018
- Addition of mutual inductance with the netlist label **K**
- Addition of the sinusoidal source (Voltage and Current)
- Various bug fixes
- AQFP now works

### v1.2 - 17/07/2018
- New Release brings RType=1 to the RCSJ model of the Josephson Junction.
- Various bugs have been fixed. 
- Data structures have been majorly altered.
- Other changes and fixes

### v1.1 - 04/06/2018
- Ability to change between plotting libraries (NONE, FLTK or Matplotlib [Default])
- Major bug fixes
- JoSIM now supports WRSpice syntax (mostly)
- Parameterization of component values with expression parsing
- Code cleanup and commenting

### v1.0 - 03/05/2018
- Initial release of JoSIM
- Default FLTK plotting engine
- Replaces JSIM as default simulator
