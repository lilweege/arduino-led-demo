
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>

//#define NUM_LIGHTS 16
#define NUM_LIGHTS 24
#define CHUNK_SIZE 8
#define OUTPUT_PIN 6

// can overflow
int htoi(char* p, int n) {
  int ans = 0;
  for (int i = 0; i < n; ++i, ++p) {
    ans <<= 4;
    ans += (*p < 'A') ? *p & 0xF : (*p & 0x7) + 9;
  }
  return ans;
}


File openFile(String filename) {
//  Serial.begin(9600);
//  while (!Serial);
//  Serial.println("Serial connection established");
//  Serial.print("Looking for SD card... ");
  if (!SD.begin()) {
//    Serial.println("Couldn't find SD card.");
    return File();
  }
//  Serial.println("Found SD card.");
//  Serial.print("Looking for '" + filename + "'... ");
  if (!SD.exists(filename)) {
//    Serial.println("Couldn't find file.");
    return File();
  }
//  Serial.println("Found file.");
//  Serial.print("Opening '" + filename + "'... ");
  File file = SD.open(filename);
  if (!file) {
//    Serial.println("Couldn't open file.");
    return File();
  }
//  Serial.println("Opened file.");
  return file;
}


// will evaluate to false if not found
File file;
// first chunk + light chunks + nlcr
char line[(1 + NUM_LIGHTS) * CHUNK_SIZE + 2];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, OUTPUT_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.show();
  file = openFile("script.txt");
}

void loop() {
  if (!file)
    return;

  // reset read head position
  file.seek(0);
  
  // read entire file line by line
  while (file.available()) {
    // line must be shorter than predefined size
    file.read(line, sizeof(line));
    
    /* first chunk
     * 0: enable
     * 1 - 2: brightness
     * 3 - 6: duration
     * 7: blank space
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
       * each two chars represent hex code 0 - 255
       * 0: enable
       * 1 - 2: red
       * 3 - 4: blue
       * 5 - 6: green
       * 7: blank space
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



























void demo() {
  colorWipe(strip.Color(255, 0, 0), 50); // R
  colorWipe(strip.Color(0, 255, 0), 50); // G
  colorWipe(strip.Color(0, 0, 255), 50); // B
  theaterChase(strip.Color(127, 127, 127), 50); // W
  theaterChase(strip.Color(127,   0,   0), 50); // R
  theaterChase(strip.Color(  0,   0, 127), 50); // B
  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); ++i) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  for(int j = 0; j < 256; ++j) {
    for(int i = 0; i < strip.numPixels(); ++i) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  for(int j = 0; j < 256 * 5; ++j) { // 5 cycles of all colors on wheel
    for(int i = 0; i < strip.numPixels(); ++i) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; ++j) {  //do 10 cycles of chasing
    for (int q = 0; q < 3; ++q) {
      for (int i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, c);
      }
      strip.show();
      for (int i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, 0);
      }
      delay(wait);
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; ++q) {
      for (int i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, Wheel((i + j) % 255));
      }
      strip.show();
      for (int i = 0; i < strip.numPixels(); i += 3) {
        strip.setPixelColor(i + q, 0);
      }
      delay(wait);
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
