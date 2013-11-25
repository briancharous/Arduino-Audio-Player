#include <SD.h>

#define buffersize 512 // biggest amount seems like 3209 

File audioFile;
byte buffer0[buffersize];
byte buffer1[buffersize];
boolean buffer;
int buffIndex;
boolean needMoreData;

const int chipSelect = 53; // 53 for mega

void setup() {

  Serial.begin(9600);

  DDRL = B11111111;

  PORTL = B00000000;

  if(!SD.begin(chipSelect)) {
    Serial.println("error initializing SD Card"); 
    return;
  }  

  audioFile = SD.open("s20.wav");

  if (audioFile) {
    audioFile.seek(44); // go to start of audio in wav file
    Serial.println("playing");
  }
  else {
    Serial.println("error opening"); 
  }

  buffer = false;
  buffIndex = 0;
  needMoreData = true;

  cli();
  // initialize timer 5
  TCCR5A = 0; // clear register A
  TCCR5B = 0; // clear register B
  TCNT5 = 0; // clear the timer counter register

  OCR5A = 800; // compare match register at 800 20kHz
  TCCR5B |= (1 << WGM12); // clear timer on compare match
  TCCR5B |= (1 << CS10); // no prescaler
  TIMSK5 |= (1 << OCIE5A); // enable timer compare interrupt
  sei();
}

ISR(TIMER5_COMPA_vect) { 
  byte play;
  
  // get byte out of correct buffer
  if (buffer) {
    play = buffer0[buffIndex];
  }
  else {
    play = buffer1[buffIndex];
  }
  
  // set pins to the byte
  PORTL = play; 
  buffIndex++;

  if (buffIndex == buffersize)  {
    needMoreData = true; 
    buffer = !buffer; 
    buffIndex = 0;
  }
}

void loop() {
  if(!audioFile.available()) {
    // no more audio, stop playing
    cli();
    PORTL = B00000000;
  }  
  if(needMoreData) {
    // if more data needs to be read, read into correct buffer
    needMoreData = false;
    if(buffer) {
      audioFile.read(buffer0, buffersize);
    }
    else {
      audioFile.read(buffer1, buffersize);
    }
  }
}









