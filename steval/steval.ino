/*
   march 7: 1 hour
   march 8: 4 hours
   march 9: 3 hours
*/


#include <Wire.h>

#define NUMBER_OF_DEVICES       3
#define NUMBER_OF_CHANNELS      12
#define DRIVER_BASE_ADDRESS     0x58

#define REG_DEV_ENABLE          0x01
#define REG_CHANNEL_CURRENT     0x09
#define REG_CHANNEL_ENABLE_HI   0x03
#define REG_CHANNEL_ENABLE_LO   0x02
#define REG_PWM_PATTERN_ZERO    0x1E

#define DEV_ENABLE              0x01
#define DEV_DISABLE             0x00
#define CHANNEL_ENABLE_ALL      0xFF
//#define DEFAULT_CHANNEL_CURRENT 0xFF
//#define DEFAULT_CHANNEL_CURRENT 0x27
#define DEFAULT_CHANNEL_CURRENT 0x10
//#define DEFAULT_CHANNEL_CURRENT 0x01


struct Color {
  int r, g, b;
};

struct Channel {
  byte number;
  byte current;
  byte patternZeroHi;
  byte patternZeroLo;
  void setPWM(int val) {
    if (0 <= val < 4096) {
      patternZeroHi = (val & 0x0F00) >> 8;
      patternZeroLo = val & 0x00FF;
    }
  }
};

struct Device {
  byte address;
  byte deviceEnable;
  byte channelEnableHi;
  byte channelEnableLo;
  Channel channels[NUMBER_OF_CHANNELS];
};


Device devices[NUMBER_OF_DEVICES];
void createDevices() {
  for (int device = 0; device < NUMBER_OF_DEVICES; device++) {
    devices[device].address = DRIVER_BASE_ADDRESS + device;
    devices[device].deviceEnable = REG_DEV_ENABLE;
    devices[device].channelEnableHi = CHANNEL_ENABLE_ALL;
    devices[device].channelEnableLo = CHANNEL_ENABLE_ALL;
    for (int channel = 0; channel < NUMBER_OF_CHANNELS; channel++) {
      devices[device].channels[channel].number = channel;
      devices[device].channels[channel].current = DEFAULT_CHANNEL_CURRENT;
    }
  }
}

void initDevices() {
  for (Device& device : devices) {
    writeData(device.address, REG_CHANNEL_ENABLE_HI, device.channelEnableHi);
    writeData(device.address, REG_CHANNEL_ENABLE_LO, device.channelEnableLo);
    for (Channel& channel : device.channels) {
      writeData(device.address, REG_CHANNEL_CURRENT + channel.number, channel.current);
    }
    writeData(device.address, REG_DEV_ENABLE, device.deviceEnable);
  }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  createDevices();
  initDevices();
}


const int minPWM = 0;
const int maxPWM = 4095 -     0;
const int a = (maxPWM - minPWM) / 2;
const int c = (maxPWM + minPWM) / 2;

Color HSVtoRGB(int h, double s, double v) {
  h = constrain(h, 0, 360);
  double r, g, b;
  int i = floor(h / 60.0);
  double f = h / 60.0 - i,
         pv = v * (1 - s),
         qv = v * (1 - s * f),
         tv = v * (1 - s * (1 - f));
  switch (i) {
    case 0:  r = v,  g = tv, b = pv; break;
    case 1:  r = qv, g = v,  b = pv; break;
    case 2:  r = pv, g = v,  b = tv; break;
    case 3:  r = pv, g = qv, b = v;  break;
    case 4:  r = tv, g = pv, b = v;  break;
    case 5:  r = v,  g = pv, b = qv; break;
    default: r = 0,  g = 0,  b = 0;  break;
  }
  return { (int)(r * maxPWM), (int)(g * maxPWM), (int)(b * maxPWM) };
}

void waving(int x) {
  for (int l = 0; l < 12; l++) {
    setLightRGB(l, HSVtoRGB((x - 25 * l) % 360, 1, 1));
  }
}

void pulsing(int x) {
  int v = a * sin(radians(x) - PI / 2) + c;
  for (int l = 0; l < 12; l++) {
    setLightRGB(l, v, v, v);
  }
}

void sparkling(int x) {
  for (int l = 0; l < 12; l++) {
    double v = a * sin(radians(x - PI * 48 * l)) + c;
    v = v < 0 ? 0 : v;
    setLightRGB(l, v);
  }
}

void crawling(int x) {
  for (int l = 0; l < 12; l++) {
    double v = a * sin(radians(x - PI * l)) + c;
    v = v < 0 ? 0 : v;
    setLightRGB(l, v);
  }
}

void rotating(int x) {
  for (int l = 0; l < 12; l++) {
    double v = maxPWM * sin(radians(x - PI * 24 * l));
    v = v < 0 ? 0 : v;
    setLightRGB(l, v);
  }
}


int tick = 0;

void pulseDelay() {
  tick = 0;
  while (a * sin(radians(tick) - PI / 2) + c > 5 || tick < 10) {
    pulsing(tick += 8);
    writeAllPWM();
  }
  
  for (int l = 0; l < 12; l++) {
    setLightRGB(l, 0);
  }
  writeAllPWM();
  delay(2000);
}

void loop() {
  waving(tick = tick % 32768 + 4);
  writeAllPWM();
}



int order(int x) {
  return x - 2 * (x % 4) + 3;
}

void setLightRGB(int lightNo, int v) {
  setLightRGB(lightNo, v, v, v);
}

void setLightRGB(int lightNo, Color col) {
  setLightRGB(lightNo, col.r, col.g, col.b);
}

void setLightRGB(int lightNo, int r, int g, int b) {
  if (0 <= lightNo < 12) {
    lightNo = order(lightNo) * 3;
    setChannelPWM(lightNo + 0, r);
    setChannelPWM(lightNo + 1, g);
    setChannelPWM(lightNo + 2, b);
  }
}

void setChannelPWM(int lightNo, int val) {
  if (0 <= lightNo < 36) {
    devices[lightNo / NUMBER_OF_CHANNELS].channels[lightNo % NUMBER_OF_CHANNELS].setPWM(val);
  }
}

void writeAllPWM() {
  for (Device& device : devices) {
    for (Channel& channel : device.channels) {
      writePWM(device, channel);
    }
  }
}

void writePWM(Device& device, Channel& channel) {
  writeData(device.address, REG_PWM_PATTERN_ZERO + 0x01 + (channel.number * 2), channel.patternZeroHi);
  writeData(device.address, REG_PWM_PATTERN_ZERO + 0x00 + (channel.number * 2), channel.patternZeroLo);
}

void writeData(byte deviceAddress, byte registerAddress, byte data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}
