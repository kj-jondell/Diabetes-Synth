# Glucose Level Wavetable Synthesizer
A wavetable-synthesizer using blood glucose levels as periodic waveforms. Controllable using MIDI.
The program is divided into a converter -- converting the glucose level data into wavetable samples -- and a controller (interfacing the underlying SuperCollider synth).

## Glucose Level Data Converter
### Usable data
The program expects the usage of datasheets taken from [Diasend](https://www.diasend.com/) and has only been tested with the Abbott FreeStyle Libre (but should be usable with other CGMs and FGMs supported by Diasend, such as Dexcom G6). The file used by the program can be downloaded from the page "Share Data" and "Export Data" in Diasend. The automatically downloaded (.xls) file can be opened straight from the program ("Choose file").

### GUI program
The program uses data taken from [Diasend](https://www.diasend.com/) and converts into periodic waveforms (in the form of wav-files). The data uses BSpline interpolation and selectable windowing. It is possible to output the samples straight into the SuperCollider [wavetable format](https://doc.sccode.org/Classes/Wavetable.html). It also outputs the information regarding spectral centroids of the waveforms.

![image](https://user-images.githubusercontent.com/30523857/84710875-5d4b6000-af65-11ea-9305-9722ac31d660.png)

### Dependencies
* SciPy
* NumPy
* SoundFile
* dateutil
* xlrd
* PySide2 (Qt for Python)

### Running
1. First make sure all dependencies are installed (using pip):
```
pip3 install scipy numpy soundfile xlrd pyside2 python-dateutil 
```
2. Then execute the following command to install and run the program. 
```
git clone https://github.com/kj-jondell/Diabetes-Synth.git ; cd Diabetes-Synth ; python3 -m python
```
3. To run the program after installation, simply cd into the python-directory and execute:
```
python3 -m python <controller | converter | synth>
``` 
<!-- ### Release page -->

### Notes
Diasend only allows exporting data from the last 202 days.

## Wavetable Synthesizer
More info coming soon...

### Dependencies
* sounddevice
* python-osc
* rtmidi

### Running
1. First make sure all dependencies are installed (using pip):
```
pip3 install sounddevice python-osc python-rtmidi
```

## Usage Examples 
This synth is used in the following songs:
* [sc-200322-231835](https://soundcloud.com/k-j-jondell/sc-200322-231835)

