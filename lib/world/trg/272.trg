#27200
�������������~
0 r 100
~
msend %actor% ����������, ������!
msend %actor% ������� ��������� ��������� �� ���� �������.
msend %actor% ������� �������� ��������� ������� � �����
msend %actor% ���� �������. �� � ���� �� �����, � ����������
msend %actor% � ���� ��������.
msend %actor% ������ ���������� ������� � ������ �� �������.
msend %actor% � ��� � ������ ����� � �������� ���������, ���
msend %actor% �������.
msend %actor% � ��� �����, ��� ���� �����������.
~
#27201
��������������~
0 f 100
~
if %world.curobjs(27202)%==0
  mload obj 27202
end
switch %random.4%
  * ������
  case 1
    if %world.curobjs(514)%==0
      mload obj 514
    end
  break
  * �������� ������
  case 2
    if %world.curobjs(509)%==0
      mload obj 509
    end
  break
  * ����
  case 3
    if %world.curobjs(525)%==0
      mload obj 525
    end
  break
  default
    if %world.curobjs(539)%==0
      mload obj 539
    end
  break
done
~
#27202
����������������~
0 j 100
~
wait 1
if %object.vnum% != 27202
  say ����� ��� ���?
  ���� %object.name%
  halt
end
wait 1
mpurge %object%
eval minlevel  24-%actor.remort%
say ������� %actor.name%, �������� ������������� ����.
* ����
if (%actor.class% == 5) && (!%actor.skill(������� ������)%)
  if (%actor.level%>=%minlevel%)
    mecho 
    wait 1
    msend %actor% � ������� ���� ���� ������.
    wait 1
    mskillturn %actor.name% �������.������ set
    msend %actor% ���������� ��� ������� ������, ������ �� ������
    msend %actor% ���� ���������� ����� ����� ���������� ������.
    halt
  else
    msend %actor% ������ ��������� �� ���.
    mechoaround %actor% ������ ��������� �� %actor.vname%.
    msend %actor% �� ��� ��� ����� �������� ������ ������, �� � ����������� ����.
  end
end
* ������
if (%actor.class% == 9) && (!%actor.spelltype(���������)%)
  if (%actor.level%>=%minlevel%)
    mecho 
    wait 1
    msend %actor% � ������� ���� ���� ������.
    wait 1
    mspellturn %actor.name% ��������� set
    msend %actor% ���������� ��� ���������, ������ �� ������ �������� ���� ��
    msend %actor% ������ ������. �� �� ����� ����� �������� �� ���� ������� ����.
    halt
  else
    msend %actor% ������ ��������� �� ���.
    mechoaround %actor% ������ ��������� �� %actor.vname%.
    msend %actor% �� ��� ��� ����� �������� ������ ������, �� � ����������� ����.
  end
end
* �����
if (%actor.class% == 12) && (!%actor.spelltype(���������� ����������)%)
  if (%actor.level%>=%minlevel%)
    mecho 
    wait 1
    msend %actor% � ������� ���� ���� ������.
    wait 1
    mspellturn %actor.name% ����������.���������� set
    msend %actor% ���������� ��� ����������, ������ �� ������ ������ ������
    msend %actor% � ��������� ����������, ����� ��� �� ������ ������ ������� ����.
    halt
  else
    msend %actor% ������ ��������� �� ���.
    mechoaround %actor% ������ ��������� �� %actor.vname%.
    msend %actor% �� ��� ��� ����� �������� ������ ������, �� � ����������� ����.
  end
end
if (%world.curobjs(27208)%<4) && (%random.3%==1)
  mload obj 27208
  ���� ����� %actor.name%
elseif (%random.1000% <= 100)
  mload obj 1741
  give ������� .%actor.name%
else
  %self.gold(+500)%
  ���� 500 ��� %actor.name%
end
~
#27203
��������~
0 q 100
~
wait 1
mecho _ ����� ����������� ������ �� ��������� ������������ �� ���.
mecho _ ����� �� ������: ����, ����, ������ ������, ���
���
wait 1
���
wait 1
mecho _ ��� ����, ��� ������, ���� ������.
mecho _ ��� �� ������ ���������� ������ ���������� �� ��� � ����� ��������� �� ���.
~
$~
