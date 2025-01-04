# mider
シンプル・コマンドラインMIDIメッセージ送受信ツール

# 説明
MIDI対応機器やMIDI対応ソフトウェアの検証や制御のためのコマンドラインツールです。  
詳細なヘルプや実行時の表示により、仕様書を参照せずともMIDIメッセージを送信することが簡単にできます。  
豊富な省略記法が用意されており、短い入力でコマンドを実行することができます。  
MIDI2.0には対応していません。


## 特徴
- MIDIデバイス一覧表示
- MIDI出力デバイスにMIDIメッセージ送信
- MIDI入力デバイスからMIDIメッセージ受信
- コマンド名またはバイト列による指定
- 10進、16進数(XXh表記、0xXX表記)の数値指定
- コマンド名は大文字小文字を区別しない
- ほとんどのコマンド名は頭文字による略称指定可能

## 使用方法

### MIDIデバイス一覧表示
```
  mider [devices|indevices|outdevices]
```

### MIDIメッセージ送信
```
  mider <dev_no> <ch_no> <message_name> [byte ...]
  mider <dev_no> <message_name> [byte ...]
  mider <device> <ch_no> channelmode <channelmode_message_name> <byte>
  mider <device> <ch_no> controlchange <controlchange_name> <byte>
  mider <dev_no> sox [byte ...] eox
  mider <dev_no> <byte> [byte ...]
```

### MIDI CC MSB/LSBメッセージ送信のための拡張構文
```
  mider <dev_no> <ch_no> controlchange <controlchange_name> <msb> <lsb>
```
CC MSBメッセージと、CC LSBメッセージを送信する。

### MIDIメッセージ受信サーバ起動
```
  mider <dev_no> receive
```

### ヘルプ表示
```
  mider help
  mider help <message_name>
  mider help controlchange
  mider help controlchange <controlchange_name>
  mider help channelmode
  mider help channelmode <channelmode_message_name>
  mider help [receive|devices|indevices|outdevices|version]
```

### バージョン表示
```
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

## ビルド方法
1. mider.jucerをJUCEのProjucerで開く
2. IDE(Visual Studio / Xcode)用のプロジェクトファイルを生成する
3. 生成されたプロジェクトファイルをIDEで開き、プロジェクトをビルドする

## 公式サイト
http://github.com/aike/mider

## クレジット
mider program is licensed under MIT License.  
Contact: X @aike1000
