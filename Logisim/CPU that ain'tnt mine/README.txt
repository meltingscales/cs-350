Instructions to run CPU:

1) Make sure you have Logisim installed on your machine
Can be downloaded from here: http://sourceforge.net/projects/circuit/

2) Open 'CPUmain.circ' and make sure 'Simulation Enabled' is on under the 'Simulate' tab 
(or use CTRL + E to toggle simulation)

3) May need to reset the simulation before you start so press CTRL + R

Loading a program into ROM:

1) Click the Logisim ROM component located just above the pin labeled 'Counter reset'

2) In the left side panel the ROM menu should appear, click 'click to edit' in the 'contents' section

3) Instructions can be typed in by selecting the memory slot and using the keyboard (need to be typed as hexadecimal). To load pre-written programs click 'Open', browse to your program and select it. (Premade programs inside 'Programs' folder)

Note the last instruction in a program should always be the stop instruction (16 1's) in order to stop the control unit fetching extra instructions and overriding values

Loading a program into memory:

1) Click the 'Populate Memory Enable' pin along with the 'Disable Clock' pin to enable them

2) Enable ticks by pressing CTRL + K or selecting 'Ticks Enabled' in the 'Simulate' tab
(Suggestion - Alter tick frequency in the 'Simulate' tab to speed up the process)

3) Wait until memory is populated, contents can be viewed in the leftmost part of the circuit. 
(Memory will be populated when the counter, located under ROM, displays value '1f')

Running a program:
(Suggestion - Disable ticks before you start this process (CTRL + K). Not necessary but may help slow things down)
1) Firstly disable the 'Populate Memory Enable' pin by clicking it. Then disable the 'Disable Clock' pin in the same way. 
(Important to do in this order if ticks still enabled, order doesn't matter if ticks are disabled)

2) Then enable ticks in the same way as before and the program will run
(If ticks were kept enabled the program will start running as soon as 'Disable Clock' is disabled)