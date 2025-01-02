/*
  ==============================================================================

    Test.h
    Created: 29 Dec 2024 2:49:33pm
    Author:  ai

  ==============================================================================
*/

#pragma once

#ifdef TEST

void shouldBe(std::vector<std::string> args, P expect);
void shouldBe(std::vector<std::string> args, std::vector<uint8_t> ebytes);

int main()
{
    std::vector<std::string> arg;

    ///////////////////////////////////////////////////////
    /// Parser Test
    ///////////////////////////////////////////////////////

    shouldBe({ "mider" }, P::NO_ARGS_HELP);

    shouldBe({ "mider", "devices" }, P::DEVICE);
    shouldBe({ "mider", "indevices" }, P::INDEVICE);
    shouldBe({ "mider", "outdevices" }, P::OUTDEVICE);
    shouldBe({ "mider", "1", "receive" }, P::DEV_RECEIVE);

    // Channel Voice Message Test
    shouldBe({ "mider", "1", "1", "NoteOn", "60", "100"}, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "noteon", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "NOTEON", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "no", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "NO", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);

    // Channel Voice Message (CC) Test
    shouldBe({ "mider", "1", "1", "CC", "BankSelectMSB", "10" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "bankselectmsb", "10" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "BANKSELECTMSB", "10" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "bsm", "10" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "BSM", "10" }, P::DEV_CH_CC_CHANNELVOICEMSG);

    // Channel Voice Message (CCÈª\L) Test
    shouldBe({ "mider", "1", "1", "BankSelectMSB", "10" }, P::DEV_CH_CC_CHANNELVOICEMSG);

    // Channel Mode Message Test
    shouldBe({ "mider", "1", "1", "CM", "AllNotesOff", "0" }, P::DEV_CH_CM_CHANNELMODEMSG);

    // Channel Mode Message (Èª\L) Test
    shouldBe({ "mider", "1", "1", "AllNotesOff", "0" }, P::DEV_CH_CM_CHANNELMODEMSG);

    // System Common Message Test
    shouldBe({ "mider", "1", "TuneRequest" }, P::DEV_SYSTEMCOMMONMSG);

    // System Realtime Message Test
    shouldBe({ "mider", "1", "start" }, P::DEV_SYSTEMRTMSG);
    shouldBe({ "mider", "1", "SystemReset" }, P::DEV_SYSTEMRTMSG);


    // Help Test
    shouldBe({ "mider", "help" }, P::HELP);
    shouldBe({ "mider", "help", "NoteOn" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "noteon" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "NOTEON" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "no" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "NO" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "CC" }, P::HELP_CC);
    shouldBe({ "mider", "help", "CC", "BankSelectMSB" }, P::HELP_CC_CCNAME);

    // Error Test
    shouldBe({ "mider", "1", "1", "NoteOn"}, P::E_SYNTAX_ERROR);
    shouldBe({ "mider", "1", "1", "NoteOn", "60"}, P::E_SYNTAX_ERROR);
    shouldBe({ "mider", "1", "1", "CC", "BankSelectMSB" }, P::E_SYNTAX_ERROR);
    shouldBe({ "mider", "1", "1", "BankSelectMSB" }, P::E_SYNTAX_ERROR);


    ///////////////////////////////////////////////////////
    /// Message Generation Test
    ///////////////////////////////////////////////////////
    shouldBe({ "mider", "1", "TuneRequest" }, { 0xF6 });
    shouldBe({ "mider", "1", "Start" }, { 0xFA });

    // Channel Voice Message Test
    shouldBe({ "mider", "1", "1", "NoteOn", "60", "100" }, { 0x90, 60, 100 });
    shouldBe({ "mider", "1", "2", "NoteOn", "60", "100" }, { 0x91, 60, 100 });
    shouldBe({ "mider", "1", "3", "ProgramChange", "10" }, { 0xC2, 10 });

    // Channel Voice Message (CC) Test
    shouldBe({ "mider", "1", "1", "CC", "BankSelectLSB", "10" }, {0xB0, 0x20, 0x0A });

    // Channel Voice Message (CCÈª\L) Test
    shouldBe({ "mider", "1", "1", "BankSelectLSB", "10" }, { 0xB0, 0x20, 0x0A });

    // Channel Mode Message Test
    shouldBe({ "mider", "1", "1", "CM", "AllNotesOff", "0" }, { 0xB0, 0x7B, 0x00 });

    // Channel Mode Message (Èª\L) Test
    shouldBe({ "mider", "1", "1", "AllNotesOff", "0" }, { 0xB0, 0x7B, 0x00 });

    // System Common Message Test
    shouldBe({ "mider", "1", "TuneRequest" }, { 0xF6 });

    // System Realtime Message Test
    shouldBe({ "mider", "1", "start" }, { 0xFA });
    shouldBe({ "mider", "1", "SystemReset" }, { 0xFF });

    // Byte List
    shouldBe({ "mider", "1", "2", "3", "4", "5", "6" }, { 2, 3, 4, 5, 6 });
    shouldBe({ "mider", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    return 0;
}

void shouldBe(std::vector<std::string> args, P expect)
{
    P actual = parser.parse(args);

    if (actual != expect)
    {
        std::cout << "Parse Test NG: ";
        for (const auto& arg : args)
        {
            std::cout << arg << " ";
        }
        std::cout << std::endl;
        std::cout << "  actual [" << parser.getMessageTypeName(actual) << "]" << std::endl;
        std::cout << "  expect [" << parser.getMessageTypeName(expect) << "]" << std::endl;
    }
}

void shouldBe(std::vector<std::string> args, std::vector<uint8_t> ebytes)
{
    parser.parse(args);
    auto abytes = parser.getBytes();

    if (abytes == ebytes)
    {
        return;
    }

    std::cout << "Byte Test NG: ";
    for (const auto& arg : args)
    {
        std::cout << arg << " ";
    }
    std::cout << std::endl;

    std::string s;    
    s = "  actual [";
    for (int i = 0; i < abytes.size(); i++)
    {
        s += std::to_string(abytes[i]);
        if (i < abytes.size() - 1)
        {
            s += " ";
        }
    }
    s += "]";
    std::cout << s << std::endl;

    s = "  expect [";
    for (int i = 0; i < ebytes.size(); i++)
    {
        s += std::to_string(ebytes[i]);
        if (i < ebytes.size() - 1)
        {
            s += " ";
        }
    }
    s += "]";
    std::cout << s << std::endl;
}

#endif