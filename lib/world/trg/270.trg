#27000
������ ������ � ��������~
0 j 100
~
wait 1
if %actor.level% > 24
if ( %object.vnum% == 27029 )
wait 1s
say �� ����� � ����� ������� �����, %actor.name%, ������������� �� ������.
exec 27030 %self.id%
wait 1
mjunk all
end
if ( %object.vnum% == 27030 )
wait 1s
say �� ����� � ����� ������� �����, %actor.name%, ������������� �� ������.
exec 27030 %self.id%
wait 1
mjunk all
end
halt
end
switch %object.vnum%
case 27029
wait 1s
say ��, ������� ���� ���� ��������?
say ��, ������ ����, �� ������ ������� ������ ���� ��.
say � ��� ����� ���� 1000 ��� ��������.
%send% %actor% �������� ��� ��� ������ �����.
%echoaround% %actor% �������� ��� %actor.dname% ������ �����.
%actor.gold(+1000)%
exec 27030 %self.id%
wait 1
%purge% ������
wait 1
break
case 27030
wait 1s
say �������-���� �����! 
say ��� ��, ��� � ������� �� ���������� �����.
%send% %actor% �������� ��� ��� ������ �����.
%echoaround% %actor% �������� ��� %actor.dname% ������ �����.
%actor.gold(+800)%
exec 27030 %self.id%
wait 1
%purge% �����
wait 1
break
default
say ��� ��� ����� ���?
drop ���
done
~
#27001
���� ���������� ������ � ����� �������.~
2 f 100
~
calcuid glashatay 27017 mob
rdelete questor %glashatay.id% 
if (%random.100% < 30)
halt
end
*��������� ����� 1 (�����)
if !%exist.mob(27018)%
if (%random.100% < 70)
exec 27004 %self.id%
exec 27005 %glashatay.id%
end
end
*��������� ����� 2 (�����)
if !%exist.mob(27019)%
if (%random.100% < 60)
exec 27010 %self.id%
exec 27005 %glashatay.id%
end
end
*��������� ����� 3 (��������� �����)
halt
if !%exist.mob(27020)%
if (%random.100% < 80)
set quest3 1
global quest3
remote quest3 %glashatay.id%
end
end
~
#27002
������ �����~
0 f 100
~
%load% obj 27029
~
#27003
���� ����� �� ������~
0 f 100
~
mload obj 27033
~
#27004
�������� �����~
2 z 100
~
wload mob 27018
calcuid target 27018 mob
switch %random.4%
case 4
wteleport %target% 33979
break
case 3
wteleport %target% 33066
break
case 2
wteleport %target% 4000
break
case 1
wteleport %target% 5012
done
~
#27005
�������� ���� � ������ ������.~
0 z 100
~
������� �������� ����, ������������! ������� ����� ������� ����� ����!
wait 3
������� ������ �� � ������ �������� �������! 
wait 3
������� ��� ���� � ����� ������, �� ������ ����� � ������ - ���������� ������ �� ���, �� ���� ����������! 
������� ��� �� � ���� � �������� ���������.
~
#27007
������ ������ � ��������~
0 d 0
�������� ����� ������ �����~
wait 1
if ( %actor.vnum% != -1 )
halt
end
if ( %actor.id% == %questor.id% )
say ���� ��� ������� ����, � ��� ������ ��������. ��� ��� �����?!
halt
end
if %actor.level% < 12
say ���� ���� �� ���� ����������� ������� - ������ ������ ������� �����.
halt
end
if !%exist.mob(27018)%
if !%exist.mob(27019)%
set test 1
end
end
if ( %test% ==  1 )
say ��� � ���� ������ ������ � ����, %actor.name%!
halt
end
eval arg %speech.cdr%
if !%arg%
if %exist.mob(27018)%
say ������ �� ���� ����������� �������-������.
set quest 1
end
wait 2
if %exist.mob(27019)%
say � ����� ��������� ������� ����� ��������.
set quest 1
end
wait 2
if %quest3% == 1
say ��������� - ���� ������� ������ � ������ ������, �������� ������� ������� ���������.
say ���������� �������� �� - ������ �� ��������� ���������...
set quest 1
end
wait 1
if ( %quest% == 1 )
say ��� ����� - � ��� ������ �������?
end
halt
end
if %arg.contains(�������)%
if %actor.haveobj(27028)%
say ���� ���� ��� ��� ����� - ���� ��� �������?!
halt
end
set setquest 1
set questor %actor%
global questor
mload obj 27028
give ���� %actor.name%
end
if %arg.contains(����)%
if %actor.haveobj(27031)%
say ���� ���� ��� ��� ����� - ���� ��� �������?!
halt
end
set setquest 1
mload obj 27031
give ���� %actor.name%
set questor %actor%
global questor
end
if %arg.contains(���)%
if %quest3% == 1
wait 1
set setquest 1
emot ������� ��� � ������ �� ���
say �� ����� - ���� ��������� - ������ ���� �� ������!
emot ���-�� ������ ������������ ���� ������ � ��� ����� ������� � ���.
mecho ������ ������� ������� � �� ������ ����������� ���� ��������� ������� ������� � �������� ������.
mload mob 27020
wait 2
mforce questmob2703 follow %actor.name%
set quest3 0
global quest3
set questor %actor%
global questor
halt
end
end
if %setquest% == 1
say ��� ��, ��� ���� ����, ��������������� �������� �����������!
say ������� � ��� ��� ����� �� �����.
end
~
#27008
�������� ������ �����~
2 z 100
~
if !%exist.mob(27019)%
set comlete 1
else
set complete 0
end
global complete
detach 27008 %self.id%
~
#27009
����� ����~
0 f 100
~
%load% obj 27030
~
#27010
�������� �����~
2 z 100
~
wload mob 27019
calcuid target 27019 mob
switch %random.3%
case 3
wteleport %target% 11443
break
case 2
wteleport %target% 4543
break
case 1
wteleport %target% 38017
done
~
#27011
���� ����� �������~
0 j 100
~
wait 1
If (%object.vnum%==721)
 If (%object.val1%>5)
   Say �������� �������!
   If  !(%self.haveobj(60064)%)
      Mload obj 60064
   Else 
      Say ��... ������� � ���� ��� ����. ������ �� ����� ����.
   End
 Else
   Say ��... ����� ������� �������  ��� ��� ��������.
 end 
End  
If (%object.vnum%==705)
 If (%object.val1%>5)
   Say �������� ������ ����� !
   If  !(%self.haveobj(60061)%)
      Mload obj 60061
   Else 
      Say ��... ����� � ���� ��� ����. ������ �� ����� ����.
   End
 Else
   Say ��... ����� ����� �� ���������� ������ ��� ��������.
 end 
End
If (%object.vnum%==722)
 If (%object.val1%>5)
   Say ������������� �����! � ����� ��� ��� ����������!
   If  !(%self.haveobj(60065)%)
      Mload obj 60061
   Else 
      Say � ���� ��� ���� �����! ��� ���� ���� �� �����!
   End
 Else
   Say ���! ���� ����� ������������ ������! ����� �� ��������.
 end 
End
If (%object.vnum%==700)
 If (%object.val1%>5)
   Say ��-�! ������������� �����!
   If  !(%self.haveobj(60060)%)
      Mload obj 60060
   Else 
      Say � ���� ��� ���� ����! ��� ������ �� �����!
   End
 Else
   Say ��-�-��! ���� ������ ��������� � ��������. ����� �� ��������!
 end 
End    
mpurge %object%
if (%self.haveobj(60060)% && %self.haveobj(60061)%)
  say �������! ������ � ������� ������ � ������� ��������!
  say � ��� ������� � �������� ��� � ����
  mecho ������� ���-�� ��������� ��� ������������� � �������� �� � �����.
  ���� ���
  mteleport all 49934
end
if (%self.haveobj(60064)% && %self.haveobj(60060)%)
  say �������! ������ � ������� ������ � ������� ��������!
  say � ��� ������� � �������� ��� � �����
  mecho ������� ���-�� ��������� ��� ������������� � �������� �� � �����.
  ���� ���
  mteleport all 34405
end   
if (%self.haveobj(60065)% && %self.haveobj(60060)%)
  say �������! ������ � ������� ������ � ������� ��������!
  say � ��� ������� � �������� ��� � �����������
  mecho ������� ���-�� ��������� ��� ������������� � �������� �� � �����.
  ���� ���
  mteleport all 25005
end
~
#27012
����������� �������~
0 r 100
~
wait 1
msend %actor% ������� ������� ����:
msend %actor% - �� ������� ���� ������� �� ������� ���, %actor.name%!
msend %actor% - ���� ��� �� ������ ��� �� ���� ���%actor.y%! ���� ���� ������!
msend %actor% - ���� �� ���� ��� ������� ��� ��� ������ �����
msend %actor% - � ������ ���� ����� ��������� �� ������� �������� ������.
~
#27013
���� ������ �������~
0 m 1
~
 if (%amount% < 200)
say �� ����� ����� ���%actor.g% ����� ���������. � � �� ������!
halt
end
wait 1
msend %actor% - ������� ������� ����:
msend %actor% - ���� � ���� ������ - ������� ��� ������ � ��������.
msend %actor% - ���� �������� ������� � ����� ��, ����� ��������� ���� � �����.
msend %actor% - � �������� � ����������� ������� - ��� ��� ������ � �����.
~
#27014
���� ����� 3 (�������� ������ � ��������)~
2 z 100
~
wload mob 68022
calcuid mercht 68022 mob
wteleport %mercht% 68078
~
#27015
��� �� ��������~
0 l 100
~
if (%random.100% < 40 )
halt
end
wait 1
if ( %random.100% < 10 )
����� ������!!!!
end
if ( %actor.vnum% != -1 )
eval target %actor.leader%
if %target%
if %target.rentable%
mecho ���������� ��������� �������� %target.vname!
mteleport %target% 27071
end
end
else
if %actor.rentable%
mecho ���������� ��������� �������� %actor.vname%!
mteleport %actor% 27071
eval buffer %actor.loadroom(27071)%
end
end
eval gopa %actor.group%
foreach char %gopa%
if %char.rentable%
mecho ���������� ��������� �������� %char.vname%!
mteleport %char% 27071
eval buffer %char.loadroom(27071)%
end
done
~
#27016
���-�� ����� �� �������~
0 r 100
~
if ( %direction% == down )
wait 15s
mload obj 27033
���� �� ����
����� ����� ����
wait 1
mjunk all
end
~
#27017
��������� ����� ��� ������ ����������~
0 ab 100
~
set char %self.leader%
if !%char%
mecho ������� ������� ��������� � ����� �� ����� �����.
wait 1
%purge% %self%
end
~
#27018
��� �������� �����~
0 n 100
~
set cmpquest 0
global cmpquest
~
#27019
���� �� ����������~
0 q 51
~
if %actor.id% == %agressor.id%
halt
end
wait 1
follow .%actor.name%
���
set victim %actor%
global victim
~
#27020
���������� �������~
0 b 8
*~
switch %random.15%
case 1
say ��� �������, �� ��� �������!
������ %victim.name%
break
case 2
����� %victim.name%
���� %victim.name%
break
case 3
say ������� ���� ��������, ���� �� �� �������... ����� ���, ����� ������ �� ����...
����
����� %victim.name%
break
case 4
emot ����� ����� � �� ������ ��������
break
case 5
���
� ������� ����...  ������������� �����!
break
case 6
tell %victim.name% ��� �������, �� ��� �������!
break
case 7
if %victim.sex% == 2
halt
end
tell %victim.name% ���� �����%victim.g% ��������� �������� ����� ����?!
break
case 8
tell %victim.name% �������������� �� ��������������!!!!!
break
case 9
�������
break
case 10
say �� ���� �������� �����!
�����
����
break
case 11
���
break
case 12
tell %victim.name% �� ��� �������, � � ���� ���� - �� � �� ������ ���������� ������ ������!
break
case 13
tell %victim.name% �� ����� ���� ������� ����� - ������ ��������� �� �������... � � ��� ����������� ��-�������!
break
case 14
��� %victim.name%
break
case 15
tell %victim.name% ��� ���� ������� ��� ������? ��� ����� ������ �����, � ���� - ��� � ������ �� �������!
break
done
~
#27021
���������� ���� �����~
0 m 1
~
switch %random.3%
case 3
���
�������
break
case 2
������ %actor.name%
���� %actor.name%
break
case 1
���� %actor.name%
follow %actor.name%
set victim %actor%
global victim
break
done
~
#27022
���������� �������~
0 k 90
~
if %actor.vnum% != -1
set agressor %actor.leader%
global agressor
else 
set agressor %actor%
global agressor
end
follow �
wait 4
flee
flee
if %random.100% > 15
halt
end
if %actor.vnum% != -1
set victim %actor.leader%
����� ������� ������������, ��������! ��������� %victim.name% ���� ������ ������ �����!
halt
end
����� ������� ������������, ��������! ��������� %actor.name% ���� ������ ������ �����!
~
#27023
���� �� �������~
0 k 30
~
wait 1
if %actor.vnum% != -1
eval target %actor.leader%
if %target%
if %target.rentable%
mechoaround %target% ����� ��������� ����� ����� ���������� � �������� %target.vname%!
msend %target%  ����� ���������� ��������� �������� ��� � ������� � �����.
mteleport %target% 27071
eval buffer %target.loadroom(27071)%
end
end
else
if %actor.rentable%
mechoaround %actor% ����� ��������� ����� ����� ���������� � �������� %actor.vname%!
msend %actor%  ����� ���������� ��������� �������� ��� � ������� � �����.
mteleport %actor% 27071
eval buffer %actor.loadroom(27071)%
end
end
calcuid turm 27023 mob
exec 27035 %turm.id%
~
#27024
���� �� ���������~
0 z 100
~
wait 1s
say �� ������� �����������, ��� ���� ���� ��������� ����� ����� ����� ����!
mload obj 27033
������ �����
��� �����
wait 300s
���� �����
����� �����
mjunk all
~
#27025
����� �� �������~
2 f 100
~
if %random.500% != 323
halt
end
calcuid turm 27023 mob
wforce ��������.������� ����� ������� ����� ������� ���� �������! 
wforce ��������.������� ����� ������� �� ��������, ��� ���� ���� ��������� ������, ����� ������ ����, �� ������ ����� �� ����������!
wait 140s
exec 27024 %turm.id%
~
#27026
��� ���������� ������ � �������~
0 z 100
~
wait 1s
say ��, �� ��� � ��� � ������ �����!
say ����� ���� ������� ���� ����? 
~
#27027
��������� ����������� �� ������~
2 z 100
~
foreach char %self.pc%
if %char.name% == %target.name%
wecho �������� ������ ��������.
wait 1
wecho �������� ����� �����.
wecho �������� ������ �����.
wsend %char% �������� ����� ��� ������.
wteleport %char% 27072
eval buffer %char.loadroom(27018)%
wechoaround %char% ������ ����� ������, � ���-�� ������� � ������.
wechoaround %char% ������� ��������� �������� ����� %char.vname% ������.
wecho �������� ����� %char.vname% ������.
wecho �������� ������ �����.
wecho �������� ����� �����.
calcuid desat 27021 mob
detach 27028 %desat.id%
detach 27029 %desat.id%
wait 1s
attach 27028 %desat.id%
halt
end
done
calcuid desat 27021 mob
exec 27026 %desat.id%
~
#27028
��������� ���� ����~
0 j 100
~
wait 1
if %object.vnum% != 27035
������ %actor.name%
mjunk all
halt
end
wait 1
mjunk all
wait 1s
emot ��������������� ������� �������� �������
say ��� �����. ��� �� - ������, ���� ������� �� ������ ���������?
attach 27029 %self.id%
wait 120s
say ��� �� - ������ ��� � ��� ���, ��� ���� �����.
say ������ � ������ ����� �����.
detach 27029 %self.id%
~
#27029
��������� ������� ���~
0 d 1
*~
set target %speech%
calcuid ostrog 27071 room
remote target %ostrog.id%
wait 1
exec 27027 %ostrog.id%
~
#27030
�������� ������ �������~
0 z 100
~
wait 2s
if %random.10% < 5
say ��� � ��� ���� ������� �� ������� ��� ����...
mload obj 27035
emot �������� ��� ������� �������, �� �� ����� ������������ � ���� �� ���� � ����� �� �����.
���� �������
end
~
#27031
����������~
0 m 100
~
wait 1
if %amount% == 100
if %world.curobjs(33901)% < 500
mload obj 33901
give ���� %actor.name%
end
end
~
#27032
�������� ������� � ������� � �������~
0 t 100
~
wait 1
if %actor.clan% != null
halt
end
foreach char %self.pc%
if %char.agressor% > 27000
if %char.agressor% < 27099
set target %char%
break
end
end
done
if !%target%
halt
end
����
mforce %target.name% �����
mforce %target.name% ������
say %target.name%, �� �������� ����� �� ������ ����� �������, �� ������ ������%target.g% � ������!
mforce %target.name% ���� korsguard
mforce %target.name% �������
mkill %target%
if %target.sex% == 1
����� %target.name%, ��������, ������ ���! ���� ���!
else
����� %target.name%, ��������, ������ ��! ���� ��!
end
eval rnd (%random.100%+%random.100%+%random.100%)/3
if %rnd% < 11
mechoaround %target% ����� ��������� ����� ����� ���������� � �������� %target.vname%!
msend %target%  ����� ���������� ��������� �������� ��� � ������� � �����.
mteleport %target% 27071
eval buffer %target.loadroom(27071)%
end
wait 1
foreach char %self.pc%
if %char.agressor% > 27000
if %char.agressor% < 27099
set target %char%
break
end
end
done
if !%target%
halt
end
����
mforce %target.name% �����
mforce %target.name% ������
say %target.name%, �� �������� ����� �� ������ ����� �������, �� ������ ������%target.g% � ������!
mforce %target.name% ���� korsguard
mforce %target.name% �������
mkill %target%
if %target.sex% == 1
����� %target.name%, ��������, ������ ���! ���� ���!
else
����� %target.name%, ��������, ������ ��! ���� ��!
end
~
#27033
������ � ��������� �������~
0 q 75
~
wait 1
if %actor.clan% != null
halt
end
if !%actor.agressor%
halt
end
if %actor.agressor% > 27000
if %actor.agressor% < 27099
say %actor.name%, �� �������� ����� �� ������ ����� �������, �� ������ ������%target.g% � ������!
����� %actor.name% - ��� ��%actor.g% ���, �����-������, ������!
mkill %actor.name% 
if %actor.sex% == 1
����� %actor.name%, ��������, ������ ���! ���� ���!
else
����� %actor.name%, ��������, ������ ��! ���� ��!
end
end
end
~
#27034
����������� � ��������~
0 q 66
~
wait 1s
emot ������ �� ��� ������
say ��� ����%actor.y% - ������ ��� ���� �� �������, � �������� ������ � �������� �� ����.
emot ����� ��������� ��� ����� �������
~
#27035
�������� ��������� ��������� )~
0 z 100
~
wait 1
say �� ���, ����� ��� ���� �����!
say �����, �� ��������� - � ������� ������ ��������, � �� �� ������ �������, ����� ������� �������.
say ���� ����, ���� �� ���� ����� ��������� - ����� ���� ������ �� ����� ����������.
say �� ����� - ���� ��������, �� ���������� ��������� �� ������� ����� �������� - �������� �� �����������!
~
#27036
����������� ��������.~
0 q 60
~
wait 1
��� .%actor.name%
say ������ ����!
say ������, %actor.name%, ������ ������� �����, ������� � ������ ���.
say �������, ������ ���������� ����� �� �������.
~
$~
