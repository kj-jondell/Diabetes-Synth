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
git clone https://github.com/kj-jondell/Diabetes-Synth.git ; cd Diabetes-Synth ; python3 Converter
```
3. To run the program after installation, simply cd into the python-directory and execute:
```
python3 Converter
``` 
4. MacOS binaries are also avaiable [here](https://github.com/kj-jondell/Diabetes-Synth/releases/tag/v0.1-alpha)


### Building
The Python code can be freezed and bundled using PyInstaller, with the following command:
```
pyinstaller Converter.spec
``` 

### Notes
Diasend only allows exporting data from the last 202 days.

## Wavetable Synthesizer
The Synthesizer implementation is mostly written in C++ and communicates with SuperCollider through Osc.

![image](https://user-images.githubusercontent.com/30523857/219492843-5869a226-cae0-48d1-afd9-320b5cb8602b.png)

### Dependencies
* SuperCollider (scsynth only)
* Qt5
* RtMidi and RtAudio

## Usage Examples 
This synth is used in the following songs:
* [sc-200322-231835](https://soundcloud.com/k-j-jondell/sc-200322-231835)

### NOTE
`scsynth` must be in $PATH! Add in `~/.bashrc` or `~/.bash_profile`. Or installed on Mac (normal installation located in Applications).
