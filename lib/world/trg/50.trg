#5000
�� ����~
0 r 100
~
tell %actor.name% �� ����������, ���� ������, ��� ��������� �� �����?
�������
~
#5001
�� ����1~
0 d 100
�����~
if %speech%==�����
tell %actor.name% �������!
������ %actor.name%
endif
~
#5002
������� �� ������� ~
0 j 100
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
#5003
������� �� �������� ~
0 j 100
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
#5004
��������������~
0 r 100
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
mteleport %factor.name% 5001
if %factor.sex%==1
mat 5001 mechoaround %factor% ������� ������������� %factor.name% ������ ������.
else
mat 5001 mechoaround %factor% ������� ������������� %factor.name% ������ ������.
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
#5005
������� �� �����~
0 q 100
~
if %actor.level% >= 10 || %actor.bank% >= 20
   halt
end
wait  1
msend %actor% ����� ������� ���:
if %actor.sex% == 1
   msend %actor% - �� �� ����������, �������� ��������, %actor.name%.
elseif
   %actor.sex% == 2
   msend %actor% - �� �� ����������, �������� ��������, %actor.name%.
end
  msend %actor% - ��� �� �������� ����� �� ���������. 
*  msend %actor% - ������ ����� ��� ���������, 
  msend   %actor% - ������, �������...
dg_cast '���������' %actor.name%
~
#5006
�������� ������� � ��������~
0 t 75
~
wait 1
if %actor.clan% != null
halt
end
foreach char %self.pc%
if %char.agressor% > 5000
if %char.agressor% < 5099
set target %char%
break
end
end
if %char.agressor% > 5100
if %char.agressor% < 5199
set target %char%
break
end
end
if %char.agressor% > 5600
if %char.agressor% < 5699
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
mforce %target.name% �����
mforce %target.name% stand
����
mforce %target.name% ������
mforce %target.name% ��������� r1guarg
group all
kill %target.name% 
~
#5007
������ ������� � ���������~
0 q 80
~
wait 1
if %actor.clan% != null
halt
end
if !%actor.agressor%
halt
end
if %actor.agressor% > 5000
if %actor.agressor% < 5099
set agr 1
end
end
if %actor.agressor% > 5100
if %actor.agressor% < 5199
set agr 1
end
end
if %actor.agressor% > 5600
if %actor.agressor% < 5699
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
mforce %actor.name% stand
mforce %actor.name% ��������� r2guard
group all
kill %actor.name% 
end
~
$~
