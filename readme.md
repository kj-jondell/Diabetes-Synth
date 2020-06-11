# WHAT IS THIS?
A wavetable-synthesizer using blood glucose levels as periodic waveforms. Controllable using MIDI.

## Python
The Python code converts data taken from the Abbott FreeStyle Libre and converts into 2048-sample long periodic waveforms (in the form of wav-files). The data uses BSpline interpolation and Tukey-windowing. It also outputs the order of spectral centroids of the waveforms from low to high.

### Dependencies
* SciPy
* NumPy
* SoundFile
* xlrd
* PySide2 (Qt for Python)

### Notes
Diasend only allows exporting data from the last 202 days.

## SuperCollider
The SuperCollider code 

