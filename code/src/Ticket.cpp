#include "Arduino.h"

#include "ticket.h"

#include "time.h"

#define item_line_height 72


StaticJsonDocument<512> JsonPayload;

bool recievedValidJSON;



Ticket::Ticket() {
    recievedValidJSON = false;
}

void Ticket::createTicket() {
    //init code? Not necessary?
}

void Ticket::ingestJSON(byte *payload, uint16_t length) {
    DeserializationError err = deserializeJson(JsonPayload, payload, length);
    
    if (err.code() == DeserializationError::Ok) {
        Serial.println("successfully ingested JSON");
        recievedValidJSON = true;
    } else {
        Serial.print("JSON ingestion failed with error: ");
        Serial.println(err.f_str());
    }
}

void Ticket::sendToPrinter(EPPrinter& RS232, struct tm timeInfo) {
    // RS232.setBold(1);
    // RS232.print("hello");
    // RS232.setBold(0);
    // RS232.print("no more");
    // RS232.cut();
    // Serial.println("ran test code");


    if (recievedValidJSON) {
        uint8_t selectedLayout = JsonPayload["layout"];

        switch (selectedLayout) {
            case 1: {
                Serial.print("Using layout ");
                Serial.println(selectedLayout);

                RS232.feed(3);

                //                  |           Lead Line, optional              |
                //                  |                                            |
                const char* lead = JsonPayload["lead"] | "";
                if (strlen(lead) == 0) {
                    Serial.println("lead is blank");
                } else {
                    Serial.println("lead is filled, printing");
                    RS232.setSize(2);
                    RS232.setJustify(1);
                    RS232.print(lead);
                    RS232.print(char_lf);
                    RS232.setSize(0);
                    RS232.setJustify(0);
                    RS232.print(char_lf);
                }


                //                  | 9/12/1995                           12:01p |
                
                char dateChars[16];
                snprintf(dateChars, sizeof(dateChars), "%d/%d/%d", timeInfo.tm_mon+1, timeInfo.tm_mday, timeInfo.tm_year-100);
                Serial.print("generated date chars: ");
                Serial.println(dateChars);

                char timeChars[16];
                if (timeInfo.tm_hour < 12) {
                    //AM code
                    snprintf(timeChars, sizeof(timeChars), "%d:%d", timeInfo.tm_hour, timeInfo.tm_min);
                } else if (timeInfo.tm_hour == 12) {
                    //Noon Code
                } else {
                    //PM code
                    snprintf(timeChars, sizeof(timeChars), "%d:%dp", timeInfo.tm_hour%12, timeInfo.tm_min);
                }

                Serial.print("generated time chars: ");
                Serial.println(timeChars);

                RS232.print(dateChars);
                uint8_t spaces = RS232.getPaperWidthInChars()
                    -strlen(timeChars)
                    -strlen(dateChars);
                for (uint8_t i = 0; i < spaces; i++) {
                    RS232.print(' ');
                }
                RS232.print(timeChars);
                RS232.print(char_lf);


                //                  | ------------------------------------------ |
                RS232.print(char_horizontal);
                RS232.print(char_lf);


                //                  |             Sent by: issuer                |
                RS232.setJustify(1);
                RS232.print("Sent by: ");
                RS232.print(JsonPayload["issuer"].as<const char*>());
                RS232.print(char_lf);
                RS232.setJustify(0);


                //                  | ------------------------------------------ |
                RS232.print(char_horizontal);
                RS232.print(char_lf);


                //                  |                                            |
                RS232.print(char_lf);


                //                  |          Begin Item Iteration             |
                JsonArray ItemArray = JsonPayload["items"];
                uint8_t itemCount = ItemArray.size();
                Serial.print("detected array of size ");
                Serial.println(itemCount);
                RS232.setLineHeight(item_line_height);
                for (uint8_t i = 0; i < itemCount; i++) {
                    JsonObject ItemPayload = ItemArray[i];
                    Serial.print("item: ");
                    Serial.println(ItemPayload["desc"].as<const char*>());

                    RS232.setBold(1);
                    RS232.print("  ");
                    RS232.print(char_dot);                                          //ADD: some way to wrap to after the dot
                    RS232.print("  ");
                    RS232.print(ItemPayload["desc"].as<const char*>());
                    RS232.setBold(0);
                    RS232.print(char_lf);

                    JsonArray AddArray = ItemPayload["adds"];
                    uint8_t addCount = AddArray.size();
                    RS232.setLineHeight(0);
                    for (uint8_t i = 0; i < addCount; i++) {
                        JsonObject AddPayload = AddArray[i];
                        Serial.print("add: ");
                        Serial.println(AddPayload["desc"].as<const char*>());

                        RS232.print("       ");
                        RS232.print(AddPayload["desc"].as<const char*>());
                        RS232.print(char_lf);
                    }
                    RS232.setLineHeight(item_line_height);
                }
                RS232.setLineHeight(0); //revert to default
                

                RS232.feed(4);
                RS232.cut();
            } break;
            default: {
                Serial.println("Valid Layout not selected in JSON payload");
                } break;
        }
    } else {
        Serial.println("No valid JSON payload to print");
    }
}

void Ticket::writeTitle(const char* input) {
    //should be a check to make sure it isn't larger than the dest
    //should append to json doc
    //again, input checking?
}

void Ticket::writeBody(const char* input) {
    //should append to json doc
    //should there be some kind of lenghth checking here?
}

void Ticket::writeScheduledTime(uint32_t input) {
    //should append to json doc
}

const char* Ticket::getValue(const char* key) {
    return JsonPayload[key];
    //for now will only return strings, not numbers
}