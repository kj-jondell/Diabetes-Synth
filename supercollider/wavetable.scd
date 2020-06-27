/**
* TODO: - Write better comments
*       - Make variable urls
*       - Revise camelCase / snake_case
*       - Handshake with Python
*       - 512 sample long waveforms are laggy...
* © Karl Johannes Jondell
*/

// -------------------- PARSE SETTINGS FILE -------------------- //
var settings = Dictionary.new;
var settings_file = CSVFileReader.read("settings"); // TODO path of file from thisProcess.argv instead (from python)
var osc_port = 1121; //TODO read from settings file (or argument when started)!
var osc_send = NetAddr.new("127.0.0.1", 1122);
var keys = nil!128;
var breakCommand = "startup finished";
var device, memsize, order, numframes, filename, samplerate, ports, mididevice = "IAC Driver Bus 1", outbus = 0, midichannel = 5; //settings variable declaration
var setDialParameter, getMidiDevice, readAndEvaluateSettings, bootFunction;
var detune = 1.5, bufferNum = 1, releaseTime = 2, attackTime = 0.01, sustainLevel = 0.8, decayTime = 0.8; //Default values

var dialList = ('/attack' : \attackTime, '/decay' : \decayTime, '/sustain' : \sustainLevel, '/detune_factor' : \detuneFactor, '/buffer_no' : \bufferNum, '/release' : \releaseTime); //Mapping OSC names to SynthDef arguments

var midiMatrix = (18 : '/attack', 20 : '/decay', 19 : '/sustain', 16 : '/detune_factor', 17 : '/release'); // connecting midi cc num to osc name

if(thisProcess.openUDPPort(osc_port) == false){
    0.exit;
};

MIDIClient.init;

readAndEvaluateSettings = {
    settings = Dictionary.new;
    settings_file = CSVFileReader.read("settings");

    if(settings_file.size == 0){ // exit program if settings file not found TODO defaults
        0.exit;
    };

    settings_file.do({
        arg value; 
        settings.put(value[0], value[1])
    });

    // TODO remove [1..] (or..? readability of settings file..)
    device = settings["device"][1..];
    mididevice = settings["mididevice"][1..];
    midichannel = settings["midichannel"][1..].asInteger-1;
    memsize = settings["memsize"][1..].asInteger;
    order = settings["order"][1..].split($ ).asInteger;
    numframes = settings["numframes"][1..].asInteger;
    filename = settings["filename"][1..];
    samplerate = settings["samplerate"][1..];
    ports = settings["port"][1..].split($-);
    outbus = ports[0].asInteger-1;

    // TODO Raise better error when using corrupt settings file!

    if(device == "Built-in Output"){
        Server.default.options.outDevice = device;
    } {
        Server.default.options.device = device;
    };

    Server.default.options.memSize = memsize;
    Server.default.options.sampleRate = samplerate;
    Server.default.options.numOutputBusChannels = ports[1].asInteger;

    MIDIIn.disconnectAll();
    MIDIIn.connect(0, getMidiDevice.value(mididevice));

};

// -------------------- SCALING FUNCTIONS -------------------- //

setDialParameter = {
    arg vel, parameter;
    var value, name = dialList[parameter];

    switch(
        parameter,
        '/attack', {
            attackTime = vel.linlin(0.0,127.0,0.001,2.5);
            value = attackTime;
        },
        '/decay', {
            decayTime = vel.linlin(0.0,127.0, 0.1, 1.5);
            value = decayTime; 
        },
        '/sustain', {
            sustainLevel = vel.linlin(0.0,127.0, 0.2, 1);
            value = sustainLevel; 
        },
        '/detune_factor', {
            detune = (vel.linlin(0.0,127.0,0,1.5)).snap(0.1,0.001,0.1);
            value = detune; 
        },
        '/buffer_no', {
            bufferNum = vel.linlin(0, 255, 1, order.size);
            value = bufferNum; 
        },
        '/release', {
            releaseTime = vel.linlin(0.0,127.0, 1,5);
            value = releaseTime; 
        }
    );

    keys.do({
        arg key;
        key.set(name, value);
    });

};

getMidiDevice = {
    arg chosenDevice;
    var returnVal;
    MIDIClient.sources.do({
        arg src;
        if((src.device+src.name).contains(chosenDevice)){returnVal = src;}
    });
    returnVal;
};

// Function accepting osc input from python-controller!
thisProcess.addOSCRecvFunc(
    { 
        arg msg, time, addr;
        if(dialList.keys.includes(msg[0])){
            setDialParameter.value(msg[1], msg[0]);
        }{
            if(msg[0] == '/reboot'){
                MIDIdef.freeAll;
                Server.default.freeAll;
                
                Server.default.quit({
                    readAndEvaluateSettings.value();
                    Server.default.waitForBoot(bootFunction);
                });
            }
        }
    };
);

bootFunction = {
    var buffers = Array.new(order.size);
    var envelop;

    breakCommand.postln;

    order.mirror.do({ // TODO mirror? setting?
        arg index, count;
        var url = format(filename, index); //put samples somewhere else... 
        buffers.add(Buffer.read(Server.default, path: url, numFrames: numframes)); 
    });

    Server.default.freeAll;

    //Envelop...
    //BUF OSC AS CONTROLBUS...
    //TODO fix names
    //TODO bufferum... (wrapping/offset)..
    SynthDef.new(
        \DiabetesPanEnvelop,
        {
            arg freq = 440, velocity = 67, attackTime = 0.01, releaseTime = 0.1, decayTime = 0.8, pan = 0, sustainLevel = 0.6, detuneFactor = 1.0, bufferNum = 1, outBus = 0, gate = 1;
            var freq_ = freq*BrownianWalk.kr(0.0002);
            var sig = VOsc3.ar(bufferNum+LFTri.kr(0.04).unipolar(mul:velocity.linlin(0,127,1,15)), freq1:freq_, freq2:freq_*(1.0-(detuneFactor)), freq3:freq_*(1.0+(detuneFactor)), mul: Lag2.kr(velocity.linlin(0,127,-15,-6).dbamp));
            var filter = BLowPass4.ar(sig, (freq*Lag2.kr(velocity.linlin(0,127,1.0,5.0))).clip(1,20000), Lag2.kr(velocity.linlin(0,127,1.0,2.0)));//TODO FIX blowpasss cutoff frequency!!
            var env = filter*EnvGen.ar(Env.adsr(attackTime: attackTime, decayTime: decayTime, sustainLevel: sustainLevel, releaseTime: releaseTime, curve: -4.0), gate: gate, doneAction: Done.freeSelf);
            var panned = Pan2.ar(env, pan);
            Out.ar(outBus, LeakDC.ar(panned));
        }
    ).add;

    Server.default.sync;

    MIDIdef.noteOff(\diabetesOff,
        {
            arg vel,num;
            if(keys[num].notNil,
            {
                keys[num].release;
                keys[num] = nil;
            });
        }, chan: midichannel
    );

    MIDIdef.noteOn(\diabetesOn, 
        {
            arg vel,num;
            if(keys[num].isNil,
            {
                keys[num] = Synth.new(\DiabetesPanEnvelop, [\outBus, outbus, \releaseTime, releaseTime, \attackTime, attackTime, \detuneFactor, detune, \bufferNum, bufferNum, \velocity, vel, \freq, num.midicps, \pan, rrand(-0.25,0.25)]);
            });
        }, chan: midichannel
    );

    MIDIdef.cc(\cc,
        {
            arg vel, num;
            var oscSymbol = midiMatrix[num];
            if (oscSymbol != nil){
                setDialParameter.value(vel, oscSymbol);
                osc_send.sendMsg(oscSymbol, vel);
            };
        }
    );

};

readAndEvaluateSettings.value();
Server.default.waitForBoot(bootFunction);
