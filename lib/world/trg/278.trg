#27800
����������~
0 r 100
~
wait 1
if %actor.class% == 8
 msend %actor.name% ������ ���� ���������� �� ���.
 mechoaround %actor% ������ ���� ���������� ��  %actor.rname%.
 tell %actor.name% �� ���� �������� ���� ������ ��������� ���, ��� �������� ����
 tell %actor.name% � ��� ����������. � ��� ����� ����� ������� ������������ ��� �����,
 tell %actor.name% ���� ��������� ��� ����� ������.
 tell %actor.name% ���� ��������� ��������, �� �������?
 ���
 wait 1s 
 tell %actor.name% �� ���� ��������.
 tell %actor.name% ����� ���� ���� ������ �������, �� ��������� ������ ���,
 tell %actor.name% ���-�� �� ������ ���������� ������, ������� �� ���� ���
 tell %actor.name% ����������� ���������� ��� ����. ���� ��� � � ����������� ����.
 ���
else
 tell %actor.name% ����� �� ������ ����?
 mkill %actor.name%
end
~
#27801
������������~
0 f 100
~
mload obj 27801
mload obj 27803
~
#27802
����������������~
0 r 100
~
wait 1
if %actor.class% == 8
 msend %actor% ����� ������� ���� ��������� �� ���.
 mechoaround %actor% ����� ������� ���� ��������� �� %actor.rname%.
 tell %actor.name% ��, �� ����������� � ������ �����, ���-�� ��� ������� ���
 tell %actor.name% ��� ����� � ��� �� ����.
 mecho ����� ������� ������ �� ��� ������ ������, ��������� �� ���� � ����� �����.
 tell %actor.name% �������� �� ��������� �����.
 tell %actor.name% ��� �� �� �������� �� ����� �� �������.
 wait 1s 
 ������
else
 wait 2s
 msend %actor% ����� ������� ���������� ��������� �� ���.
 mechoaround %actor% ����� ������� ���������� ��������� ��  %actor.rname%.
 tell %actor.name% ������� ������� ��������� � ���, ��� � ���� �� �������,  �� � �������
 tell %actor.name% ��������, �� ������� ������� �� ����. � ��� ��� �� ���, ������� ����
 tell %actor.name% ���������� ���. ��� ���� �� �������� ��� ������� ������ ���, ����
 tell %actor.name% ��� �������. ���� ������, ������ ���, ��������� � ����, � � �� ��������
 tell %actor.name% � �����. �����, ��� ��� ��� ������, ������� ����� ���������� ���������,
 tell %actor.name% ������� ���, � ����� ����� ��� ����������.
end
~
#27803
������������������~
0 f 100
~
mload obj 27802
~
#27804
��������������~
0 j 100
~
wait 1
if %object.vnum%==27807
mecho ������� ����������� �������� ����.
mecho ������� ����, ����� ������� ������� ���.
wait 1s
 if (%random.5%==1) && (%world.curobjs(27808)%<10)
wait 5s
mecho ������� ����� ������� � ������, ���-�� ����� � ���.
mecho ����� ������� ����, ��������� ����� �� ���� �����������.
say ��� ��� ����������, ������.
mload obj 27808
wait 1
���� ��� %actor.name%
 else
mecho ������� ���� ������� ����, ��� ����� �� �������.
���
wait 1
say ���� �� ����������.
 end
 mpurge ����
end
~
#27805
����������������~
0 j 100
~
wait 1
if (%object.vnum%==27802) && (%actor.class%==8)
msend %actor% ��� � �������.
mecho ������ ����� ����������� � ��������� � ������.
msend %actor% ������ ���� ��� ���, ������ ���, � ����� � �� ������ ���.
msend %actor% �� ��� �������, ������� ����.
if (!%actor.spell(������� ����)% && (%actor.level%>13)
msend %actor% ��� �����, � ������� �� ����� �������������� �� ��� � ��������.
mspellturn %actor.name% �������.���� set
msend %actor% ������� ���� ������ ���������� ��� �������������.
else
�����
msend %actor% �� ��� ������ ���������� ������� ���� - ���� ������ ����� ���������.
%self.gold(+300)%
���� 300 ��� %actor.name%
end
mpurge ������
end
~
#27806
���������������~
0 j 100
~
wait 1
if %object.vnum%==27803
say %actor.name%, �������� ������.
mecho ����� ������� ����������� �������� ������.
say ������ � ��� ������ �����������, ����� ������ � ��� ������ ��� ������.
say %actor.name%, ��� �����, �����������.
switch %actor.class%
* ������
case 0
 if (!%actor.spelltype(���������)%) && (%actor.level%>11)
  mspellturn %actor.name% ��������� set
  say ��� ���� ������ ���������, %actor.name%. 
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* ������
case 1
 if (!%actor.spelltype(����������)%) && (%actor.level%>17)
  mspellturn %actor.name% ���������� set
  say ��� ���� ������ ����������, %actor.name%. 
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* ���
 case 2
 if (!%actor.skill(��������)%) && (%actor.level%>11)
  mskillturn %actor.name% �������� set
  say ��� ���� ������ ��������, %actor.name%.
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* ��������
 case 3
 if (!%actor.skill(����������� �����)%) && (%actor.level%>11)
  mskillturn %actor.name% �����������.����� set
  say ��� ���� ������ ����������� �����, %actor.name%.
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* �������
 case 4
 if (!%actor.skill(��������)%) && (%actor.level%>11)
  mskillturn %actor.name% �������� set
  say ��� ���� ������ ��������, %actor.name%.
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* ���������
 case 5
 if (!%actor.skill(���������)%) && (%actor.level%>11)
  mskillturn %actor.name% ��������� set
  say ��� ���� ������ ���������, %actor.name%.
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* ��������
case 6
 if (!%actor.spelltype(����������)%) && (%actor.level%>11)
  mspellturn %actor.name% ���������� set
  say ��� ���� ������ ����������, %actor.name%. 
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* ���������
case 7
 if (!%actor.spelltype(����������)%) && (%actor.level%>13)
  mspellturn %actor.name% ���������� set
  say ��� ���� ������ ����������, %actor.name%. 
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* ������
case 9
 if (!%actor.spelltype(���������)%) && (%actor.level%>19)
  mspellturn %actor.name% ��������� set
  say ��� ���� ������ ���������, %actor.name%. 
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* �����
case 12
 if (!%actor.spelltype(����� ��������)%) && (%actor.level%>18)
  mspellturn %actor.name% �����.�������� set
  say ��� ���� ������ ����� ��������, %actor.name%. 
 else
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
* �����
case 13
 if (%world.curobjs(207)% < 50) && (%random.2% == 2)
   say ���, ���� ��������� ������, %actor.name%!
   mload obj 207
   ���� ��� %actor.name%
 else
  say ��� � ���� ������ ������ ���������� ������ ������, ��� ��, %actor.name%!
  say ������ �� ������ � �������.
  %self.gold(+300)%
  ���� 300 ��� %actor.name%
 end
 break
default
%self.gold(+500)%
���� 500 ��� %actor.name%
 break
done
mpurge ������
end
~
$~
