#11600
�������� ��������~
0 q 100
~
wait 1
switch %random.2%
case 1
  say ��, �� ����� ������! � ��������� ���� ������������ �������!?!
  wait 2
  say �� ��� ������ �� �����!
  wait 2
  ��� %actor.name%
 break
default
___say ���, ��������! ������������! �� ��� � ������ ���� �����!
   wait 3
   ��� %actor.name%
 break
done
~
#11601
����� ������~
0 f 20
~
if %world.curobjs(11600)% < 50
   mload obj 11600
end
~
$~
