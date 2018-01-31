################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
..\AdvancedWebServer.ino 

CPP_SRCS += \
..\sloeber.ino.cpp 

LINK_OBJ += \
.\sloeber.ino.cpp.o 

INO_DEPS += \
.\AdvancedWebServer.ino.d 

CPP_DEPS += \
.\sloeber.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
AdvancedWebServer.o: ..\AdvancedWebServer.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Programming\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\Programming\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/include" "-IC:\Programming\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/lwip2/include" "-IC:\Programming\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/libc/xtensa-lx106-elf/include" "-IC:\Users\rina\git\watering_system_esp\WateringSystemEsp/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC -DTCP_MSS=536 -DDEBUG_ESP_PORT=Serial -DDEBUG_ESP_HTTP_CLIENT -DDEBUG_ESP_HTTP_SERVER -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\libraries\MYESP8266WebServer\src" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\libraries" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\variants\nodemcu" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266mDNS" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi\src" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\src\Model" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\src" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\libraries" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

sloeber.ino.cpp.o: ..\sloeber.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Programming\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\Programming\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/include" "-IC:\Programming\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/lwip2/include" "-IC:\Programming\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0/tools/sdk/libc/xtensa-lx106-elf/include" "-IC:\Users\rina\git\watering_system_esp\WateringSystemEsp/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC -DTCP_MSS=536 -DDEBUG_ESP_PORT=Serial -DDEBUG_ESP_HTTP_CLIENT -DDEBUG_ESP_HTTP_SERVER -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\libraries\MYESP8266WebServer\src" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\libraries" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\cores\esp8266" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\variants\nodemcu" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266mDNS" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi" -I"C:\Programming\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi\src" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\src\Model" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\src" -I"C:\Users\rina\git\watering_system_esp\WateringSystemEsp\libraries" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


