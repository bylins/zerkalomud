#42100
���� �������~
0 b 100
~
if %random.50% == 1
mecho _������ ������� ������ ������� �����.
end
~
#42101
���������� ������~
2 c 0
���������� �����~
if !(%arg.contains(������)%) 
   wsend       %actor% ���� ��� �� ������ ��������???
   return 0
   halt
end
  wsend       %actor% �� ������� � ������ �������� ����.
  wechoaround %actor% %actor.name% �����%actor.q% ���� ��.
  wait 1s
  wsend %actor% .- �� ��������� � ������ �������.
  wteleport %actor.name% 42110
  wat 42110 wechoaround %actor% ��� �� ��������� ����.
~
#42102
������� �����~
2 c 0
���������� �����~
if !(%arg.contains(�����)%) 
   wsend       %actor% ���� ��� �� ������ ��������???
   return 0
   halt
end
  wsend       %actor% �� ������� �� �������.
  wechoaround %actor% %actor.name% �����%actor.q% ���� ��.
  wait 1s
  wsend %actor% .- �� ��������� � ������� �������.
  wteleport %actor.name% 42113
  wat 42113 wechoaround %actor% ��� �� ��������� ����.
~
#42103
�������� ����� ������ (� ������ ������)~
0 j 100
~
%echo% _������� ����� �������� %object.iname%.
if (%object.type%==19)
  wait 2s
  if %exist.mob(42113)%
    %echo% _������� ����� ������ %object.iname% ���������.
    %echo% _�������� ������ ���� %object.iname%.
    %purge% %object.name%
    halt
  endif
  wait 1s
  %echo% _������� ����� ����� %object.iname%.
  %purge% %object.name%
  wait 1s
  %echo% _������� ����� ������� � ������� ���� ����.
  wait 5s
  %load% mob 42113
  %load% mob 42113
  %load% mob 42113
  %load% mob 42113
  %load% mob 42113
  %echo% _������� ����� ��������� �� ����� � ������������� ������ ������� ����.
elseif
  %echo% _������� ����� ���������� ��������.
  return 0
  halt
endif
~
#42104
����� ����~
1 ghijl 100
~
wait 1
if %actor.clan% == ��
  halt
end
%purge% %self%
~
$~
