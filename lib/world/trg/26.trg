* ������������� BZEditor
* ���������� ��������� : 8
* ��������� 28.08.2011 12:20:57
#2600
������ ����� ��� ������~
2 e 100
~
if (%actor.clan%==��)
wecho �������� �� ������� ������ � ������ �� ���������...
halt
end 
wait 1
%actor.wait(2)%
wecho �������� �� ��������������� � ����� ������ ������� ��� ��������.
wecho ���������� ����������� ����������, �� ��������� ������...
foreach victim %self.all%
  if (%victim.maxhitp% < %victim.hitp%)
    eval dam126 %victim.hitp%*17/20
  else
    eval dam126 %victim.maxhitp%*17/20
  end
  wdamage %victim% %dam126%
  done
~
#2601
������ �� ��� ����~
2 e 100
~
if (%actor.clan%==��)
wecho �������� �� ������� ������ � ������ �� ���������...
halt
end 
wait 1
%actor.wait(10)%
wecho ��� ����� �����... ������ ��� ��� �����������... ���� ������ �����������...



~
#2602
�������� ����~
2 c 0
�����~
if (%actor.clan%!=��)
halt
end 
wechoaround %actor% %actor.name% ������� ������ ��������. 
wechoaround %actor% %actor.name% �����%actor.q%.
wsend %actor.name% �� �������� ������ ��������.
wteleport %actor.name% 2617
~
#2603
������ ����~
0 d 0
������~
if (%actor.clan%!=��)
� �� ��� �� ������??
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
dg_cast '���� �����' %actor.name%
���� ���
halt
end 
wait 1s
� ��� ��� ������� � ����� ����!
dg_cast '���������' %actor.name%
dg_cast '��� ���' %actor.name%
dg_cast '��� ����' %actor.name%
dg_cast '��� ����' %actor.name%
dg_cast '��� ���' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '������' %actor.name%
dg_cast '�������' %actor.name%
dg_cast '����� ����' %actor.name%
dg_cast '�����' %actor.name%
~
#2604
������� ���� :)~
0 c 100
�������~
mload obj 2600
mload obj 2601
mload obj 2602
mload obj 2603
mload obj 2604
mload obj 2605
mload obj 2606
mload obj 2607
mload obj 2608
mload obj 2608
mload obj 2609
mload obj 2609
mload obj 2610
mload obj 2610
mload obj 2611
���� ��� %actor.name%
~
#2605
����� �������~
0 c 100
�����~
� ���������! 
dg_cast '�����' %arg.words%
~
#2606
������ ����~
1 c 100
������~
if (%actor.clan%!=��)
halt
end 
dg_cast '���������' %actor.name%
dg_cast '��� ���' %actor.name%
dg_cast '��� ����' %actor.name%
dg_cast '��� ����' %actor.name%
dg_cast '��� ���' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '� �' %actor.name%
dg_cast '������' %actor.name%
dg_cast '�������' %actor.name%
dg_cast '����� ����' %actor.name%
dg_cast '�����' %actor.name%
~
#2607
���� �����~
1 c 100
��������~
if (%actor.clan%!=��)
halt
end 
oecho &R��� ���������� ����� ��������! ������ �� �������� ��� ���� ��������������� ����������.&n
dg_cast '���� ��'
dg_cast '���� ����'
dg_cast '���� �����'
dg_cast '���� ����'
dg_cast '���� ����'
dg_cast '���� ����'
~
$
$
