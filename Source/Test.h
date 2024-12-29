/*
  ==============================================================================

    Test.h
    Created: 29 Dec 2024 2:49:33pm
    Author:  ai

  ==============================================================================
*/

#pragma once

#ifdef TEST

void shouldBe(std::vector<std::string> arg, P expect);


int main()
{
    std::vector<std::string> arg;

    shouldBe({ "mider" }, P::NO_ARGS_HELP);

    shouldBe({ "mider", "devices" }, P::DEVICE);

    shouldBe({ "mider", "1", "1", "NoteOn" }, P::DEV_CH_MSGNAME);
    shouldBe({ "mider", "1", "1", "noteon" }, P::DEV_CH_MSGNAME);
    shouldBe({ "mider", "1", "1", "NOTEON" }, P::DEV_CH_MSGNAME);
    shouldBe({ "mider", "1", "1", "no" }, P::DEV_CH_MSGNAME);
    shouldBe({ "mider", "1", "1", "NO" }, P::DEV_CH_MSGNAME);

    shouldBe({ "mider", "1", "1", "CC", "BankSelect" }, P::DEV_CH_CC_CCNAME);
    shouldBe({ "mider", "1", "1", "CC", "bankselect" }, P::DEV_CH_CC_CCNAME);
    shouldBe({ "mider", "1", "1", "CC", "BANKSELECT" }, P::DEV_CH_CC_CCNAME);
    shouldBe({ "mider", "1", "1", "CC", "bs" }, P::DEV_CH_CC_CCNAME);
    shouldBe({ "mider", "1", "1", "CC", "BS" }, P::DEV_CH_CC_CCNAME);

    shouldBe({ "mider", "help" }, P::HELP);
    shouldBe({ "mider", "help", "NoteOn" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "noteon" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "NOTEON" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "no" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "NO" }, P::HELP_MSGNAME);
    shouldBe({ "mider", "help", "CC" }, P::HELP_CC);
    shouldBe({ "mider", "help", "CC", "BankSelect" }, P::HELP_CC_CCNAME);

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


#endif