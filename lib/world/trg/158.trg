* BRusMUD trigger file v1.0
#15802
����~
0 r0 0
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
1 j0 100
~
wait 1
if %actor.clan% == ���
odamage %actor% 200
  halt
endif

~
#15804
�������~
0 m0 1
~
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
$
$
