* BRusMUD trigger file v1.0
#13600
����� �� �����~
2 c0 1
����� ���������� �����������~
wait 1
if %arg.contains(����)%
  wsend %actor% �� ������ ����������� �� �����.
  wechoaround %actor% ~~%actor.iname% �����~%actor.q% ����������� �� �����.
  wteleport %actor% 13670 horse
  detach 13600 %self%
else
  wsend %actor% ���� ������?
end





~
#13601
ZoneReset~
2 f0 100
~
attach 13600 %self%











~
$
$
