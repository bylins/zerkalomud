#35700
� ������~
0 r 100
~
if %actor.clan%==��
wait 5
say �� ��������� ������?
say ��� ����� ������ ��� 100 ���.
else
say ����� ����� �� �����!
end
~
#35701
���� ���� ������~
0 m 100
100~
if %actor.clan%==��
tell %actor.name% ������!
if %world.curobjs(35710)% < 50
tell %actor.name% ����� ���� ��������!
mload obj 35727
give �������� %actor.name%
else
tell %actor.name% ���, ���� ����������.
end
else
say ����� ����� �� �����!
end
~
#35702
���� ������~
1 g 100
~
wait 1
if %actor.clan% == ��
  halt
endif
%purge% %self%
~
#35703
������~
0 m 0
~
wait 1
��� %actor.name%
halt
if %amount% < 20
msend %actor.name% �� ����� ������ � ���� ������� �� ��������!
halt
endif
switch %amount%
case 20
mecho �������� ������� ������.
dg_cast '������ ���������' %actor.name%
break
case 30
mecho �������� �������� ���������� �� �����.
dg_cast '��������' %actor.name%
break
case 40
mecho �������� ��������� � ���� � ���-�� �����������.
dg_cast '�����������' %actor.name%
break
case 50
mecho �������� ������� ������.
dg_cast '������' %actor.name%
break
case 60
mecho �������� ������� ������.
dg_cast '�������������' %actor.name%
break
case 70
mecho �������� ���������� ��� ���� �����.
dg_cast '����' %actor.name%
break
case 80
mecho �������� �������� ������.
dg_cast '�����' %actor.name%
break
case 90
mecho �������� �������� ������.
dg_cast '���� �����' %actor.name%
break
case 350
mecho �������� ������� ������
dg_cast '��������' %actor.name%
dg_cast '�����������' %actor.name%
dg_cast '������' %actor.name%
dg_cast '�������������' %actor.name%
dg_cast '����' %actor.name%
dg_cast '�����' %actor.name%
dg_cast '���� �����' %actor.name%
break
default
msend %actor% ��������, �� � �� ���� ��� �� ������ �� ���� �� ��� ������.
msend %actor% ��� ��� � ���� ������� ��� ���:
msend %actor%  �� 20 ��� - ������� ���� �����.
msend %actor%  �� 30 ��� - ��� ����� � �����.
msend %actor%  �� 40 ��� - ���������� ������.
msend %actor%  �� 50 ��� - �� ������� ����� �������.
msend %actor%  �� 60 ��� - ���� ���� ������ ��������.
msend %actor%  �� 70 ��� - ������� �� ������� �������.
msend %actor%  �� 80 ��� - ������� ������,��� ����� � ����.
msend %actor%  �� 90 ��� - ������� ������� ����� � ������� ���������������.
msend %actor%  �� 350 ��� - ���� ����� ��� ���������...
done
~
#35704
������ � �������~
0 g 100
~
wait 1
msend %actor% ��� ��� � ���� ������� ��� ���:
msend %actor%  �� 20 ��� - ������� ���� �����.
msend %actor%  �� 30 ��� - ��� ����� � �����.
msend %actor%  �� 40 ��� - ���������� ������.
msend %actor%  �� 50 ��� - �� ������� ����� �������.
msend %actor%  �� 60 ��� - ���� ���� ������ ��������.
msend %actor%  �� 70 ��� - ������� �� ������� �������.
msend %actor%  �� 80 ��� - ������� ������,��� ����� � ����.
msend %actor%  �� 80 ��� - ������� ������� ����� � ������� ���������������.
msend %actor%  �� 350 ��� - ���� ����� ��� ���������...
end
~
#35705
���������� ��������~
1 c 3
����������~
oechoaround %actor% %actor.name% ���������%actor.g% ��������.
osend %actor% �� ��������� ���������� ��������.
oload mob 35710
calcuid cover 35727 obj
opurge %cover%
~
#35706
�������� ��������~
0 c 100
��������~
if (%actor.clan%==��)
mechoaround %actor% %actor.name% ��������� ������%actor.g% ��������.
msend %actor% �� ��������� ������� ��������.
mload obj 35727
wait 1
mpurge %self%
else
mechoaround %actor% %actor.name% �������%actor.u% ������� ��������, �� �� ��������� �� ���.
msend %actor% �� ���������� ������� ��������, �� ��� ��������� �� ����� ���.
end
~
$~
