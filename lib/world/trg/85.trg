#8500
����� �� �������� ����~
2 c 0
����� ����������� ���������~
wait 1
if !%arg.contains(���)%
  wsend    %actor% ���� �� �����-�� ���������?
  halt
end
wsend %actor% �� ������ ����������� ����� �� ������.
wechoaround %actor% %actor.name% �����%actor.g% ����������� ����� �� ������.
wait 1s
if %actor.rentable%
  wteleport %actor% 8591 horse     
  wait 1
  wsend %pc% _�� ������������� �� �������� ����.
  wechoaround %actor% %actor.name% �����%actor.g% �� �������� ����.
end 
end
~
#8501
����������~
2 c 0
�������� ���������~
wait 1
if !%arg.contains(����)%
  wsend    %actor% ������������, ���� �������-�� ���������?
  halt
end
wsend %actor% ���������� ��������, �� ��������� � ���� � ������������ �� ����� �����.
wechoaround %actor% %actor.name% ��������%actor.g% � ����, ���� �� ���� ��� � ���.
wait 1
wteleport %actor% 8571 horse     
~
#8502
��� �����~
2 c 1
������ ������ ������~
wait 1
if !%arg.contains(������)%
  wsend %actor% ��� �� ����������� ����������?
  halt
end
wsend %actor% �� ���������, ���� �� �� �� ������, �������� ������.
wechoaround %actor% %actor.name% �����%actor.g% ����������� ���������� ������ �������� ���.
wait 1s
wecho __� ������ ���-�� �����������. ������ �� ��� ������� �������� �����.
wload mob 8503
calcuid target 8503 mob
wforce %target% kill .%actor.name%
detach 8502 %self.id%
~
#8503
����� ��������~
0 q 100
~
wait 5
switch %random.4%
  case 1
    ��� �������� ��� � ���������.
    say � ���� ��� �� � ����� �������?
  break
  case 2
    say ������-�� ������� ������� �� ��������
    ��
  break
  case 3
    ����
    say � �� �? � �� � ��������?
  break
  case 4
    say ���� ����� ����, ���� ���� ������� ����. ���� �� ������� �������.
    ����
  break
done
~
#8504
�������� �������� ����~
0 q 100
~
wait 1
if %self.fighting%
  halt
end
say ��������� ����. 
say ������������� ������? ��� �����.
set j 0
set needgold 30
foreach target %self.pc%
  eval needgold %needgold%+%target.level%*(%target.remort%+1)
  eval j %j%+1
done
if %j% > 1
  say �� ��������� � ��� ���� ������...
else 
  say �� ��������� � ���� �����...
end
mecho �������� ���������� �������� ��� � ��� �� ������.
wait 2
say ...%needgold% ��� �����, � ���� ������ - �� ���������.
���
~
#8505
���� ����� ���������.~
0 m 1
~
wait 1
if %self.fighting%
  ���
  say �� �� ���� ������, ��%actor.q% ���!
  halt
end
set needgold 30
foreach target %self.pc%
  eval needgold %needgold%+%target.level%*(%target.remort%+1)
done
mecho �������� ���������� ������.
wait 1s
if %amount% < %needgold%
  ����
  say ������� �� - %needgold% ���! � �� ����� ������.
  give %amount% ��� .%actor.name%
  halt
end
emot �������� ������
wait 1
mecho �������� �������� ���������, � ��� ������ ���������� �� �����.
mecho ����� � ������� � ������� ������� �� �������.
mdoor 8512 south purge
mdoor 8511 north purge
wait 4s
mecho ����� ������������ �� ������ ������.
wait 4s
mecho ��� ���������� ��������� ��������.
wait 4s
mecho ����� � ���������� ������ ������� � ������.
mdoor 8512 south room 24007
mdoor 24007 north room 8512
wait 2s
say ���������, ������� �������, �� ������������.
wait 5s
if %self.fighting%
  halt
end
mecho �������� �������� ���� � ������.
mforce all stand
foreach target %self.pc%
  mforce %target% south
done
mecho �������� � ���������� ����� ������� �� �����.
mdoor 8512 south purge
mdoor 24007 north purge
mdoor 8512 south room 8511
mdoor 8511 north room 8512
%self.gold(0)%
~
$~
