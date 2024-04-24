# ece60022

This is the repository for Jason Lei's ECE 60022 final project. The directory is structured in the following way:

include/
- Header files defining module-specific data structures and behavior. 

lib/
- Data structures used throughout design.

src/
- Source code for each module. The primary driver file is driver.c.

test/
- Test and debugging code.

traces/
- Example tracefiles. Traces are organized in order of: flow ID, source user, flow size in packets, number of content IDs accessed by flow.

scripts/
- Important scripts, such as tracefile generation.

graphs/
- Collected data, and scripts to process data into graphs.

Use run_0.sh and run_1.sh as example to compile and run code.