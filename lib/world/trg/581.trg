#58101
���~
1 gijl 100
~
wait 1
if !%actor.anem% || %actor.vnum% != -1
  halt
end
oforce %actor% ����
~
#58102
�������~
1 c 1
��������~
wait 1
if %arg%
  oecho &c������� ���������� �������&n �������� � ����� ������, %arg%.
else
  oecho &c������� ���������� �������&n �������� � ����� ������.
end
~
#58103
�����-��� ����~
0 c 100
������~
wait 1
if !%actor.name% || %actor.iname% != �������� && %actor.iname% != ������
  %echo% You're restricted to use me!
  halt
else
end
~
#58104
������ �����~
1 c 4
������~
wait 1
%actor.wait(15s)%
osend %actor% ������������, � ������������ �� �������� � �����!
oechoaround %actor% ������������, � ������������ %actor.name% �������%actor.q% � �����!
wait 1s
osend %actor% &Y����� �������� � ������. &R�� ��������� ������ ����� � ���� �����!&n
oechoaround %actor% ��� ������������ %actor.rname% �� ������ ���������� &Y�����&n, �������� &R��� ���� ������������� ������&n.
wait 1s
osend %actor% �� ��� �������� � �������� ��������.
oechoaround %actor% %actor.name% ��� �������� � �������� �������.
wait 3s
osend %actor% �� ��� �������� � �������� ��������.
oechoaround %actor% %actor.name% ��� �������� � �������� �������.
wait 10s
osend %actor% �, ��� ��� ����� ���� ����������?
~
$~
