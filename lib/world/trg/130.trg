* BRusMUD trigger file v1.0
#13000
black_wolf_killed~
0 f0 100
~
mload obj 13014


~
#13001
grey_wolf_killed~
0 f0 100
~
if (%world.curobjs(13001)% < 5) && (%random.100% < 15)
  mload obj 13001
end









~
#13002
wolf_dying_in_day~
0 f0 100
~
if (%random.100% < 30)
   mat 13099 mpurge �������.�����.����
end







~
#13003
�������� ���� ������~
0 j0 100
~
if %object.vnum% == 13000 then
  wait 8
  say ������ ����� ������� �����.
  wait 8
  say ���� ����� ��������� ����� ������ ��������.
  wait 8
  say ��������, ��� � ����� �������.
  wait 8
  mecho ������ ������� �������� ����������� ����� �� ����� � ������� �� ������.
  calcuid var 13000 obj
  mpurge %var.name%
  wait 5 sc
 if (%world.curobjs(13002)% < 10) && (%random.100% < 20) then
   mload obj 13002
   say ���, ������ ����� ������, � ��� ���� ������� ����� �� �����.
   wait 5
   ��� ����.����.���� %actor.name%
 elseif (%world.curobjs(13003)% < 15) && (%random.100% < 40) then
   mload obj 13003
   say ��� ��� ���� �������� ����� ��������� ���� ����� ����������.
   wait 5
   ��� ����.����.���� %actor.name%
 elseif (%world.curobjs(13004)% < 10) && (%random.100% < 20) then
   mload obj 13004
   say � ���� ��������� ����� ���� ����� �����.
   wait 5
   ��� ����.����.���� %actor.name%
 elseif (%world.curobjs(13005)% < 10) && (%random.100% < 20) then
   mload obj 13005
   say ���, ��� ���, ��� ��������.
   wait 5
   ��
   wait 5
   say �� ���� ��� ���� ������ � ���� ���� �����, ����� �������� �� ������%actor.q%.
   wait 5
   ��� �����.���� %actor.name%
 else
   say � ���������, �� ���� ��� � ���� ������ �� ����������.
   wait 8
   say �� ���� �� � � ����� ������.
 end
end


if %object.vnum% == 13015 then
  wait 8
  calcuid var 13015 obj
  mpurge %var.name%
  say ���... ������ ������ ����?.. ����� ������� ������-��.
  wait 8
  say �� ��� �, ���, ����� �� �����.
  wait 8
  ��� 400 ��� %actor.name%
  wait 4 s
  say ��! ���� �� �����. ��� ��� ��� ��������� �� �������� �������.
  wait 8
  say �� � � ��� �� �������, ��� ��� �� ��� ����.
  wait 8
  mload obj 13012
  drop ����
end


~
#13004
load hunter~
0 n0 100
~
if (%world.curobjs(13010)% < 10) && (%random.100% < 20)
     mload obj 13010
     wear ����.����
end

if (%world.curobjs(13009)% < 4) && (%random.100% < 5)
     mload obj 13009
     ���� ���
end







~
#13005
take key~
1 g0 100
~
if %actor.str% > 20 then
  wait 1
  oechoaround %actor% %actor.name% �������������� ����%actor.g% ���� � ���� � ���� ������ ���� ���������� ������ ������...
  osend %actor% �� �����������������, ����� ������� ���� �� ��������� ����������� � ��� ������.
  osend %actor% �� ������� ������ ����.
  opurge ������.����
end

if %actor.dex% < 20 then
  wait 1
  oechoaround %actor% %actor.name% ������� �������%actor.g% ���� �����, � �� ��������... �����!
  osend %actor% ��� ����� �������������... ������������ ������, ������ �� ������...
  osend %actor% �� ������� ������ ����.
  opurge ������.����
end





~
#13006
load black wolf corpse~
1 n0 100
~
wait 1
opurge ����.���������.�����


~
#13007
����������� ����� �����~
1 c0 4
����������~
if %actor.vnum% != -1
  halt
end

if !%arg.contains(����)% && !%arg.contains(�����)%
  return 0
  halt
end

if %actor.class% != 11
  osend %actor% ����������� � ������� ����� ������ �����, �� ���������� �� ��� ������������ �����.
  halt
end

wait 1
oload obj  13015
oechoaround %actor% ��� ���������� ������, %actor.name% ������%actor.u% ����������� ���� �����.
osend %actor% �� ������� ������� �������� � �������� ������ ����� �� ��������.
if (%world.curobjs(13000)% < 5) && (%random.100% < 50)
  wait 8
  oload obj 13000
  osend %actor% ��������� �������, �� ����� � ����� ����� �����.
end
opurge ����.���������.�����

~
$
$
