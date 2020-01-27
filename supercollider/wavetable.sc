s.boot;
s.reboot;
s.options.device = "Soundflower (64ch)"

/*
*
* TODO: 1. ControlBus controll detuning
* 2. individual outs rather than stereo out
* 3. (Python) automate code to extract ALL bloodsugar samples. cleanup
* 4. general cleanup
* 5. create git
* 6. create nanokontrol/pad-control
*
* ® Karl Johannes Jondell
*/

(
    SynthDef.new(
        \diabetes,
        {
            arg freq = 440, rel = 0.1, bufL = 1, bufR = 2, outL = 0, outR = 1;
            var sigL = OscN.ar(bufL, freq);
            var sigR = OscN.ar(bufR, freq*1.01);
            var env = EnvGen.kr(Env.perc(releaseTime:rel), doneAction: Done.freeSelf);
            Out.ar(outL,env*[sigL,sigR]);
        }
    ).add;
)
x = Synth.new(\diabetes);

s.meter;
(
    Pbindef.new(
        \player,
        \instrument, \diabetes,
        \degree, Prand([1,2,3,4,5,6], repeats:inf),
        \octave, 3,
        \root, 9,
        \bufL, Pwrand([0,1,2], [3,2,3].normalizeSum, inf), 
        \bufR, Pwrand([2,3,4], [5,2,1].normalizeSum, inf), 
        \outL, Prand([0,1,2,3,4,5,6], inf),
        \scale, Scale.whole,
        \rel, Pwhite(1.4,3,inf),
        \dur, Pxrand([1/2,1/3,1/8]/0.8, repeats:inf)
    ).play;
)


s.record;
s.stopRecording;
s.scope;

b = SoundFile.collectIntoBuffers("/Users/kj/Documents/school/supercollider/experiments/samples/*")
b[0].plot;
