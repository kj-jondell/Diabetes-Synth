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

b = SoundFile.collectIntoBuffers("/Users/kj/Documents/school/supercollider/projects/diabetes/samples/*") //No interpolation
b = SoundFile.collectIntoBuffers("/Users/kj/Documents/school/supercollider/projects/diabetes/samples/wavetable/*") //WAVETABLE interpolation
b = SoundFile.collectIntoBuffers("/Users/kj/Documents/school/supercollider/projects/diabetes/samples/wavetable2048/*") //WAVETABLE interpolation (2048 buffers)

//GRANULAR
(
    SynthDef.new(
        \diabetesgrain,
        {
            arg freq = 440, rel = 1.5, bufL = 1, t_trig = 1, bufR = 2, outL = 0, outR = 1;
            var sigL = BufGrain.ar(t_trig, rel, bufL, rate: freq/110); //Can only control rate(!)
            var sigR = BufGrain.ar(t_trig, rel, bufR, rate: freq/110*1.01); //Can only control rate(!)
            var env = EnvGen.kr(Env.perc(attackTime: 1, releaseTime:rel), doneAction: Done.freeSelf);
            Out.ar(outL,0.1*env*[sigL,sigR]);
        }
    ).add;
)
//WAVETABLE (interpolating)
//TODO: controlbus for detuning AND buffernr
//Vosc3 for detuning
//map velocity to amplitude and filter differently
(
    SynthDef.new(
        \diabetes,
        {
            arg freq = 440, velocity = 67, attack = 0.01, rel = 0.1, buf = 1, pan = 0;
            var sig = VOsc.ar(buf, freq:freq, mul: velocity/150);
            var env = EnvGen.kr(Env.perc(attackTime: attack, releaseTime:rel), doneAction: Done.freeSelf);
            var filter = LPF.ar((env*sig), freq*velocity.linlin(0,127,0.75,12));
            Out.ar(pan,filter);
        }
    ).add;
)

//DIABETES DRONE
x = Synth.new(\diabetes, [\attack,10,\rel,100,\velocity,60.rand+60,\pan,10.rand,\freq,154.1,\buf,29.0.rand])
x = Synth.new(\diabetes, [\attack,10,\rel,100,\velocity,111,\pan,1.rand,\freq,146,\buf,29.0.rand])

y = Synth.new(\diabetes, [\rel, 100, \freq, 220, \buf, 22])
y.set(\buf,28) //do same experiment but map to nanokontrol 

//TEST PBIND
(
    Pbindef.new(
        \player,
        \instrument, \diabetesgrain,
        \degree, Prand([1,2,3,4,5,6], repeats:inf),
        \octave, 4,
        \root, -2,
        \bufL, Prand([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14], inf), 
        \bufR, Prand([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14], inf), 
        \outL, Prand([0,1,2,3,4,5,6], inf),
        \scale, Scale.whole,
        \rel, Pwhite(5,10,inf),
        \dur, Pxrand([1/2,1/3,1/8]/1, repeats:inf)
    ).play(quant:4);
)


s.record;
s.stopRecording;
s.scope;

