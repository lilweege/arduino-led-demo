#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoSound.h>

#define NUM_LIGHTS 24
#define CHUNK_SIZE 8
#define OUTPUT_PIN 6
#define VOLUME 10

// can overflow
int htoi(char* p, int n) {
  int ans = 0;
  for (int i = 0; i < n; ++i, ++p) {
    ans <<= 4;
    ans += (*p < 'A') ? *p & 0xF : (*p & 0x7) + 9;
  }
  return ans;
}

#define NUM_LIGHT_FILES 2
const char* lightFilenames[] = {
  "fade.txt",
  "move.txt"
};
File textFiles[NUM_LIGHT_FILES];

#define NUM_SOUND_FILES 5
const char* soundFilenames[] = {
  "Visa.wav",
  "TTP.wav",
  "one.wav",
  "two.wav",
  "KrisMorton.wav"
};
SDWaveFile waveFiles[NUM_SOUND_FILES];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, OUTPUT_PIN, NEO_GRB + NEO_KHZ800);
void setup() {
//  Serial.begin(9600);
//  while (!Serial);
//  Serial.println("start");

  if (!SD.begin())
    while (1);
  for (int i = 0; i < NUM_LIGHT_FILES; ++i) {
    textFiles[i] = SD.open(lightFilenames[i]);
  }
  for (int i = 0; i < NUM_SOUND_FILES; ++i) {
    waveFiles[i] = SDWaveFile(soundFilenames[i]);
    bool b = waveFiles[i];
  }
  
  
  strip.begin();
  strip.show();
  AudioOutI2S.volume(VOLUME);
}

void playWave(SDWaveFile waveFile) {
  if (!waveFile)
    return;

  if (!AudioOutI2S.isPlaying() && AudioOutI2S.canPlay(waveFile))
    AudioOutI2S.play(waveFile);
}

char line[(1 + NUM_LIGHTS) * CHUNK_SIZE + 2];
void lightShow(File textFile) {
  if (!textFile)
    return;

  // set head position
  textFile.seek(0);
  
  // read file line by line
  while (textFile.available()) {
    // line must be shorter than set size
    textFile.read(line, sizeof(line));
    /* first chunk
       0: enable
       1 - 2: brightness
       3 - 6: duration
       7: blank space
    */
    bool verify = line[0] != '0' &&
                  line[0] != '#' &&
                  line[0] != '/';
    if (!verify)
      continue;

    int brightness = htoi(line + 1, 2);
    strip.setBrightness(brightness);

    int duration = htoi(line + 3, 4);

    for (int i = CHUNK_SIZE; i < (NUM_LIGHTS + 1) * CHUNK_SIZE; i += CHUNK_SIZE) {
      /* chunk breakdown
         each two chars represent hex code 0 - 255
         0: enable
         1 - 2: red
         3 - 4: blue
         5 - 6: green
         7: blank space
      */

      int light = i / CHUNK_SIZE - 1;
      bool en = line[i] != '0';
      if (en) {
        int r = htoi(line + i + 1, 2),
            g = htoi(line + i + 3, 2),
            b = htoi(line + i + 5, 2);
        strip.setPixelColor(light, r, g, b);
      }
      else {
        strip.setPixelColor(light, 0);
      }
    }
    strip.show();
    delay(duration);
  }
}

void wait(int t) {
  for (int i = 0; i < NUM_LIGHTS; ++i)
    strip.setPixelColor(i, 0);
  strip.show();
  delay(t);
}

//int currentSound = 0;
void loop() {
  for (const File textFile : textFiles) {
    lightShow(textFile);
    wait(1000);
  }

//  while (!waveFiles[currentSound]) {
//    currentSound = (currentSound + 1) % NUM_SOUND_FILES;
//  }
//  if (!AudioOutI2S.isPlaying() && AudioOutI2S.canPlay(waveFiles[currentSound])) {
//    AudioOutI2S.play(waveFiles[currentSound]);
//    currentSound = (currentSound + 1) % NUM_SOUND_FILES;
//  }
}
