//instantiate a SinOscillator called 'osc'
SinOsc osc; // => dac;

//instantiate a reverb called 'r'
JCRev r;

//hook up the reverb to the dac (output)
//r => dac;

//set reverb value to .03
.03 => r.mix;

// create our OSC receiver
OscRecv recv;

// use port 4567
7400 => recv.port;

// start listening (launch thread)
recv.listen();

// create an address in the receiver, store in new variable
// expects message to come from /playtone address
// expects message to be a float (therefore f)
// when message is received, store contents to oe of type OscEvent
recv.event( "/playtone, f, s" ) @=> OscEvent oe;
//recv.event( "/playtone, f, string" ) @=> OscEvent oe;
//recv.event( "/playtone, string" ) @=> oe;


// infinite event loop
while ( true )
{
    // wait for event to arrive
    oe => now;
    
    // grab the next message from the queue. 
    while ( oe.nextMsg() != 0 )
    { 
        //create a float called velocity.
        float velocity;
        
        //reverb will be velocity, modified to work within reverb parameters
        float reverb;
        string color;
        
        // getFloat fetches the expected float (as indicated by "f")
        oe.getFloat() => velocity;
        velocity * 6 => reverb;
        reverb => r.mix;
        //<<< reverb >>>;
        
        oe.getString() => color; 
        <<< color >>> ;
        
        // set play pointer to beginning
        587 => osc.freq;
        //color => osc.freq;
        
        //hook up the oscillator to a reverb (start playing)
        r => dac;
        osc => r;
        
        //play for 200ms
        //when you can get xPossDiff in here, make ms = to xPosDiff
        //200::ms => now;
        200::ms => now;
        
        //disconnect the oscillator from reverb (stop playing)
        osc =< r;
        r =< dac;

    }
}

