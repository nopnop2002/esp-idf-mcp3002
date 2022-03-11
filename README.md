# esp-idf-mcp3002
Driver for A/D converter with SPI serial interface.   
esp-idf includes an ADC driver, but it cannot be used at the same time as the WiFi driver.   
If you use it for a long time, you will lose your WiFi connection.   

# Supported converters
|Model|Number of channels|Resolution|MinRange|MaxRange|
|:-:|:-:|:-:|:-:|:-:|
|MCP3001|1|10Bits|0|1023|
|MCP3002|2|10Bits|0|1023|
|MCP3004|4|10Bits|0|1023|
|MCP3008|8|10Bits|0|1023|
|MCP3201|1|12Bits|0|4095|
|MCP3202|2|12Bits|0|4095|
|MCP3204|4|12Bits|0|4095|
|MCP3208|8|12Bits|0|4095|
|MCP3301|1|12Bits+Sign|-4095|4095|
|MCP3302|4|12Bits+Sign|-4095|4095|
|MCP3304|8|12Bits+Sign|-4095|4095|

# Software requirements
esp-idf v4.4 or later.   
This is because this version supports ESP32-C3.   


# Installation
```Shell
git clone https://github.com/nopnop2002/esp-idf-mcp3002
cd esp-idf-mcp3002/
idf.py set-target {esp32/esp32s2/esp32c3}
idf.py menuconfig
idf.py flash
```

__Note for ESP32C3__   
For some reason, there are development boards that cannot use GPIO06, GPIO08, GPIO09, GPIO19 for SPI clock pins.   
According to the ESP32C3 specifications, these pins can also be used as SPI clocks.   
I used a raw ESP-C3-13 to verify that these pins could be used as SPI clocks.   

# Configuration   

![config-top](https://user-images.githubusercontent.com/6020549/157990108-4b4dafde-4c32-45e6-92b1-32eb39b94d58.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/157990185-fa6beb49-a6fb-471b-87e5-41394b045868.jpg)

## Device Setting
![config-device-1](https://user-images.githubusercontent.com/6020549/157990203-d91e19fd-5f5b-43a5-b39a-9d5c3774a6b0.jpg)
![config-device-2](https://user-images.githubusercontent.com/6020549/157990205-3479fcd0-c476-404f-9422-e78ddced1e45.jpg)
![config-device-3](https://user-images.githubusercontent.com/6020549/157990689-2a7484ce-88ae-4e4f-9bc2-46b32973fb16.jpg)

## SPI Setting
![config-spi](https://user-images.githubusercontent.com/6020549/157990782-388f121a-ac8b-4c04-91b6-1261451ef041.jpg)


# Wirering
|MCP3xxx||ESP32|ESP32-S2|ESP32-C3||
|:-:|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO33|GPIO3||
|MOSI|--|GPIO23|GPIO35|GPIO0|*1|
|SCK|--|GPIO18|GPIO36|GPIO1||
|CS|--|GPIO5|GPIO34|GPIO5||
|Vdd|--|3.3V|3.3V|3.3V||
|Vss|--|GND|GND|GND||

In addition to this, Vref and Agnd connections are required.   
See the data sheet for details.   

(*1)Not required for MCP3001/3201/3301.   


# Screen Shot
![stdout](https://user-images.githubusercontent.com/6020549/157990842-77d6ee7c-c518-4d1d-94ec-283badc7fe34.jpg)


