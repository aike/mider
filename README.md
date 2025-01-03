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

### Send MIDI Message
```
 mider device channel channel_voice_message_command [byte ...]
 mider device channel "ControlChange" control_change_command byte
 mider device channel "ChannelMode" channel_mode_message_command byte
 mider device system_common_message_command [byte ...]
 mider device system_realtime_message_command [byte ...]
 mider device byte "SOX" [byte ...] "EOX"
 mider device byte [byte ...]
```

### Send MIDI Message Extended Syntax for CC MSB/LSB
```
 mider device channel "ControlChange" control_change_command MSB LSB
```
Sends a CC MSB message and a CC LSB message.


### Launch MIDI Receive Server
```
 mider device "receive"
```

### List MIDI Devices
```
 mider "devices"
 mider "indevices"
 mider "outdevices"
```

### Show Help
```
 mider "help"
 mider "help" command
 mider "help" "ControlChange"
 mider "help" "ControlChange" controlchange_command
 mider "help" "ChannelMode"
 mider "help" "ChannelMode" channelmode_command
```

### Show Version
```
 mider --version
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

## WEBSITE
http://github.com/aike/mider

## CREDIT
mider program is licensed under MIT License.  
Contact: X @aike1000
