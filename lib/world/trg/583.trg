#58300
���� �� �����~
2 c 1
�����~
* ���� �� ����� [�������� -- �����������]
* �������� �� ������: 58116
wait 1
if !%actor.rentable%
  wsend %actor% � &R��&n ������ �� �����.
  halt
end
if %arg.contains(��)%
  wsend %actor% �� ����� �� ������������� ���� � \&R���\&n.
  wechoaround %actor% %actor.name% ���%actor.y% �� ������������� ����.
  wteleport %actor% 58323 horse
elseif %arg.contains(�����)%
  wsend %actor% �� ����� �� ������������� ���� � \&R������\&n.
  wechoaround %actor% %actor.name% ���%actor.y% �� ������������� ����.
  wteleport %actor% 58303 horse
elseif %arg.contains(�����)%
  wsend %actor% �� ����� �� ������������� ���� � \&R������\&n.
  wechoaround %actor% %actor.name% ���%actor.y% �� ������������� ����.
  wteleport %actor% 58311 horse
elseif %arg.contains(������)%
  wsend %actor% �� ����� �� ������������� ���� � \&R�������\&n.
  wechoaround %actor% %actor.name% ���%actor.y% �� ������������� ����.
  wteleport %actor% 58315 horse
else
  eval rnd %random.num(4)%
  wsend %actor% �� ����� �� ������������� ����.
  wechoaround %actor% %actor.name% ���%actor.y% �� ������������� ����.
  switch %rnd%
    case 1
      wteleport %actor% 58323 horse
    break
    case 2
      wteleport %actor% 58303 horse
    break
    case 3
      wteleport %actor% 58311 horse
    break
    case 4
      wteleport %actor% 58315 horse
    break
  done
end
~
#58301
����� � �����~
2 c 1
�����~
* ����� � ����� [��� ����������]
* �������� �� ������: 58303 58311 58315 58323
wait 1
wsend %actor% �� �������� ������������� ����.
wechoaround %actor% %actor.name% ���%actor.y% � �������������� ����.
wteleport %actor% 58116 horse
~
#58302
������� ���~
0 c 100
�������~
wait 1
if %actor.vnum% != -1 || %actor.leader% == %self%
  mechoaround %actor% %actor.name% �������%actor.u% ������� �� ���� �� ��������%actor.g%!
  halt
end
msend %actor% �� ������� �� ����!
if %socker1balllag% == 1
  %actor.wait(2)%
end
mechoaround %actor% %actor.name% ������%actor.g% �� ����!
set sockerpinalter1name %actor.name%
global sockerpinalter1name
set sockerroom1 %self.realroom%
if %arg.contains(��)%
  if %sockerroom1.south%
    if %random.100% < 25
      ��
    end
    ��
  else
    mecho ��� �������� �� ������.
    %actor.wait(4)%
    �����
  end
elseif %arg.contains(�����)%
  if %sockerroom1.north%
    if %random.100% < 25
      �����
    end
    �����
  else
    mecho ��� �������� �� ������.
    %actor.wait(4)%
    ��
  end
elseif %arg.contains(�����)%
  if %sockerroom1.west%
    if %random.100% < 25
      �����
    end
    �����
  else
    mecho ��� �������� �� ������.
    %actor.wait(4)%
    ������
  end
elseif %arg.contains(������)%
  if %sockerroom1.east%
    if %random.100% < 25
      ������
    end
    ������
  else
    mecho ��� �������� �� ������.
    %actor.wait(4)%
    �����
  end
elseif %arg.contains(����)%
  if %sockerroom1.down%
    ����
  else
    %actor.wait(4)%
    �����
  end
elseif %arg.contains(�����)%
  if %sockerroom1.up%
    �����
  else
    mecho ��� �������� �� ������.
    %actor.wait(4)%
    ����
  end
else
  msend %actor% � ����� ����������� ����� ������� ���? (�����, ������, ��, �����, ����, �����)
end
~
#58303
��� ��������� � ������~
0 g 100
~
wait 1
if %actor.vnum% != 58300
  halt
end
detach 58302 %actor.id%
eval sockergoal1counter %sockergoal1counter% + 1
global sockergoal1counter
set sockergate1id %self%
global sockergate1id
remote sockergate1id %actor.id%
exec 58305 %actor.id%
wait 2
*������� &W%sockerpinalter1name% �������� %sockergoal1counter% ��� � ������ ������� '&K%sockercommand1name%&W'!&n
������� &W%sockerpinalter1name% �������� %sockergoal1counter% ��� � ������ ������� '&K%sockercommand1name%&W'!&n
mpurge %actor%
if %ballloadroom%
  set ballbackroom %self.realroom%
  global ballbackroom
  eval pause 3+%random.3%
  wait %pause%s
  * �.�. �� �������� mat ������� �������� �����������
  *mat %ballloadroom% mload mob 58300
  mteleport %self% %ballloadroom%
  mload mob 58300
  mteleport %self% %ballbackroom%
end
~
#58304
��������� �������� �������~
0 c 100
���������~
wait 1
if %actor.level% > 30
  if !%arg%
    msend %actor% ������� ������������ �����: "%sockercommand1name%"
    msend %actor% ������� ���������� �����: "%sockergoal1counter%"
  elseif %arg.car% == �������
    set sockercommand1name %arg.cdr%
    global sockercommand1name
    set sockergoal1counter 0
    global sockergoal1counter
    msend %actor% ����� ������������ �����: "%sockercommand1name%"
  elseif %arg.car% == �����
    set ballloadroom %arg.cdr%
    global ballloadroom
    msend %actor% ��� ����� ������������� ����������� � ������� %ballloadroom% (%ballloadroom.name%).
  else
    msend %actor% ��������, �������� �������. ����������� "��������� �������" ��� "��������� �����".
  end
else
  msend %actor% ���� ������� ������������ ��� �����!
end
~
#58305
������� ����� ����� ���� ���������� ��� ������� ���~
0 z 100
~
remote sockerpinalter1name %sockergate1id.id%
~
#58306
���������� ���� � ���� ��� ��� ���~
0 c 100
���������~
wait 1
if %actor.level% > 30
  if %socker1balllag% == 1
    set socker1balllag 0
    global socker1balllag
  else
    set socker1balllag 1
    global socker1balllag
  end
  msend %actor% ���� �� ��� � ����: %socker1balllag%
else
  msend %actor% ���� ������� ������������ ��� �����!
end
~
#58307
�������� �� ���� �������� �������~
0 r 100
~
wait 1
if %actor.vnum% == -1
  tell %actor.name% �� 15000 ��� �� ������ ���������� � ���� &W�&K�&W�&K�&W�&K�&W� &K�&W�&K�&C!&n
end
~
#58308
�������� �� ���� �������� �������~
0 m 1
~
wait 1
if %world.curmobs(58300)% < 25
  if %amount% < 15000
    tell %actor.name% ����, ��� ����� 500 ���, � �� ������� �����? ����� %amount%!
    tell %actor.name% � ������ ����� �� ����� - ��� ���� ��������, ��� ����� �� ������%actor.g%.
  else
    tell %actor.name% ��� ����, ���� ���
    mload mob 58300
  end
else
  say ��������� ����, ������ �� ��������
  �����
end
~
#58309
��� ����������� � ���~
0 n 100
~
wait 1
mecho ��� ���� �� ����� � ���������, ������ �����������.
~
$~
