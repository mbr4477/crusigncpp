#include <Arduino.h>
#include <FastLED.h>
#include "FunctionMap.h"
#include "logger.h"
#include <string>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "color.h"

#define BLE_DEVICE_NAME "Cru LED Sign"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define MODE_CHAR_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DELAY_CHAR_UUID "c0ffabca-c385-4bee-b42f-1e23a4baaa0e"
#define DOCSTR_CHAR_UUID "45a5a8af-c4c0-4c6b-9915-6cf0716ff7ac"

#define NUM_LEDS 144
#define LED_PIN 5

CRGB leds[144];
FunctionMap modes;
uint16_t delay_ms = 30;
std::string activeFunction = "";

CRGB alternate(uint8_t x, uint8_t i, bool &reset, std::vector<std::string> args)
{
  if ((x / atoi(args[2].c_str()) + i) % 2)
  {
    return str2color(args[0]);
  }
  return str2color(args[1]);
}

CRGB trace(uint8_t x, uint8_t i, bool &reset, std::vector<std::string> args)
{
  if (i >= NUM_LEDS)
  {
    reset = true;
  }
  return (x < i) ? str2color(args[0]) : CRGB(0, 0, 0);
}

class WriteToMode : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic)
  {
    std::string rxValue = characteristic->getValue();
    println(rxValue);
    activeFunction = rxValue;
  }
};

class WriteToDelay : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic)
  {
    uint8_t *data = characteristic->getData();
    char buf[64];
    sprintf(buf, "0x%02x 0x%02x", data[0], data[1]);
    println(buf);
    delay_ms = data[0] + (data[1] << 8);
  }
};

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, NUM_LEDS);
  modes.registerFunc({"alternate", "alternate(COLOR,COLOR,NUMBER)", 3, alternate});
  modes.registerFunc({"trace", "trace(COLOR)", 1, trace});

  BLEDevice::init(BLE_DEVICE_NAME);
  BLEServer *server = BLEDevice::createServer();
  BLEService *service = server->createService(SERVICE_UUID);

  BLECharacteristic *characteristic = service->createCharacteristic(MODE_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);
  characteristic->setCallbacks(new WriteToMode());
  BLECharacteristic *delayChar = service->createCharacteristic(DELAY_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);
  delayChar->setCallbacks(new WriteToDelay());
  BLECharacteristic *docstrChar = service->createCharacteristic(DOCSTR_CHAR_UUID, BLECharacteristic::PROPERTY_READ);
  docstrChar->setValue(modes.getDocstring());

  service->start();
  auto advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID);
  advertising->setScanResponse(true);
  BLEDevice::startAdvertising();
  println("BLE online");
}

uint8_t i = 0;
void loop()
{
  bool reset = false;
  for (uint8_t x = 0; x < NUM_LEDS; x++)
  {
    try
    {
      if (activeFunction.size() > 0)
      {
        leds[x] = modes.call(activeFunction, x, i, reset);
      }
    }
    catch (std::string e)
    {
      println(e);
      delay(500);
    }
  }
  if (reset)
  {
    i = 0;
  }
  FastLED.show();
  delay(delay_ms);
  i++;
}