#2500
�������� �������~
2 c 100
��������������~
*�� ������� 2500 ��������� ���� ������� ������� �������, �������� � ������� ���� ������
wait 1
unset mobpozdravitel
calcuid mobpozdravitel 2500 mob
if (%mobpozdravitel% && (%mobpozdravitel.position% != 7))
  wteleport %mobpozdravitel% %self.vnum%
  wecho �������� � ������� ����� ����� �����, ���-�� ������ ���������.
  wait 1 s
  wforce %mobpozdravitel% ������
  wait 2 s
  wforce %mobpozdravitel% ������ .�������
  wait 1 s
  wforce %mobpozdravitel% ���� ��� .�������
  wait 1 s
  wforce %mobpozdravitel% � ����������� � ����� �����!
  wait 1 s
  wforce %mobpozdravitel% �������
  wait 3 s
  wecho ����� �� ������������ ���������� �� ������ (����� ��������, �� ����� �����������) ����������.
  wpurge %mobpozdravitel%
  %world.zreset(25)%
else
  wecho ������-�� ����������� ����� ���������� �������� � ������.
end
~
#2501
���� �� ������~
1 c 3
*~
* ���� ����������� ������� "����" ��� "drink" � �������� ����� �� ���� ������ -- �� ���������
if (%cmd.mudcommand% == ���� || %cmd.mudcommand% == drink || %cmd.mudcommand% == ��������� || %cmd.mudcommand% == taste)
  if (%arg.contains(���)% || %arg.contains(���)%)
    osend %actor% �� �������� &C���� �����&n �� &W������� &Y������ &R�����&n.
    osend %actor% �� ������������� ������ &G��������� ���&n!
    osend %actor% �������� ���������� ��������� ���!
    osend %actor% ������-�� �� �������� ������: "&R����� � ����� ���� ���� ������!&n"
    if !%actor.name%
      oechoaround ������� ����� ������� &C���� �����&n �� &W������� &Y������ &R�����&n.
    else
      oechoaround %actor% %actor.name% �����%actor.g% ������� &C���� �����&n �� &W������� &Y������ &R�����&n.
    end
    wait 1
  end
end
return 0
~
#2502
���� �� ������� �����������~
1 c 3
*~
* ���� ����������� ������� "����" ��� "drink" � �������� ����� �� ���� ������ -- �� ���������
if !%actor.name%
  return 0
  halt
end
if (%cmd.mudcommand% == ���� || %cmd.mudcommand% == drink || %cmd.mudcommand% == ��������� || %cmd.mudcommand% == taste)
  if (%arg.contains(���)% || %arg.contains(���)%)
    osend %actor% �� ������ ����������� �� &K��&g����� &K��&g���������&n.
    oechoaround %actor% %actor.name% �����%actor.g% ����������� �� &K��&g����� &K��&g���������&n.
    wait 1
    %actor.wait(2)%
  end
end
return 0
~
$~
