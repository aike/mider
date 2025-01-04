# mider
simple command-line MIDI messaging tool

## DESCRIPTION
This is a command line tool for testing and controlling MIDI devices and software.  
With detailed help and runtime display, you can easily send MIDI messages without referring to the specification documents.  
Many abbreviations are available, allowing you to execute commands with short inputs.  
Not support MIDI 2.0.

## FEATURES
- list MIDI devices
- send MIDI message to MIDI output device
- receive MIDI message from MIDI input device
- support command name / raw bytes parameters
- support decimal / hex(XXh sytle and 0xXX style) number parameters
- commands are case-insensitive
- most commands can use acronyms

## Usage

### List MIDI Devices
```
  mider [devices|indevices|outdevices]
```

### Send MIDI Message
```
  mider <dev_no> <ch_no> <message_name> [byte ...]
  mider <dev_no> <message_name> [byte ...]
  mider <device> <ch_no> channelmode <channelmode_message_name> <byte>
  mider <device> <ch_no> controlchange <controlchange_name> <byte>
  mider <dev_no> sox [byte ...] eox
  mider <dev_no> <byte> [byte ...]
```

### Extended Syntax for Sending CC MSB/LSB Message
```
  mider <dev_no> <ch_no> controlchange <controlchange_name> <msb> <lsb>
```
Send a CC MSB message and a CC LSB message.


### Launch MIDI Receive Server
```
  mider <dev_no> receive
```

### Show Help
```
  mider help
  mider help <message_name>
  mider help controlchange
  mider help controlchange <controlchange_name>
  mider help channelmode
  mider help channelmode <channelmode_message_name>
  mider help [receive|devices|indevices|outdevices|version]
```

### Show Version
```
 mider version
```

## EXAMPLE
```
 mider version
 mider devices
 mider 1 receive
 mider 1 1 NoteOn 60 100
 mider 1 1 noteon 0x3C 0x64
 mider 1 90h 3Ch 64h
 mider 1 16 ControlChange BankSelectMSB 10
 mider 1 16 CC BankSelectMSB 10
 mider 1 16 BankSelectMSB 10
 mider 1 16 BSM 10
 mider 1 16 CC BankSelect 10 20
 mider 1 16 BankSelect 10 20
 mider 1 16 BS 10 20 
 mider 1 5 ChannelMode AllNotesOff 0
 mider 1 5 CM AllNotesOff 0
 mider 1 5 AllNotesOff 0
 mider 1 5 ANO 0
 mider 1 TuneRequest
 mider 1 Start
 mider 1 SOX 7Dh 01h 02h 03h 04h EOX
 mider help
 mider help noteon
 mider help cc
 mider help cc bankselect
 mider help cm
 mider help cm allnotesoff
```

## BUILD
1. Open the mider.jucer file in JUCE Projucer application.
2. Generate the project files for your IDE (Visual Studio / Xcode).
3. Open the generated project in your IDE and build the project.

## WEBSITE
http://github.com/aike/mider

## CREDIT
mider program is licensed under MIT License.  
Contact: X @aike1000
