# mider
simple command-line MIDI messaging tool

## FEATURES
- list MIDI output devices
- send MIDI message to MIDI output device
- support command name / raw bytes parameters
- support decimal, hex parameters

## Usage
```
 mider devices

 mider help
 mider help noteon
 mider help cc
 mider help cc allnotesoff

 mider device channel command [byte1 [byte2]]

 mider device byte0 byte1 byte2
```

## Command
- noteon
- noteoff
- allnoteoff
- cc

## Example
```
 mider list
 mider 1 1 noteon 60 100
 mider 1 1 noteon 3ch 64h
 mider 1 90h 3ch 64h
```

## WEBSITE
http://github.com/aike/mider

## CREDIT
mider program is licensed under MIT License.  
Contact: X @aike1000
