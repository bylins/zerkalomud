* BRusMUD trigger file v1.0
#5100
�������� ~
1 c0 4
����������~
if %arg.contains(�������)%
odoor 5104 up room 5117
osend %actor.name% ��������������, �� ���������� �������� � ������ ����� ��������� ������.
oechoaround %actor% %actor.name% ��������� �������� � ������ ����� ��������� ������.
oload obj 5105
calcuid target 5104 obj
opurge %target%
else
oecho ���� ?
  halt
end

~
#5101
����� ~
2 f0 100
�����~
wecho ooo %room.up% ooo
if %world.curobjs(5105)% > 0 || %room.up% == up
wdoor 5104 up purge
wecho �������� ����������� � ����� �� ���.
calcuid target 5105 obj
wpurge %target%
end

~
#5102
����������~
1 o0 100
����������~
wait 1
oecho ��������� ���� ���������� � �����.
if %world.curobjs(517)% < 1 && %random.5%==1
osend %actor% ����������, ����� �������� ������, �� �������� ����� �������� 
osend %actor% ������� - ��� ������������ ��������
oechoaround %actor.name% ����� ������������ ��������.
oload obj 517
end
calcuid target 5103 obj
opurge %target%

~
$
$
