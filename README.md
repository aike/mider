# mider
simple command-line MIDI messaging tool

## FEATURES
- list MIDI output devices
- send MIDI message to MIDI output device
- support command name / raw bytes parameters
- support decimal, hex parameters

## Usage

### Send MIDI Message
```
 mider device channel command [byte ...]
 mider device channel "ControlChange" command byte
 mider device channel "ChannelMode" command byte
 mider device byte [byte ...]
```

### Launch MIDI Receive Server
```
 mider device receive
```

### List MIDI Devices
```
mider devices
 mider indevices
 mider outdevices
```

### Show Help
```
 mider help
 mider help command
 mider help "ControlChange"
 mider help "ControlChange" controlchange_command
 mider help "ChannelMode"
 mider help "ChannelMode" channelmode_command
```

### Show Version
```
 mider --version
```

## Example
```
 mider version
 mider devices
 mider 1 receive
 mider 1 1 NoteOn 60 100
 mider 1 1 noteon 3ch 64h
 mider 1 90h 3ch 64h
 mider 1 16 ControlChange BankSelectMSB 10
 mider 1 16 CC BankSelectMSB 10
 mider 1 16 BankSelectMSB 10
 mider 1 16 BSM 10
 mider 1 5 ChannelMode AllNotesOff 0
 mider 1 5 CM AllNotesOff 0
 mider 1 5 AllNotesOff 0
 mider 1 5 ANO 0
 mider 1 Start
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
