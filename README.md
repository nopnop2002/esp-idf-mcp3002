# esp-idf-mcp3002
Driver for A/D converter with SPI serial interface.   
esp-idf includes an ADC driver, but the convertible voltage is up to 3.3V.   
By giving Vdd and Vref appropriately and level-shifting MOSI,MISO,CS and SCK, Higher voltages can be converted.   
For example, when converting 5V, High Level Input Voltage of MOSI and SCK must be 0.7*5V=3.5V or higher.   
And the high level output voltage of MISO will be 4.1V or higher.   

# Supported converters
|Model|Number of channels|Resolution|MinValue|MaxValue|Operating voltage|
|:-:|:-:|:-:|:-:|:-:|:-:|
|MCP3001|1|10Bits|0|1023|2.7V - 5.5V|
|MCP3002|2|10Bits|0|1023|2.7V - 5.5V|
|MCP3004|4|10Bits|0|1023|2.7V - 5.5V|
|MCP3008|8|10Bits|0|1023|2.7V - 5.5V|
|MCP3201|1|12Bits|0|4095|2.7V - 5.5V|
|MCP3202|2|12Bits|0|4095|2.7V - 5.5V|
|MCP3204|4|12Bits|0|4095|2.7V - 5.5V|
|MCP3208|8|12Bits|0|4095|2.7V - 5.5V|
|MCP3301|1|12Bits+Sign|-4095|4095|4.5V - 5.5V|
|MCP3302|4|12Bits+Sign|-4095|4095|4.5V - 5.5V|
|MCP3304|8|12Bits+Sign|-4095|4095|4.5V - 5.5V|

# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   


# Installation
```Shell
git clone https://github.com/nopnop2002/esp-idf-mcp3002
cd esp-idf-mcp3002/
idf.py set-target {esp32/esp32s2/esp32s3/esp32c2/esp32c3/esp32c6}
idf.py menuconfig
idf.py flash
```

__Note for ESP32C3__   
For some reason, there are development boards that cannot use GPIO06, GPIO08, GPIO09, GPIO19 for SPI clock pins.   
According to the ESP32C3 specifications, these pins can also be used as SPI clocks.   
I used a raw ESP-C3-13 to verify that these pins could be used as SPI clocks.   

# Configuration   

![config-top](https://github.com/nopnop2002/esp-idf-mcp3002/assets/6020549/2210d655-57ca-4511-9588-7c7a291007cf)
![config-app](https://user-images.githubusercontent.com/6020549/157990185-fa6beb49-a6fb-471b-87e5-41394b045868.jpg)

## Device Setting
![config-device-1](https://user-images.githubusercontent.com/6020549/157990203-d91e19fd-5f5b-43a5-b39a-9d5c3774a6b0.jpg)
![config-device-2](https://user-images.githubusercontent.com/6020549/157990205-3479fcd0-c476-404f-9422-e78ddced1e45.jpg)
![config-device-3](https://user-images.githubusercontent.com/6020549/157990689-2a7484ce-88ae-4e4f-9bc2-46b32973fb16.jpg)

## SPI Setting
![config-spi-1](https://user-images.githubusercontent.com/6020549/211113887-027f8ec0-74c8-4427-b5ab-c59a4997b24a.jpg)

## SPI BUS selection   
![config-spi-2](https://user-images.githubusercontent.com/6020549/211113850-35a5a9a4-1dc1-460b-98da-5ca63e4ef558.jpg)

The ESP32 series has three SPI BUSs.   
SPI1_HOST is used for communication with Flash memory.   
You can use SPI2_HOST and SPI3_HOST freely.   
When you use SDSPI(SD Card via SPI), SDSPI uses SPI2_HOST BUS.   
When using this module at the same time as SDSPI or other SPI device using SPI2_HOST, it needs to be changed to SPI3_HOST.   
When you don't use SDSPI, both SPI2_HOST and SPI3_HOST will work.   
Previously it was called HSPI_HOST / VSPI_HOST, but now it is called SPI2_HOST / SPI3_HOST.   


# Wirering
|MCP3xxx||ESP32|ESP32-S2/S3|ESP32-C2/C3||
|:-:|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO37|GPIO4||
|SCK|--|GPIO18|GPIO36|GPIO3||
|MOSI|--|GPIO23|GPIO35|GPIO2|*1|
|CS|--|GPIO5|GPIO34|GPIO1||
|Vdd|--|3.3V|3.3V|3.3V||
|Vss|--|GND|GND|GND||

In addition to this, Vref and Agnd connections are required.   
See the data sheet for details.   

(*1)Not required for MCP3001/3201/3301.   


# Screen Shot
![stdout](https://user-images.githubusercontent.com/6020549/157990842-77d6ee7c-c518-4d1d-94ec-283badc7fe34.jpg)


# How to use this component in your project   
Create idf_component.yml in the same directory as main.c.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
                         +-- CMakeLists.txt
                         +-- idf_component.yml
```

Contents of idf_component.yml.
```
dependencies:
  nopnop2002/mcp3002:
    path: components/mcp3002/
    git: https://github.com/nopnop2002/esp-idf-mcp3002.git
```

When you build a projects esp-idf will automaticly fetch repository to managed_components dir and link with your code.   
```
YourProject --+-- CMakeLists.txt
              +-- main --+-- main.c
              |          +-- CMakeLists.txt
              |          +-- idf_component.yml
              +-- managed_components ----- nopnop2002__mcp3002
```
