/*
  ==============================================================================

    Help.h
    Author:  aike

  ==============================================================================
*/

#pragma once

#include <string>
#include <map>

class Help
{
public:
    Help() {
        int n;

        // initialize
        for (n = 0; n < 256; n++)
        {
            byte0[n] = "Undefined";
            byte1[n] = "-";
            byte2[n] = "-";
            cc1[n] = "Undefined";
            cc2[n] = "-";
        }

        // Message
        n = 0x80;  byte0[n] = "Note Off";                byte1[n] = "note number(0-127)";     byte2[n] = "velocity(0-127)";
        n = 0x90;  byte0[n] = "Note On";                 byte1[n] = "note number(0-127)";     byte2[n] = "velocity(0-127)";
        n = 0xA0;  byte0[n] = "Polyphonic Key Pressure"; byte1[n] = "note number(0-127)";     byte2[n] = "pressure(0-127)";
        n = 0xB0;  byte0[n] = "Control Change";          byte1[n] = "control number(0-127)";  byte2[n] = "control value(0-127)";
        n = 0xC0;  byte0[n] = "Program Change";          byte1[n] = "program number(0-127)";  byte2[n] = "no use";
        n = 0xD0;  byte0[n] = "Channel Pressure";        byte1[n] = "note number(0-127)";     byte2[n] = "pressure(0-127)";
        n = 0xE0;  byte0[n] = "Pitch Bend Change";       byte1[n] = "LSB(0-127)";             byte2[n] = "MSB(0-127)";

        // CC
        n = 0x00; cc1[n] = "Bank Select";          cc2[n] = "MSB(0-127)";
        n = 0x01; cc1[n] = "Modulation Wheel";     cc2[n] = "MSB(0-127)";
        n = 0x02; cc1[n] = "Breath Controller";    cc2[n] = "MSB(0-127)";
        n = 0x04; cc1[n] = "Foot Pedal";           cc2[n] = "MSB(0-127)";
        n = 0x05; cc1[n] = "Portamento Time";      cc2[n] = "MSB(0-127)";
        n = 0x06; cc1[n] = "Data Entry";           cc2[n] = "MSB(0-127)";
        n = 0x07; cc1[n] = "Volume";               cc2[n] = "MSB(0-127)";
        n = 0x08; cc1[n] = "Balance";              cc2[n] = "MSB(0-127)";
        n = 0x0A; cc1[n] = "Pan";                  cc2[n] = "MSB(0-127)";
        n = 0x0B; cc1[n] = "Expression";           cc2[n] = "MSB(0-127)";
        n = 0x0C; cc1[n] = "Effect Controller 1";  cc2[n] = "MSB(0-127)";
        n = 0x0D; cc1[n] = "Effect Controller 2";  cc2[n] = "MSB(0-127)";
        n = 0x10; cc1[n] = "General Purpose 1";    cc2[n] = "MSB(0-127)";
        n = 0x11; cc1[n] = "General Purpose 2";    cc2[n] = "MSB(0-127)";
        n = 0x12; cc1[n] = "General Purpose 3";    cc2[n] = "MSB(0-127)";
        n = 0x13; cc1[n] = "General Purpose 4";    cc2[n] = "MSB(0-127)";

        n = 0x20; cc1[n] = "Bank Select";          cc2[n] = "LSB(0-127)";
        n = 0x21; cc1[n] = "Modulation Wheel";     cc2[n] = "LSB(0-127)";
        n = 0x22; cc1[n] = "Breath Controller";    cc2[n] = "LSB(0-127)";
        n = 0x24; cc1[n] = "Foot Pedal";           cc2[n] = "LSB(0-127)";
        n = 0x25; cc1[n] = "Portamento Time";      cc2[n] = "LSB(0-127)";
        n = 0x26; cc1[n] = "Data Entry";           cc2[n] = "LSB(0-127)";
        n = 0x27; cc1[n] = "Volume";               cc2[n] = "LSB(0-127)";
        n = 0x28; cc1[n] = "Balance";              cc2[n] = "LSB(0-127)";
        n = 0x2A; cc1[n] = "Pan";                  cc2[n] = "LSB(0-127)";
        n = 0x2B; cc1[n] = "Expression";           cc2[n] = "LSB(0-127)";
        n = 0x2C; cc1[n] = "Effect Controller 1";  cc2[n] = "LSB(0-127)";
        n = 0x2D; cc1[n] = "Effect Controller 2";  cc2[n] = "LSB(0-127)";
        n = 0x30; cc1[n] = "General Purpose 1";    cc2[n] = "LSB(0-127)";
        n = 0x31; cc1[n] = "General Purpose 2";    cc2[n] = "LSB(0-127)";
        n = 0x32; cc1[n] = "General Purpose 3";    cc2[n] = "LSB(0-127)";
        n = 0x33; cc1[n] = "General Purpose 4";    cc2[n] = "LSB(0-127)";

        n = 0x40; cc1[n] = "Damper Pedal";         cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x41; cc1[n] = "Portamento";           cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x42; cc1[n] = "Sostenuto Pedal";      cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x43; cc1[n] = "Soft Pedal";           cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x44; cc1[n] = "Legato Foot Switch";   cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x45; cc1[n] = "Hold 2";               cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x46; cc1[n] = "Sound Controller 1";   cc2[n] = "value(0-127)";
        n = 0x47; cc1[n] = "Sound Controller 2";   cc2[n] = "value(0-127)";
        n = 0x48; cc1[n] = "Sound Controller 3";   cc2[n] = "value(0-127)";
        n = 0x49; cc1[n] = "Sound Controller 4";   cc2[n] = "value(0-127)";
        n = 0x4A; cc1[n] = "Sound Controller 5";   cc2[n] = "value(0-127)";
        n = 0x4B; cc1[n] = "Sound Controller 6";   cc2[n] = "value(0-127)";
        n = 0x4C; cc1[n] = "Sound Controller 7";   cc2[n] = "value(0-127)";
        n = 0x4D; cc1[n] = "Sound Controller 8";   cc2[n] = "value(0-127)";
        n = 0x4E; cc1[n] = "Sound Controller 9";   cc2[n] = "value(0-127)";
        n = 0x4F; cc1[n] = "Sound Controller 10";  cc2[n] = "value(0-127)";
        n = 0x50; cc1[n] = "General Purpose 5";    cc2[n] = "value(0-127)";
        n = 0x51; cc1[n] = "General Purpose 6";    cc2[n] = "value(0-127)";
        n = 0x52; cc1[n] = "General Purpose 7";    cc2[n] = "value(0-127)";
        n = 0x53; cc1[n] = "General Purpose 8";    cc2[n] = "value(0-127)";
        n = 0x54; cc1[n] = "Control";              cc2[n] = "value(0-127)";
        n = 0x58; cc1[n] = "High Resolution Velocity Prefix";      cc2[n] = "value(0-127)";
        n = 0x5B; cc1[n] = "Effect 1 Depth";       cc2[n] = "value(0-127)";
        n = 0x5C; cc1[n] = "Effect 2 Depth";       cc2[n] = "value(0-127)";
        n = 0x5D; cc1[n] = "Effect 3 Depth";       cc2[n] = "value(0-127)";
        n = 0x5E; cc1[n] = "Effect 4 Depth";       cc2[n] = "value(0-127)";
        n = 0x5F; cc1[n] = "Effect 5 Depth";       cc2[n] = "value(0-127)";
        n = 0x60; cc1[n] = "Data Increment";       cc2[n] = "value(0-127)";
        n = 0x61; cc1[n] = "Data Decrement";       cc2[n] = "value(0-127)";
        n = 0x62; cc1[n] = "NRPN LSB";             cc2[n] = "value(0-127)";
        n = 0x63; cc1[n] = "NRPN MSB";             cc2[n] = "value(0-127)";
        n = 0x64; cc1[n] = "RPN LSB";              cc2[n] = "value(0-127)";
        n = 0x65; cc1[n] = "RPN MSB";              cc2[n] = "value(0-127)";
        n = 0x78; cc1[n] = "All Sound Off";        cc2[n] = "0";
        n = 0x79; cc1[n] = "Reset All Controller"; cc2[n] = "0";
        n = 0x7A; cc1[n] = "Local on/off Switch";  cc2[n] = "0:Off/127:On";
        n = 0x7B; cc1[n] = "All Notes Off";        cc2[n] = "0";
        n = 0x7C; cc1[n] = "Omni Mode Off";        cc2[n] = "0";
        n = 0x7D; cc1[n] = "Omni Mode On";         cc2[n] = "0";
        n = 0x7E; cc1[n] = "Mono Mode";            cc2[n] = "0(Omni On)/number of voices(Omni Off)";
        n = 0x7F; cc1[n] = "Poly Mode";            cc2[n] = "0";


        // command name to number
        for (n = 0; n < 256; n++)
        {
            // ƒRƒ}ƒ“ƒh–¼‚©‚ç”Ô†‚ðˆø‚¯‚é‚æ‚¤‚ÉŽ«‘“o˜^‚·‚é
            // ex. cmd["Control Change"] = 0xB0
            if (byte0[n] != "Undefined")
            {
                cmd[toLowerNoSpace(byte0[n])] = n;
            }

            // •¡”’PŒê‚©‚ç‚È‚éƒRƒ}ƒ“ƒh–¼‚Í—ªŒê‚àŽ«‘“o˜^‚·‚é
            // ex. "Control Change"¨"cc"  cmd["cc"] = 0xB0
            if (byte0[n].find(' ') != std::string::npos)
            {
                cmd[toAbbreviation(byte0[n])] = n;
            }
        }

        // CC command name to number
        for (n = 0; n < 256; n++)
        {
            if (cc1[n] != "Undefined")
            {
                cc_cmd[toLowerNoSpace(cc1[n])] = n;
            }

            if (cc1[n].find(' ') != std::string::npos)
            {
                cmd[toAbbreviation(cc1[n])] = n;
            }
        }
    };

    ~Help() {};

    std::string commandName(int n);
    std::string ccName(int n);
    std::string Help::toString(int byte0, int byte1, int byte2);

private:
    std::string byte0[256];
    std::string byte1[256];
    std::string byte2[256];
    std::string cc1[256];
    std::string cc2[256];

    std::map<std::string, int> cmd;
    std::map<std::string, int> cc_cmd;

    std::string toLowerNoSpace(const std::string& in_s);
    std::string toAbbreviation(const std::string& in_s);

};


