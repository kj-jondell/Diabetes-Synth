# WHAT IS THIS?
A wavetable-synthesizer using blood glucose levels as periodic waveforms. Controllable using MIDI.

## Python
The Python code converts data taken from the Abbott FreeStyle Libre and converts into 2048-sample long periodic waveforms (in the form of wav-files). The data uses BSpline interpolation and Tukey-windowing. It also outputs the order of spectral centroids of the waveforms from low to high.

![image](https://user-images.githubusercontent.com/30523857/84592134-968baf00-ae43-11ea-9e0a-e377a076c4a4.png | width = 320)

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

