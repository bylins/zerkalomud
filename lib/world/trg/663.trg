* BRusMUD trigger file v1.0
#66300
����������� �������~
0 q0 100
~
wait 1
� ����������!
� ��������� � ���� ���� ������. ����� ��������� � ������� � �������.
� � ���� �� �������, �� ������� ��� ������� �����.
� � �����������.


~
#66301
������ �������������~
0 j0 100
~
wait 1
if %object.vnum% == 66300
  wait 8
  say �� ����. � ��������� ����������.
  wait 8
  say ����� ���� �������.
  wait 8
  %self.gold(+100)%
  ���� 100 ��� %actor.name%
  calcuid shkura 66300 obj
  mpurge %shkura.name%
end
wait 1
if %object.vnum% == 66301
  wait 8
  say �� ����. � ��������� ����������.
  wait 8
  say ����� ���� �������.
  wait 8
  %self.gold(+140)%
  ���� 140 ��� %actor.name%
  calcuid shkura 66301 obj
  mpurge %shkura.name%
end
wait 1
if %object.vnum% == 66302
  wait 8
  say �� ����. � ��������� ����������.
  wait 8
  say ����� ���� �������.
  wait 8
  %self.gold(+200)%
  ���� 200 ��� %actor.name%
  calcuid shkura 66302 obj
  mpurge %shkura.name%
end
wait 1
if %object.vnum% == 66303
  wait 8
  say �� ����. � ��������� ����������.
  wait 8
  say ����� ���� �������.
  wait 8
  %self.gold(+260)%
  ���� 260 ��� %actor.name%
  calcuid shkura 66303 obj
  mpurge %shkura.name%
end
wait 1
if %object.vnum% == 66304
  wait 8
  say �� ����. � ��������� ����������.
  wait 8
  say ����� ���� �������.
  wait 8
  %self.gold(+500)%
  ���� 500 ��� %actor.name%
  calcuid shkura 66304 obj
  mpurge %shkura.name%
end
wait 1
if %object.vnum% == 66305
  wait 8
  say ��! ����� �������� ������ ����.
  wait 8
  say ������ ��������� �������� ����.
  wait 8
  say ������� ���� �� ������ ����!
  wait 8
   if (%actor.level%>10) 
  mecho ������ ������� ������� ���� � �������� � �������� � ������� ������.
  wait 8
  mload obj 66306
  ���� ������ %actor.name%
  calcuid shkura 66305 obj
  mpurge %shkura.name%
   else
   mecho ������ ������� ���� ������ ���� � ������ ��. ��� �������!
   calcuid shkura 66305 obj
   mpurge %shkura.name%
end
end


~
#66302
����� �����~
0 f0 100
~
if (%world.curobjs(66300)% < 30) && (%random.10% <= 5)
   mload obj 66300
end










~
#66303
����� �����~
0 f0 100
~
if (%world.curobjs(66301)% < 30) && (%random.10% <= 5)
   mload obj 66301
end










~
#66304
����� �������~
0 f0 100
~
if (%world.curobjs(66302)% < 30) && (%random.10% <= 5)
   mload obj 66302
end










~
#66305
����� �����~
0 f0 100
~
if (%world.curobjs(66303)% < 30) && (%random.10% <= 5)
   mload obj 66303
end










~
#66306
����� �������~
0 f0 100
~
if (%world.curobjs(66304)% < 30) && (%random.10% <= 5)
   mload obj 66304
end










~
#66307
����� ��������~
0 f0 100
~
if (%world.curobjs(66305)% < 10) && (%random.10% <= 5)
   mload obj 66305
end










~
#66308
����� � ����~
2 c0 100
��������~
if !(%arg.contains(����)%)
   wsend       %actor% ���� ��� �� ������ �������� ?
   return 0
   halt
end
if (%actor.move%>50) && (%actor.level%>10) 
  wsend       %actor% �� ������� � ����� ����. �� � ������ �� ��� ������.
  wait 2s
  %actor.move(-100)%
  wsend %actor% �� ���� ������� � ��������� �������.
  wteleport %actor.name% 66347
else
  wsend %actor% � ��� �� ������ ��� ��� �����.
  return 0
end


~
#66309
����� �� ����~
2 c0 100
��������~
if !(%arg.contains(����)%)
   wsend       %actor% ���� ��� �� ������ �������� ?
   return 0
   halt
end
if (%actor.move%>50) && (%actor.level%>10) 
  wsend       %actor% �� ������� � ����� ����. �� � ������ �� ��� ������.
  wait 2s
  %actor.move(-100)%
  wsend %actor% �� ���� ������� ������.
  wteleport %actor.name% 66346
else
    wsend %actor% � ��� �� ������ ��� ��� �����.
   return 0
end


~
$
$
