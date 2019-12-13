/*
Copyright (C) AC SOFTWARE SP. Z O.O.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <SPI.h>
#include <DHT.h>
#include <SuplaDevice.h>

#include <Wire.h>
#include "Adafruit_Si7021.h"
Adafruit_Si7021 Si7021 = Adafruit_Si7021();

// Choose proper network interface for your card:
// Arduino Mega with EthernetShield W5100:
#include <supla/network/ethernet_shield.h>
// Ethernet MAC address
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
// WiFi.macAddress(mac);
//Supla::EthernetShield ethernet(mac);
//
// Arduino Mega with ENC28J60:
// #include <supla/network/ENC28J60.h>
// Supla::ENC28J60 ethernet(mac);
//
// ESP8266 based board:
#include <supla/network/esp_wifi.h>
Supla::ESPWifi wifi("SSID", "WIFI_PASS");

/*
 * This example requires DHT sensor library installed. 
 * https://github.com/adafruit/DHT-sensor-library
 */

#define DHTPIN 24
#define DHTTYPE DHT22
 
// Setup a DHT instance
DHT dht(DHTPIN, DHTTYPE);


// DHT22 Sensor read implementation
void get_temperature_and_humidity(int channelNumber, double *temp, double *humidity) {

       *temp = Si7021.readTemperature();
    Serial.print("Temperature = ");
    Serial.print(Si7021.readTemperature());
    Serial.println(" *C");
    
    *humidity = Si7021.readHumidity();
    Serial.print("Humidity = ");
    Serial.print(Si7021.readHumidity());
    Serial.println(" %");
    if ( isnan(*temp) || isnan(*humidity) ) {
      *temp = -275;
      *humidity = -1;
    }
}

void setup() {

  Serial.begin(9600);

  // Init DHT library 
    Wire.begin(5,4);
    Si7021.begin();
  
  // Set temperature/humidity callback
  SuplaDevice.setTemperatureHumidityCallback(&get_temperature_and_humidity);

  // Replace the falowing GUID with value that you can retrieve from https://www.supla.org/arduino/get-guid
  char GUID[SUPLA_GUID_SIZE] = {mac[WL_MAC_ADDR_LENGTH - 6], mac[WL_MAC_ADDR_LENGTH - 5], mac[WL_MAC_ADDR_LENGTH - 4], mac[WL_MAC_ADDR_LENGTH - 3],                                
                                 mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1], mac[WL_MAC_ADDR_LENGTH - 1], mac[WL_MAC_ADDR_LENGTH - 2], 
                                 mac[WL_MAC_ADDR_LENGTH - 3], mac[WL_MAC_ADDR_LENGTH - 4], mac[WL_MAC_ADDR_LENGTH - 5], mac[WL_MAC_ADDR_LENGTH - 6]};

  // Replace the following AUTHKEY with value that you can retrieve from: https://www.supla.org/arduino/get-authkey
  char AUTHKEY[SUPLA_AUTHKEY_SIZE] = {mac[WL_MAC_ADDR_LENGTH - 3], mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1], mac[WL_MAC_ADDR_LENGTH - 4],                                
                                 mac[WL_MAC_ADDR_LENGTH - 5], mac[WL_MAC_ADDR_LENGTH - 3], mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 4], 
                                 mac[WL_MAC_ADDR_LENGTH - 6], mac[WL_MAC_ADDR_LENGTH - 3], mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 6]};

  /*
   * Having your device already registered at cloud.supla.org,
   * you want to change CHANNEL sequence or remove any of them,
   * then you must also remove the device itself from cloud.supla.org.
   * Otherwise you will get "Channel conflict!" error.
   */
    
  // CHANNEL0 - RELAY
  //SuplaDevice.addRelay(44, true);           // ﻿44 - ﻿Pin number where the relay is connected      
                                      // Call SuplaDevice.addRelay(44, true) with an extra "true" parameter 
                                      // to enable "port value inversion"
                                      // where HIGH == LOW, and LOW == HIGH   

  // CHANNEL1 - RELAY
  //SuplaDevice.addRelay(45, true);           // 45 - ﻿﻿Pin number where the relay is connected   

  // CHANNEL3 - TWO RELAYS (Roller shutter operation)
  //SuplaDevice.addRollerShutterRelays(46,     // 46 - ﻿﻿Pin number where the 1st relay is connected   
//                                     47, true);    // 47 - ﻿Pin number where the 2nd relay is connected  

  // CHANNEL4 - Opening sensor (Normal Open)
 // SuplaDevice.addSensorNO(A0); // A0 - ﻿Pin number where the sensor is connected
                               // Call SuplaDevice.addSensorNO(A0, true) with an extra "true" parameter
                               // to enable the internal pull-up resistor


  // CHANNEL5 - Opening sensor (Normal Open)
  //SuplaDevice.addSensorNO(A1); // A1 - ﻿Pin number where the sensor is connected


  // CHANNEL6 - DHT22 Sensor
  // SuplaDevice.addDHT11();
  // SuplaDevice.addAM2302();
    SuplaDevice.addDHT22();


  /*
   * SuplaDevice Initialization.
   * Server address, LocationID and LocationPassword are available at https://cloud.supla.org 
   * If you do not have an account, you can create it at https://cloud.supla.org/account/create
   * SUPLA and SUPLA CLOUD are free of charge
   * 
   */
  SuplaDevice.setName("Twoja_NAZWA");
  SuplaDevice.begin(GUID,              // Global Unique Identifier 
                    "svr1.supla.org",  // SUPLA server address
                    "email@email.com",   // Email address used to login to Supla Cloud
                    AUTHKEY);          // Authorization key
    
}

void loop() {
  SuplaDevice.iterate();
}
