#58000
�����~
1 ghijlt 100
~
wait 1
if %actor.clan% == ��
  halt
endif
%purge% %self%
~
#58001
������ �� �����~
0 m 0
~
wait 1
��� %actor.name%
halt
switch %amount%
  case 20
    mecho ������� ������� ������.
    dg_cast '������ ���������' %actor.name%
    break
  case 30
    mecho ������� �������� ���������� �� �����.
    dg_cast '��������' %actor.name%
    break
  case 40
    mecho ������� ��������� � ���� � ���-�� �����������.
    dg_cast '�����������' %actor.name%
    break
  case 50
    mecho ������� ������� ������.
    dg_cast '������' %actor.name%
    break
  case 60
    mecho ������� ������� ������.
    dg_cast '�������������' %actor.name%
    break
  case 70
    mecho ������� ���������� ��� ���� �����.
    dg_cast '����' %actor.name%
    break
  case 80
    mecho ������� �������� ������.
    dg_cast '�����' %actor.name%
case 350
mecho ������� ����� � ������� ����� ���������� ������ ���������� ���� ��� ���...
dg_cast '����' .%actor.name%
dg_cast '��������' .%actor.name%
dg_cast '�����������' .%actor.name%
dg_cast '������' .%actor.name%
dg_cast '�������' .%actor.name%
dg_cast '����� �����' .%actor.name%
dg_cast '�����' .%actor.name%
    break
  default
    msend %actor% ��������, �� � �� ���� ��� �� ������ �� ���� �� ��� ������.
    msend %actor% ��� ��� � ���� ������� ��� ���:
    msend %actor% _�� 20 ��� - ������� ���� �����.
    msend %actor% _�� 30 ��� - ��� ����� � �����.
    msend %actor% _�� 40 ��� - ���������� ������.
    msend %actor% _�� 50 ��� - �� ������� ����� �������.
    msend %actor% _�� 60 ��� - ���� ���� ������ ��������.
    msend %actor% _�� 70 ��� - ������� �� ������� �������.
    msend %actor% _�� 80 ��� - ������� ������,��� ����� � ����.
msend %actor.name% �� 350 ��� - �������� ��� � �����...
done
~
#58002
����� � �������~
0 q 0
~
msend %actor% ��� ����� �������� �������� ��� � ����� ����������.
msend %actor% ��� ��� � ���� ������� ��� ���:
msend %actor% _�� 20 ��� - ������� ���� �����.
msend %actor% _�� 30 ��� - ��� ����� � �����.
msend %actor% _�� 40 ��� - ���������� ������.
msend %actor% _�� 50 ��� - �� ������� ����� �������.
msend %actor% _�� 60 ��� - ���� ���� ������ ��������.
msend %actor% _�� 70 ��� - ������� �� ������� �������.
msend %actor% _�� 80 ��� - ������� ������,��� ����� � ����.
~
#58003
�������� �������~
1 c 2
������~
if %actor.clan% != ��
  opurge %self.name%
  halt
endif
if %arg.contains(��������)%
  osend %actor% _�� ������ �������� ������ ������� ��������...
  oechoaround %actor% _%actor.iname% �����%actor.g% �������� ������ ������� ��������...
  oload mob 58017 
  opurge %self.name%
endif 
~
#58004
�������� �������~
0 c 1
����~
msend %actor% _\&Y_\&4_�� ��������� ������� ������� � ��� ��������� �������...\\c07\\c24
mpurge %self%
~
#58005
������� ������������~
0 b 100
*~
���� \&C������������ ���� ����� �������.\&w
~
#58006
�������� �������~
0 c 1
��������~
if (%actor.clan%)!=��
   eval hitpmin %actor.hitp%*3/15
   mdamage %actor% %hitpmin% 
   msend %actor% _\&R������� �� ������� ������ ��� ����� ��������...
   msend %actor% _�������� ��������� �������� �� ������� � �������...\&w
   mpurge %self%
   halt
  else
   mechoaround %actor% _\&G%actor.iname% ����� ������� ������� � ������� ������ ������ !!!\&w
   msend %actor% _\&W������� ������ ������ ������ ��� !!!
   halt
endif
~
#58007
������� �������~
0 c 1
����������~
if (%actor.clan%)!=��
   eval hitpmin %actor.hitp%*3/15
   mdamage %actor% %hitpmin% 
   msend %actor% _\&R������� �� ������� ������ ��� ����� ��������...
   msend %actor% _�������� ��������� �������� �� ������� � �������...\&w
   mpurge %self%
   halt
  else
   mechoaround %actor% _\&B%actor.iname% �������� ������� �� ����� � ��� ��������� �� �������� ������ !!!\&w
   msend %actor% _\&B������� ����� ����� ����� ����� �������� ��� �� �������������!!!\&w
   rest
   wait 1s
   ���
   halt
  end
~
#58008
������� �������~
1 c 2
����������~
if (%arg.contains(�������)%)
eval orelresh %random.10%
if %orelresh%<5 
   osend %actor% _\&C�� ���������� ������ ������� � ������._\&n
   osend %actor% _\&C�������������� ��������� ��� ��� ����� �� �����._\&n
   osend %actor% _\&G������ "�����". _\&n
   oechoaround %actor%  _\&C%actor.name% ���������%actor.g% ������ ������� � ������._\&n
   oechoaround %actor%  _\&G������� ����� �� ����� � ������ "�����"._\&n
   oechoaround %actor%  _\&Y%actor.name% ��������� �����%actor.g% ������ �� ����� ������ �������._\&n
   wait 2s
elseif %orelresh%>5 
   osend %actor% _\&C�� ���������� ������ ������� � ������._\&n
   osend %actor% _\&C�������������� ��������� ��� ��� ����� �� �����._\&n
   osend %actor% _\&G����� "����"._\&n
   oechoaround %actor%  _\&C%actor.name% ���������%actor.g% ������ ������� � ������._\&n
   oechoaround %actor%  _\&G������� ����� �� ����� � ����� "����"._\&n
   oechoaround %actor%  _\&Y%actor.name% ��������� �����%actor.g% ������ �� ����� ������ �������._\&n
   wait 2s
end
end    
~
#58009
������� ������� 1~
0 c 1
�������~
mload obj 58034
���� ���� %actor.name%
mpurge %self%
~
#58010
����������~
2 d 100
����������~
wforce �������.������  say ������ ������
if (%actor.bank%<1050)
wforce �������.������ say ��� �� ��� ����� ������... � ��� � ���� ���������� ���?
halt
end
wload obj 58010
wload obj 58011
wload obj 58012
wload obj 58013
wload obj 58016
wload obj 58017
wload obj 58018
wload obj 58019
wload obj 58020
wload obj 58021
wload obj 58022
wecho ������ ������ �� ��� ���� ����������...
wforce %actor.name% ����� ����
wforce %actor.name% ����� ����
wforce %actor.name% ����� �����
wforce %actor.name% ����� ����
wforce %actor.name% ����� ����
wforce %actor.name% ����� ����
wforce %actor.name% ����� ����
wforce %actor.name% ����� �����
wforce %actor.name% ����� �����
wforce %actor.name% ����� ������
wforce %actor.name% ����� �����
wforce %actor.name% ����� ����
set %actor.bank(-1050)%
wforce %actor.name% ����� ���
~
$~
