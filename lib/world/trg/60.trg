* BRusMUD trigger file v1.0
#6000
�� ����~
0 r0 100
~
tell %actor.name% �� ����������, ���� ������, ��� ��������� �� ����������?
�������

~
#6001
�� ����1~
0 d0 100
�����~
if %speech%==�����
tell %actor.name% �������!
������ %actor.name%
endif

~
#6002
������� �� ������� ~
0 j0 100
����~
wait 1
switch %object.name%
case ���� ��������
  say  ��� ����� ����� ������! �������, ��������� ������, ����� ������.
  %self.gold(+1)%
  ���� 1 ��� %actor.name%
  mpurge ����
 break
case ���� �����
  say  ��� ����� ����� ������! �������, ��������� ������, ����� ������.
  %self.gold(+1)%
  ���� 1 ��� %actor.name%
  mpurge ����
 break
case ���� ����
  say  ��� ����� ����� ������! �������, ��������� ������.
  mload obj 101
  ���� ������ %actor.name%
  mpurge ����
 break
case ���� �����
  say  ��� ����� ����� ������! �������, ��������� ������.
  mload obj 101
  ���� ������ %actor.name%
  mpurge ����
 break
case ���� ����� ����
  say  �, ������, ��������� ������! ������ ������.
  %self.gold(+5)%
  ���� 5 ��� %actor.name%
  mpurge ����
 break

default
  say  ����� ��� ���?
  eval getobject %object.name%
  if  %getobject.car% == ����
     mpurge ����
  else
     ����� %getobject.car%.%getobject.cdr%
  end
  break
done



~
#6003
������� �� �������� ~
0 j0 100
����~
wait 1
switch %object.name%
case ���� �����
  say  �� ����� ������� ������ ���������, ������ ����� ��������!
  ����
  %self.gold(+10)%
  ���� 10 ��� %actor.name%
  mpurge ����
 break
case ���� �����
  say  �� ����� �������� ������ ���������, ������ ����� ��������!
  %self.gold(+75)%
  ���� 75 ��� %actor.name%
  mpurge ����
 break
case ���� ����
  say  �� ���� �������� ������ ���������, ������ ����� ��������!
  %self.gold(+100)%
  ���� 100 ��� %actor.name%
  mpurge ����
 break
case ���� �����
  say  � �� ���� �����! ��� ������, �� ����� ����!
  %self.gold(+20)%
  ���� 20 ��� %actor.name%
  mpurge ����
 break
default
  say  ����� ��� ���?
  eval getobject %object.name%
  if  %getobject.car% == ����
     mpurge ����
  else
     ����� %getobject.car%.%getobject.cdr%
  end
  break
done



~
#6004
��������������~
0 r0 100
~
wait 1
set factor %self.people%
while %factor%
       set pc %factor.next_in_room%

if (%factor.vnum% == -1) && (%factor.level%>19) && (%factor.level%<31)
if %factor.sex%==1
tell %factor.name% ����� �� ���, ������ ���� ����� ����� �����������.
else
tell %factor.name% ������� �� ���, ������ ���� ����� ���� �����������.
end
tell %factor.name% �� ���� � ���� ����� �� ������, ��� � �����, ��� ��� ������� �����.
tell %factor.name% � �� ��� ������ �������� ����� �����.
mechoaround %factor% ������� ���������� %factor.rname% � �������.
msend %factor.name% ������� ���������� ��� � �������.
mteleport %factor.name% 6000
if %factor.sex%==1
mat 6000 mechoaround %factor% ������� ������������� %factor.name% ������ ������.
else
mat 6000 mechoaround %factor% ������� ������������� %factor.name% ������ ������.
end
end
end

       if %pc%
         makeuid factor %pc.id%
       else
         set factor 0
       end


done




~
#6005
���������������~
0 k0 100
~
� %actor.name%, � ��� �������� ������ �� ����� ���������!
msend %actor% ����� �������� �������� ������ � ������� ���.
mechoaround %actor% �������� �������� ������, ������� %actor.rname% � ����-�� ������.
wait 1
msend %actor% �� �������� ��������.
wait 1
msend %actor.name% �� ������ � �������� � �������� ��� �� ����� � �����.
if %actor.hitp%>0
msend %actor.name% ���-�� ������ ������ ��������, ������� ���� ����� ������������� �� �������� ������.
msend %actor.name% ���-�� ������ ������: ������ ����� ���� �� �����������.
msend %actor.name% ��, ���� ������ ����� ������: �������� ���� ������, ����� � ������.
mteleport %actor% 6050
mat 6050 mechoaround %actor% ������� � ��� ��������� � %actor.name% ���� ������ � ���.
mat 6050 mechoaround %actor% ���-�� ������ ��������, ������� ���� ������������� %actor.rname% �� �������� ������.
mat 6050 mechoaround %actor% ���-�� ������ ������: 
mat 6050 mechoaround %actor% ������ ����� ���� �� �����������.
wait 1
if %actor.sex%==1
mat 6050 mechoaround %actor% ��, ������ �����������!
mat 6050 mechoaround %actor% ���� ������ ����� ������: �������� ���� ������, ����� � ������.
mat 6050 mechoaround %actor% ��� ������������, ���� ��������: � �������� ���� ������
mat 6050 mechoaround %actor% � �������� ������ ����� �� � ������� ���.
else
mat 6050 mechoaround %actor% ��, ������ �����������!
mat 6050 mechoaround %actor% ���� ������ ����� ������: �������� ���� ����, ����� � ������.
mat 6050 mechoaround %actor% ��� ������������, ���� ��������: � �������� ���� ����
mat 6050 mechoaround %actor% � �������� ������ ����� �� � ������� ���.
endif
end



~
#6006
�����������~
2 d0 0
"�������� ���� ������" "�������� ���� ����"~
wait 1
if %random.10%<2
wecho ������� � ��� ���������, ������ ���������� ����������� ���������� ����������.
wecho �����, ������ �������, ������!
wecho � ��������� ��� �����, ������ ��� ������� ������� �����!
if %actor.hitp%>0
wteleport %actor.name% 6020
wat 6020 wechoaround %actor% � ������� ������� �������� ������, ������ �����.
wat 6020 wechoaround %actor% ������ ������� � ����� ��������� ������� ����-��.
wat 6020 wechoaround %actor% ����� ������� ��������� �����.
wat 6020 wechoaround %actor% %actor.name% ������ �����, ������ � ��� ��� ���� ��������.
end
else
wechoaround %actor% ����� �� ������������� �� ����� %actor.rname%.
wsend %actor% ����� �� ������������� �� ���� �����.
wsend %actor% �������� �� ����� �������� ��� ����������, ��������� � ��������.
end




~
#6007
����������������~
0 r0 100
~
if %actor.level% >= 10 || %actor.bank% >= 20
   halt
end
wait  1
msend %actor% ������� ������� ���:
if %actor.sex% == 1
   msend %actor% - �� �� ����������, �������� ��������, %actor.name%.
elseif
   %actor.sex% == 2
   msend %actor% - �� �� ����������, �������� ��������, %actor.name%.
end
  msend %actor% - ��� �� �������� ����� �� ���������, 
  msend %actor% - ������ ����� ��� ���������, 
  msend   %actor% - ������, �������...
dg_cast '���������' %actor.name%

~
$
$
