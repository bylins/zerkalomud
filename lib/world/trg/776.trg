#77600
������~
1 ghijlt 100
~
wait 1
if (%actor.clan% == �� ) || (%actor.vnum% == 77601 )
  halt
end
wait 1
%purge% %self%
~
#77601
���������������~
1 c 2
���������~
if %actor.clan% != ��
wait 1
  opurge %self%
  halt
end
if %arg.contains(��������)%
  osend %actor% _�� ������ ������ ���� �� ��������...
  oechoaround %actor% _%actor.iname% �����%actor.g% ������� ������ ����������� ���� �� ��������...
if %world.curmobs(77609)% > 30
osend %actor% _� ������ �� ���������.
halt
end
  oload mob 77609 
wait 1
  opurge %self%
end 
~
#77602
���������������~
0 c 1
���~
if (%actor.clan%)!=��
   msend %actor% _\&R������ �� ������� ������ ��� ����� ��������...
   halt
  else
   mechoaround %actor% _\&G%actor.iname% ����� ������� ������� � ������� ������� ������ !!!\&w
   msend %actor% _\&W������� ������� ������ ������������ � ���� !!!
   mload obj 77630
   ���� ���� %actor.name%
wait 1
   mpurge %self%
   halt
end
~
#77603
���������������������~
0 r 100
~
wait 1
if %actor.clan% != ��
���� ��������� � �������
�� ������ ����������!!! %actor.name%!
   �� ������� ����������!!!  %actor.name%!
else
   ���� ������� �������.
end
~
#77604
������ �����~
0 d 3
����~
wait 1s
  �����
wait 2s
  if (%world.curobjs(77629)% < 10)
  mecho ���������� ������ �� ������� ��������� ������.
wait 2s
  say  ������ ���, �� ����� ������� ��� ��� ����.
  mload obj 77629
  wait 2s
  ��� ����� %actor.name%
end
~
#77605
�����������������������~
0 d 8
�����~
wait 1s
mecho �������� ����� ������ �������� � ������:'�����'
mechoaround %actor% �������� ����� ��������� %actor.dname% �� �����.
wait 2
wsend %actor% ... �� ������ �� ���������� �����.
mteleport %actor% 77632
wechoaround %actor% ���-�� ����� �� ����� � �������� �����.
end
~
#77606
�����������~
0 r 100
~
wait 1s
� ������ ����!
wait 1s
� �����%actor.g% �� �� ��������� ���� ������� � ��������� �� �����?
~
#77607
�������������~
2 c 100
������������~
if !(%arg.contains(��������)%) 
  wsend %actor% ��� �� ������ ������������?
  return 0
  halt
end
wsend %actor% ������������, �� �������� ����� ��������.
wechoaround %actor% ������������, %actor.name% �����������%actor.g% ����� ��������.
  wait 1s
  wsend %actor% ... �� ��������� � ������ ������� ���������.
  wteleport %actor% 77631
  wechoaround %actor% ���-�� ����������� �������� � �������� �����.
~
$~
