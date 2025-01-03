# mider
シンプル・コマンドラインMIDIメッセージ送受信ツール

# 説明
MIDI対応機器やMIDI対応ソフトウェアの検証や制御のためのコマンドラインツールです。  
詳細なヘルプや実行時の表示により、仕様書を参照せずともMIDIメッセージを送信することが簡単にできます。  
豊富な省略記法が用意されており、短い入力でコマンドを実行することができます。


## 特徴
- MIDIデバイス一覧表示
- MIDI出力デバイスにMIDIメッセージ送信
- MIDI入力デバイスからMIDIメッセージ受信
- コマンド名またはバイト列による指定
- 10進、16進数(XXh表記、0xXX表記)の数値指定
- コマンド名は大文字小文字を区別しない
- ほとんどのコマンド名は頭文字による略称指定可能

## 記法

### MIDIメッセージ送信
```
 mider device channel channel_voice_message_command [byte ...]
 mider device channel "ControlChange" control_change_command byte
 mider device channel "ChannelMode" channel_mode_message_command byte
 mider device system_common_message_command [byte ...]
 mider device system_realtime_message_command [byte ...]
 mider device byte "SOX" [byte ...] "EOX"
 mider device byte [byte ...]
```

### MIDI CC MSB/LSBメッセージ送信のための拡張構文
```
 mider device channel "ControlChange" control_change_command MSB LSB
```
CC MSBメッセージと、CC LSBメッセージを送信する。


### MIDIメッセージ受信サーバ起動
```
 mider device "receive"
```

### MIDIデバイス一覧表示
```
 mider "devices"
 mider "indevices"
 mider "outdevices"
```

### ヘルプ表示
```
 mider "help"
 mider "help" command
 mider "help" "ControlChange"
 mider "help" "ControlChange" controlchange_command
 mider "help" "ChannelMode"
 mider "help" "ChannelMode" channelmode_command
```

### バージョン表示
```
 mider --version
 mider version
```

## コマンド例
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

## 公式サイト
http://github.com/aike/mider

## クレジット
mider program is licensed under MIT License.  
Contact: X @aike1000
