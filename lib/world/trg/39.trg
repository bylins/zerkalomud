#3900
�������� ������ � �������� - ���� ���~
0 r 100
~
switch %self.vnum%
case 62007
set vnum 3932
break
default
halt
break
done
mecho %self.name% ������ �� ��������� �����-�� ���� � �������� ����������� �� �� ��������.
set i 0
while %i% < 8
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
mload obj %vnum%
eval i %i%+1
eval vnum %vnum%+1
done
detach 3900 %self.id%
~
$~
