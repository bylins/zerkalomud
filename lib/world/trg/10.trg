#1000
����� ������~
1 c 4
���������~
wait 1
%echo% � ������� ������ ������ ����� ������ � ����������� � ��������� ������� ������.
%purge% %self%
~
#1001
�����~
1 j 100
~
wait 1
* oechoarround %actor.name% ����� ������ ���� ������.
* osend %actor% �� ������ ������ ���� ������.
oecho ������ ��������� ����� ������.
~
#1002
�������� ����~
2 g 100
~

~
#1003
new trigger~
2 d 100
�����~
%actor.remort(10)%
wecho ������!!!!
~
#1004
������� ������������ ������� (1092) - �������~
1 j 100
~
if ( %actor.level% > 30 )
return 1
halt
end
if ( %actor.name% == ������� )
return 1
wait 1
%echoaround% %actor% ���� ����� ������ ���������� �� ������ �������, � �� ������������� ��� ���� ���� �������� � ������� ���.
%send% %actor% ���� ����� ������ � ���� �������� �� ������������ ������ �������.
else
return 0
%echoaround% %actor% %actor.name% �������%actor.u% �������� ������, �� ���� �������%actor.u% �� ������ �����.
%send% %actor% �� ���������� �������� ������, �� ���� ����������� �� ������ ����� ��������.
eval dmg %actor.hitp%/10
odamage %actor% %dmg%
end
~
#1005
������....~
0 b 10
~
switch %random.21%
case 1
����
break
case 2
���
����
break
case 3
����� �� ��������� ���� ������ - ������� �����!
����� � ��� ����� �������, ��� ���� �� ��������� �������!!!
break
case 4
emot ������������ ������� : '� �� ���� ��� ������ ���������� ������, � ��-�� ����-�-���� ������� ������...'
break
case 5
say � ���� - �� ��� �� ����� ��� �����? �������� �������... ���������..
����
break
case 6
say � ��� ��������, �� ���� ����� - ��������?
������
break
case 7
say ��, �� ���������, ����!!! ������ ���� �����!
break
case 8
say �� �������, ������� - ���� � ������? 
say �������� ������������ - �����-����-�����-����..
say ���� �� ������ ��� ��������!
break
case 9
say �� ��� �� ������� - �� �������� � �������, �� ��������...
say �� �� ���� - ����������...
�����
break
case 10
say ��������� �������� �����!
emot : ��������������������������������������!!!!!!!!!!!!!!!!!
break
case 11
say ������� � �������, ������� ���������. ������� ��� �������� �������...
wait 1
say ��, � ��� ��� �, ������ ���� ��������� ��������.
break
case 12
say ������-����, ������-����, � ����� ������� �... �... ���..
�����
break
case 13
���
�����
break
case 14
%force% ��������� ����� ������
break
case 15
say ��, ����������� ��� ������...
emot : "���� �� �������� � ������� �����, ������������ ������� ����� �������..."
wait 2
say �� ��� ������ ����� �� ��� ����� ������... � � ������! ������! 
break
case 16
emot : '�������� �������� � ��������, � � ������� ������ ��������... ����� � ����� ����� ���� ����������� ������ �����...'
����
break
case 17
eval target %random.pc%
mforce ��������� ���� %target.name%
break
case 18
say �� � ����� � ������� �������! ������� ���� "�����"! � � ����� ������� ��������!!!
done
break
case 19
eval target %random.pc%
���� %target.name%
break
case 20
say ��, �� ���, �������! � ��� ��� �������������! ������� ������� ������ ����! � �� ��� ����� � �����!!!
break
case 21
say ��� ������� ������� ���... �� �����������-��! ��� ������, ��� ������... ��� ��������� �������� :-(
����
dreak
done
~
#1006
������� �� ����� ����������.~
1 c 1
���������~
if %arg.contains(�������)%
calcuid victim 193 mob
%echoaround% %victim% �������� �������� ���� � ��������� ����� ����� � ������ ����!
wait 1
opurge ���������.�����
halt
end
if %arg.contains(�������)%
if %world.curmobs(193)% > 9
%send% %actor% ������ �� ���������!
halt
end
%echo%  �������� �������� ���� � ��������� ����� ������ ����� � ������ ����!
oload mob 193
halt
end
if %arg.contains(���������)%
set cmmd %arg.cdr%
oforce ���������.����� %cmmd%
halt
end
~
#1007
zone reset~
2 c 0
zreset~
%world.zreset(%arg%)%
~
#1008
�������~
0 i 100
~
wait 1s
eval target %self.people%
if %target% == %self%
eval target2 %target.next_in_room%
if %target2%
mkill %target2%
end
end
~
#1009
sword~
1 c 1
����������~
eval spch %arg.car%
eval string %arg.cdr%
if ( %spch% == ���� )
oforce ��������.������� tell %string%
halt
end
if ( %spch% == ����� )
oforce ��������.������� ������� %string%
end
~
#1010
flash~
1 c 1
������~
eval victim %arg.car%
eval value %arg.cdr%
foreach target %self.all%
eval string %target.iname%
set targetname %string.car%
if ( %targetname% == %victim% )
log  &B%actor.name% �������� ������� �� %value% ����� �� %victim% !!!&g
%echoaround% %target% ~~%actor.name% ������ ������ ������� � �������� ������ ������� �� ��������� ������ � %target.vname%!
%send% %target% ~~%actor.name% ������ ������ ������� � �������� ������ ������� ����� � ���!
eval buffer %target.hitp(-%value%)%
odamage %target% %value%
halt
end
done
~
#1011
Rafail's wings~
1 c 1
���������~
wait 1
%echo% ����������� ������ �������� �������� � �� ������� �������� ����� ��������� �����.
~
#1012
��������� ����~
0 f 100
~
*������
if (%actor.class% == 0)
mspellturn %actor.name% ��������� set
*������ ���
elseif (%actor.class% == 1)
mspellturn %actor.name% ��������.��� set
*���
elseif (%actor.class% == 2)
mskilladd %actor.name% �������� 10
*��������
elseif (%actor.class% == 3)
mskilladd %actor.name% �����������.����� 20
*�������
elseif (%actor.class% == 4)
mskillturn %actor.name% ����������.����� set
*���������
elseif (%actor.class% == 5)
mskillturn %actor.name% �������.������ set
*���-��������
elseif (%actor.class% == 6)
mspellturn %actor.name% ���������.���� set
*���-���������
elseif (%actor.class% == 7)
mspellturn %actor.name% ����������.���������� set
*���-���������
elseif (%actor.class% == 8)
mspellturn %actor.name% �������.���� set
*������
elseif (%actor.class% == 9)
mskilladd %actor.name% ������.����� 10
*�������
elseif (%actor.class% == 10)
mskilladd %actor.name% ��������������.������� 25
*������
elseif (%actor.class% == 11)
mskilladd %actor.name% �������� 20
*�����
elseif (%actor.class% == 12)
mskillturn %actor.name% ����������.����� set
*�����
elseif (%actor.class% == 13)
mload obj 215
mload obj 209
mload obj 243
end
~
#1013
skull~
1 c 1
������~
eval string1 %arg.car%
if %string1% == load
if %world.curobjs(1049)% < 30
oload obj 1049
halt
end
end
if !%actor.name%
oecho ����� � ���� ����-�� ����������� � ��������� : '%arg%'
halt
else
oecho ����� � ���� %actor.rname% ����������� � ��������� : '%arg%'
end
~
#1014
������ � ��������� )~
1 c 1
���~
eval victim %arg.car%
eval value %arg.cdr%
foreach target %self.all%
eval string %target.iname%
set targetname %string.car%
if ( %targetname% == %victim% )
%echoaround% %target%  %actor.name% ����������� �������� �� %target.vname% - � ������� ����� ��������� ������!
%echoaround% %target%  %target.name% �����%actor.q% � ����� �������� ����.
%send% %target%  %actor.name% ����������� �������� �� ��� � �������� ����� ������ ���� ����!
eval buffer %target.wait(%value%)%
halt
end
done
~
#1015
���������������~
1 c 1
��������~
wait 1
%send% %actor% �� �������� ���� ���� ��������������.
%echoaround% %actor% %actor.name% �������%actor.g% ���� ���� �������������� (�� �������� � ����).
%echo% ������ ������!
~
#1016
rafail wings2~
1 c 1
loa~
oload %arg%
osend %actor% &R loa %arg% - &C������&n
~
$~
