# Glucose Level Wavetable Synthesizer
A wavetable-synthesizer using blood glucose levels as periodic waveforms. Controllable using MIDI.

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
* python-osc
* sounddevice

### Running
1. First make sure all dependencies are installed (using pip):
```
pip3 install scipy numpy soundfile xlrd pyside2 python-dateutil python-osc sounddevice
```
More to come...
<!-- 2. Then execute the following command to install and run the program. 
```
git clone https://github.com/kj-jondell/Diabetes-Synth.git ; cd Diabetes-Synth/python/ ; python3 .
```
3. To run the program after installation, simply cd into the python-directory and execute:
```
python3 .
``` -->
<!-- ### Release page -->

### Notes
Diasend only allows exporting data from the last 202 days.

## Wavetable Synthesizer
More info coming soon...

### Dependencies
* sounddevice

### Running
1. First make sure all dependencies are installed (using pip):
```
pip3 install sounddevice
```

## Usage Examples 
This synth is used in the following songs:
* [sc-200322-231835](https://soundcloud.com/k-j-jondell/sc-200322-231835)

