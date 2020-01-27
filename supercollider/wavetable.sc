s.boot;
s.reboot;
s.options.device = "Soundflower (64ch)"

/*
*
* TODO: 1. ControlBus controll detuning
* 2. individual outs rather than stereo out
* 3. (Python) automate code to extract ALL bloodsugar samples.(DONE)
* 4. general cleanup
***** a) make synth into class.
* 5. create git
* 6. create nanokontrol/pad-control
* 7. Use interpolating Osc UGen (VOsc...) instead... (buffer -> signal -> asWavetable)?
* 8. use ambisonics instead...?
* 9. granular synth or wavetable???
*
* ® Karl Johannes Jondell
*/

(
    SynthDef.new(
        \diabetesgrain,
        {
            arg freq = 440, rel = 0.1, bufL = 1, t_trig = 1, bufR = 2, outL = 0, outR = 1;
            var sigL = BufGrain.ar(t_trig, 2, bufL, rate: 2.00); //Can only control rate(!)
            var sigR = BufGrain.ar(t_trig, 2, bufR, rate: 2.04); //Can only control rate(!)
            var env = EnvGen.kr(Env.perc(releaseTime:rel), doneAction: Done.freeSelf);
            Out.ar(outL,env*[sigL,sigR]);
        }
    ).add;
)
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
        \instrument, \diabetesgrain,
        \degree, Prand([1,2,3,4,5,6], repeats:inf),
        \octave, 3,
        \root, 0,
        \bufL, Prand([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14], inf), 
        \bufR, Prand([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14], inf), 
        \outL, Prand([0,1,2,3,4,5,6], inf),
        \scale, Scale.whole,
        \rel, Pwhite(2,5,inf),
        \dur, Pxrand([1/2,1/3,1/8]/2, repeats:inf)
    ).play;
)


s.record;
s.stopRecording;
s.scope;

b = SoundFile.collectIntoBuffers("/Users/kj/Documents/school/supercollider/experiments/samples/*")
b[0].plot;
