## InterpretaꓭBle

---

### Running
Launch prediction.command script.
Launch the OpenFramework program then, click several times on the key 'o' of the keyboard to start the prediciton.

Place (X,Y) and resize (W,H) the drawing zone. Take care of opacity and thresold, for having a clear line of sight (no dark pixels on the drawing zone) and no auto-focusing all the time (change opacity for that).

After you setted up all of the parameters, launch the translating server with this command, executed at the root of the
project, using a terminal.


```
node Translations/translator/translate.js
```

Then, open *(CHROME ONLY)* the address:
```
localhost:9092
```
Choose the languages and the type of scenario then click on the the yellow button. 

Make sure the right microphone is connected to the web page.

You can now talk into the microphone, search for scenario and display all of that.

In case of bug, relaunch the OpenFramework program.

### Working

For the device to work, you must draw something on the drawing zone, for it to oper.

If what you draw continues switching between labels, remember that the Web site (localhost:9092) has a play/pause button to fix the scenario.
If you don't want to draw, you can select the scenario through the web site. 

In other case, you can talk for it to detect that you speech.

Therefore, it will translate what you've said in the language you specified.

### Training app : Running 

Launch the server of the app with this command, executed at the root of the project, using a terminal.

```
node Interpretabble_training/app.js
```
Connect a tablet with the IP adress of the computer and the port number  8090.

### Training app : Working

The user can draw, save and choose a label for its drawing.

The drawing will be analized to propose automatically a label. 

If a label doesn't exist yet a file is created in Translations/prediction/tf_files/drawings (where all the drawings are saved).

### Author list.

Chloé Angeloni (Designer teammate)

Elodie Lorthios (Designer teammate)

Maureen Troel (Designer teammate)

Marco Alghisi (Designer teammate)

Martial Geoffre-Rouland (Technician teammate)

Thomas Nicollet (Technician teammate)

Jacques Marques (Technician teammate)

### Special credits

ERASME Urban Lab for permitting us to achieve this projects

Martial Geoffre-Rouland for caming up with a B-plan 
