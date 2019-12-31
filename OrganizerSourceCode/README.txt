--
lastsince
--

current problem
-doesnt record first letter of entry properly
--clicking last since -> clicking gear -> adding entry -> clicking last since btn
?? however same process but clicking gear icon again results in proper insertion
??? only the first last since click messes with insertion, reclicking gear icon and going into last since is fine
fixes
+ clicking on update button before adding entry fixes the no first character bug
+ 2nd and later entries will all enter fine
?+? sometimes the glitch doesnt show and is not reproducible




function to add insert new lastsince into mainView
function write to file
finish parsing tags into vector<string>
finish gear box to add through gui interface
finish tag group checkbox for tags



--
code improvements
--

change old functions to word_word_word
+typical cpp standard
-takes time for something i dont need

move buttoninteraction functions into its own lastsince classes
+easier to maintain+add onto code
+code is a lot more legible
-takes time

divide menubar and toolbar into their own classes?
+easier to mantain code
-unnecessary?

fix memory leak
+the right thing to do
+a learning experience
-win10 OS seems to take care and free memory somehow anyways //needs more careful observation


--
new features?
--
make loading bar at bottom to show file loading status
+cool
-useless for the most part

fanspeed


make a bar somewhere to show quote of the day
+cool
-takes space

give option to checkbox certain toolbar items
+customizable and allows easy modability
-feature bloat and increased filesize
-will require change from hardcoded values to malleable (use fl pack::horizontal())
--will require more ram to keep this loaded? or maybe extra cpu to load things in and out?