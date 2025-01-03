/*
  ==============================================================================

    Help.h
    Author:  aike

  ==============================================================================
*/

#pragma once

#include <string>
#include <vector>
#include <map>

enum MSG {
    ChannelVoice,
    ChannelVoiceCc,
    ChannelMode,
    SystemCommon,
    SystemRealtime,
    NotStatus
};

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

        // Cannel Voice Message
        n = 0x80;  byte0[n] = "Note Off";                byte1[n] = "note number(0-127)";     byte2[n] = "velocity(0-127)";
        n = 0x90;  byte0[n] = "Note On";                 byte1[n] = "note number(0-127)";     byte2[n] = "velocity(0-127)";
        n = 0xA0;  byte0[n] = "Polyphonic Key Pressure"; byte1[n] = "note number(0-127)";     byte2[n] = "pressure(0-127)";
        n = 0xB0;  byte0[n] = "Control Change";          byte1[n] = "control number(0-127)";  byte2[n] = "control value(0-127)";
        n = 0xC0;  byte0[n] = "Program Change";          byte1[n] = "program number(0-127)";  byte2[n] = "-";
        n = 0xD0;  byte0[n] = "Channel Pressure";        byte1[n] = "note number(0-127)";     byte2[n] = "pressure(0-127)";
        n = 0xE0;  byte0[n] = "Pitch Bend Change";       byte1[n] = "LSB(0-127)";             byte2[n] = "MSB(0-127)";
        // System Common Message
        n = 0xF0;  byte0[n] = "SOX";                     byte1[n] = "Manufacturer ID";       byte2[n] = "-";
        n = 0xF1;  byte0[n] = "MIDI Time Code";          byte1[n] = "type / value(nnndddd)";  byte2[n] = "-";
        n = 0xF2;  byte0[n] = "Song Position Pointer";   byte1[n] = "LSB(0-127)";             byte2[n] = "MSB(0-127)";
        n = 0xF3;  byte0[n] = "Song Select";             byte1[n] = "song(0-127)";            byte2[n] = "-";
        n = 0xF6;  byte0[n] = "Tune Request";            byte1[n] = "-";                      byte2[n] = "-";
        n = 0xF7;  byte0[n] = "EOX";                     byte1[n] = "-";                      byte2[n] = "-";
        // System Realtime Message
        n = 0xF8;  byte0[n] = "Timing Clock";            byte1[n] = "-";                      byte2[n] = "-";
        n = 0xFA;  byte0[n] = "Start";                   byte1[n] = "-";                      byte2[n] = "-";
        n = 0xFB;  byte0[n] = "Continue";                byte1[n] = "-";                      byte2[n] = "-";
        n = 0xFC;  byte0[n] = "Stop";                    byte1[n] = "-";                      byte2[n] = "-";
        n = 0xFE;  byte0[n] = "Active Sensing";          byte1[n] = "-";                      byte2[n] = "-";
        n = 0xFF;  byte0[n] = "System Reset";            byte1[n] = "-";                      byte2[n] = "-";

        // Control Change
        n = 0x00; cc1[n] = "Bank Select MSB";          cc2[n] = "MSB(0-127)";
        n = 0x01; cc1[n] = "Modulation Wheel MSB";     cc2[n] = "MSB(0-127)";
        n = 0x02; cc1[n] = "Breath Controller MSB";    cc2[n] = "MSB(0-127)";
        n = 0x04; cc1[n] = "Foot Pedal MSB";           cc2[n] = "MSB(0-127)";
        n = 0x05; cc1[n] = "Portamento Time MSB";      cc2[n] = "MSB(0-127)";
        n = 0x06; cc1[n] = "Data Entry MSB";           cc2[n] = "MSB(0-127)";
        n = 0x07; cc1[n] = "Channel Volume MSB";       cc2[n] = "MSB(0-127)";
        n = 0x08; cc1[n] = "Balance MSB";              cc2[n] = "MSB(0-127)";
        n = 0x0A; cc1[n] = "Pan MSB";                  cc2[n] = "MSB(0-127)";
        n = 0x0B; cc1[n] = "Expression MSB";           cc2[n] = "MSB(0-127)";
        n = 0x0C; cc1[n] = "Effect Controller 1 MSB";  cc2[n] = "MSB(0-127)";
        n = 0x0D; cc1[n] = "Effect Controller 2 MSB";  cc2[n] = "MSB(0-127)";
        n = 0x10; cc1[n] = "General Purpose 1 MSB";    cc2[n] = "MSB(0-127)";
        n = 0x11; cc1[n] = "General Purpose 2 MSB";    cc2[n] = "MSB(0-127)";
        n = 0x12; cc1[n] = "General Purpose 3 MSB";    cc2[n] = "MSB(0-127)";
        n = 0x13; cc1[n] = "General Purpose 4 MSB";    cc2[n] = "MSB(0-127)";

        n = 0x20; cc1[n] = "Bank Select LSB";          cc2[n] = "LSB(0-127)";
        n = 0x21; cc1[n] = "Modulation Wheel LSB";     cc2[n] = "LSB(0-127)";
        n = 0x22; cc1[n] = "Breath Controller LSB";    cc2[n] = "LSB(0-127)";
        n = 0x24; cc1[n] = "Foot Controller LSB";      cc2[n] = "LSB(0-127)";
        n = 0x25; cc1[n] = "Portamento Time LSB";      cc2[n] = "LSB(0-127)";
        n = 0x26; cc1[n] = "Data Entry LSB";           cc2[n] = "LSB(0-127)";
        n = 0x27; cc1[n] = "Volume LSB";               cc2[n] = "LSB(0-127)";
        n = 0x28; cc1[n] = "Balance LSB";              cc2[n] = "LSB(0-127)";
        n = 0x2A; cc1[n] = "Pan LSB";                  cc2[n] = "LSB(0-127)";
        n = 0x2B; cc1[n] = "Expression LSB";           cc2[n] = "LSB(0-127)";
        n = 0x2C; cc1[n] = "Effect Controll 1 LSB";    cc2[n] = "LSB(0-127)";
        n = 0x2D; cc1[n] = "Effect Controll 2 LSB";    cc2[n] = "LSB(0-127)";
        n = 0x30; cc1[n] = "General Purpose 1 LSB";    cc2[n] = "LSB(0-127)";
        n = 0x31; cc1[n] = "General Purpose 2 LSB";    cc2[n] = "LSB(0-127)";
        n = 0x32; cc1[n] = "General Purpose 3 LSB";    cc2[n] = "LSB(0-127)";
        n = 0x33; cc1[n] = "General Purpose 4 LSB";    cc2[n] = "LSB(0-127)";

        n = 0x40; cc1[n] = "Damper Pedal";         cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x41; cc1[n] = "Portamento";           cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x42; cc1[n] = "Sostenuto";            cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x43; cc1[n] = "Soft Pedal";           cc2[n] = "Off(0-63)/On(64-127)";
        n = 0x44; cc1[n] = "Legato Footswitch";    cc2[n] = "Off(0-63)/On(64-127)";
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
        n = 0x54; cc1[n] = "Portamento Control";   cc2[n] = "value(0-127)";
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

        // Channel Mode Message
        n = 0x78; cc1[n] = "All Sound Off";        cc2[n] = "always zero";
        n = 0x79; cc1[n] = "Reset All Controllers";cc2[n] = "always zero";
        n = 0x7A; cc1[n] = "Local Control";        cc2[n] = "0:Off/127:On";
        n = 0x7B; cc1[n] = "All Notes Off";        cc2[n] = "always zero";
        n = 0x7C; cc1[n] = "Omni Mode Off";        cc2[n] = "always zero";
        n = 0x7D; cc1[n] = "Omni Mode On";         cc2[n] = "always zero";
        n = 0x7E; cc1[n] = "Mono Mode On";         cc2[n] = "0(Omni On)/number of voices(Omni Off)";
        n = 0x7F; cc1[n] = "Poly Mode On";         cc2[n] = "always zero";

        // Control ChangeŠg’£\•¶
        n = 0x00; ccx[n] = "Bank Select";
        n = 0x01; ccx[n] = "Modulation Wheel";
        n = 0x02; ccx[n] = "Breath Controller";
        n = 0x04; ccx[n] = "Foot Pedal";
        n = 0x05; ccx[n] = "Portamento Time";
        n = 0x06; ccx[n] = "Data Entry";
        n = 0x07; ccx[n] = "Channel Volume";
        n = 0x08; ccx[n] = "Balance";
        n = 0x0A; ccx[n] = "Pan";
        n = 0x0B; ccx[n] = "Expression";
        n = 0x0C; ccx[n] = "Effect Controller 1";
        n = 0x0D; ccx[n] = "Effect Controller 2";
        n = 0x10; ccx[n] = "General Purpose 1";
        n = 0x11; ccx[n] = "General Purpose 2";
        n = 0x12; ccx[n] = "General Purpose 3";
        n = 0x13; ccx[n] = "General Purpose 4";
        n = 0x63; ccx[n] = "NRPN";
        n = 0x65; ccx[n] = "RPN";

        // command name to number
        for (n = 0; n < 256; n++)
        {
            // ƒRƒ}ƒ“ƒh–¼‚©‚ç”Ô†‚ðˆø‚¯‚é‚æ‚¤‚ÉŽ«‘“o˜^‚·‚é
            // ex. cmd["Control Change"] = 0xB0
            if (byte0[n] != "Undefined")
            {
                cmd[toLowerNoSpace(byte0[n])] = n;

                // •¡”’PŒê‚©‚ç‚È‚éƒRƒ}ƒ“ƒh–¼‚Í—ªŒê‚àŽ«‘“o˜^‚·‚é
                // ex. "Control Change"¨"cc"  cmd["cc"] = 0xB0
                if (byte0[n].find(' ') != std::string::npos)
                {
                    cmd[toAbbreviation(byte0[n])] = n;
                }
            }
        }
        cmd[toLowerNoSpace("Channel Mode")] = 0xB0;
        cmd[toAbbreviation("Channel Mode")] = 0xB0;

        // CC command name to number
        for (n = 0; n < 256; n++)
        {
            if (cc1[n] != "Undefined")
            {
                cc_cmd[toLowerNoSpace(cc1[n])] = n;
                if (cc1[n].find(' ') != std::string::npos)
                {
                    cc_cmd[toAbbreviation(cc1[n])] = n;
                }

                if ((n < 0x14) || (n == 0x63) || (n == 0x65))
                {
                    ccx_cmd[toLowerNoSpace(ccx[n])] = n;
                    if (ccx[n].find(' ') != std::string::npos)
                    {
                        ccx_cmd[toAbbreviation(ccx[n])] = n;
                    }
                }
            }
        }
    };

    ~Help() {};

    std::string commandName(int n);
    std::string commandHelp(int n);
    std::string commandHelp1(int n);
    int commandNumber(std::string s);
    std::string ccCommandName(int n);
    std::string ccCommandHelp(int n);
    std::string ccCommandHelp1(int n);
    int ccCommandNumber(std::string s);
    int ccxCommandNumber(std::string s);
    int ccxMsbToLsb(int msb);
    std::string ccName(int n);
    std::string toString(std::vector<uint8_t> byte);
    MSG getMessageType(std::vector<uint8_t> byte);
    std::string getMessageTypeName(MSG msgtype);
    int getMessageLength(int n);

private:
    std::string byte0[256];
    std::string byte1[256];
    std::string byte2[256];
    std::string cc1[256];
    std::string cc2[256];
    std::string ccx[256];

    std::map<std::string, int> cmd;
    std::map<std::string, int> cc_cmd;
    std::map<std::string, int> ccx_cmd;

    std::string toLowerNoSpace(const std::string& in_s);
    std::string toAbbreviation(const std::string& in_s);
    std::string fmt(std::string s, int width);

    std::string msgtypename[6] = {
        "Channel Voice Message",
        "Channel Voice Message(CC)",
        "Channel Mode Message",
        "System Common Message",
        "System Realtime Message",
        "Not Status Byte"
    };
};


