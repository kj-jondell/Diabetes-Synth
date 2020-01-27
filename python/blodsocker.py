### TODO: Cleanup code

from dateutil import parser
import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
import soundfile as sf

def parseDate(string):
    return parser.parse(string)

times = []
values = []
with open('python/blodsocker.txt', 'r') as reader:
    lines = reader.readlines()
    amtData = len(lines)
    for line in lines:
        splitline = line.split()
        date = parseDate(splitline[0] + " " + splitline[1])
        values.append(float(splitline[2]))
        times.append(date.month*60*744+date.day*60*24+date.hour*60+date.minute)
    times = [int(x - times[0]) for x in times]
    tck = interpolate.splrep(times[0:1024], values[0:1024], k=1)
    xvalues = np.linspace(0,times[1024],times[1024])

    day = []
    for x in range(1024*2,1024*3):
        day.append(interpolate.splev(x,tck))
    day = [(x-min(day)) for x in day]
    day = [ 2*(x/max(day)-0.5) for x in day] #normalize and center sound
    #plt.plot(day)

    sf.write('out.wav', day, 48000)
    #plt.show()


