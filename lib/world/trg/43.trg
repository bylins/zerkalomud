* BRusMUD trigger file v1.0
#4300
������~
0 g0 30
~
if %actor.id%!=-1
  exit
end
wait 1
say ����������� ���� ��������, ��� ���������� ����� �������, ���� �������� ����.
�����
wait 1
say � ������� ����� ����������� ��������� ��������� �����, ��������� ������, �� �����. �� ��� �� �� ������ ��� � ����? �� ������ ���-�� �� ������ ������.

~
#4301
������ ������~
0 j0 100
~
wait 1
if %actor.vnum%!=-1
 halt
end
if %object.vnum% == 4300
say �������������, �� ����� �����%actor.q% ���.
wait 2
say ������ �� ��� �������� ����� �� ����.
 switch %random.5%
 case 1
       mload obj 202
       ���� ��� %actor.name%
  break
 case 2
       mload obj 225
       ���� ��� %actor.name%
  break
 case 3
       mload obj 527
       ���� ����� %actor.name%
  break
 default
   %self.gold(+20)%
   ���� 20 ��� %actor.name%
  break
done
mpurge ������
else
say ����� ��� ���???
eval getobject %object.name%
���� %getobject.car%.%getobject.cdr%
end

~
$
$
