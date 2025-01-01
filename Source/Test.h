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

    shouldBe({ "mider" }, P::NO_ARGS_HELP);

    shouldBe({ "mider", "devices" }, P::DEVICE);
    shouldBe({ "mider", "indevices" }, P::INDEVICE);
    shouldBe({ "mider", "outdevices" }, P::OUTDEVICE);
    shouldBe({ "mider", "1", "receive" }, P::DEV_RECEIVE);

    // Message Test
    shouldBe({ "mider", "1", "1", "NoteOn", "60", "100"}, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "noteon", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "NOTEON", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "no", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "NO", "60", "100" }, P::DEV_CH_CHANNELVOICEMSG);

    // CC Test
    shouldBe({ "mider", "1", "1", "CC", "BankSelect" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "bankselect" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "BANKSELECT" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "bs" }, P::DEV_CH_CC_CHANNELVOICEMSG);
    shouldBe({ "mider", "1", "1", "CC", "BS" }, P::DEV_CH_CC_CHANNELVOICEMSG);

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
    shouldBe({ "mider", "help", "CC", "BankSelect" }, P::HELP_CC_CCNAME);

    // Error Test
    shouldBe({ "mider", "1", "1", "NoteOn"}, P::E_SYNTAX_ERROR);
    shouldBe({ "mider", "1", "1", "NoteOn", "60"}, P::E_SYNTAX_ERROR);


    shouldBe({ "mider", "1", "1", "NoteOn", "60", "100" }, { 0x90, 60, 100 });
    shouldBe({ "mider", "1", "2", "NoteOn", "60", "100" }, { 0x91, 60, 100 });
    shouldBe({ "mider", "1", "3", "ProgramChange", "10"}, {0xC2, 10});
    shouldBe({ "mider", "1", "1", "TuneRequest" }, { 0xF6 });

    shouldBe({ "mider", "1", "2", "3", "4", "5", "6" }, { 2, 3, 4, 5, 6 });
    shouldBe({ "mider", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});


    return 0;
}

void shouldBe(std::vector<std::string> args, P expect)
{
    P actual = parser.parse(args);

    if (actual != expect)
    {
        std::cout << "[" << actual << "] != [" << expect << "] ";
        for (const auto& arg : args)
        {
            std::cout << arg << " ";
        }
        std::cout << std::endl;
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