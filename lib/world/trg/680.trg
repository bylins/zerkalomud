#68000
������� �� ����� � ������ (����� ���������)~
0 g 100
~
if %actor.vnum% == -1
halt
end
wait 1
if %actor.vnum% != 27020
halt
end
wait 1s
set questor %actor.leader%
teleport questmob2703 68099
%purge% questmob2703
���
say ���������, �����! ����� �� ������, ������ �������...
emot ������� ����� ���������
say ������ ������ � ���, � � ���������� ����� ��������.
mecho ����� ������� ������� � ������ �������� �� ������ � �������.
wait 2
say ��� �, ������� �� ������... ��� �� ��������� ����?
switch %random.3%
case 2
if %random.100% < 60
  if %world.curobjs(68057)% < 5
mload obj 68057
���� ���� .%questor.name%
end
else
say ������ ��������� ��������� �� ������.
set bufer %self.gold(+2000)%
���� 2000 ��� %questor.name%
end
break
case 1
if %random.100% < 60
if %world.curobjs(68056)% < 10
mload obj 68056
���� ������� .%questor.name%
end
else
say ������ ��������� ��������� �� ������.
set bufer %self.gold(+2000)%
���� 2000 ��� %questor.name%
end
break
default
say ������ ��������� ��������� �� ������.
set bufer %self.gold(+2000)%
���� 2000 ��� %questor.name%
done
~
$~
