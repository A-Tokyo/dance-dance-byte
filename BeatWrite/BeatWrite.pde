import processing.serial.*;
import ddf.minim.*;
import ddf.minim.analysis.*;
import cc.arduino.*;

Minim minim;
AudioPlayer song;
BeatDetect beat;
BeatListener bl;
boolean finished_playing = false;
long count =0;
Serial myPort;  // Create object from Serial class to make serial communication with arduino

float kickSize, snareSize, hatSize;

void setup() {
  size(512, 200, P3D);

  minim = new Minim(this);

  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port

  myPort = new Serial(this, portName, 9600); // u can use myPort.write(something) to send it to arduino
  delay(3000);
  song = minim.loadFile("complex.MP3", 2048);
  song.play();
  beat = new BeatDetect(song.bufferSize(), song.sampleRate());
  beat.setSensitivity(20); //in milli secs  
  kickSize = snareSize = hatSize = 16;
  // make a new beat listener, so that we won't miss any buffers for the analysis
  bl = new BeatListener(beat, song);  
  textFont(createFont("Helvetica", 16));
  textAlign(CENTER);
}

void draw() {
  if (song.isPlaying()) { 
    background(0);
    fill(255);
    if (beat.isKick()) {
      kickSize = 32;
    }
    if (beat.isSnare()) {
      snareSize = 32;
    }
    if (beat.isHat()) {
      hatSize = 32;
    }
    //---------------------------------------------------------------------------------------
    myPort.write('a'); //valid beat

    if (beat.isRange(1, 3, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }

    if (beat.isRange(3, 6, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }

    if (beat.isRange(6, 9, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }

    if (beat.isRange(9, 12, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }

    if (beat.isRange(12, 15, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }

    if (beat.isRange(15, 18, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }

    if (beat.isRange(18, 21, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }

    if (beat.isRange(21, 26, 1)) {
      myPort.write(1);
    } else {
      myPort.write(0);
    }
    //-----------------------------------------------------------------------------------
    print(beat.isRange(1, 3, 1) + " "+beat.isRange(3, 6, 1)+" "+beat.isRange(6, 9, 1)+" ");
    print(beat.isRange(9, 12, 1) + " "+beat.isRange(12, 15, 1)+" "+beat.isRange(15, 18, 1)+" ");
    print(beat.isRange(18, 21, 1) + " "+beat.isRange(21, 26, 1)+'\n');

    //GUI stuff
    textSize(kickSize);
    text("KICK", width/4, height/2);
    textSize(snareSize);
    text("SNARE", width/2, height/2);
    textSize(hatSize);
    text("HAT", 3*width/4, height/2);
    kickSize = constrain(kickSize * 0.95, 16, 32);
    snareSize = constrain(snareSize * 0.95, 16, 32);
    hatSize = constrain(hatSize * 0.95, 16, 32);
  } else if (!finished_playing) {
    print("processing is complete!");
    myPort.write('b'); //invalid beat >>>>> indicating processing is finished
    finished_playing = true;
  }
  delay(1000);
}

void stop() {
  song.close();
  minim.stop();
  super.stop();
}