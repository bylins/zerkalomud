#6200
���������~
0 f 100
~
if %world.curobjs(6200)% < 1
   mload obj 6200
end
~
#6201
������������~
0 f 100
~
if %world.curobjs(6205)% < 1
   mload obj 6205
   mecho ������� ������, ������������� � ��������� ����� �����.
   mecho ���, ��� ������ ���� ������� �������� ���-�� �������������.
end
if (%world.curobjs(6206)% < 50) && (%random.5%==1)
   mload obj 6206
   mecho ����� ������ ��� ���-�� ��������.
end
~
#6202
����������~
1 o 100
~
oecho ��������� ������ ���� ���������� � �����.
if %world.curobjs(517)% < 1 && %random.5%==1
osend %actor% �������� ������, �� �������� ����� �������� �������, ������������ ��������.
oechoaround %actor.name% ����� ������������ ��������.
oload obj 517
end
calcuid target 6200 obj
opurge %target%
~
#6203
��������~
1 c 4
�������~
if !%exist.obj(6205)%
halt
end
if !%arg.contains(����)%
osend %actor% ����?
  halt
end
calcuid vedro62 6205 obj
if ( %actor.eq(17)% != %vedro62% )
osend %actor% �� ���???
  halt
end
* ������ � ����
osend %actor% ������������, �� ����������� ������� ���� � �����.
oechoaround %actor% %actor.name% ����������, ����� ������� ���� �� �������.
osend %actor% �����, �� ����� �� ������ ������ ������� �� ���-�� ��������.
osend %actor% ��������� ���� ���� � � ��� � ����� �������� �������.
osend %actor% ����� ������������ �� ��� � ����� � �������.
if %random.5%<2
oload obj 6208
oforce %actor.name% ����� �������
else
osend %actor% ������� ����������� �� ��� � ���� � �������. 
end
calcuid target 6205 obj
opurge %target%
detach 6203 %self.id%
~
#6204
�����������~
0 f 50
~
if %world.curobjs(6209)% < 10000
   mload obj 6209
end
~
#6205
�����~
2 f 100
~
calcuid colodec 6207 obj
detach 6203 %colodec.id%
attach 6203 %colodec.id%
~
$~
