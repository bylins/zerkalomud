* BRusMUD trigger file v1.0
#2000
������� ������ ����������~
0 d0 100
����������~
* ������� ������� �� �������. ��� ������� ����� ��� ��������� � ������� ����
* � ��������/������� ����, �������� ������������ ��� ������������ ���������,
* �������������� ����� ���������.
wait 1
if %actor.vnum% != -1
halt
end 
say ������-������...
wait 1
if %actor.bank% < 220
���
say �, �� � ���� ���-�� ���� �� ������� �� ������!
halt
end
eval buffer %actor.bank(-220)%
mload obj 2045
mload obj 2046
mload obj 2048
mload obj 2049
mload obj 2049
mload obj 2049
mload obj 2049
mload obj 2049
mload obj 2049
mload obj 2049
mload obj 2049
mload obj 2049
mload obj 139
mload obj 139
put all ���
say ��� ������� ����.
give all .%actor.name%
drop all
wait 2
say ������ ���������� ���� ��������...
if %actor.bank% < 600
���
say ��... � ��� �� ���� � ���� ����� ���.
halt
end
eval buffer %actor.bank(-600)%
switch %actor.class%
*������ ���
case 1
*���
case 2
*�������
case 4
*���-��������
case 6
*���-���������
case 7
*���-���������
case 8
*�������
case 10
*�����
case 12
*�����
case 13
mload obj 2011
mload obj 2012
mload obj 2014
mload obj 2015
mload obj 2016
mload obj 2017
mload obj 2018
mload obj 2019
mload obj 2020
mload obj 2021
break
*������
case 0
*��������
case 3
*���������
case 5
*������
case 9
*������
case 11
mload obj 2000
mload obj 2001
mload obj 2003
mload obj 2004
mload obj 2005
mload obj 2006
mload obj 2007
mload obj 2008
mload obj 2009
mload obj 2010
done
mload obj 2022
mload obj 2023
give all .%actor.name% 
drop all
wait 1
say ������ ���%actor.g% ���� � ����� ���� - �� ������%actor.w% ���.
















~
#2001
�������� �����~
1 g0j0 100
~
* ������� ������������ ����������� ������������ ����-�����
* �������������� ������. ��������������� ������ ��������
* ���� ���������� ��������, ����������������� � �� ��������������, ��
* ����� ������������ ���� ������� - ������ �� ����������� ��� � �������� � ������.
* ������ "���" ���� ������� ������������ �������� ����� � ������ ��������
if %actor.clan% == ���
halt
end
osend %actor% %self.name% ��������%self.u% � ����� �����.
wait 1
opurge %self%
















~
#2002
���� ����~
1 c0 2
����������~
* ������� �� ���� ��������� ����. ��������� � �������.
* ����������, ���� �� ������� �������� �������� (�������)
* ��� ����, �� � �������� ���������� ������ ���������
wait 1
if !%arg.contains(�������)%
osend %actor% ��� �� �������?
halt
end
osend %actor% �� ���������� ������� � ������ �� ��� ���� ���������.
if %actor.clan% != ���
halt
end
if %world.curmobs(2008)% > 50
osend %actor% ...� ������ �� ���������!
halt
end
oechoaround %actor% %actor.name% ���������%actor.g% �������.
oload mob 2008
oecho ��������� ���� ����� � ����� ���� �������� ������ ������.
oforce %actor% �������� ����
wait 1
opurge %self%
















~
#2003
���������� ����~
0 c0 100
����������~
* ������� �� ����������� ��������� ����. ��������� � ����-����.
* ����������, ���� �� ������� �������� �������� (�������)
* ��� ����, �� � �������� ���������� ������ ���������
wait 1
if %self.fighting%
halt
end
if !%arg.contains(����)%
msend %actor%  ���� �� ������ ����������?
halt
end
if (%actor.clan% != ���) || (%actor% != %self.leader%)
msend %actor% ��� �� ��� ������.
halt
end
msend %actor% �� ���������� ����.
mechoaround %actor% %actor.name% ���������%actor.g% ����.
mload obj 2052
give ������� .%actor.name%
wait 1
mecho ���� ������� �������.
wait 1
mpurge %self%
















~
#2004
������ �������~
0 m0 1
~
* ���� ����� �� ������
wait 1
if %amount% < 50
wait 2
say �������� �����!
give %amount% ��� .%actor.name%
halt
end
if %world.curobjs(2052)% >= 200
wait 2
say ���� �������, ���������!
give %amount% ��� .%actor.name%
halt
end
wait 2
say ���, ��������.
mload obj 2052
give ������� .%actor.name%
















~
$
$
