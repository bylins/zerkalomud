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
2 c 0
������~
wait 1
%actor.wait(8s)%
wsend %actor% ������������, � ������������ �� �������� � �����!
wechoaround %actor% ������������, � ������������ %actor.name% �������%actor.q% � �����!
wait 1s
wsend %actor% &Y����� �������� � ������&n. &R�� ��������� ������ ����� � ���� �����!&n
wechoaround %actor% ��� ������������ %actor.rname% �� ������ ���������� &Y�����&n, �������� &R��� ���� ������������� ������&n.
wait 1s
wsend %actor% �� ��� �������� � �������� ��������.
wechoaround %actor% %actor.name% ��� �������� � �������� �������.
wait 3s
wsend %actor% �� ��� �������� � �������� ��������.
wechoaround %actor% %actor.name% ��� �������� � �������� �������.
wait 3s
wsend %actor% �, ��� ��� ����� ���� ����������?
~
$~
