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
    s.options.device = "Soundflower (64ch)";
    MIDIClient.init;
    MIDIIn.connectAll;

    s.waitForBoot({
        var leftSynth, rightSynth;
        //order wavetable-buffers from low to hi spectral centroid frequency
        var order = #[17, 2, 19, 4, 9, 22, 13, 20, 23, 12, 24, 8, 18, 5, 0, 16, 7, 3, 10, 6, 1, 27, 14, 21, 26, 28, 11, 29, 25, 15]; //spectral centroid ordered from low to hi

        var buffers = Array.new(order.size);
        order.do({
            arg index, count;
            var url = format("/Users/kj/Documents/school/supercollider/projects/diabetes/samples/wavetable2048/blodsocker%.wav", index+1);
            buffers.add(Buffer.read(s, path: url, numFrames: 2048, bufnum: count));
        });

        ~oscProcessing = NetAddr.new("192.168.2.2", 7771);
        //WAVETABLE (interpolating)
        //TODO: controlbus for detuning AND buffernr
        //Vosc3 for detuning (?)
        //map velocity to amplitude and filter differently
        //Line.kr? have some argument controlling end buf num?
        //how should buf num be controlled? with a ControlBus?
        //Rel?? attack??
        SynthDef.new(
            \diabetes,
            {
                arg freq = 440, velocity = 67, detuneFactor = 1.0, buf = 1, pan = 0;
                var sig = VOsc3.ar(buf, freq1:freq, freq2:freq*(1.0-(detuneFactor*0.5)), freq3:freq*(1.0+(detuneFactor*0.5)), mul: Lag2.kr(velocity)/325+0.15);
                var filter = LPF.ar(sig, freq*Lag2.kr(velocity.linlin(0,127,0.75,12)));
                Out.ar(pan,filter);
        }).add;

        s.sync; //wait for Synthdef and buffers...

        leftSynth = Synth.new(\diabetes, [\velocity, 10, \pan, 0, \freq, 70,\buf, 5, \detuneFactor, 1.0]);
        rightSynth = Synth.new(\diabetes, [\velocity, 10, \freq, 80, \buf, 0, \pan, 1, \detuneFactor, 0.9]);

        MIDIFunc.cc({|val,num|
            ~oscProcessing.sendMsg("/terrainHeight", val);
            ~oscProcessing.sendMsg("/flySpeed", val.linlin(0,127,0,1));
            leftSynth.set(\freq,val.linexp(0,127,70,110)); 
            leftSynth.set(\buf,val.linlin(0,127,5,28)); 
            leftSynth.set(\velocity,val.linexp(0,127,10,120)); 
            leftSynth.set(\detuneFactor, val.linlin(0,127,1.0,0));

            rightSynth.set(\detuneFactor, val.linlin(0,127,0.9,0));
            rightSynth.set(\velocity,val.linexp(0,127,10,120)); 
            rightSynth.set(\freq,val.linexp(0,127,80,110)); 
            rightSynth.set(\buf,val.linlin(0,127,0,28)); 
        }, ccNum: 16);

        //Trigger decay and than free?
        MIDIFunc.noteOn({arg val, num;
           leftSynth.free;
           rightSynth.free;
           //TRIGGER OSC SHIFT OF SCENE
        }, noteNum: 32, chan: 1);
    });
)
/// SERVER FUNCTIONS
s.queryAllNodes();
s.addr
s.meter;
s.scope;
s.freeAll;

s.boot;
s.reboot;

s.record;
s.stopRecording;
s.scope;

OSCFunc.trace(false)
MIDIdef.freeAll;
