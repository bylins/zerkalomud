#13500
� ���� � ������� ����� �� [Great-ALL PC]~
0 r 100
~
* � ���� � ������ ����� PC *
if !%actor.name% || %actor.vnum% != -1
  halt
end
wait 1
switch %self.vnum%
  case 13511 * [������� �������]
    switch %actor.clan%
      case ���
        say �������� �������� ��������...
      break
      case ���
        ����
        say ��������� ��� ���������...
      break
      case ��
        ����
        say ������, �� ���� � ����� ������
      break
    done
  break
  case 13513 * [������� ������]
    if %acor.clanrank% < 4
      emote �������������� ��������� �� ���
    else
      �������
    end
  break
  case 13516 * [������]
    msend %actor% ������ ������ : '������, � - ������ ��������.'
    msend %actor% ������ ������ : '���� ������, ���� �� ������,'
    msend %actor% ������ ������ : '� ���� ������� ��� ���������, ��� ��� �������!'
    msend %actor% ������ ������ : '��� ���� ����, ������ ��� �� ������?'
    msend %actor% &K������ �������� �� ���, ������ ��� ������ �����-�� ������.&n
    * ����� ����� �� ��� "���������� 2"
  break
done
~
#13501
���� ������������ ����~
1 c 1
�������~
wait 1
if %arg.contains(���)%
  if %world.curobjs(13560)% >= 500
    osend %actor% �� �������� ����� � �����, �� ���� ���������!
    halt
  end
  oload obj 13560
  osend %actor% �� ������� ����������� ��� �� ����� � ������������ ������
  oechoaround %actor% ~~%actor.iname% ������~~%actor.g% ����������� ��� �� ����� � ������������ ������
  oforce %actor% ����� �����������.���
else
  osend %actor% ��� �� ������ �������?
end
~
$~
