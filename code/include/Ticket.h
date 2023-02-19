#ifndef Ticket_h
#define Ticket_h

#include "Arduino.h"

#include "ArduinoJson.h"

#include "Ticket.h"
#include "EPPrinter.h"

// at some point it'd be neat to have all this read in from a file on the spiffs

class Ticket {
    public:
        Ticket();
        void createTicket();
        void ingestJSON(byte *payload, uint16_t length);
        void sendToPrinter(EPPrinter& RS232, struct tm timeInfo);
        void writeTitle(const char* input);
        void writeBody(const char* input);
        void writeScheduledTime(uint32_t input);
        const char* getValue(const char* key);
        
    private:
        StaticJsonDocument<512> JsonPayload;
};

#endif