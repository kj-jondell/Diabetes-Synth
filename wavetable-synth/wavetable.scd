/**
* TODO: - Write better comments
*       - Fix GUI
*           - Run through Python! With csv settings and osc commands to control SuperCollider 
*       - Make variable urls
* © Karl Johannes Jondell
*/

// -------------------- PARSE SETTINGS FILE -------------------- //
var settings = Dictionary.new;
var settings_file = CSVFileReader.read("settings");
var device, memsize, order, numframes, filename; //settings variable declaration

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

// -------------------- PREPARATIONS -------------------- //

Server.default.options.device = device;
Server.default.options.memSize = memsize;

MIDIClient.init;
MIDIIn.connectAll; //Choose mididevice...

Server.default.waitForBoot(
    {
        var buffers = Array.new(order.size);
        var envelop, keys;
        var detune = 1.5, releaseTime = 2, attackTime = 0.01; //Default values

        order.mirror.do({
            arg index, count;
            var url = File.getcwd++format(filename, index+1); //put samples somewhere else... 
            buffers.add(Buffer.read(s, path: url, numFrames: numframes)); 
        });

        s.freeAll;

        //Envelop...
        //BUF OSC AS CONTROLBUS...
        SynthDef.new(
            \DiabetesPanEnvelop,
            {
                arg freq = 440, velocity = 67, attackTime = 0.01, releaseTime = 0.1, pan = 0, sustainLevel = 0.6, detuneFactor = 1.0, buf = 1, outBus = 0, gate = 1;
                var freq_ = freq*BrownianWalk.kr(0.0002);
                var sig = VOsc3.ar(buf+LFTri.kr(0.04).unipolar(mul:velocity.linlin(0,127,1,15)), freq1:freq_, freq2:freq_*(1.0-(detuneFactor)), freq3:freq_*(1.0+(detuneFactor)), mul: Lag2.kr(velocity.linlin(0,127,-15,-6).dbamp));
                var filter = BLowPass4.ar(sig, freq*Lag2.kr(velocity.linlin(0,127,1.0,20.0)), Lag2.kr(velocity.linlin(0,127,1.0,2.0)));
                var env = filter*EnvGen.ar(Env.asr(attackTime: attackTime, sustainLevel: sustainLevel, releaseTime: releaseTime, curve: -4.0), gate: gate, doneAction: Done.freeSelf);
                var panned = Pan2.ar(env, pan);
                Out.ar(outBus, LeakDC.ar(panned));
            }
        ).add;

        s.sync;

        keys = nil!128;

        MIDIdef.noteOff(\diabetesOff,
            {
                arg vel,num;
                if(keys[num].notNil,
                {
                    keys[num].release;
                    keys[num] = nil;
                }
            );
        }, chan: 5
    ).add;

    MIDIdef.noteOn(\diabetesOn, 
        {
            arg vel,num;
            if(keys[num].isNil,
            {
                keys[num] = Synth.new(\DiabetesPanEnvelop, [\releaseTime, releaseTime, \attackTime, attackTime, \detuneFactor, detune, \buf, 28.0.rand, \velocity, vel, \freq, num.midicps, \pan, rrand(-0.25,0.25)]);
            });
        }, chan: 5
    ).add;

    MIDIdef.cc(\cc,
        {
            arg vel, num;
            switch(num,
                16, {
                    keys.do({
                        arg key;
                        detune = vel.linlin(0.0,127.0,0,1.5);
                        key.set(\detuneFactor, detune);
                    });
                },
                17, {
                    keys.do({
                        arg key;
                        releaseTime = vel.linlin(0.0,127.0, 1,5);
                        key.set(\releaseTime, releaseTime);
                    });
                },
                18, {
                    keys.do({
                        arg key;
                        attackTime = vel.linlin(0.0,127.0,0.001,2.5);
                        key.set(\attackTime, attackTime);
                    });
                }
            );

        }
    ).add;
});
