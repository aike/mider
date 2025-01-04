# mider
�V���v���E�R�}���h���C��MIDI���b�Z�[�W����M�c�[��

# ����
MIDI�Ή��@���MIDI�Ή��\�t�g�E�F�A�̌��؂␧��̂��߂̃R�}���h���C���c�[���ł��B  
�ڍׂȃw���v����s���̕\���ɂ��A�d�l�����Q�Ƃ����Ƃ�MIDI���b�Z�[�W�𑗐M���邱�Ƃ��ȒP�ɂł��܂��B  
�L�x�ȏȗ��L�@���p�ӂ���Ă���A�Z�����͂ŃR�}���h�����s���邱�Ƃ��ł��܂��B  
MIDI2.0�ɂ͑Ή����Ă��܂���B


## ����
- MIDI�f�o�C�X�ꗗ�\��
- MIDI�o�̓f�o�C�X��MIDI���b�Z�[�W���M
- MIDI���̓f�o�C�X����MIDI���b�Z�[�W��M
- �R�}���h���܂��̓o�C�g��ɂ��w��
- 10�i�A16�i��(XXh�\�L�A0xXX�\�L)�̐��l�w��
- �R�}���h���͑啶������������ʂ��Ȃ�
- �قƂ�ǂ̃R�}���h���͓������ɂ�闪�̎w��\

## �g�p���@

### MIDI�f�o�C�X�ꗗ�\��
```
  mider [devices|indevices|outdevices]
```

### MIDI���b�Z�[�W���M
```
  mider <dev_no> <ch_no> <message_name> [byte ...]
  mider <dev_no> <message_name> [byte ...]
  mider <device> <ch_no> channelmode <channelmode_message_name> <byte>
  mider <device> <ch_no> controlchange <controlchange_name> <byte>
  mider <dev_no> sox [byte ...] eox
  mider <dev_no> <byte> [byte ...]
```

### MIDI CC MSB/LSB���b�Z�[�W���M�̂��߂̊g���\��
```
  mider <dev_no> <ch_no> controlchange <controlchange_name> <msb> <lsb>
```
CC MSB���b�Z�[�W�ƁACC LSB���b�Z�[�W�𑗐M����B

### MIDI���b�Z�[�W��M�T�[�o�N��
```
  mider <dev_no> receive
```

### �w���v�\��
```
  mider help
  mider help <message_name>
  mider help controlchange
  mider help controlchange <controlchange_name>
  mider help channelmode
  mider help channelmode <channelmode_message_name>
  mider help [receive|devices|indevices|outdevices|version]
```

### �o�[�W�����\��
```
  mider version
```

## �R�}���h��
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

## �r���h���@
1. mider.jucer��JUCE��Projucer�ŊJ��
2. IDE(Visual Studio / Xcode)�p�̃v���W�F�N�g�t�@�C���𐶐�����
3. �������ꂽ�v���W�F�N�g�t�@�C����IDE�ŊJ���A�v���W�F�N�g���r���h����

## �����T�C�g
http://github.com/aike/mider

## �N���W�b�g
mider program is licensed under MIT License.  
Contact: X @aike1000
