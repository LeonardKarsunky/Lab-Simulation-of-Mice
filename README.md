    1) Project display

Program simulating a dynamic laboratory (the external environment) containing boxes, each containing cheese and at most one mouse.
The mice and the pieces of cheese belong to a super-class SimulatedEntity (simulated entity), which models them as disks, having a precise position (a vector of two coordinates) that can collide and interact with each other (e.g. a mouse can eat cheese). When the entities die, they disappear from the simulation.
 
Mice belong to an Animal class, so that we can potentially model other animals. An animal is characterized by: its velocity, its current state (eating, moving, ...), and its organ (here the liver) which plays a fundamental role in the project. It moves randomly around the box and, if it encounters a wall, changes its orientation. If it meets cheese and is hungry, it can eat it by increasing its energy level.
 
The pieces of cheese can be made to eat (and thus their amount of energy can be reduced).
 
It is necessary to "stalk" an animal to access its internal view.
The liver of the animal belongs to the class Organ (internal environment). Each organ has a blood network (consisting of an artery and capillaries) and is viewed as a two-dimensional set of strata. Each stratum allows to superpose up to three different cell types: blood cells, liver cells and extracellular matrix (ECM), which are subclasses of CellOrgan. The CellHandler class (the stratum) is used to access the different cells and the ECM.
Cells are characterized by: the substance it carries and the stratum to which it belongs. Liver cells are supported by ECM and irrigated by portions of the blood system. Also, there are exchanges of substances between the ECM, the liver cells and the blood system.
 
The Substance class represents in a synthetic way all the substances present at a given moment on a cell of the ECM: the fraction of VGEF, the fraction of glucose, and that of bromopyruvate (an inhibitor). This class makes it possible to do different operations between the substances conveyed.
 
There are two types of blood cells: those that form the capillaries and those that form the artery. Capillaries are formed randomly around the artery.
 
Liver cells are characterized by their level of ATP, and by the fact that they can divide. Their level of ATP may increase or decrease proportionally with the glucose fraction in the cell. They gain ATP through glycolysis and the Krebs cycle, and they lose it when they divide.
There is a subclass of liver cells: liver cancer cells. Cancer cells are distinguished from healthy liver cells by the fact that they use only glycolysis, and by the different conditions of division. Cancer cells can invade the entire organ if the glucose level is high enough but stop dividing if the bromopyruvate level is high enough.
 
 
    2) Compilation of the program
 
To compile / execute the general program (FinalApplication), it is necessary to launch the command: "scons application-run" from the directory containing the SConscript. We can also add --cfg = appSmall.json, --cfg = appSmall.json, --cfg = appDiff.json, --cfg = appOrgan1.json, --cfg = appOrgan2.json and --cfg = appTest. at the end to have different starting conditions.
 
 
    3) Use of the program
 
For the general program (to run with app.json or appSmall.json): appSmall.json launches the same thing as app.json but with a smaller window.
 
Make a reset: "R"
Pause the program: "Spacebar"
Update the program if changes have been made while the program is open: "C"
Adding the mouse to the cursor position: "M" key
To switch to debug mode: "D" key
Adding cheese at the cursor position: "F" key
Stalking an animal: put the cursor on the animal and press the "T" key
Stop tracking an animal: "Z" key
Switch to organ view: "O" key
Once in the view of the organ, to see the concentration of the current substance: hit "S"
To change the current substance: "N" key
To increase the rate of the current substance: "PageUp and 2"
To decrease the rate of the current substance: "PageDown and 3"
Add a cancer cell to the cursor position: "X" key
Switch to lab view: "L" key
For the LabTest, choose the number of boxes: keys "1" to "5"
 
    Here are the specific tests that are useful:
 
- scons LabTest-run (only for part 5 and before)
This allows to choose the number of boxes in the lab
 
- scons DrawOrganTest-run
This allows liver cells ("H" key) and blood cells ("B" key) to be placed at the selected point with the cursor.
 
- scans BloodSystemTest-run
The "+" and "-" keys on the numeric keypad allow to invoke generateCapillaryOneStep (once at each key press) from the point chosen with the cursor. The "0" key allows you to leave a capillary to grow a new one.
 
- scans DiffusionTest-run --cfg = appDiff.json (only for part 5)
By switching to the CONCENTRATION view using the "S" key, and after creating a CellBlood ("A" key for an artery cell and "B" for a capillary cell), glucose diffusion should be seen only by capillary cells.
 
- scons SubstControlTest-run --cfg = appDiff.json
By switching to the CONCENTRATION view by means of the "S" key and increasing the rate of the substance, the substance around the capillaries is diffused.
 
- LiverTest-run scones
For this test:
- Create a hepatic cell ('H' key) outside the blood network and in the immediate vicinity of the blood network (just above or above a 'capillary' cell, for example). The one outside dies after a while while the one that is "irrigated" by the blood network does not die.
- Observe the same phenomenon for cancer cells (X key to create cancer cells).
- Then increase the level of inhibitors using the control keys. Healthy liver cell should survive (thanks to Krebs cycle) and cancer cell die.
