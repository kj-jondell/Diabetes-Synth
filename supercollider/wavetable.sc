s.boot;
s.reboot;
s.options.device = "Soundflower (64ch)"
MIDIClient.init;
MIDIIn.connectAll;

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
//Vosc3 for detuning (?)
//map velocity to amplitude and filter differently
//Line.kr? have some argument controlling end buf num?
(
    SynthDef.new(
        \diabetes,
        {
            arg freq = 440, velocity = 67, attack = 0.01, rel = 0.1, buf = 1, pan = 0;
            var sig = VOsc.ar(Lag2.kr(Line.kr(buf,0,rel),0.1), freq:freq, mul: Lag2.kr(velocity)/150);
            var env = EnvGen.kr(Env.perc(attackTime: attack, releaseTime:rel), doneAction: Done.freeSelf);
            var filter = LPF.ar((env*sig), freq*Lag2.kr(velocity.linlin(0,127,0.75,12)));
            Out.ar(pan,filter);
        }
    ).add;
)

//DIABETES DRONE
x = Synth.new(\diabetes, [\attack,10,\rel,100,\velocity,60.rand+60,\pan,10.rand,\freq,154.1,\buf,29.0.rand])

s.queryAllNodes();
s.addr
s.meter;

x.free;
y.free;
x = Synth.new(\diabetes, [\attack,0.1,\rel,5, \velocity,55,\pan,0,\freq, 100,\buf,20]);
y = Synth.new(\diabetes, [\attack, 10, \rel, 350, \freq, 80, \buf, 11+15.rand, \pan, 1])
y.set(\buf,10) //do same experiment but map to nanokontrol 
y.set(\velocity,50)
x.set(\velocity,80)

s.record;
s.stopRecording;
s.scope;
(
MIDIFunc.cc({|val,num|
        y.set(\buf,val.linlin(0,127,20,28)); //do same experiment but map to nanokontrol 
}, ccNum: 17);
MIDIFunc.cc({|val,num|
        x.set(\buf,val.linlin(0,127,15,25)); //do same experiment but map to nanokontrol 
}, ccNum: 16)
)

~buf = 0;
~notes = Array.newClear(128);
(
~cc = MIDIFunc.cc({|val,num|
    val.postln;
    ~buf=val.linlin(0,127,0,30);
})
)
~cc.free

(
~on = MIDIFunc.noteOn({|val,num|
    num.postln;
   ~notes[num] = Synth(\diabetes, [\pan, 5.rand, \freq, ((128-num)%64).midicps, \attack, 10, \rel, 100, \buf, ~buf]); 
})
)
MIDIdef.freeAll;
~on.free;
