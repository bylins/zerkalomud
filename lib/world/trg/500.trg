#50000
������_������~
1 h 100
~
context 500quest
wait 1
set room %self.room%
attach 50083 %room.id%
oecho ����� ����� !
wait %pause%s
oecho ����� �����������!
detach 50083 %room.id%
attach 50084 %room.id%
oecho ���������� - ��� ��� ������� ��������������!
set schet 2
while (%schet% > 1)
set schet 0
set igrok %room.people%
while %igrok%
if %igrok.vnum% == -1
eval schet %schet%+1
end
set igrok %igrok.next_in_room%
done
wait 10s
done
oecho �� ���������� !
wait 5s
set schet 0
set igrok %room.people%
while %igrok%
if %igrok.vnum% == -1
eval schet %schet%+1
end
set igrok %igrok.next_in_room%
done
if %schet% == 0
oecho ����� ��� �������� ������!!!
oecho ������ ���� �����������!!!
detach 50084 %room.id%
oecho ������ ��� ����� �����.
opurge %self%
halt
else
oecho �������� �������������� ����� ������.
oload obj 50001
end
opurge %self%
~
#50001
������_�������_��������_�_����_500~
2 c 100
������~
context 500quest
if %pause%
wecho ���� ����� ��� �����!
halt
end
if !%arg%
wecho ������� ����� ����� ������ !
halt
end
if %arg% == ������
wdoor 50082 west purge
wdoor 50082 west room 50001
wdoor 50082 west flags a
wforce �������.�����.����� ����� ���� ����� ��� �������� ���� ���������, �������� � �����, � �������� ��������������.
wforce �������.�����.����� ����� ���������� ������� ��� ������� � ��� ��������� �����. ����� �� �� ����� ����.
eval pause 900+%random.1100%
worlds pause
exec 50008 %self.id%
elseif %arg% == �����
wdoor 50082 east purge
wdoor 50082 east room 50014
wdoor 50082 east flags 0
wforce �������.�����.����� ����� ���� ����� ��� �� &R��������&Y �����, �������� � �����������, � ��������
wforce �������.�����.����� ����� ��� ������ �� �������� ����� � ����� ���� ��������� ��� ��������� � �����!
wforce �������.�����.����� ����� � ��� ���� �������, ��� �� ������ � ����� �����! ����������!
eval pause 900+%random.120%
worlds pause
else
wecho ��� ������ ������ !
halt
end
wecho ��������� ������ ������� � ���� ����� ���������� ����������.
wecho �������: ����� 27030, �� - 49954, ����� - 34453, ������ - 66001, �� - 60050
wat 60050 %load% mob 50000
wat 66001 %load% mob 50000
wat 34453 %load% mob 50000
wat 49954 %load% mob 50000
wat 27030 %load% mob 50000
wait %pause%s
wat 27030 %purge% ���������.�����
wat 27030 wecho ��������� ��������� � ������������ � �������.
wat 49954 %purge% ���������.�����
wat 49954 wecho ��������� ��������� � ������������ � �������.
wat 34453 %purge% ���������.�����
wat 34453 wecho ��������� ��������� � ������������ � �������.
wat 66001 %purge% ���������.�����
wat 66001 wecho ��������� ��������� � ������������ � �������.
wat 60050 %purge% ���������.�����
wat 60050 wecho ��������� ��������� � ������������ � �������.
wdoor 50082 east flags abcd
~
#50002
����_�_����������~
0 q 100
~
wait 1
say ����������� ����, ������ ������� � ��� ������� � ������������ ?
~
#50003
����� ������ ����~
0 cd 0
���� �� �������~
if %actor.level% > 1
say �� ��� ��, ����� ������� ������.
*
end
calcuid room %self.realroom% room
attach 50005 %room.id%
exec 50005 %room.id%
detach 50005 %room.id%
~
#50004
������������_�����~
2 j 100
~
*if %drection% == west
wait 3s
wecho � ����� �������� ������� ����� ���������.
wdoor 50082 west flags b
*end
* � ����� � ��� ��� ���� �����-�� � ����������� �� ����������, ������ ������� �����
~
#50005
������_�_������_�����-����~
2 z 100
~
wportal 50080 1
~
#50006
�������_��_������~
0 cd 1
���������~
say �� ���� ���, ��������� ��� ���� ���������, �� ���� ��� �������� ��� ���.
msend %actor% �� ��, ������ �� ���-��, ���� � ����� �����.
dg_cast '����� ��������' %actor.name%
say ����, %actor.name%.
~
#50007
�����~
2 g 100
~
wait 1
wecho ���� ������� �����������!
eval temp %actor.move(-30)%
~
#50008
������_�����_������~
2 z 100
~
calcuid nablud 50001 mob
attach 50009 %nablud.id%
run 50009 %nablud.id%
detach 50009 %nablud.id%
~
#50009
�����_������_�������_������~
0 z 100
~
mload obj 50000
west
south
�� ������
north
east
����
~
#50010
���� ��������������~
1 g 100
~
if %actor.vnum% != -1
return 0
halt
else
wait 1
set room %self.room%
detach 50084 %room.id%
oecho ������ ��� ����� �����.
osend %actor% ��� ���� ������ �������������� �����������, �� ����� �����!
end
detach 50010 %self.id%
~
#50011
���� �������������� �����������~
0 j 100
~
if %object.vnum% != 50001
say ����� ��� ��� ?
�� ���
halt
else
wait 1
mpurge %object%
����� %actor.name% �� ��� ��, ����� ���� � ����, ����� ������ �����, ��� ��� �����
wait 1s
mecho ����������� ����������, � �� �������� ���� �������� ������.
wait 1s
switch %random.3%
case 1
mload obj 50012
say ��� ������� �������, ��� ����� ���.
� �����.���� %actor.iname%
break
case 2
mload obj 50013
say ��� ������, ��� ����� �����.
� ������.���� %actor.iname%
break
case 3
say ��� ������, ����� ���.
mload obj 50014
� �����.���� %actor.iname%
break
done
context 500quest
unset pause
end
~
#50012
����� ����~
2 f 100
~
context 500quest
if !%pause%
wdoor 50082 east flags abcd
wdoor 50082 west flags abcd
calcuid nablud 50001 mob
wpurge %nablud%
wat 50002 wload mob 50001
end
~
#50020
��������_�����_5~
1 j 100
~

~
#50021
�������� ����~
1 j 100
~
wait 1
oecho _�������� ���� ����� ���������� � ����� ������������ ���, ������� �����.
~
#50048
������ �������� ����� - ��������~
0 q 100
~
wait 1
if (%actor.name% == ����)
if (%world.curobjs(50063)% == 0)
say ��� ��������! ��� ���� ������, �� �������!
mload obj 50063
���� ������ .����
end
elseif (%actor.name% == ��������)
if (%world.curobjs(50064)% == 0)
say ���, ��������! ���������.. ������� ���� �������...
����
�����
mload obj 50064
���� ������ .��������
end
elseif (%actor.name% == ������)
if (%world.curobjs(50065)% == 0)
����
say �����������, ������� ��� ��������! ��� � ����� ����.
mload obj 50065
���� ����� .������
end
end
~
#50049
��������� ������ ����~
0 q 100
~
wait 1
if (%actor.name% == ������)
if (%world.curobjs(50054)% == 0)
say ������ ��� ���������, ����� ���� ���, �������� ��, �������, ���� ������� ������ ��� ��� ? �� �����.
mload obj 50054
� ����� .������
end
end
if (%actor.name% == ����)
if (%world.curobjs(50051)% == 0)
say ��, ���� ���� ����, ���-�� ��� � ���� �������� ��� ������� ��������...
mload obj 50051
� ����� .����
end
end
if %actor.name% == �������
if %world.curobjs(1091)% == 0
say ����������, �������! ���� � ����� �� ������.
say ������, ������, �����.
wait 1s
say ��, ��� ��!
mload obj 1091
� ���� �������
end
end
if %actor.name% == �����
if %world.curobjs(50050)% == 0
say ����������� ����, �����. ��� ������ � ���� � ������, ��� � ��������� ������.
wait 1s
mload obj 50050
� ������� c����
end
elseif %actor.name% == �����
if %world.curobjs(50052)% == 0
say ��, ��... ������ �����. ������, ���� � ����� �� ���������, ����� ���� �������.
��� �����
mload obj 50052
� �����.���� .�����
end
elseif %actor.name% == ������
if %world.curobjs(50053)% == 0
say ���, ������ �� ����� ���� ������ ? �� � ���� �������� ��� �� ������, �������, �� ��� � ���� ���� ����� ����.
mload obj 50053
� ���.��� .������
end
elseif %actor.name% == ������
if %world.curobjs(50055)% == 0
say �� �������� ������? ������� ��� ��������� - ��� ��� ��� ����� ������� ��������!
mload obj 50055
give ������� .������
end
elseif %actor.name% == ������
if %world.curobjs(50056)% == 0
say �� ������ ������? �� �������... � �� ��� ��� ����� ������ �� ���.
say � ��������� ����������� ����� ����� �������!
mload obj 50056
give ����� .������
end
elseif %actor.name% == ����
if %world.curobjs(50057)% == 0
say ���, ����, ���������� �������!
say ������� ���� ������� ��������� - � ���� �� ��� �������.
mload obj 50057
give ������ .����
end
elseif %actor.name% == �����
if %world.curobjs(50059)% == 0
say ������, �������, �����!
����
say ��� ���� ������� - �������. ���� ��� �����, �� ��� � �����������, �������!
mload obj 50059
give ������ .�����
end
elseif %actor.name% == ������
if %world.curobjs(50058)% == 0
say ����������, ����������, ������!
����
say ��� ���� ������� - �� �������.
mload obj 50058
give ����� .������
end
elseif %actor.name% == �������
if %world.curobjs(50060)% == 0
say ������-�������� ��� ��������!
����
say ��� � ����� ���� - �� ���������.
mload obj 50060
give ����� .�������
end
elseif %actor.name% == �����
if %world.curobjs(50061)% == 0
say ������ ����, �����!
����
say �����-�����, �� ��� �� ������... ��� ���, ����������.
mload obj 50061
give ������� .�����
end
elseif %actor.name% == ������
if %world.curobjs(50062)% == 0
say ����� � ����, ����� ������? 
: ����� ����������� � ��������� ���
say ����, ����� �� ������, ����... ��� ��, ������ ����.
mload obj 50062
give ������ .������
end
end
�� ���
~
#50050
�������_����_�����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% == �����
oechoaround %actor% ����� �� ������� ����� ������������, ���������, ������ ��������� �����������.
osend %actor% ����� ��������� � ��� ���� ������, � ������������. ����� ����� �������� ��� ������.
halt
else
osend %actor% ����� ������ ������, � ���������.
osend %actor% ������� �������� �������� � ��� � �����.
%purge% %self%
~
#50051
������� ���� ����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% == ����
osend %actor% ����� ������ ��� ������ �����, ��� �� ������� � ��� ������������.
oechoaround %actor% ��������� �� ����� ������� �������� ������ ������� ��� � �����.
halt
else
oechoaround %actor% �� ����� ��������� � �����������, %actor.iname% ��������%actor.g% ����� � ������.
osend %actor% �� ����� ��������� � �����������, �� ��������� ����� � ������.
%purge% %self%
end
~
#50052
������� ���� �����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% == �����
oechoaround %actor% ����������� ���������, ������������ �� ������� �����, ������ ���������� ���.
osend %actor% ����������� ��������� ������������ ���������� ���. �� ������������� �����������.
halt
else
osend %actor% ������� ����������� � ��� � �����, ������� �� ��������.
oechoaround %actor% ����� ������� ������ ������������ � ����� %actor.rname%, � ������ �����������.
%purge% %self%
~
#50053
������� ���� ������~
1 gjp 100
~
if (%actor.level% > 30 ) & (%actor.vnum% == -1 )
halt
end
if %actor.name% == ������
wait 1
oechoaround %actor% ���� ������� ������ ����������� � ������������ ���������.
osend %actor% ���� ���� ������������� � ������� ������� ������.
halt
else
oecho _���� �������  ����������� ��������, ����� ������ , ��� ������ ������ ���������� ������ ������ �������.
return 0
halt
end
~
#50054
������� ���� ������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% == ������
osend %actor% �������� �������� �� ������ ��������� �������� ������ �����.
oechoaround %actor% �������� �������� �� ������ ������� ��������� ��� � ������� ����� ���-������ ��������.
halt
else
oechoaround %actor% %actor.iname% ��������� �������� ���� �� ������, ��� ���������� ��� ���-�� ������.
osend %actor% �������� �������� �� ������ ������� ��� �� �����, �� ������������� �� ��������� � ������ ����������.
%purge% %self%
end
~
#50055
������� ���� - ������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% == ������
wait 1
oechoaround %actor% ��������� ����� ����� �� ������� %actor.vname% ������ ����������� �� �����.
osend %actor% ��������� ����� ����� �� ������� ������ ����������� �� �����.
halt
else
oechoaround %actor% %actor.iname% ���������%actor.g% � ��������%actor.g% � ������� �� �������� �������.
osend %actor% �� ������� ����� ���������� ���� � ������� ������� � �� ���� �� ����������� ������� �� ����������.
end
~
#50056
������� ���� ������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% != ������
return 0
oechoaround %actor% ������� �� ����� ����� ����� �������� �������� � ������ ������� %actor.vname%!
oechoaround %actor% %actor.iname% �������%actor.g% � ��������%actor.g% � �������.
osend %actor% ����� ����� �������� ������ � ����� ������ ������� ���!
halt
else
wait 1
*oecho _����� ������� ����������, ����������� ���������.
*oecho __����� ����������� �������� ������ ������� ���� � ���������...
end
~
#50057
������� ���� - ����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% != ����
return 0
oechoaround %actor% %actor.name% ��������� ����� �������� ��������.
oecho ___������� ����� ������� ��������: "����� �� �����!"
halt
else
wait 1
oecho _������� ��������� ���������� �� ���� ������.
end
~
#50058
������� ���� ������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% != ������
return 0
oechoaround %actor% ����� ����� ������ ��������, � ���� �� ������ %actor.name% �����!
oecho ___����� ����� ������ ��������, � ���� �� ������ ��� �����!
halt
else
wait 1
oecho _����� ������������ �������� �� ����� ������ ���� ��������.
end
~
#50059
������� ���� �����~
1 gjp 100
~
if (%actor.level% > 30 ) 88 (%actor.vnum% == -1 )
halt
end
if %actor.name% != �����
return 0
oechoaround %actor% %actor.name% ��������� ����� �������.
oecho ___������������ �� ������ �������� ���� ���� �� ��������� %actor.vname% �����!
halt
else
wait 1
oecho _���� �� ������� ������ ����� ���������� � ��������� ���.
oecho ___���������� �������� ������ ����� � ���� �������� �������� �����!
end
~
#50060
������� ���� - �������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% != �������
return 0
oechoaround %actor% %actor.name% �������%actor.u% ����� �����, �� ����� ��������%actor.u% � ���� �� ����%actor.g%.
oecho __����� �� �������� ����������� ����� ��� ��������� ������, ��� �� �� ������� �������� ��������.
halt
else
wait 1
oecho _������� ������� ��� ���� ���������� � ����� ��� ����� ����.
end
~
#50061
������� ���� - �����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% != �����
return 0
oechoaround %actor% %actor.name% ��������%actor.u% ��� ������, �� ����� ���������%actor.g% � ��������%actor.g%.
osend %actor%  _������� ����� ������������, � ��� ���� ����������� ����� ���� �� �������� ��� ��� �������!
halt
else
wait 1
oecho ����� �������� �� ������� ����� ��������� �������� �������.
end
~
#50062
������� ���� - ������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% != ������
return 0
oechoaround %actor% %actor.name% ��������%actor.u% ����� ����� ������...
oechoaround %actor% __...�� ������-�� �������� ���, �������� ������ � ��� �����.
osend %actor% __��� ������������ �� ������ ������ ���� �� ������� ��� ���� � �������.
*opurge %self%
halt
else
wait 1
oecho �� ��������� ��� ����������, ��� ���� ���� ��������� � ������ ������.
end
~
#50063
������� ���� - ����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.name% != ����
oechoaround %actor% %actor.name% ��������%actor.u% � �������� ���� � ������� ������.
oechoaround %actor% ���� ����� ������� � ���� %actor.name%!
osend %actor% &R __������ ���� ������� ��� � ����! 
osend %actor% &R __��� ������������� ������!&n
return 0
halt
else
wait 1
oecho ������ ������ �������� ������, ������ ���� � ����������.
end
~
#50064
������� ���� - ��������~
1 gjp 100
~
if (%actor.level% > 30 ) & (%actor.vnum% == -1 )
halt
end
if %actor.name% != ��������
oechoaround %actor% _������ ��������, ������ �������� �� ������� ���� %actor.rname% !
osend %actor%  __������ ��������, ������ �������� �� ����� ����!
return 0
halt
else
wait 1
oecho _������ �������� ���� ���������� ������� �������.
end
~
#50065
������� ���� - ������~
1 gj 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
halt
end
if %actor.clan% != ��
oechoaround %actor%  ����������� �� ����� ��������� ��������� ������� � ������ %actor.vname% �� �����.
osend %actor% _����������� �� ����� �������� ��������� ������� � ����������.
return 0
halt
else
wait 1
oecho _����������� �� ����� ��� ��� ��������� ��������� ��� ���������.
end
~
#50070
������ 1~
2 d 5
����� ���� �� �������~
wait 1
%echo% ���������� ������ �����, ������ ������� ������������ � �� ���� ���� ����� ���������� ������� ���������.
%load% obj 50070
%echo% ������ ������� � ������� ������ ������������.
%purge% quest501
detach 50070 %self.id%
~
#50071
������ 2~
2 d 5
��� ������� ������ ���� �� �����~
wait 1
%echo% ���������� ������ �����, ������ ������� ������������ � �� ���� ���� ����� ���������� ������� ���������.
%load% obj 50070
%echo% ������ ������� � ������� ������ ������������.
%purge% quest502
%purge% quest501
%purge% quest500
detach 50071 %self.id%
~
#50072
������ 3~
2 g 5
������ ������ ����������~
wait 1
%echo% ���������� ������ �����, ������ ������� ������������ � �� ���� ���� ����� ���������� ������� ���������.
%load% obj 50070
%echo% ������ ������� � ������� ������ ������������.
%purge% quest503
%purge% quest500
detach 50072 %self.id%
~
#50073
������ 4~
2 d 5
������� ����~
wait 1
%echo% ���������� ������ �����, ������ ������� ������������ � �� ���� ���� ����� ���������� ������� ���������.
%load% obj 50070
%echo% ������ ������� � ������� ������ ������������.
%purge% quest504
%purge% quest500
detach 50073 %self.id%
~
#50074
������ 5~
2 d 5
������ ������ ����, ��� ���� ����� ������~
wait 1
%echo% ���������� ������ �����, ������ ������� ������������ � �� ���� ���� ����� ���������� ������� ���������.
%load% obj 50070
%echo% ������ ������� � ������� ������ ������������.
%purge% quest505
detach 50074 %self.id%
~
#50075
������� �����~
0 f 100
~
mecho ������� ��������� :
mecho - ��� �� :((:(*:?% !!!
mecho - ���� �� ����... �� ����, �� �������� ���� �� ���������!
if (%world.curmobs(50025)% < 2)
mecho - ��� ������ ���� �� ����� � ���������� �� �� ���� ���� ����������.
mecho - ������ �� �� ������ ������ � ������ ��������� �����
mecho ������� ������ �� �����
mecho �� �������� �� ��� ������� ������ ������� �� ��������� ��� �������:
mecho - ������� ����
mecho - ����� ���� �� �������
halt
end
if (%world.curmobs(50025)% < 3)
mecho - ��� ������ ���� �� ����� � �������� ����������� � ������� �������� ����������
mecho - � ����� ����
mecho - ������ �� � ���������� �� �� ���� ���� ����������.
mecho ������� ������ �� �����
mecho �� �������� �� ��� ������� ������ ������� �� ��������� ��� �������:
mecho - ������� ����
mecho - ������ ������ ����, ��� ���� ����� ������
halt
end
if (%world.curmobs(50025)% < 4)
mecho - ��� ������ ���� �� ����� ����� ���� � ����� � ����� ������ �����, ������������ �������
mecho - � ������ � �������� ����������� � ������� �������� ����������
mecho - � ����� ����
mecho ������� ������ �� �����
mecho �� �������� �� ��� ������� ������ ������� �� ��������� ��� �������:
mecho - ������ ������ ����������!
mecho - ������� ����
halt
end
if (%world.curmobs(50025)% < 5)
mecho - ��� ������ ���� �� ����� �� ������ � ��� ��.
mecho - � ������ ����� ���� � ����� � ����� ������ �����, ������������ �������
mecho ������� ������ �� �����
mecho �� �������� �� ��� ������� ������ ������� �� ��������� ��� �������:
mecho - ��� ������� ������ ���� �� �����
mecho - ������ ������ ����������!
halt
end
if (%world.curmobs(50025)% < 6)
mecho - ��� ������ ���� �� ����� �� ������ ������ � ������ ��������� �����
mecho - � ������ �� �� ������ � ��� ��!
mecho ������� ������ �� �����
mecho �� �������� �� ��� ������� ������ ������� �� ��������� ��� �������:
mecho - ����� ���� �� �������
mecho - ��� ������� ������ ���� �� �����
halt
end
~
#50076
Little ghoul killed~
0 f 100
~
if ( %random.100% < 3 )
return 0
mload mob 50027
end
mecho ��-��-��-�����!!!!
~
#50077
�����~
1 gj 100
~
if %actor.name% != ������
return 0
*%actor.wait(6)%
*%actor.position(6)%
%send% %actor% ������� ������������, � �������� ��� ������� ��������, �� ������� �� �������� �������.
%echoaround% %actor% ������� ������������, � �������� %actor.dname% ������� ��������, �� ������� %actor.name% �������%actor.g% �������.
halt
end
%send% %actor% ������� ������� ��� ������ � �����������.
%echoaround% %actor% ������� ������� ������ � ����������� %actor.vname%.
~
#50080
���� ������ ��� �����~
0 m 1
~
say ������, ���� ��� ���� ������.
say ��� ������ �� ��������� :)
return 0
halt
wait 1
if %amount% == 100
���
wait 1
say ������ ���� �������� ? �� ��� �, ����� ��� ��������.
msend %actor% ������ ��������� �����, � �� ������ �������� � �����.
mechoaround %actor% ������ ��������� ����� � %actor.name% ������ �������%actor.g% � �����.
~
#50081
�������� ������~
0 e 100
~
if %actor.is_killer%
eval dushik 1
else
eval dushik 0
endif
dg_affect %actor.name% ����������� 10 100
~
#50082
���������� ������~
1 c 100
~

~
#50083
���� � ������� � ����~
2 g 100
~
if %actor.vnum% != -1
halt
end
set schet 0
set igrok %self.people%
while %igrok%
if %igrok.vnum% == -1
eval schet %schet%+1
end
* wecho %igrok.name%  ��� ����! %schet% - ����� ���� ! %actor.vnum% - ����
set igrok %igrok.next_in_room%
done
if %schet% > 1
return 0
wsend %actor% �� �� ������ ���� ������, ��� ������.
else
return 1
end
halt
~
#50084
��_�������_�_����������~
2 g 100
~
if %actor.vnum% != -1
halt
end
set schet 0
set igrok %self.people%
while %igrok%
if %igrok.vnum% == -1
eval schet %schet%+1
end
* wecho %igrok.name%  ��� ����! %schet% - ����� ���� ! %actor.vnum% - ����
set igrok %igrok.next_in_room%
done
if %schet% > 0
return 0
wsend %actor% �� �� ������ ���� ������, ��� ������.
else
return 1
end
~
#50085
������� ����� ������~
1 g 100
~
osend %actor% �� �� ������ ����� ���� ������.
return 0
~
#50090
���������� ������~
0 z 100
~
�������
purg %self%
~
#50091
���������� ������~
1 c 100
�������~
wait 1
osend %actor% �� ����������� ��������������� �������.
oechoaround %actor% %actor.iname% ����������%actor.a% ��������������� �������.
oload mob 50090
calcuid razhech 50090 mob
attach 50090 %razhech.id%
run 50090 %razhech.id%
~
$~
