#40000
������ � ����~
0 r 100
~
if %actor.class%==2 
  say ������ ����, %Actor.name%
  say ���� �, ������� ���� ��������� ���������.
  �����
  wait 1s
  say ��� ����� ������ �������� ����, ���������?
else
  ���
  wait 1s
  say � - ������� ������ � �� ���� ����� � ����� ���
end
~
#40001
����������� � �����~
0 d 0
�������� ������ �� ~
if %actor.class%==2 
  say �������! � ����, ��� �� �� ������� ����������.
  ssy ���� ����� - ����� ����� ����� ������ �������. 
  say ���� � ��� ���� ������ ����.
  say �� ���� ���������� ����� ���������, �� ���� �������� ����������.
  say ������ �� ������ �� ������ ���������, � ������� �������� �� �� ���������.
  say � �� � ��� ��� ������ �� ���� ��� �� ��..
  say � ��, �����, ��� ���������.
  say ��� ��.
  mdoor 40000 south purge
  mdoor 40000 south room 40001
  detach 40000 %self.id%
  attach 40005 %self.id%
  detach 40001 %self.id%
end
~
#40002
���������� �����~
2 c 0
��������~
if !%arg.contains(�����)% 
  wsend %actor% ��� ������?
  halt
else
  if %actor.skill(��������)%<50
    wsend %actor% �� �������� ������� � �����, �� �� ������ ����������� � ����������� � ������� ��
  else 
    wsend %actor% �� ����� ������� �����
    wdoor 40082 south purge
    wdoor 40082 south room 40061
  end
end
~
#40003
��������� ������~
2 c 0
�����������~
if !%arg.contains(�������)% 
  wsend %actor% ��� �������������?
  halt
else 
  if %actor.skill(��������)%<75
    wsend %actor% �� ���������� ����������� �������, �� ��� �� ������� ������. ��� ���������.
    wdamage %actor% 450
  else
    wsend %actor% �� ����� ����������� �������.
    calcuid sun1  40012 obj
    wpurge %sun1%
    wload obj 40015
    wsend %actor% ������, ���-�� ������� ��� - �� ��������� ������ ����, ��������������� ���� ��������.
    wsend %actor% �������, ����� ���������� � ���������
    wait 3s
    if %actor.skill(����������)%<40 
      wsend %actor% �������� �� ���� ��������, ��� ��������.
      wecho %actor% ������ ������ ��� ��������
      wteleport %actor% 40077
    else 
      wsend %actor% �� ������������ ���������� � ���� � ������ ���� ������ �������.
    end
    attach 40004 %self.id%
    detach 40003 %self.id%
  end
end
~
#40004
���������� ������~
2 c 0
�������� ��� ���� ����� ������ �������~
if !%arg.contains(������)%
  wsend %actor% ��� ���������?
  halt
else
  if %actor.skill(��������)%<80
    wsend %actor% �� �� ������ ������� ���� �����.
  else 
    wsend %actor% �� ������� �����.
    wload obj 40013 
    detach 40004 %self.id%
  end
end
~
#40005
���� ����� ����~
0 j 100
~
if %object.vnum%!=40013
  ��� 
  say ��� ���? 
  say � �����, �� ������� ���
  ��� ���
else
  ���
  wait 1s
  say � ��� � ����, ��� �� ����������! ���� � �� ����������! ������ ��� ���� ��� ����� ��� ��?
  say � ������ �������
  wait 1s
  say ��� ��, ��������, �������, ����� ���� �� ������ ������� �� ����, ���� �� �� ��������
  say ��� ���, ����� � ���� �������� ������� � ����.  
  mecho �� �� ������ � ������ ��������, ��� ������ �����.
  wait 1s 
  mecho �  ������ �������� � ��� �� ������
  if %actor.skill(����������)%<100
    mecho ����� �� �������� ��� ��� ��� �������
    mskilladd %actor.name% ���������� 10
  else
    mecho �� ��� �� ������������� �����������. ������ ������ ��� ��� �� ���������.
  end
  detach 40005 %self.id%
end
~
#40006
����� ����~
0 c 0
������ ���������~
mecho ��� ��������� � ����� �� ����
say ������ ����, �������..
say ����� ������ ��� ������ ������.
say ��� ������� ���� �.. ������ �� ������ ������ �������
say ������ �, ��� ������ ��������� ����� �� ��� ��������������� ������, ���������� �������
say �����, ���� �� ������� �������, ��������� �� ��� �����?
attach 40007 %self.id%
detach 40006 %self.id%
~
#40007
���� ���� ������~
0 j 100
~
if %object.vnum%!=40014
  say �� �� ���.. 
  ��� ���
else 
  say ��, ��� �� �����!
  say ������� � ����.
  say ������ � ���� ����, ������ ����.
  say ������ � ��� ����� �������� � ���� ������� �� ������� � ����-������ ���-������.
  say �������� ����, ��� ��� �������
  if %actor.skill(�������)%<80
    mskilladd %actor% ������� 10
  end
  %purge% %self.id%
  detach 40007 %self.id%
end
~
#40008
reset~
2 f 100
~
calcuid room 40082 room
attach 40082 %room.id%
calcuid mob 40011 mob
attach 40000 %mob.id%
attach 40001 %mob.id%
calcuid mob 40017 mob
attach 40006 %mob.id%
~
#40009
������ � ������~
2 e 100
~
wecho ������ ������� ���� ���� ��������.
~
#40010
�����~
2 c 0
������~
if !%arg.contains(�����)%
  wsend %actor% ���� ���� ?
else
  wsend %actor% �� ������ �� ����� � ����� �������� � �������.
  wdoor  40061 north purge
  wdoor 40061 north room 40082
~
#40011
������ �����~
0 q 100
~
wait 1
����
say �� ��� �����, �����?
say ��� ��� ��������?
����
wait 2
say � �� ����� ���!
���� %actor.name%
wait 2
say �� ���� ������ � ������ �����!
wait 3
���� %actor.name%
~
#40012
����� ������~
0 f 100
~
if (%world.curobjs(40006)% < 5) && (%random.10% <= 2)
  mload obj 40006
end
if (%world.curobjs(40007)% < 5) && (%random.10% <= 2)
  mload obj 40007
end
if (%world.curobjs(40008)% < 5) && (%random.10% <= 2)
  mload obj 40008
end
~
#40013
����� �������~
0 f 100
~
if (%world.curobjs(40010)% < 3) && (%random.25% <= 2)
  mload obj 40010
end
~
#40014
����� �������~
0 f 100
~
if (%world.curobjs(40011)% < 3) && (%random.25% <= 2)
  mload obj 40011
end
~
#40015
����� ��������� ����~
0 f 100
~
if (%world.curobjs(40000)% < 3) && (%random.25% <= 2)
  mload obj 40000
end
~
#40016
����� ��������1~
0 f 100
~
if (%world.curobjs(40001)% < 4) && (%random.20% <= 2)
  mload obj 40001
end
~
#40017
����� �������1~
0 f 100
~
if (%world.curobjs(40002)% < 4) && (%random.20% <= 2)
  mload obj 40002
end
~
#40018
����� �������2~
0 f 100
~
if (%world.curobjs(40003)% < 4) && (%random.20% <= 2)
  mload obj 40003
end
~
#40019
����� �������3~
0 f 100
~
if (%world.curobjs(40004)% < 4) && (%random.20% <= 2)
  mload obj 40004
end
~
#40020
����� �������4~
0 f 100
~
if (%world.curobjs(40026)% < 4) && (%random.20% <= 2)
  mload obj 40026
end
~
#40021
����� �������5~
0 f 100
~
if (%world.curobjs(40027)% < 4) && (%random.20% <= 2)
  mload obj 40027
end
~
#40022
����� ���������1~
0 f 100
~
if (%world.curobjs(40005)% < 3) && (%random.20% <= 2)
  mload obj 40005
end
~
#40023
����� ���������2~
0 f 100
~
if (%world.curobjs(40009)% < 3) && (%random.20% <= 2)
  mload obj 40009
end
~
#40024
����� ���������3~
0 f 100
~
if (%world.curobjs(40020)% < 3) && (%random.20% <= 2)
  mload obj 40020
end
if (%world.curobjs(40023)% < 6) && (%random.20% <= 2)
  mload obj 40023
end
~
#40025
����� ��������2~
0 f 100
~
if (%world.curobjs(40016)% < 5) && (%random.20% <= 2)
  mload obj 40016
end
~
#40026
����� �������~
0 f 100
~
if (%world.curobjs(40017)% < 2) && (%random.25% <= 2)
  mload obj 40017
end
~
#40027
����� ���������� ������~
0 f 100
~
if (%world.curobjs(40019)% < 3) && (%random.20% <= 2)
  mload obj 40019
end
~
#40028
����� ���~
0 f 100
~
if (%world.curobjs(40014)% < 4) && (%random.10% <= 2)
  mload obj 40014
end
~
#40029
����� ��������~
0 f 100
~
if (%world.curobjs(40021)% < 5) && (%random.15% <= 2)
  mload obj 40021
end
~
#40030
����� ��������2~
0 f 100
~
if (%world.curobjs(40025)% < 5) && (%random.15% <= 2)
  mload obj 40025
end
~
#40031
����� ��������3~
0 f 100
~
if (%world.curobjs(40022)% < 5) && (%random.15% <= 2)
  mload obj 40022
end
~
#40032
����� ������~
0 f 100
~
if (%world.curobjs(40024)% < 5) && (%random.15% <= 2)
  mload obj 40024
end
~
$~
