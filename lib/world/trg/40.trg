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
  msend %name2% ���-�� ������ ������: ������ �����, ���� �� �����������.
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
  wecho ������� � ��� ���������, ������ ���������� ����������� ���������� ����������.
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
0 d 1
����~
wait 1
say q - %actor.q%
say g - %actor.g%
say y - %actor.y%
say w - %actor.w%
say u - %actor.u%
say ��� ����� - %actor.class%
say ��� ���� - %actor.clan%
if (%actor.clan% == null)
  say ��� ��������� � null
end
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
#4008
������~
0 b 25
~
switch %random.28%
  case 1
    ���
  break
  case 2
    ���
  break
  case 3
    ����
    ����
    ����
  break
  case 4
    ������� �� ��-��-��-��-��-��, ��� ������ �� ����, � �� ������ ���� ����� ��� �������!
  break
  case 5
    ���
    ����
  break
  case 6
    �����
    ������� �-�������.. �? �������� �� ���?!
  break
  case 7
    �����
    ����
    �����
    ���
  break
  case 8
    n
    e
  break
  case 9
    s
    e
  break
  case 10
    u
    n
  break
  case 11
    e
    s
  break
  case 12
    ������� ������� ���, ������, ������� ����!!!
  break
  case 13
    ������� �� ������ ������ ��������� �������� - ���� ����� �������� � ������� ������... ��!
  break
  case 14
    emot ���� : '� ���� �� ������ ������ ���� ��������, � �� ��� ������ � ����� �������...'
  break
  case 15
    ������� ���� ��� ��� ���������, ��� ���������� �����. �� ����� � ���� ������ � ��������� ���� ����
  break
  case 16
    ������� ������� ��� � ��������, ��� � �����,- ������������������ ����������, �� ����� ������ �������, ��������, ����� ������ �������, ����������. 
  break
  case 17
    ������� � ���� �������, �� ������, � ���������������� ������ � ����� � ������� ���������, � ��������� �� ������...
  break
  case 18
    say ����������, ��� �����, ����������, �������...
  break
  case 19
    say ����������, ��� �����, ����������, �������, ����������, ����-������� ���... 
  break
  case 20
    say ��������� ��������! ���� ������ ����?!
  break
  case 21
    ������� ��, ����!! ��� ����� �� ���������?!
  break
  case 22
    emot ���� :  ����������, ��� �����, ����������, �������, ����������, ����-������� ���, ��� ���� ������ ����� ����� ����, ��� �� ������� �������?
  break
  case 23
    emot ���� : '������� ��� ����� - ����� ��� ������ - ����...'
  break
  case 24
    ������� � �� ��� ���� ������!
  break
  case 25
    say � ���� ����� ���� �������� � �������� �������!
  break
  case 26
    emot ���������� : '� �p������ ��p����p, ������p���� ��p��� ��p, �� ����� ������� ���p, ��, ����p!'
  break
  case 27
    say ����� ���������� ����, ��� ��� ����� �� ������, ����������� - �� �����, � ������.
  break
  case 28
    ������
    ����
    �����
  break
done
~
#4009
������~
1 c 1
�����~
wait 1
set target %arg.car%
eval skill %arg.cdr%
foreach char %self.pc%
  if %char.name% ==  %target%
    eval valueskill %char.skill(%skill%)%
    global valueskill
    halt
  end
done
~
#4010
����������� ������~
1 c 2
���������~
if !%arg.contains(�����)%
  %send% %actor% ���� �������?
  halt
end
osend %actor% �� ������������ ����� ��� � ��� ������� ����� �������� �������...
oechoaround %actor% %actor.name% ������%actor.g% ����� ������ - ������� ������ ����� ������ � ���.
attach 4008 %actor.id%
wait 1
opurge %self%
~
#4011
���� �������~
2 z 100
~
wportal 50040 2
~
#4012
��������� ������~
1 g 100
~
wait 1
oforce %actor% ��������� �����
osend %actor% �� ��������� ������ �����.
~
#4014
�����~
0 f 100
~
log ������ �������� ������� �� ����������� �����, ������� %actor.name%
~
#4015
�����~
0 q 100
~
wait 1
%actor.loadroom(63671)%
teleport %actor% 63671
log ����� - %actor.name%
~
#4016
������ �����~
0 j 100
~
wait 1
mecho ����� ������� %object.vname%.
mjunk all
wait 1
���
~
#4017
����� �����~
0 b 10
~
wait 1
switch %random.8%
  case 1
    emot ������� ������ ������� � ������������ � ���������� �������
  break
  case 2
    emot ������ � ��������, ������� �����-�� ������� ��������� � ������ ���� ����������� � ���
    � ����-���-���, ����-����-����, ����-���, ���
  break
  case 3
    emot ������� ��� ������ � ��������� ������� � ����� ������ ������������ �� �� ������ �������
  break
  case 4
    emot �� ������ ��������� ���� � ��������� ������� � ������ ���-�� ��� ���������
  break
  case 5
    ������
  break
  case 6
    mecho ������ ����� ��������� ������ ���-�� �����.
  break
  case 7
    set target %random.pc%
    emot ��������� �������� %target.vname%
    ���
  break
  case 8
    emot ������� ������-�� ��� ��������� ������ � ��������� �� ����������
  break
done
~
#4018
��������� ����~
0 b 15
~
����� ���������!!!
~
$~
