#5900
����20�������~
0 m 1
~
if %actor.vnum% != -1
  halt
end
eval needgold %actor.level%
if %amount% < %needgold%
  ���� %amount% ��� %actor.name%
  tell %actor.name% �������� �����!
else
  mecho ����� ��������� ����-�� � ������ � ������ ��������.
  mecho ��-�� ������ �������� ��������� � �������.
  msend %actor.name% � �������� �������, ������ �������� �� �������!
  mecho ��������� ������ ������ ������, ������� �� ������:
  mecho � �� ��������!
  msend %actor.name% ������ ��������!
  mecho ��������� ������ ��������� ������ � ����� �������.
  mteleport %actor.name% 5901 horse
end
~
#5901
����20��������~
0 m 1
~
if %actor.vnum% != -1
  halt
end
eval needgold %actor.level%
if %amount% < %needgold%
  ���� %amount% ��� %actor.name%
  tell %actor.name% �������� �����!
else
  mecho ����� ��������� ����-�� � ������ � ������ ��������.
  mecho ��-�� ������ �������� ��������� � �������.
  msend %actor.name% � �������� �������, ������ �������� �� �������!
  mecho ��������� ������ ������ ������, ������� �� ������:
  mecho � �� ��������!
  msend %actor.name% ������ ��������!
  mecho ��������� ������ ��������� ������ � ����� �������.
  mteleport %actor.name% 5900 horse
end
~
#5902
��������������~
0 r 100
~
tell %actor.name% ������ ������� ���� �� ������ �����, ��� ���� �� �� �����������!
eval needgold %actor.level%
tell %actor.name% �����-�� ��� ���� %needgold% � �� ���!
~
#5903
�������������~
0 r 100
~
tell %actor.name% ������ ������� ���� �� ������ �����, ��� ���� �� �� �����������!
eval needgold %actor.level%
tell %actor.name% �����-�� ��� ���� %needgold% � �� ���!
~
$~
