s.boot;
s.reboot;
s.options.device = "Soundflower (64ch)";
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

s.boot;

//order wavetable-buffers from low to hi spectral centroid frequency
(
    var order = #[17, 2, 19, 4, 9, 22, 13, 20, 23, 12, 24, 8, 18, 5, 0, 16, 7, 3, 10, 6, 1, 27, 14, 21, 26, 28, 11, 29, 25, 15]; //spectral centroid ordered from low to hi
    b = Array.new(order.size);
    order.do({
        arg index, count;
        var url = format("/Users/kj/Documents/school/supercollider/projects/diabetes/samples/wavetable2048/blodsocker%.wav", index+1);
        b.add(Buffer.read(s, path: url, numFrames: 2048, bufnum: count));
    });
    b;
)

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
//how should buf num be controlled? with a ControlBus?
(
    SynthDef.new(
        \diabetes,
        {
            arg freq = 440, velocity = 67, attack = 0.01, rel = 0.1, buf = 1, pan = 0;
            var sig = VOsc.ar(buf, freq:freq, mul: Lag2.kr(velocity)/150);
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

s.freeAll
x.free;
y.free;
x = Synth.new(\diabetes, [\attack,10,\rel,750, \velocity,80,\pan,0,\freq, 80,\buf,2]);
y = Synth.new(\diabetes, [\attack, 10, \rel, 750, \velocity,10,\freq, 80, \buf, 4.5, \pan, 1])
y.set(\buf,10) //do same experiment but map to nanokontrol 
y.set(\velocity,50)
x.set(\velocity,80)

s.record;
s.stopRecording;
s.scope;

p = NetAddr.new("127.0.0.1", 7771);

(
MIDIFunc.cc({|val,num|
    y.set(\velocity,val.linlin(0,127,10,120)); //do same experiment but map to nanokontrol 
    y.set(\freq,val.linexp(0,127,80,110)); //do same experiment but map to nanokontrol 
    y.set(\buf,val.linexp(0,127,0,29)); //do same experiment but map to nanokontrol 
}, ccNum: 17);
MIDIFunc.cc({|val,num|
    p.sendMsg("/test", val.linlin(0,127,0,6));
    x.set(\velocity,val.linlin(0,127,10,120)); //do same experiment but map to nanokontrol 
    x.set(\freq,val.linexp(0,127,80,110)); //do same experiment but map to nanokontrol 
    x.set(\buf,val.linexp(0,127,0,29)); //do same experiment but map to nanokontrol 
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
