* BRusMUD trigger file v1.0
#5400
�������2 ~
0 j 100
~
wait 2
switch %object.vnum%
case 5402
  say  - � ��� ����� ��� �����, ���� ������� �� ����������� �� ����,
  say  ���� �� ������� ���������� ����!
  say  - �������, ������ �����.
  %self.gold(+100)%
  ���� 100 ��� %actor.name%
  mpurge ���������� ����
  if (%world.curobjs(229)% < 50) && (%random.10% == 1)
   mload obj 229
   ���� ��� %actor.name%
  end
break
default
  say  ���� �@^*, ����� ��� ���?
  eval getobject %object.name%
  ����� %getobject.car%.%getobject.cdr%
  break
done
~
#5401
�������1 ~
0 q0 100
~
������
msend %actor.name% - ������, ������, ������, ��� ������� ����� ��������!
msend %actor.name% - ������, ���� ������� ... ����, ���� �����, � � ����� �� ��������.
~
$
$
