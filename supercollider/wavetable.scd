/**
* TODO: - Write better comments
*       - Fix GUI
*           - Run through Python! With csv settings and osc commands to control SuperCollider 
*       - Make variable urls
*       - 512 sample long waveforms are laggy...
* © Karl Johannes Jondell
*/

// -------------------- PARSE SETTINGS FILE -------------------- //
var settings = Dictionary.new;
var settings_file = CSVFileReader.read("../settings"); // TODO path of file from thisProcess.argv instead (from python)
var osc_port = 1121;
var keys = nil!128;
var device, memsize, order, numframes, filename, samplerate; //settings variable declaration
var setReleaseTime, setAttackTime, setBufferNum, setDetuneFactor, setSustainLevel, setDecayTime;
var detune = 1.5, bufferNum = 1, releaseTime = 2, attackTime = 0.01, sustainLevel = 0.8, decayTime = 0.8; //Default values

if(settings_file.size == 0){ // exit program if settings file not found TODO defaults
    0.exit;
};

settings_file.do({
    arg value; 
    settings.put(value[0], value[1])
});

// TODO remove [1..] (or..? readability of settings file..)
device = settings["device"][1..];
memsize = settings["memsize"][1..].asInteger;
order = settings["order"][1..].split($ ).asInteger;
numframes = settings["numframes"][1..].asInteger;
filename = settings["filename"][1..];
samplerate = settings["samplerate"][1..];
// TODO Raise better error when using corrupt settings file!

setBufferNum = {
    arg vel = 0;
    keys.do({
        arg key;
        bufferNum = vel.linlin(0, 255, 1, order.size);
        key.set(\buf, bufferNum);
    });
};

setDecayTime = {
    arg vel = 0;
    keys.do({
        arg key;
        decayTime = vel.linlin(0.0,127.0, 0.1, 1.5);
        key.set(\decayTime, decayTime);
    });
};

setSustainLevel = {
    arg vel = 0;
    keys.do({
        arg key;
        sustainLevel = vel.linlin(0.0,127.0, 0.2, 1);
        key.set(\sustainLevel, sustainLevel);
    });
};

setAttackTime = {
    arg vel = 0;
    keys.do({
        arg key;
        attackTime = vel.linlin(0.0,127.0,0.001,2.5);
        key.set(\attackTime, attackTime);
    });
};

setDetuneFactor = {
    arg vel = 0;
    keys.do({
        arg key;
        detune = vel.linlin(0.0,127.0,0,1.5);
        key.set(\detuneFactor, detune);
    });
};

setReleaseTime = {
    arg vel = 0;
    keys.do({
        arg key;
        releaseTime = vel.linlin(0.0,127.0, 1,5);
        key.set(\releaseTime, releaseTime);
    });
};

// -------------------- PREPARATIONS -------------------- //

thisProcess.openUDPPort(osc_port);

Server.default.options.device = device;
Server.default.options.memSize = memsize;
Server.default.options.sampleRate = samplerate;

// [device,memsize,order,numframes,filename,samplerate].postln;

MIDIClient.init;
MIDIIn.connectAll; //Choose mididevice...

Server.default.waitForBoot(
    {
        var buffers = Array.new(order.size);
        var envelop;

        order.mirror.do({ // TODO mirror? setting?
            arg index, count;
            var url = format(filename, index); //put samples somewhere else... 
            buffers.add(Buffer.read(s, path: url, numFrames: numframes)); 
        });

        s.freeAll;

        //Envelop...
        //BUF OSC AS CONTROLBUS...
        //TODO fix names
        //TODO bufferum... (wrapping/offset)..
        SynthDef.new(
            \DiabetesPanEnvelop,
            {
                arg freq = 440, velocity = 67, attackTime = 0.01, releaseTime = 0.1, decayTime = 0.8, pan = 0, sustainLevel = 0.6, detuneFactor = 1.0, buf = 1, outBus = 0, gate = 1;
                var freq_ = freq*BrownianWalk.kr(0.0002);
                var sig = VOsc3.ar(buf+LFTri.kr(0.04).unipolar(mul:velocity.linlin(0,127,1,15)), freq1:freq_, freq2:freq_*(1.0-(detuneFactor)), freq3:freq_*(1.0+(detuneFactor)), mul: Lag2.kr(velocity.linlin(0,127,-15,-6).dbamp));
                var filter = BLowPass4.ar(sig, freq, Lag2.kr(velocity.linlin(0,127,1.0,2.0)));//TODO FIX blowpasss cutoff frequency!!
                var env = filter*EnvGen.ar(Env.adsr(attackTime: attackTime, decayTime: decayTime, sustainLevel: sustainLevel, releaseTime: releaseTime, curve: -4.0), gate: gate, doneAction: Done.freeSelf);
                var panned = Pan2.ar(env, pan);
                Out.ar(outBus, LeakDC.ar(panned));
            }
        ).add;

        s.sync;

        MIDIdef.noteOff(\diabetesOff,
            {
                arg vel,num;
                if(keys[num].notNil,
                {
                    keys[num].release;
                    keys[num] = nil;
                }
            );
        }, chan: 5 // TODO variable
    ).add;

    MIDIdef.noteOn(\diabetesOn, 
        {
            arg vel,num;
            if(keys[num].isNil,
            {
                keys[num] = Synth.new(\DiabetesPanEnvelop, [\releaseTime, releaseTime, \attackTime, attackTime, \detuneFactor, detune, \buf, bufferNum, \velocity, vel, \freq, num.midicps, \pan, rrand(-0.25,0.25)]);
            });
        }, chan: 5 // TODO variable
    ).add;

    // TODO make into cc matrix? 
    MIDIdef.cc(\cc,
        {
            arg vel, num;
            switch(num,
                16, {
                    setDetuneFactor.value(vel);
                },
                17, {
                    setReleaseTime.value(vel);
                },
                18, {
                    setAttackTime.value(vel);
                },
                19, {
                    setDecayTime.value(vel);
                },
                20, {
                    setSustainLevel.value(vel);
                }
            );

        }
    ).add;

    // Function accepting osc input from python-controller!
    thisProcess.addOSCRecvFunc(
        { |msg, time, addr|
            switch(
                msg[0],
                '/attack', {setAttackTime.value(msg[1]);},
                '/decay', {setDecayTime.value(msg[1]);},
                '/sustain', {setSustainLevel.value(msg[1]);},
                '/detune_factor', {setDetuneFactor.value(msg[1]);},
                '/buffer_no', {setBufferNum.value(msg[1]);},
                '/release', {setReleaseTime.value(msg[1]);}
            );
        };
    );

});
