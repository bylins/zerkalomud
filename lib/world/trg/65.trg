#6500
����20�������~
0 m 1
~
if %actor.vnum% != -1
 halt
end
eval needgold %actor.level%
if %amount% < %needgold%
���� %amount% ��� %actor.name%
tell %actor.name% ����, ������� ������� �����!
else
mecho �������� ������� ������� ��� ����.
msend %actor.name% ��-�� ������ ����� ��� ���� �������� � ������ ����� ����� ��������� �� ���.
 if %actor.sex%==1
mechoaround %actor% ��-�� ������ ����� ��� ���� �������� � ������ ����� ��� %actor.name% ����� �������� �� ���.
mechoaround %actor% %actor.name% ����� � ������ ������ � �����������.
 else
mechoaround %actor% ��-�� ������ ����� ��� ���� �������� � ������ ����� ��� %actor.name% ����� ��������� �� ���.
mechoaround %actor% %actor.name% ������� � ������ ������ � �����������.
 endif
mteleport %actor.name% 6500 horse
msend %actor% ���� ����������� ����������.
* if %actor.sex%==1
*  mat 6500 mechoaround %actor% %actor.name% �������� � �����������.
* else
*  mat 6500 mechoaround %actor% %actor.name% ��������� � �����������.
* end
*  mat 6500 mecho ��������� ������� ��� �������.
*end
end
~
#6501
����20��������~
0 m 1
~
if %actor.vnum% != -1
halt
end
eval needgold %actor.level%
if %amount% < %needgold%
���� %amount% ��� %actor.name%
tell %actor.name% �������� ����������!
����
else
mecho �������� ������ �������� ��� �����, ����-�� ���-�� ������ �� ����������� �����.
msend %actor%  �� ��� ������ ����� ������ ������� ��������, ����� ��������� � ����� ��� �� �����.
mechoaround %actor% �� ��� ������ ����� ������ ������� ��������, ����� ��������� � ����� %actor.name% �� �����.
mechoaround %actor% %actor.name% ��%actor.y% ������ � �����������.
mteleport %actor% 6501 horse
msend %actor% ���� ����������� ����������.
mechoaround %actor% %actor.name% ������%actor.u% � �����������.
end
~
#6502
��������������~
0 r 100
~
tell %actor.name% ���� ������ ��������� ������, ���� ������!
tell %actor.name% ���� � ����� �������, ��� �������, ����� � �� �������!
tell %actor.name% ������ � ����� �� �������� ������ �� ������, � ����!
eval needgold %actor.level%
tell %actor.name% ����� �� %needgold% ���!
~
#6503
�������������~
0 r 100
~
tell %actor.name% ���� �������� ������������ ������ �������� �� ������� �������!
eval needgold %actor.level%
tell %actor.name% ����� �� %needgold% ���!
~
$~
