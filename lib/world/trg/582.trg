#58200
���� ���� �� ������� � ����� ��� ����� �� ����������~
1 g 100
~
* ���� ������ ����-���� ��� ������� ����� ��� ������� ����������-���������� � ����� ��
if ( %actor.vnum% != -1 )
  halt
end
wait 1
* �������� ���� ����� ���� -- ����� ������
if %actor.clan% == �� || %actor.clan% == ��2 || %actor.clan% == ��3
  halt
end
* �� �� � �� �������� -- �� �����
if ( %actor.clan% != �� )
  oecho �������� %self.iname% �����%self.q% � ����� �������.
  wait 1
  opurge %self.id%
end
~
#58201
�������� => ����~
1 c 3
��������~
wait 1
if (%actor.clan%!=��)
  oechoaround %actor% %actor.name% �����%actor.u% ������ ������ ��������. � ��� ��� � �������?
  osend %actor% �� ������ ������ ��������. ��������� �������.
  halt
end
if %world.curmobs(58112)% < 50
  oechoaround %actor% %actor.name% �����%actor.g% ������ �������� � ��� ����������� � ����.
  osend %actor% �� ������� �������� � �� ����������� � ����!
  oload mob 58112
  wait 1
  opurge %self%
else
  osend %actor% �� ������� �������� �� ������ �� ���������.
end
~
#58202
���� => ��������~
0 c 0
���������~
wait 1
if (%actor.clan%!=��)
  mechoaround %actor% %actor.name% ����������%actor.g% �������� ����, �� ��� ���������� ������� ��� �� �����.
  msend %actor% �� ����������� ��������� ����, �� �� ���������� ������ ��� �� �����!
  halt
end
mechoaround %actor% %actor.name% ��������� ��������%actor.g% ����. ���� � ������� ������� �����.
msend %actor% �� ��������� ��������� ����. ���� � ������� ������� ������.
mload obj 58258
wait 1
mpurge %self%
~
$~
