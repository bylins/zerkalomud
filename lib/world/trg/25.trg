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
1 c 2
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
#2503
������ ���������� ������~
0 c 4
������~
wait 1
mechoaround %actor% %actor.name% �����%actor.g% ������ &G���������� ���������� ������&n.
msend %actor% �� ������ ������ &G���������� ���������� ������&n.
%actor.wait(5s)%
wait 5
if %random.100% < 20
  mecho �������� &R�&Y�&G�&C�&B� &R�&Y�&G� &R�&Y�&G��&C��&B��&M��&R �&Y�&G�&C�&n ���� � &G���������� ���������� ������&n!
end
~
#2504
������ ���������� ����� �� ������~
0 j 100
~
wait 1
if %object.vnum% == 2504
  wait 1
  mpurge %object%
  mechoaround %actor% %actor.name% �������%actor.g% &R�&Y�&G�&C�&B� &R�&Y�&G� &R�&Y�&G��&C��&B��&M��&R �&Y�&G�&C�&n �� &G���������� ���������� ������&n.
  msend %actor% �� �������� &R�&Y�&G�&C�&B� &R�&Y�&G� &R�&Y�&G��&C��&B��&M��&R �&Y�&G�&C�&n �� &G���������� ���������� ������&n.
  * ������ ����� �� ���� ���������� �������
  eval randomnum2504 %random.1000%
  if %randomnum2504% < 10
    mload obj 1144
    log &Y��������: &R%actor.name%&G ������� � ���������� ���� (&W�����-���� :)&G): &M���� ����&G
  elseif %randomnum2504% < 50
    switch %random.6%
      case 1
        mload obj 1130
        log &Y��������: &R%actor.name%&G ������� � ���������� ����: &M������ ���������� ����&G
      break
      case 2
        mload obj 1131
        log &Y��������: &R%actor.name%&G ������� � ���������� ����: &M��������� ���������� ����&G
      break
      case 3
        mload obj 1132
        log &Y��������: &R%actor.name%&G ������� � ���������� ����: &M������ ���������� �����&G
      break
      case 4
        mload obj 1133
        log &Y��������: &R%actor.name%&G ������� � ���������� ����: &M������ ���������� ����&G
      break
      case 5
        mload obj 1134
        log &Y��������: &R%actor.name%&G ������� � ���������� ����: &M������ ���������� �������&G
      break
      case 6
        mload obj 1135
        log &Y��������: &R%actor.name%&G ������� � ���������� ����: &M��������� ���������� ������&G
      break
    done
  elseif %random.1000% < 300
    * ������� ����������� + ������� � ��������� - 10% ����
    mload obj 2506
    mload obj 2505
    mload obj 11225
    mload obj 11225
    mload obj 11225
    mload obj 11225
    mload obj 11225
    �������� ���.���� ������
    * ������ ������������� ����� 
  elseif %random.10% < 5
    mload obj 2508 * �������� ���������
  else
    mload obj 11225 * ������� ����� � �����
  end
  give all .%actor.name%
  drop all
else
  tell %actor.name% ���� � ��� ���� &R�&Y�&G�&C�&B� &R�&Y�&G� &R�&Y�&G��&C��&B��&M��&R �&Y�&G�&C�&n �� ��������� ���.
  wait 2
  drop all
end
~
#2505
���� ������ ��������~
0 q 100
~
wait 1
if !%actor.name%
  halt
end
if %actor.remort% == 0 && %actor.level% < 29 && %actor.age% < 25
  wait 3
  mload obj 2506
  mload obj 2505
  mload obj 11225
  mload obj 11225
  mload obj 11225
  mload obj 11225
  mload obj 11225
end
�������� ���.���� ������
give all .%actor.name%
~
#2506
������� ����~
0 c 1
�������~
wait 1
log %actor.name% &C ������ ���������� ����.
mecho &G���������� ���������� ����&n � �������� ����� �� �����!
if %random.5% > 3
  mecho &R�&Y�&G�&C�&B� &R�&Y�&G� &R�&Y�&G��&C��&B��&M�� &R�&Y�&G�&C�&n ���� �� �����.
  mload obj 2504
end
%purge% %self%
~
$~
