#4000
������� �� �������� ~
0 j 100
~
wait 2
switch %object.name% 
case ���� ����
  say  �������������, �� ���� ���������, ������ �����. 
  %self.gold(+1)%
  ���� 1 ��� %actor.name%
  mpurge ���� 
break
case ���� �����
  say    �������������, ��������� ������� ������ �� ����, ������ �����. 
  %self.gold(+20)%
  ����   20 ��� %actor.name%
  mpurge ���� 
break
case ���� ����
  say    �������������, ��������� ������� ������ �� ����, ������ �����. 
  %self.gold(+30)%
  ����   30 ��� %actor.name%
  mpurge ���� 
break
case ���� �������
  say    �������������, ��������� ������������ �����, ������ �����. 
  %self.gold(+80)%
  ����   80 ��� %actor.name%
  mpurge ���� 
break
case ���� �����
  say    �������������, ��������� ������� �����, ������ �����. 
  %self.gold(+10)%
  ����   10 ��� %actor.name%
  mpurge ���� 
break
case ���� �������
  say    �������������, ��������� ������� ��������, ������ �����. 
  %self.gold(+40)%
  ����   40 ��� %actor.name%
  mpurge ���� 
break
case ���� �����
  say    �������������, ������� ���, ������ �����. 
  %self.gold(+25)%
  ����   25 ��� %actor.name%
  mpurge ���� 
break
case ���� �������
  say    �������������, ������� ���, ������ �����. 
  %self.gold(+25)%
  ����   25 ��� %actor.name%
  mpurge ���� 
break
case ���� �����
  say    �������������, ������� ���, ������ �����. 
  %self.gold(+22)%
  ����   22 ��� %actor.name%
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
#4001
������� �� ������� �������� ~
0 q 100
~
if %actor.level% >= 10 || (%actor.gold% >= 20) || (%actor.bank% >= 20) 
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
  msend %actor% - ��� �� �������� �����, ���� �� ���������. 
*  msend %actor% - ������ ����� ��� ���������, 
  msend   %actor% - ������, �������...
dg_cast '���������' %actor.name%
~
#4002
������������~
0 r 100
~
wait 1
eval factor %self.people%
while %factor%
  eval pc %factor.next_in_room%
  if (%factor.vnum% == -1) && (%factor.level%>19) && (%factor.level%<31)
    if %factor.sex%==1
      tell %factor.name% ����� �� ���, ������ ���� ����� ����� �����������.
    else
      tell %factor.name% ������� �� ���, ������ ���� ����� ���� �����������.
    end
    tell %factor.name% �� ���� � ���� ����� �� ������, ��� � �����, ��� ��� ������� �����.
    tell %factor.name% � �� ��� ������ �������� ����� �����.
    mechoaround %factor% ������ ��������� %factor.vname% �� �����.
    msend %factor.name% ������ ��������� ��� �� �����.
    mteleport %factor.name% 4014
    mteleport %factor.name% 4014
    mteleport %factor.name% 4014
    mteleport %factor.name% 4014
    mteleport %factor.name% 4014
    mteleport %factor.name% 4014
    mteleport %factor.name% 4014
    if %factor.sex%==1
      mat 4014 mechoaround %factor% ������� ������������� %factor.name% ������ � ������.
    else
      mat 4014 mechoaround %factor% ������� ������������� %factor.name% ������ � ������.
    end
  end
  if %pc.id% && %pc.id%!=%factor.id%
    makeuid factor %pc.id%
  else
    unset factor
  end
done
~
#4003
���������������~
0 k 100
~
set name2 %actor%
if %actor.leader%
   set name1 %actor.leader%
   if (%name1.vnum% == -1) && (%actor.vnum% != -1))
     set name2 %actor.leader%
   end
end
if !%name2.rentable%
msend %actor% %self.name% ��������H� ������%self.g% ���.
mdamage %actor% 300
������
   halt
end
� %name2.name%, � ��� �������� ������ �� ����� ���������!
msend %actor.name% ����� �������� �������� ������ � ������� ���.
mechoaround %actor% �������� �������� ������, ������� %actor.rname% � ����-�� ������.
wait 1
msend %name2.name% �� �������� ��������.
wait 1
msend %name2.name% �� ������ � �������� � �������� ��� �� ����� � �����.
if %name2.hitp%>0
msend %name2% ���-�� ������ ������ ���������, ������� ���� ����� ������������� �� �������� ������.
msend %name2% ���-�� ������ ������: ������ ����� ���� �� �����������.
msend %name2% ��, ���� ������ ����� ������: �������� ���� ������, ����� � ������.
mteleport %name2% 4058
*mat 4058 mechoaround %name2% ������� � ��� ��������� � %name2.name% ����%name2.a% ������ � ���.
*mat 4058 mechoaround %name2% ���-�� ������ ���������, ������� ���� ������������� %actor.rname% �� �������� ������.
*mat 4058 mechoaround %name2% ���-�� ������ ������: 
*mat 4058 mechoaround %name2% ������ ����� ���� �� �����������.
*wait 1
*if %name2.sex%==1
*mat 4058 mechoaround %name2% ��, ���� ������ ����� ������: �������� ���� ������, ����� � ������.
*else
*mat 4058 mechoaround %name2% ��, ���� ������ ����� ������: �������� ���� ����, ����� � ������.
*end
end
~
#4004
�����������~
2 d 0
"�������� ���� ������" "�������� ���� ����"~
wait 1
if %random.10%<2
wecho ������� � ��� ��������, ������ ���������� ����������� ���������� ����������.
wecho �����, ������ �������, ������!
wecho � ��������� ��� �����, ������ ��� ������� ������� �����!
if %actor.hitp%>0
wteleport %actor.name% 4000
wat 4000 wechoaround %actor% � ������� ������� �������� ������, ������ �����.
wat 4000 wechoaround %actor% ������ ������� � ����� ��������� ������� ����-��.
wat 4000 wechoaround %actor% ����� ������� ��������� �����.
if (%actor.sex% == 1)
wat 4000 wechoaround %actor% %actor.name% ������ �����, ������ � ��� ��� ���� ��������.
else
wat 4000 wechoaround %actor% %actor.name% ������� �����, ������ � ��� �� ���� ��������.
end
end
else
wechoaround %actor% ����� �� ������������� �� ����� %actor.rname%.
wsend %actor% ����� �� ������������� �� ���� �����.
wsend %actor% �������� �� ����� �������� ��� ����������, ��������� � ��������.
end
~
#4005
�������� ���� �� ����~
0 f 100
~
%actor.exp(+200000)%
~
#4006
������� ������� � ����������~
0 q 80
~
wait 1
if %actor.clan% != null
halt
end
if !%actor.agressor%
halt
end
if %actor.agressor% > 4000
if %actor.agressor% < 4099
set agr 1
end
end
if %actor.agressor% > 4300
if %actor.agressor% < 4399
set agr 1
end
end
if %actor.agressor% > 4800
if %actor.agressor% < 4899
set agr 1
end
end
if %agr% == 1
if %actor.sex% == 1
����� %actor.name%, - ��� ��, ������, ������ ���! ���� ���!
else
����� %actor.name%, ��������, ������ ��! ���� ��!
end
����� %actor.name% - ��� ��%actor.g% ���, ������, ������!
mkill %actor% 
end
~
#4007
�������� ������� � ��������~
0 t 100
~
wait 1
if %actor.clan% != null
halt
end
foreach char %self.pc%
if %char.agressor% > 4000
if %char.agressor% < 4099
set target %char%
break
end
end
if %char.agressor% > 4300
if %char.agressor% < 4399
set target %char%
break
end
end
if %char.agressor% > 4800
if %char.agressor% < 4899
set target %char%
break
end
end
done
if !%target%
halt
end
if %target.sex% == 1
����� %target.name%, - ��� ��, ������, ������ ���! ���� ���!
else
����� %target.name%, ��������, ������ ��! ���� ��!
end
����� %target.name% - ��� ��%target.g% ���, ������, ������!
����
mforce %target.name% ������
mkill %target% 
~
$~
