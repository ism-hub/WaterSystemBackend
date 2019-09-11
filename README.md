# Water System Backend
The backend of the Water System project, for a more user oriented (features) review check the front-end [Water-System GUI](https://github.com/ism-hub/WaterSystemGUI).

# Run
Clone from git with the submodules with that command - 
```git
git clone --recursive https://github.com/ism-hub/WaterSystemBackend.git
```
Open using vscode with [Platformio](https://platformio.org/) installed, and that's it now you can compile, upload, run and edit code.

# Hardware
### Overview 
We have the ESP8266 as the main controller, connected to it we have nodes with the sensors and the valve, the nodes sit on top of the plant.
![Big Picture](https://i.imgur.com/P7ZPayj.png)

### Detailed
This version is using SPI as the communication scheme and only has the valves in the nodes. The ESP8266 is connected, via SPI interface, to a SPI chip (a bigger one with more CS pins, named MySPI). The MySPI chip is connected to a Switches Board (you can turn switches on and off via SPI). In turn, the Switches Board connected to the valves and pumps (via transistors) and you can turn them on and off.

### The MySPI Chip
Gives us a bigger SPI board to work with (more CS pins), based on the 74HC595 shift-register (storage + parallel out shift register) and few NAND gates to make it interface with the SPI.
![MySPI](https://i.imgur.com/4M85QdC.png)

### The Switches Board
Can turn switches on and off via an SPI interface, based on the same idea as the MySPI chip
![Switches Board](https://i.imgur.com/jn5NaiG.png)

### Connecting The Pump And The Valves 
We connect the transistors to the pump, valves and switches in the Switches Board.
I had a PSpice simulation with the values of the resistors and the transistor I've used, I switched computers and need to find and install it again, so soon.


### Next Version Hardware Updates 
The project will move to use the [RS485](https://www.youtube.com/watch?v=3wgKcUDlHuM) specifications for communication with the [Modbus](https://www.youtube.com/watch?v=txi2p5_OjKU) protocol on an [Attiny85](https://www.microchip.com/wwwproducts/en/ATtiny85) as the nodes (the main MCU will stay the ESP8266).

# Software
### Understanding The Architecture
The main idea is that our model fire events before it changes and after the it changes (the before it changes event can fail, if so, the after it changes event won't fire), services/controllers register themselves to the events to add functionality. For example, on 'pump on' event the hardware controller will turn on the pump. The functionality is packaged inside modules.

## Modules
The project is composed from a set of modules, each module adds a different feature/functionality. For a better understanding on how the modules work and how to use them checkout the [Module Framework](https://github.com/ism-hub/ModuleFramework) I'm using.

### HTTP Module
Adds the HTTP server capabilities (can get and send HTTP messages), and adds the [HTTP Framework](https://github.com/ism-hub/HttpFramework) so the user can write controllers and handle the requests.

### Hardware Module
Connects between the domain model and the hardware (adds controllers to listen to events on model changes and turn on/off/reads the hardware).

### DAL Module
Adds the Repository Design Pattern and json serialization service.

### Access-Point Module
Adds the AP-service (can control the AP via it e.g. turn it on/off change the password etc..) and the REST API for the service (the controller for the framework) see [Water-System GUI](https://github.com/ism-hub/WaterSystemGUI) to see a graphical interface for the REST API.

### Station Module
The same as the AP-Module only the ESP8266 acts like a station.

### Scheduler Module
Adds a service that can run a function/task at fixed intervals or at some time. 

### ServiceFramework Module
Not really a framework, for now just holding the services we that we want to stay on for the duration of the program (even if no-one is referencing them), can stop/restart/start services that registered to the framework (supposed to remind you the 'systemctl' from Linux).

### Time Module 
Work in progress, a service that shows you the time, you can set the time, get the time from a NTP server etc... Has an REST API for the user to control.



