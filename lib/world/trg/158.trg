#15802
����~
0 q 30
~
wait 1
if (%actor.clan% ==���)
  if (%actor.clanrank% >= 8)
    msend %actor% ��� ���� ���, ��� ���������� ����� �� ����.
    mechoaround %actor% ��� ���� %actor.name%, ��� ���������� ����� �� ����.
  else
    switch %ramdom.4%
      case 1
        ���� %actor.name% "����������� ����, %actor.name%"
      case 2
        ���� %actor.name%
    done
  endif
else
  msend %acto.namer% ��� ���������� ����������� �������� ���.
  mechoaround %actor.name% ��� ���������� ����������� ��������� �� %actor.name%.
        ��� %actor.name%
        insult %actor.name%
endif
~
#15803
���~
1 gj 100
~
wait 1
if (%actor.clan% == ���) || (%actor.vnum% != -1)
halt
end
wait 1
opurge %self%
~
#15804
�������~
0 m 1
~
wait 1s
�������
wait 2
��� .%actor.name%
���� .%actor.name%
wait 3
����� .%actor.name%
halt
wait 1
if %amount% < 100
  msend %actor.name% �� ����� ������ � ���� ������� �� ��������!
  halt
endif
switch %amount%
  case 100
    mecho ������� �������� ������.
    dg_cast '��������� ���������' %actor.name%
    break
  case 200
    mecho ������� �������� ������.
    dg_cast '������' %actor.name%
    break
  case 300
    mecho ������� �������� ������.
    dg_cast '�������������' %actor.name%
    break
  case 400
    mecho ������� �������� ������.
    dg_cast ����' %actor.name%
    break
  case 500
    mecho ������� �������� ������.
    dg_cast '�����' %actor.name%
    break
  default
    msend %actor% ��������, �� � �� ���� ��� �� ������ �� ���� �� ��� ������.
    msend %actor% ��� ��� � ���� ������� ��� ���:
    msend %actor% 100 ��� - ��������� ���������.
    msend %actor% 200 ��� - ������.
    msend %actor% 300 ��� - �������������.
    msend %actor% 400 ��� - ����.
    msend %actor% 500 ��� - �����.
done
~
$~
