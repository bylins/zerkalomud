#12600
����� � �����~
2 c 0
�����~
wait 1
   wechoaround %actor% %actor.name% ���%actor.y% � ���� � �����%actor.q%.
   wsend %actor.name% ����� ������� �������� ��� �� ���������.
wait 1
wteleport %actor.name% 12664
wsend %actor.name% �� ��������� � ������� �� �����.
~
#12601
������� (�����)~
2 i 100
0~
wait 1
wechoaround %actor% ��� ������ %actor.name% ���������%actor.g% ���� � �����, ��� ��� ���� ��������.
wsend %actor.name% ������������ ���� � �����, �� ��������, ��� ���-�� �������� ��� ��������. 
wsend %actor.name% � ����� ��� �������� � ��� ��� ������.
if (%random.99% <= 15) 
  wait 1
  wechoaround %actor% ����� �������� ����� ��� ������ ������ �����������, � �� ���������� � ������
  wechoaround %actor% �������� ������������� �������.
  wsend %actor.name% �������� ����� ���� ����������� ��� ������ ������, � �� ���������� � ������
  wsend %actor.name% �������� ������������� �������.
  wait 1
else
  wait 2s
  wecho ������, ����� ���� ������ ������ ��� �����������. 
   halt
end
~
#12602
������� (�����)~
0 f 100
~
mecho ������, ����� ����� ���-�� ������������.
wait 2
if (%random.100% <= 99) 
  mecho ����� �������� ����� ��� ������ ������ �����������, � �� ���������� � ������
  mecho �������� ������������� �������.
  eval fchar %self.people%
  while %fchar%
    if %fchar.vnum%==-1
      mteleport %actor.name% 12614
    end
    eval pc %fchar.next_in_room%
    if %pc.id% && %pc.id%!=%fchar.id%
      makeuid fchar %pc.id%
    else
      unset fchar
    end
  done
  halt
else
  mecho �� ������ ���������� �� ���������.
   halt
end
~
#12603
����� � ������~
0 r 100
0~
wait 1
if (%direction%==west) 
  mecho ����� ����� �������� �� ���, � ��������:
  mecho _- ������, ���� ������ ������ - ������ ������!
  mecho ��� ������ ����, ����������� ������.
  attach 12604 %self.id%
  halt
end
~
#12604
���� ������� �����~
0 j 100
~
if (%object.vnum% == 12400)
  wait 1
  mpurge %object%
  wait 1s
  tell %actor.name% ��, ��� ��� ����� ����, %actor.name%!
  tell %actor.name% �� �� ���������... ��� �� ������ ������� ���?!
  �����
  wait 2s
  mkill %actor.name%
  halt
else
  wait 1
  say ������� ��� �������, ��� �� ������ ������� ������.
  say �� ��� �� �� ��� ����.
  eval getobject %object.name%
  ����� %getobject.car%.%getobject.cdr%
  halt
end
~
#12606
���� �� �����~
2 c 0
�����~
wait 1
wechoaround %actor% �������� �� ��������� �������, %actor.name% ������%actor.g% � �������� ����.
wsend %actor.name% ���������� ��������, �� ������� � �������� ����.
wait 1
switch %random.4%
  case 1
wteleport %actor.name% 12604
wat 12604 %echoaround% %actor% %actor.name% ������%actor.u% �� ��������.
    break
  case 2
wteleport %actor.name% 12624
wat 12624 %echoaround% %actor% %actor.name% ������%actor.u% �� ��������.
    break
  case 3
wteleport %actor.name% 12646
wat 12646 %echoaround% %actor% %actor.name% ������%actor.u% �� ��������.
    break
  default
wteleport %actor.name% 12656
wat 12656 %echoaround% %actor% %actor.name% ������%actor.u% �� ��������.
  break
done
wait 1
%send% %actor% �� ��������� �� ��������� ��������� ������.
~
#12608
���� ���������~
2 c 0
������~
wait 1
if !(%arg.contains(��������)%)
   wechoaround %actor% %actor.name% ���-�� ���������%actor.g%.
   wsend %actor.name%  ����, ����?
   return 0
   halt
end
if %actor.clan%!=�� || %actor.clanrank%<7
 wecho �������� ������.
 wsend %actor.name%  _- ���� ������ ��� ��������� ������� ������.
 return 0
 halt
end
wechoaround %actor% %actor.name% ���-�� �������� ������%actor.g%.
wechoaround %actor% ��������� ���� �����������, ����� ��������� � %actor.name% ������%actor.g% ������.
wechoaround %actor% ����� ���� ����� ����� ��������� �������.
wsend %actor.name% ��������� ���������, ����� ��������� ����� ����.
wait 1s
wteleport %actor.name% 12636
wait 1
wsend %actor.name% �� ������ ���� ������, � ��������� � ������ ��������� ������.
wat 12636 wechoaround %actor% %actor.name% ����%actor.y% � ���.
~
#12609
���������~
0 k 35
~
eval damag 30+%random.30%
msend %actor% �� ��������� �� ����� ��������� ������ � �������� ���.
%actor.hitp(-%damag%)%
~
#12610
from trap~
2 c 0
�����~
wait 1
if %actor.clan%== ��
%teleport% %actor.name% 12662
end
~
#12611
�������� ��������~
1 c 3
��������~
wait 1
if (%actor.clan%!=��)
  oechoaround %actor% %actor.name% �����%actor.u% ������ ������ ��������. � ��� ��� � �������?
  osend %actor% �� ������ ������ ��������. ��������� �������.
  halt
end
  oechoaround %actor% %actor.name% �����%actor.g% ������ �������� � ��� ����������� � �������� �����������.
  osend %actor% �� ������� �������� � �� ����������� � �������� �����������!
  oload mob 12611
opurge %self%
~
#12612
��������� �����������~
0 c 0
���������~
wait 1
if (%actor.clan%!=��)
  mechoaround %actor% %actor.name% ����������%actor.g% �������� �����������, �� �� ���������� ������� ��� �� �����.
  msend %actor% �� ����������� ��������� �����������, �� ��� ���������� ������� ��� �� �����!
  halt
end
mechoaround %actor% %actor.name% ��������� ��������%actor.g% �����������. ����������� � ������� ������� �������.
msend %actor% �� ��������� ��������� �����������. ����������� � ������� ������� �������.
mload obj 12635
mpurge %self%
~
#12613
����� ��������~
1 g 100
~
wait 1
if %actor.clan%==��
halt
end
purge %self.id%
~
$~
