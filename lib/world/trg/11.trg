#1100
����� � ����������~
2 e 100
~
if %actor.level% > 30
halt
end
if !%actor.name%
halt
end
if %actor.rentable% == 0
wecho %actor.iname% ��������� ����� ����, �� &R�����&n �������� ����� �� ������ � ���� ���.
wzoneecho 1101 %actor.iname% � &R�����&n, ��� ������������ �������� �������! %actor.iname%, �������� �����!
wsend %actor% &R�����&n � ��� ��� �� ������ ���� � ���� ������� �����������!
return 0
halt
end
wait 1s
wecho - ����� ���������� �� ���� �����.
wecho - ����� ��������� ������ ����� ������ �� ����� ������ �����!
wecho - ���� �� ������ �������� ���������� �� ��� - � ��� ��� ���, ������ �������� -
wecho - "&R��������&n"
if %actor.class% == 8
halt
end
if %actor.race% == 3
eval buffer %actor.hitp(%actor.maxhitp%)%
halt
end
if %actor.hitp% > %actor.maxhitp%
eval buffer %actor.hitp(%actor.maxhitp%)%
wzoneecho 1101 &R�����&W %actor.dname%, ���������� �� ����� �� ��������� �������� ������, ��� ������ �������������! &n
end
~
#1101
�������� ���� � ���������~
0 c 100
��������~
if %actor.vnum% != -1
halt
end
if !%actor.name%
halt
end
if %actor.iname% == ������
mload obj 1095
���� ������� ������
halt
end
if %actor.iname% == ��������
mload obj 1150
���� ����� ��������
halt
end
if %actor.level% > 30
say ������� ���, %actor.iname%, �� ����, ��� ���� ���� ��� ???
halt
end
wait 1
  ��� 
  say ����������, ������� ��� �� ���� ����������...
wait 1  
  say ���, �������, ��� ��������.
wait 1
    if (%actor.class% == 2)
say �� ����, ��� ���������� ��� ����. 
mload obj 1103
mload obj 1104
mload obj 1104
mload obj 1105
mload obj 1108
mload obj 1115
���� ��� %actor.name%
���� ������ %actor.name%
���� ������ %actor.name%
���� ���� %actor.name%
���� ����� %actor.name%
���� ���� %actor.name%
���� %actor.iname% ������ - ��� ������ ���������� ��� ��������� ���� �� ������. ��������� ��� � ����.
mload obj 1109
���� ������ %actor.name%
      elseif (%actor.class% == 3)
say �� ��������, ��� ���������� ��� ����.
mload obj 1100
mload obj 1101
mload obj 1103
mload obj 1104
mload obj 1105
mload obj 1106
mload obj 1107
mload obj 1108
���� ��� %actor.name%
���� ����� %actor.name%
���� ��� %actor.name%
���� ������ %actor.name%
���� ���� %actor.name%
���� ������ %actor.name%
���� ��������� %actor.name%
���� ����� %actor.name%
mload obj 1116
mload obj 1109
mload obj 1126
���� ������ %actor.name%
���� ���� %actor.name%
���� ��� %actor.iname%
      elseif (%actor.class% == 4) 
say �� �������, ��� ������ ��� ����.
mload obj 1103
mload obj 1104
mload obj 1104
mload obj 1105
mload obj 1108
mload obj 1115
���� ��� %actor.name%
���� ������ %actor.name%
���� ������ %actor.name%
���� ���� %actor.name%
���� ����� %actor.name%
���� ������ %actor.name%
���� %actor.iname% ������ - ��� ������ ���������� ��� ��������� ���� �� ������. ��������� ��� � ����.
mload obj 1109
���� ������ %actor.name%
      elseif (%actor.class% == 5) 
say �� ���������, ��� ������ ��� ����.
mload obj 1100
mload obj 1101
mload obj 1103
mload obj 1104
mload obj 1105
mload obj 1106
���� ��� %actor.name%
���� ����� %actor.name%
���� ��� %actor.name%
���� ������ %actor.name%
���� ���� %actor.name%
���� ������ %actor.name%
mload obj 1109
���� ������ %actor.name%
mload obj 1118
mload obj 1126
���� ��� %actor.iname%
���� ��� %actor.iname%
     elseif (%actor.class% == 9) 
say �� ������, ��� ������ ��� ����.
mload obj 1106
mload obj 1100
mload obj 1101
mload obj 1103
mload obj 1104
mload obj 1105
mload obj 1107
mload obj 1108
���� ����� %actor.iname%
���� ��� %actor.name%
���� ����� %actor.name%
���� ��� %actor.name%
���� ������ %actor.name%
���� ���� %actor.name%
���� ��������� %actor.name%
���� ����� %actor.name%
mload obj 1109
���� ������ %actor.name%
mload obj 1118
mloa obj 1126
���� ��� %actor.iname%
���� ��� %actor.name% 
      elseif (%actor.class% == 10)
say �� �������, ��� ������ ��� ����.
mload obj 1102
���� ��� %actor.name%
mload obj 1100
mload obj 1101
mload obj 1103
mload obj 1104
mload obj 1105
mload obj 1106
���� ��� %actor.name%
���� ����� %actor.name%
���� ��� %actor.name%
���� ������ %actor.name%
���� ���� %actor.name%
���� ������ %actor.name%
mload obj 1109
���� ������ %actor.name%
mload obj 1118
���� ��� %actor.name% 
mload obj 1126
���� ��� %actor.iname%
     elseif (%actor.class% == 11) 
say �� ������, ��� ������ ��� ����.
mload obj 1100
mload obj 1101
mload obj 1103
mload obj 1104
mload obj 1105
mload obj 1106
mload obj 1107
mload obj 1108
���� ��� %actor.name%
���� ����� %actor.name%
���� ��� %actor.name%
���� ������ %actor.name%
���� ���� %actor.name%
���� ������ %actor.name%
���� ��������� %actor.name%
���� ����� %actor.name%
mload obj 1117
���� ����� %actor.iname%
���� %actor.iname% ������  - ���� ������� ���������� ��� ��������� �������� ����������. ��������� �� � ����.
mload obj 1109
���� ������ %actor.name%
mload obj 1118
���� ��� %actor.name% 
mload obj 1126
���� ��� %actor.iname%
         elseif (%actor.class% == 12) 
say �� �����, ��� ������ ��� ����.
mload obj 1106
mload obj 1103
mload obj 1104
mload obj 1104
mload obj 1105
mload obj 1108
���� ����� %actor.iname%
���� ��� %actor.name%
���� ������ %actor.name%
���� %actor.iname% ��������� ������ ���������� ��� ������� ����� ���������� �����. ��������� ��� � ����.
���� ������ %actor.name%
���� ���� %actor.name%
���� ����� %actor.name%
mload obj 1109
���� ������ %actor.name%
mload obj 1126
���� ��� %actor.iname%
  * ��� ��� �������
 else
say �, �� ��� ����� � ���� ��������� ����� ����... ���, �����!
mload obj 1111
mload obj 1112
���� ����� .%actor.iname%
���� ���� .%actor.iname%
end
wait 1
say �����, ����, %actor.iname%
���� ���
~
#1102
��� �����~
2 e 100
~
if %actor.level% > 30
halt
end
if !%actor.name%
halt
end
if %actor.rentable% == 1
halt
end
wecho %actor.iname% ��������� ����� ����, �� &R�����&n �������� ����� �� ������ � ���� ���.
wzoneecho 1101 %actor.iname% � &R�����&n, ��� ������������ �������� �������! %actor.iname%, �������� �����!
wsend %actor% &R�����&n � ��� ��� �� ������ ���� � ���� ������� �����������!
return 0
~
#1103
����� � ���������� (�������)~
0 r 100
~
if %actor.level% > 30
halt
end
if !%actor.name%
halt
end
wait 1
say �, ������ ���� ������� ����, ��� � ���� ���� ������?
say ���� �� ������ ���������� �������� - ����� ���������� ��� ���, �� ���� ���������, �� ������ ������� ���� ���������, ��� ���� ���������� �� �����.
say ��, ���� �� �����, ���� ������� ��� ������ �����������, �������, ������� ������� ��� ���������!
say ����������
dg_cast '� ���' .%actor.iname%
dg_cast '� ����' .%actor.iname%
say ��, ������, � ������ ��������� ����.
wait 1
~
#1104
������ ���� ������ � ����~
2 g 100
~
if !%actor.iname%
halt
end
if %actor.level% <31
wsend %actor% �� �� ����� �����������, ���� ������ ���� ������!
return 0
halt
end
~
#1105
������ ����� (���� ������)~
2 g 100
~
if !%actor.name%
halt
end
if %actor.level% > 30
halt
end
if %actor.class% != 8
wecho %actor.iname% ��������� ����� ����, �� ���������.
wsend %actor% -�� ������ ����� � ������ ������, �� ������� ������������.
wsend %actor% -������ ��� �� ��� ���.
return 0
halt
end
    * * * ���� ������ - ��� ������ ����� ��� �������� :)
wait 1s
wecho �������� �� ����� ������� ������ �������, � ��� �� ����� � ����� ���!
wload mob 1119
wdamage ������� 100
wecho �������� �� ����� ������� ������ ��������, � ��� �� ����� � ����� ���!
wload mob 1112
wdamage ������� 100
wecho �������� �� ����� ������� ������ ������, � ��� �� ����� � ����� ���!
wload mob 1113
wdamage ������ 100
wecho �������� �� ����� ������� ������ �������, � ��� �� ����� � ����� ���!
wload mob 1114
wdamage ������ 100
end
~
#1106
����� � ������� (���)~
0 r 100
~
if !%actor.name%
halt
end
if %actor.level% > 30
halt
end
wait 1
say ������, �������� ����! �� �������� �������? ������, ��� ���������!
msend %actor% ������� ���� ��� ��������� ������� ������� � �������, ������ �� ��� ����� �� �������� ����!
dg_cast '�����' .%actor.iname%
mechoaround %actor% ������� ������� � %actor.dname% � �������� ��� ����������� ��������.
wait 1
�� .%actor.iname%
~
#1107
����� � �������� (���, ����� ������)~
0 r 100
~
if !%actor.name%
halt
end
if %actor.level% > 30
halt
end
wait 1
mecho ������� �������� �������� ������ ���� � ������� � ��������� ������� �������.
dg_cast '���' .%actor.iname%
dg_cast '�� �����' .%actor.iname%
dg_cast '�� ����' .%actor.iname%
dg_cast '� ��' .%actor.iname%
dg_cast '�� ����' .%actor.iname%
dg_cast '��� ���' .%actor.iname%
dg_cast '�����' .%actor.iname%
dg_cast '�� ����' .%actor.iname%
mecho ������� ����� ������� ���� � ����������� ����� �� �����.
~
#1108
����� � ������~
0 r 100
~
wait 1
mecho %self.iname% �������� ����������, ������� ���...
wait 1
mecho ...� ������-������ �������� ����������, ������� �������� ������
~
#1109
�������� ������ ����� (31+)~
2 c 100
��������~
if !%actor.name%
halt
end
if %actor.level% < 31
wsend %actor% �� ������ � ��� ���� �������, ���� ��� ����� ��������!
halt
end
      * �� ���� ������������ �������� ����� 
      * ��� ����� - ��� ����������� ������������ :���
   wdoor  1101 north purge
   wdoor  1101 north flags a 
   wdoor  1101 north room  1103
   wdoor  1101 north description �����
   wdoor  1103 south purge
   wdoor  1103 south flags a
   wdoor  1103 south room  1101
   wdoor  1103 south description �����
wechoaround %actor% %actor.iname% �������� ����� - � ...
wsend %actor% �� ��������� ����� - � ...
wat 1101 wecho ...������� ����� � &Y�������&n ����������� ����������.
wat 1103 wecho ...������� ����� � &Y�������&n ����������� ����������.
~
#1110
�������� ������ ����� (31+)~
2 c 100
��������~
if !%actor.name%
halt
end
if %actor.level% < 31
wsend %actor% �� ������ � ��� ���� �������, ���� ��� ����� ��������!
halt
end
      * �� ���� ������������ �������� ����� 
   wdoor  1101 north flags a b c d
   wdoor  1101 north description �����
   wdoor  1103 south flags a b c d
   wdoor  1103 south description �����
wechoaround %actor% %actor.iname% �������� ����� - � ...
wsend %actor% �� ��������� ����� - � ...
wat 1101 wecho ...������� ����� � &Y�������&n ����������� �����������.
wat 1103 wecho ...������� ����� � &Y�������&n ����������� �����������.
~
#1111
������� ������ (��� �������)~
2 c 100
�������~
if !%actor.name%
halt
end
wzoneecho 1101 &W%actor.iname% ������� ���� &R�����������&W � ����� ��������� � ���� ������.&n
wsend %actor% ������� ����� ��������� ��� � ���� � �����.
wechoaround %actor% ������� ����� ��������� %actor.rname% � ���� � �����.
   wteleport %actor% 1101
~
#1112
����� ����� � �������� (������)~
2 e 100
~
if !%actor.name%
halt
end
if %actor.level% > 30
halt
end
if %actor.class% != 12
halt
end
wait 1s
if %world.curmobs(1103)% < 4
wload mob 1103
wecho ������� �������� �������� �������� ����� ����� ���� ����� �� �� ��������.
end
if %world.curmobs(1104)% < 4
wload mob 1104
wecho ������ ����� �������� �������� ����� ����� ���� ����� �� �� ��������.
end
if %world.curmobs(1105)% < 4
wload mob 1105
wecho ������ ������� �������� �������� ����� ����� ���� ����� �� �� ��������.
end
~
#1113
�������� ������ ��� �����.~
0 n 100
~
wait 1s
mecho %self.iname% ������ ��������� ������� �������� � � ��� ����� �������� ��������� ���������!
mload obj 1107
���� ���
~
#1114
���������� ���������~
0 f 100
~
mload mob 1120
mecho ����� ��������� ��������� �������� �� ����...
~
#1115
�������~
2 c 100
��3~
if !%actor.name%
halt
end
if %actor.level% < 31
wsend %actor% ������ ������������� ������� ����� ������������ ��� �������!
halt
end
wait 1s
wzoneecho 1101 &W �� ���������� �������� 3 ������. &R�������������!!!&n
wait 2s
wzoneecho 1101 &W �� ���������� �������� 3 ������. &R����� �����!!!&n
wait 60s
wzoneecho 1101 &W �������� &R2&W ������.&n
wait 60s
wzoneecho 1101 &W �������� &R1&W ������.&n
wait 45s
wzoneecho 1101 &W �������� &R15&W ������.&n
wait 5s
wzoneecho 1101 &W �������� &R10&W ������.&n
wait 5s
wzoneecho 1101 &W �������� &R5&W ������.&n
wait 5s
wzoneecho 1101 &W����� �� ���������� &R�����!!!&n
~
#1116
�������� �������~
2 c 100
��0~
if !%actor.name%
halt
end
if %actor.level% < 31
wsend %actor% ������ ������������� ������� ����� ������������ ��� �������!
halt
end
wait 1s
wzoneecho 1101 &W ���������� &R���������&W, ��� ��������� ������!!!&n
detach 1115 %self.id%
attach 1115 %self.id%
~
#1117
�������� ������~
2 c 100
��10~
if !%actor.name%
halt
end
if %actor.level% < 31
wsend %actor% ������ ������������� ������� ����� ������������ ��� �������!
halt
end
wait 1s
wzoneecho 1101 &W �� ������ ��� �������� 10 ������! &R�������������!!!&n
wait 1s
wzoneecho 1101 &W �� ������ ��� �������� &R9&W ������!&n
wait 1s
wzoneecho 1101 &W �� ������ ��� �������� &R8&W ������!&n
wait 1s
wzoneecho 1101 &W �� ������ ��� �������� &R7&W ������!&n
wait 1s
wzoneecho 1101 &W �� ������ ��� �������� &R6&W ������!&n
wait 1s
wzoneecho 1101 &W �� ������ ��� �������� &R5&W ������!&n
wait 1s
wzoneecho 1101 &W �� ������ ��� �������� &R4&W �������!&n
wait 1s
wzoneecho 1101 &W &R3&W �������!!!&n
wait 1s
wzoneecho 1101 &W &R2&W �������!!!&n
wait 1s
wzoneecho 1101 &W &R1&W �������!!!&n
wait 1s
wzoneecho 1101  _______&R���������!!!&n
~
#1118
���� �������� ������ (���� ����)~
2 e 100
~
if !%actor.name%
halt
end
if %actor.level% > 30
halt
end
wait 1s
if %world.curmobs(1100)% < 4
wload mob 1100
wecho ������ ����������� �������� ���������� ���������� � ������ ��� �������� � ������ �������.
end
if %world.curmobs(1107)% < 4
wload mob 1107
wecho ������ ����������� �������� ���������� ���������� � �������� ���� �������� � ������ �������.
end
if %world.curmobs(1102)% < 4
wload mob 1102
wecho ������ ����������� �������� ���������� ���������� � ����������� ������� �������� � ������ �������.
end
~
#1119
���� ����� (���� ����)~
2 e 100
~
if !%actor.name%
halt
end
if %actor.level% > 30
halt
end
wait 1s
if %world.curmobs(1101)% < 5
wload mob 1101
wecho ���-�� ������� ��������� ����� ������... � ����� ���� ������ ����� ������ ���������� ���� �������� �����!
end
~
#1120
����� � ���������~
0 r 100
~
if !%actor.name%
halt
end
if %actor.level% > 30
halt
end
wait 1s
say �����������, � ����� ����� � ���� ���� ��� ������ ���������� �� ����������� � ��������� ��������.
say ����������, ������������ � �������, �� ��� �������� �������� �������!
say �� 700 ��� - ������ ��� ���������� ������� 2 �� 2, ������������ 12 ��� 2004 ���� � 20.00 �� ���!
say �� 1000 ��� - ���������� ������� 4 �� 4, ������������ 23 ��� 2004 ���� � 15.00 �� ���!
mecho %self.iname% ��������� ��� ������, ���������� � ������.
~
#1121
���� ������ ���������~
0 m 0
~
wait 1s
if %amount% == 500
mecho %self.iname% ������� ������� � �������, � ������ ���� �� ���.
mload obj 1121
���� ������ .%actor.iname%
���� ������
halt
end
if %amount% == 700
mecho %self.iname% ������� ������� � �������, � ������ ���� �� ���.
mload obj 1127
���� ������ .%actor.iname%
���� ������
halt
end
if %amount% == 1000
mecho %self.iname% ������� ������� � �������, � ������ ���� �� ���.
mload obj 1128
���� ������ .%actor.iname%
���� ������
halt
end
say �� %amount% ��� � ������ �� ������... �� ���� �������� ���� �����.
���
halt
end
~
#1122
�� �������.~
2 e 100
~
if !%actor.name% || %actor.iname% == ��������
return 0
halt
end
wsend %actor% �� ����� ������ �����, � ������, ��������� ����� ���������.
wat 1159 wecho %actor.iname% ����� ������� ���� � ������ �� ��������� �������.
~
#1123
���� ���������~
1 c 4
������~
if !%arg.contains(��������)%
osend %actor% ��� �� ��� ����� ��� �� ��������!?
halt
end
osend %actor% �� ������� ������� �������� ������� ��������! ����� �� ���!?
oechoaround %actor% %actor.iname% ������� ������ �������� ������� ��������!
opurge %self%
~
#1124
��������� �������~
1 j 100
~
if !%actor.name%
halt
end
oechoaround %actor% %actor.iname% ��������� �������%actor.u% � ������, � ����������%actor.u% �� ���.
osend %actor% �� ��������� ���������� � ������, � ������������� �� ���.
~
#1125
���� �� �������~
1 j 100
~
if !%actor.name%
halt
end
oechoaround %actor% %actor.iname% �������%actor.g% ������ �� ������� � �������� ������%actor.g% �����.
osend %actor.iname% �� �������� ������ �� ������� � �������� ������� �����.
~
#1126
���� �� �������~
1 j 100
~
if !%actor.name%
halt
end
oechoaround %actor% %actor.iname% � ������� ���������%actor.g% �� �������. � �� ��� ����?
osend %actor% �� � ������� ���������� �� �������. �������-�� ����� ���������!
~
#1140
��������� ������� - �����~
1 gj 100
~
if !%actor.name% || %actor.level% > 30
halt
end
**������ ���������� ����**
if %self.vnum% == 1130
if %actor.clan% == ���
halt
end
end
**��������� ���������� ����**
if %self.vnum% == 1131
halt
end
**������ ���������� �����**
if %self.vnum% == 1132
if %actor.clan% == ���
halt
end
end
**������ ���������� ����**
if %self.vnum% == 1133
if %actor.iname% == ������
halt
end
end
**������ ���������� �������**
if %self.vnum% == 1134
if %actor.iname% == �������
halt
end
end
**��������� ���������� ������**
if %self.vnum% == 1135
if %actor.clan% == ���
halt
end
end
**������ ������ ���������**
if %self.vnum% == 1136
if %actor.clan% == ���
halt
end
end
**������ ������ ����������**
if %self.vnum% == 1137
if %actor.iname% == �����
halt
end
end
************************
*��������� �� ����� �����*
************************
return 0
osend %actor% �������� �����, �� ���� �� �� �������� ���� ������!
~
#1150
�������� �� ����� ���������~
1 c 4
��������~
if !%actor.name%
halt
end
if %actor.iname% != ��������
osend %actor% ��� ���� �������! �� ��� �����! ������� ��� � ������ ������� ������!!!
halt
end
oechoaround %actor% %actor.iname% �� ������� ���������� ������ �� &Y���������&W�&Y ����&W�&n.
osend %actor% �� �� ������� ���������� ������� �� &Y���������&W�&Y ����&W�&n.
oecho �� ������� ��...
wait 5s
end
~
#1151
������ ����� ����� ���������~
1 g 100
~
if !%actor.name%
return 0
halt
end
if %actor.iname% == ��������
halt
else
return 0
osend %actor% �������� ����� � ������� �������� ��������... ��� ��� ��������!
halt
end
~
#1152
��������� ������ ���������~
1 c 1
�����~
wait 1
if %arg.car% == �������
eval prefix %arg.cdr%
global prefix
halt
end
if !%prefix%
halt
end
set color %arg.car%
switch %color%
case 1
oecho %prefix% : '&r%arg.cdr%&n'
break
case 2
oecho %prefix% : '&R%arg.cdr%&n'
break
case 3
oecho %prefix% : '&g%arg.cdr%&n'
break
case 4
oecho %prefix% : '&G%arg.cdr%&n'
break
case 5
oecho %prefix% : '&K%arg.cdr%&n'
break
case 6
oecho %prefix% : '%arg.cdr%'
break
case 7
oecho %prefix% : '&c%arg.cdr%&n'
break
case 8
oecho %prefix% : '&C%arg.cdr%&n'
break
case 9
oecho %prefix% : '&b%arg.cdr%&n'
break
case 10
oecho %prefix% : '&B%arg.cdr%&n'
break
case 11
oecho %prefix% : '&m%arg.cdr%&n'
break
case 12
oecho %prefix% : '&M%arg.cdr%&n'
break
case 13
oecho %prefix% : '&W%arg.cdr%&n'
break
case 14
oecho %prefix% : '&y%arg.cdr%&n'
break
case 15
oecho %prefix% : '&Y%arg.cdr%&n'
break
default
oecho %prefix% : '%arg.cdr%'
done
~
#1156
������� ����~
1 hi 100
~
if %victim.vnum%  !=  -1
osend %actor% ���! ���� �������!
return 0
halt
end
~
#1160
new trigger~
1 g 100
~
halt
~
#1180
��������� ��������~
1 c 100
� �� ��� ���� ����� n no nor nort north~
return 0
osend %actor% ���
~
#1181
��������~
1 bz 4
~
wait 1
switch %random.8%
case 1
oecho �������� ������� �������� ��������� � ����.
oecho �������� ������� �������� ������ ������!
oecho &K� ��������� ������� �������� ������� ������ � �� ��������� ����������.&n
oecho �������� ������� �������� ���� ������.
oecho &K�������� ������� �������� ������ �������� �� ��������!&n
break
case 2
oecho �������� ������� �������� ������ : '� ������� �������...'
oecho &K�������� ������� �������� ���������.&n
break
case 3
oecho �������� ������� �������� ������� ���� � ����� �������� ���������.
break
case 4
oecho �������� ������� �������� ������ : '������ �� ������ ? �� ����� � ���� ���������.. ��� ���� �� ����� � ����.. ���...'
break
case 5
oecho �������� ������� �������� ������ : '� ������ �� �� ���� �����������...'
oecho &K�������� ������� �������� ���������.&n
oecho �������� ������� �������� ������ : '�... �� ��� ��� ����� ����'
oecho &K�������� ������� �������� ��������� ���������.&n
break
case 6
oecho �������� ������� �������� ������ : '� �� ������� ���� ����� ����� ����?'
oecho &K�������� ������� �������� ����� ������� � �����.&n
oecho &K�������� ������� �������� ����� ������� � �����.&n
oecho &K�������� ������� �������� ����� ������� � �����.&n
break
case 7
oecho �������� ������� �������� ������ : '��� � ���� ��������! � ��������!'
wait 1s
oecho �������� ������� �������� ������ : '����� ��� � � ���� ������� ����!'
wait 1s
�������� ������� �������� ������ : '� ������ - �������� � ������ ���� �� 3 ������� � ���������� ������!'
wait 3s
oecho �������� ������� �������� ������ : '� ���... �.. �.....'
wait 2
oecho &K�������� ������� �������� ������ ����� ��������� ������� � ����� ������ ���� ������.&n
break
case 8
oecho &K�������� ������� �������� ������� ���������� ����� �����.&n
break
done
~
#1198
������-��������������~
1 c 1
*~
if (%cmd.mudcommand% == quit) || (%cmd.mudcommand% == write) || (%cmd.mudcommand% == �����) || (%cmd.mudcommand% == ������)
if !%actor.name% || ( %actor.name% == ����� )
return 0
halt
end
end
~
$~
