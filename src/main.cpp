//
// Created by Gustaw on 02.10.2019.
//
#include <Arduino.h>
#include <Wire.h>

//communication
#define MESS_PC_REQUEST_SLAVE_DATA 0xD0
#define MESS_PC_SEND_DATA_TO_SLAVE 0xD1

#define MESS_MASTER_TRANSFER_SLAVE_DATA 0xD2


//flags for serial communication
uint8_t FLAGS_PC = 0;

#define FLAG_PC_REQUEST_SLAVE_DATA (1<<0)
#define FLAG_PC_SEND_SLAVE_DATA (1<<1)

uint8_t bytes_to_send_to_slaves = 0;

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);
    Wire.begin();
    Serial.begin(9600);
}

//void serialEvent()
//{
//    uint8_t FFlag;
//
//
//        FFlag = Serial.read();
//
//
//
//    switch (FFlag)
//    {
//        case MESS_PC_REQUEST_SLAVE_DATA:
//        {
//            FLAGS_PC |= FLAG_PC_REQUEST_SLAVE_DATA;
//            break;
//        }
//        case MESS_PC_SEND_DATA_TO_SLAVE:
//        {
//            FLAGS_PC |=FLAG_PC_SEND_SLAVE_DATA;
//            bytes_to_send_to_slaves = Serial.read();
//            break;
//        }
//        default:
//        {
//            Serial.write("0xFF");
//            break;
//        }
//    }
//
//}

void loop()
{

    uint8_t buffer[32] = {0};
    uint8_t buffer_index = 0;
    if( Serial.available() > 0)
    {
        Serial.readBytes(buffer,32);
        uint8_t FFlag = buffer[buffer_index++];
        switch (FFlag)
        {
            case MESS_PC_REQUEST_SLAVE_DATA:
            {
                FLAGS_PC |= FLAG_PC_REQUEST_SLAVE_DATA;
                break;
            }
            case MESS_PC_SEND_DATA_TO_SLAVE:
            {
                FLAGS_PC |=FLAG_PC_SEND_SLAVE_DATA;
                bytes_to_send_to_slaves = Serial.read();
                break;
            }
            default:
            {
                Serial.write("0xFF");
                break;
            }
        }
    }


    if(FLAGS_PC & FLAG_PC_REQUEST_SLAVE_DATA)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        uint8_t n_of_devices = buffer[buffer_index++];
        uint8_t master_buffer[5] = {0};
        master_buffer[0] = MESS_MASTER_TRANSFER_SLAVE_DATA;
        uint8_t master_buffer_counter = 1;
        for (int i = 0; i < n_of_devices; i++) {
            uint8_t slave_id = buffer[buffer_index++];//0xFA;//Serial.read();

            Wire.requestFrom(slave_id, (uint8_t) 5);
            while (Wire.available()) {
                master_buffer[master_buffer_counter++] =  Wire.read();
            }
        }
            Serial.write(master_buffer, sizeof(master_buffer));
            digitalWrite(LED_BUILTIN, LOW);
           FLAGS_PC &= ~FLAG_PC_REQUEST_SLAVE_DATA;
     }
}
