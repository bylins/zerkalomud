#73000
������� �������~
0 m 1
~
wait 1
emot ����������%self.g% ������
eval target 0
switch %amount%
  * ������
  case 5200
    eval target 30224
  break
  * ��
  case 4800
    eval target 49947
  break
  default
    ���
    say � ���� �� �� �� ��� ������ ������?
    give %amount% ��� .%actor.name%
    halt
  done
  ���
  emot ������%self.g% ��������� �������� ������
  mechoaround %actor% %actor.name% �����%actor.q% � ������ ����.
  msend %actor% � ��� ����������� ������, � �� ��� �� �������� ��������...
  msend %actor% 
  mteleport %actor% %target% horse
  mechoaround %actor% %actor.name% ������%actor.u% ����� � ������ ����.
~
$~
