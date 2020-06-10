~isLoaded = False;
(

    s.options.device = "Soundflower (64ch)";
    s.options.memSize = 8192*4;
    MIDIClient.init;
    MIDIIn.connectAll;

    s.waitForBoot(
        {
            var order = #[17, 2, 19, 4, 9, 22, 13, 20, 23, 12, 24, 8, 18, 5, 0, 16, 7, 3, 10, 6, 1, 27, 14, 21, 26, 28, 11, 29, 25, 15]; //spectral centroid ordered from low to hi
            var buffers = Array.new(order.size);
            var envelop, keys;
            var detune = 1.5, releaseTime = 2, attackTime = 0.01;

            if(~isLoaded == False,
                {
                    order.mirror.do({
                        arg index, count;
                        var url = File.getcwd++format("/samples/wavetable2048/blodsocker%.wav", index+1); //put samples somewhere else...
                        buffers.add(Buffer.read(s, path: url, numFrames: 2048));
                    });
                    ~isLoaded = True;
                }
            );

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

        }
    );
)
~isLoaded = False;

s.boot;
s.meter
s.queryAllNodes();
3.octcps
{LFTri.kr(1).unipolar(mul:15);}.plot(1);
rrand(-1.0,1.0);
File.getcwd[..File.getcwd.size-9]++"/hello".postln
