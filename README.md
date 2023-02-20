# Project

An MQTT-connected embedded program that recieves, parses, and formats JSON payloads. Outputs via a standard RS-232 interface, driven at the proper voltage levels using a MAX232 chip. Speaks Epson's ESC/POS format.

Current target is an ESP32. Potential for port to ESP8266 given the modest compute resources required.

Arduino framework is used.

Libraries include:

- ArudinoJSON, maintained by bblanchon https://github.com/bblanchon/ArduinoJson
- PubSubClient, maintained by Nick O'Leary https://github.com/knolleary/pubsubclient



# Build Information

Arudino Framework

Libraries:

- ArduinoJSON, maintained by bblanchon https://github.com/bblanchon/ArduinoJson
- PubSubClient, maintained by Nick O'Leary https://github.com/knolleary/pubsubclient

`code/include/credentials-example.h` should be populated with WiFi and MQTT broker information. Rename to `credentials.h` for proper include. This header file is referenced from `code/src/main.cpp`. May also uncomment relevant section and enter direction in `main.cpp`.



# Thoughts

So the idea would be to have a little box – probably wifi enabled, maybe cell capable – which would print out all the SMS's that I receive, and maybe note all the calls I make and receive. Could go two routes, both with very different meanings: Could print all the messages I send and recieve. Or it could print all I send and recieve, and also allow for handwritten responses on the literally paper tape, which would then be OCR'd before being sent.

More and more this is feeling like a product of an obsession to have it all recorded. I'm wrestling with how to keep my creative work, and I had this thought to just get a dot matrix printer and those huge continuous paper stack and just print *everything* there. Feels like it would speak to my anxiety. But maybe I should confront that anxiety? The reality that all *will* be lost. No matter how much you fight it. And the idea that you can bring yourself to a space, and that can be enough without a need for some portfolio or "body" with you.


## Parts/Components

| Part                                                         | Cost |
| ------------------------------------------------------------ | ---- |
| Raspberry Pi (probably a Zero W)                             | $10  |
| Receipt Printer                                              |      |
| Scanner/Camera module                                        |      |
| Power supply (Battery or wall? 5v with option to use a bank?) |      |
| Enclosure                                                    | DIY  |

