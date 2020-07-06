(
SynthDef.new(
    \Diabetes,
    {
        arg freq = 440, velocity = 67, attackTime = 0.01, releaseTime = 0.1, decayTime = 0.8, pan = 0, sustainLevel = 0.6, detuneFactor = 1.0, bufferNum = 1, outBus = 0, gate = 1, flutter = 0.0002, orderSize = 10 ;
        var freq_ = freq*BrownianWalk.kr(flutter);
        var sig = VOsc3.ar((bufferNum+LFTri.kr(0.04).unipolar(mul:velocity.linlin(0,127,1,15))).clip(0, orderSize), freq1:freq_, freq2:freq_*(1.0-(detuneFactor)), freq3:freq_*(1.0+(detuneFactor)), mul: Lag2.kr(velocity.linlin(0,127,-15,-6).dbamp));
        var filter = BLowPass4.ar(sig, (freq*Lag2.kr(velocity.linlin(0,127,1.0,5.0))).clip(1,20000), Lag2.kr(velocity.linlin(0,127,1.0,2.0)));//TODO FIX blowpasss cutoff frequency!!
        var env = filter*EnvGen.ar(Env.adsr(attackTime: attackTime, decayTime: decayTime, sustainLevel: sustainLevel, releaseTime: releaseTime, curve: 0.0), gate: gate, doneAction: Done.freeSelf);
        var panned = Pan2.ar(env, pan);
        Out.ar(outBus, LeakDC.ar(panned));
    }
).writeDefFile;
)

(
var server = Server.new("Diabetes", NetAddr.new("127.0.0.1", 1234));
server.options.device = "Soundflower (64ch)";
server.boot;
server.waitForBoot({
    server.sendMsg("d_load", "Diabetes");
});
)
