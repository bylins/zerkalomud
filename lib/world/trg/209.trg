#20900
�������� ���� �����~
1 gj 100
~
wait 1
if %actor.clan% == ���
 halt
end
%send% %actor% %self.name% ��������%self.u% � ��� � �����.
%purge% %self%
~
#20901
��������� ������ ���~
0 ab 10
~
%echo% ����������� ����� �������� ����-�� �� ����.
~
#20902
����� ��������~
0 q 10
~
if (%actor.clan%==���)
��� %actor.name%
else
��� %actor.name%
end
~
#20903
[�������] [���������] ������~
2 c 0
������� ���������~
if %cmd% == �������
 if (%world.curmobs(20950)% > 0)
  wsend %actor% ����� ��� �����, �� ��� �����.
  halt
 end
 if %actor.clanrank% != 9
  wsend %actor% ����� �� ������� ���.
  halt
 end
 wsend %actor% ����� ������� ���.
 wecho &K����� ����� ����� ��������� ������ ���.&n
 wait 1s
 wecho �� ����� ������ �������� ����� ��������� ��� � ������.
 wait 1s
 wecho  �� ��������� ��������� ������������ �� ��������,
 wecho  � ������� ��������, � ������� �� ���� � ���������� � ���� �������.
 wload mob 20950
elseif %cmd% == ���������
if %actor.clanrank% != 9
wsend %actor% ����� �� ������� ���.
halt
end
 if (%world.curmobs(20950)% > 0)
  wecho &K����� ����� ����� ������ �� ���.&c
  calcuid volhv 20950 mob
  wpurge %volhv%
  wait 1s
  wecho �������� ����� ��������, � ����� ����� ������ � ���.
 else
  wsend %actor% �� ��� ��! ���� �� ������ ��������� ?
 end
end
~
#20904
�������� ��������~
0 ab 25
~
*if (%actor.clan%==���)
%echo% ������� �������� �� �������� ������ � �������� ��� � ��� �� ������.
*end
~
#20905
����� ���������~
0 c 100
�����~
set enemy %arg%
if !(%enemy.clan%==%actor.clan%)||(%enemy.level%>30)
 %echo% ������� ����� ��������� ��� � ����������.
 halt
else
 set enroom %enemy.realroom% 
* %echo% ������� �� ��������� ���������.
 ��� 
 wait 2
 � ��� �������� %enemy.name% ��������� � %enroom.name%
end
~
#20906
��� �� ����� ����~
2 b 20
~
%echo% ����� ��������� ��� �� ����� ���� ���� ���������.
~
#20907
������ �� ������� �����~
0 c 100
*~
if (%cmd.mudcommand% == ��)||(%cmd.mudcommand% == south)
 if %actor.clan% != ���
  %echoaround% %actor% %actor.name% �������%actor.u% ������ � ���, �� ����� �� ������ ���.
  %send% %actor% �� ���������� ������ � ���, �� ����� �� ������ ���.
  return 1
  halt
 end
end
*%send% %actor% ����� ����������� �������� ���, � ������ � �������, ��������� ���� � ���.
return 0 
~
#20908
���� �� �����~
2 e 100
~
wait 2
wsend %actor.name% �� �� ���������! 
wsend %actor.name% ��� ��� ����� ����������� ���� � ��� ���������.
~
#20909
����� � �����~
2 c 100
���������~
wait 1
wechoaround %actor% %actor.name% �������%actor.q% ����� ���������� ���� ���������� � ������%actor.g%.
wsend %actor.name% _�� ��������� ����� ���������� ���� ����������.
wteleport %actor% 20934 horse
wat 20934 wechoaround %actor% %actor.name% � �������� ������� ������%actor.u% � ���������� ���� ����������.
end
~
#20910
����������� ��������~
0 q 100
~
%send% %actor.name% ������� ������ ��� : "������������ � ������� ������...."
%send% %actor.name% ������� ������ ��� : "�� 20 ��� � �������� ����."
~
#20911
���� ������ ��������~
0 m 0
~
if %actor.vnum% != -1
 halt
end
tell %actor.name% ����� ��� ���� ������� �������. �����, ����� ��� � ������ �����.
return 0
halt
if %amount% < 20
 tell %actor.name% �� ����� ������ ��� ��� ��� ���� ���
else
 %send% %actor% ������� ������ ��� �� ��������� ����, � ����� �������.
 eval temp %actor.wait(5)%
 dg_cast '���������' .%actor.name%
end
~
#20912
�������� 2��� �� �����~
2 c 100
*~
if (%cmd.mudcommand% == ��)||(%cmd.mudcommand% == south)
 if %world.people(20969)% > 1
  %send% %actor% ������� ���� �����.
  return 1
  halt
 end
end
return 0
~
#20913
������ �����~
0 ab 5
~
if %random.4% == 1
 %echo% ������ ���-�� ������������ �������.
 ���
else
 %echo% ������ ��������� �������� ���-��, �� ������ ��� ��� ������.
end
~
#20914
�������� ������~
0 ab 5
~
%echo% ������ ���������� �� ��� � ������� �����������.
wait 1s
%echo% ������ �����������, � ��������� ������ �����-�� ��������.
~
#20915
[��������] ���������~
1 c 3
��������~
if (%actor.clan%==���)
 oechoaround %actor% %actor.name% ����� �������%actor.g% ���������, � �� ����.
 osend %actor% �� ����� �������� ���������, � �� ����.
 oload mob 20924
 oforce %actor% ������ ��������
 opurge %self%
else
 oechoaround %actor% %actor.name% �������%actor.u% �������� ���������, �� �� ��������� ��������.
 osend %actor% �� ���������� �������� ���������, �� �� ��������� ��������.
end
~
#20916
[���������] ���������~
0 c 0
���������~
if (%actor.clan%==���)
 if (%self.leader% == %actor%)
  mechoaround %actor% %actor.name% ��������%actor.g% ��������� � ��� �������� ���� ��������.
  msend %actor% �� ��������� ��������� � ��� ����� ���� ��������.
  �� ���.������
  calcuid room %self.realroom% room
  attach 20919 %room.id%
  exec 20919 %room.id%
  if %actor.affect(������ ��� ��� ������)%
   mforce %actor% ���������
  end
  mforce %actor% ����� ������
  mpurge %self%
 else
  msend %actor% ��� �� ��� ��������.
 end
else
 mechoaround %actor% %actor.name% �������%actor.u% ��������� ���������, �� ��� ��������.
 msend %actor% �� ���������� ��������� ���������, �� �� �������� �� ����� ���.
end
~
#20917
[���� ������ ��������]~
0 j 100
~
if (%actor.clan%!=���)
 if (%object.vnum%!=20906)
  return 0
  mecho �������� �����������.
  halt
 end
end
wait 1
���
mecho �������� ������� ������ � ���� � �������� ���� �� �����.
~
#20918
[�������] ������ � ���������~
0 c 0
�������~
wait 1
if %self.leader%!=%actor%
 halt
end
if (%actor.clan%==���)
 if %self.haveobj(20907)%
  ������� .%actor.name%
  ���� ������ .%actor.name%
 else
  ������ .%actor.name%
 end
end
~
#20919
�������� �������� ��������~
2 z 0
~
wload obj 20905
detach 20919 %self.id%
~
#20920
������ ������ [���] ���������~
0 c 0
���~
if (%actor.clan%==���)
 if %self.leader%==%actor%
  mforce %actor% say ���!
  ��� %arg%
  else
  msend %actor% ��� �� ��� ��������.
 end
else
 msend %actor% �������� ���� ��������� �� ���.
end
~
#20921
���� 50 ��� ������~
0 m 0
~
wait 1
if (%amount% < 50)
say �� ��� �� ��, ������ ���� ������� ? ������ ���� ������ �������.
���� %amount% ��� %actor.iname%
elseif (%amount% == 50)
say �� ���, ����� ����� ���������.
mload obj 20905
���� ���.���� %actor.iname%
wait 1
�� ���
elseif (%amount% > 50)
say ������� �� ���� ��������, ������� ���.
mload obj 20905
���� ���.���� %actor.iname%
wait 1
�� ���
end
~
#20922
�������� ���� � �����~
0 e 0
�����������.~
wait 1
if %actor% == %self.leader%
 ����
 ����� �����������
end
~
#20923
�������� �� ���� ���~
0 k 100
~
if (%actor.clan%==���)
 msend %actor% ����������� ��� ����� �������� ��������.
 ������
end
~
#20950
[� �����] ������� ������~
0 d 100
�����~
if %actor.clanrank% == 9
 attach 20951 %self.id%
 ����
 say ����� ������
 wait 1
 ���
calcuid volhvroom %self.realroom% room
detach 20953 %volhvroom.id%
end
~
#20951
����� ��������� �����������~
0 q 100
~
calcuid volhvroom %self.realroom% room
attach 20953 %volhvroom.id%
wait 1
����� .%actor.iname%
say ����������, %actor.iname%, ���� � ���� �����, ���� � �� ���.
wait 1
���
if %actor.level% > 24
say �����%actor.w% �� ���, ��� �� ������� ���� ?
attach 20952 %self.id%
wait 100s
if !%bylotvet%
say �� ������ ����� ����%actor.y% ? ����� � ������ ���� ��� ������.
msend %actor% ����� ������ ������� �� ���, � ��� ���������� �������� ��������.
mforce %actor% north
rdelete bylotvet %self.id%
detach 20952 %self.id%
detach 20953 %volhvroom.id%
detach 20951 %self.id%
halt
else
detach 20951 %self.id%
halt
end
elseif %actor.level% < 11
say �����%actor.a% �� ���, ��� ����� ���� ?
say ������ ����� ����������� !
msend %actor% ����� ������ ������� �� ���, � ��� ���������� �������� ��������.
mforce %actor% north
detach 20953 %volhvroom.id%
detach 20951 %self.id%
halt
else
say �����%actor.g% ��, ������� ������ ��� ?
* ��������� ������ ��� 11-24 �������
attach 20954 %self.id%
detach 20951 %self.id%
halt
end
~
#20952
����� ������~
0 d 0
������ ������ ����� ������ �������� ���� ����� ������� ����� ���� sex �����~
set bylotvet 1
remote bylotvet %self.id%
wait 1
say �� ��� �� ��� ������� ����� ��������� ���������.
����
wait 1s
say ���� �� ����, ���������.
msend %actor% ������������� ���� ��������� ��� �����.
mforce %actor% �����
eval templag %actor.wait(12)%
wait 1s
say ��� ������, �����.
mecho ����� ���� ����� � ������� ������� � ����.
mload obj 20970
���� ���� %actor.iname%
wait 2s
mforce %actor% ���
mforce %actor% ������ �����
wait 3s
mforce %actor% ���� ����
wait 1s
msend %actor% ������ ���� ������� ��� � ������.
wait 3s
mforce %actor% ������� ����
�� ����
mpurge ����
wait 1s
msend %actor% �������� �������� �������� ���.
msend %actor% ����� ��������� ������� �����.
wait 1s
msend %actor% �� ������� ����� � ����� �����.
mechoaround %actor% ������ ����� ����� �������� ��� ��������, ����� �� ������� �����
mechoaround %actor% %actor.rname% ��� �� ��������� �� �����.
mteleport %actor% 20990 horse
wait 1s
msend %actor% ����� ������ ������� �� ��� ��������:
msend %actor% - ����� ������, ���� ������ ���.
msend %actor% - �����.
calcuid volhvroom %self.realroom% room
detach 20953 %volhvroom.id%
detach 20952 %self.id%
~
#20953
�� ������� � �������� ����������~
2 e 100
~
wsend %actor% ���-�� ���� �� ���� ��� ������� ��� ������.
return 0
~
#20954
11-24�� [� ������]~
0 d 100
������~
say �� ��� ��. ������ � �� �������� �������.
wait 2s
say �������, ��� ������� ���� ?
attach 20952 %self.id%
wait 100s
if !%bylotvet%
say �� ������� ���� �����, �� ������� ����� �����.
say ������ ���� ����� ������!
wait 1s
msend %actor% ����� ������ ������� �� ���, � ��� ���������� �������� ��������.
mechoaround %actor% %actor.iname% �������%actor.u% �� ������.
mforce %actor% north
detach 20952 %self.id%
calcuid volhvroom %self.realroom% room
detach 20953 %volhvroom.id%
detach 20954 %self.id%
halt
else
detach 20954 %self.id%
halt
end
~
#20955
��������� �����~
2 e 100
~
if !%exist.mob(20951)%
wecho ������� ������� ����� �� ����.
wload mob 20951
end
calcuid volchish 20951 mob
run 20957 %volchish.id%
~
#20956
������ ��������� � ���~
2 g 100
~
if %actor.realroom% == 20991
wsend %actor% ������� ��� � ��� ���� �� ��������.
return 0
end
if %actor.realroom% == 20993
wsend %actor% ������� ��� � ��� ���� �� ��������.
return 0
end
~
#20957
� �������� �������~
0 z 100
~
wait %random.9%s
if %self.fighting%
 halt
end
mecho ������� ������� ������ ��������� �� ���.
wait %random.9%s
if %self.fighting%
 halt
end
���
wait %random.9%s
if %self.fighting%
 halt
end
mecho ������� ������� ������� ����� � �������� �� ���.
wait %random.9%s
if %self.fighting%
 halt
end
wait 20s
mecho ������� ������� �����. ��� ������.
wait 2s
msend %actor% �������������� � ��� �����, �� ������ : - �� � ���� ��� �������� �����!
msend %actor% ��������� ������� � ����, �� ������ ���� � �����, � �������� ������ ��������.
mload obj 20971
��� ����.����� .%actor.iname%
wait 2s
mecho ������� ������� ������� �������.
����� .%actor.iname%
wait 2s
mecho �� ����� ���� ����� ��� � �� �������� �����, ������� � �� ������ ���.
mecho ������ �� ����������� � �������� ���� ������� � ������������ ����.
mecho ���� ��������.
calcuid volhv 20950 mob
set kvest 1
remote kvest %volhv.id%
mpurge %self%
~
#20958
������ �������� �������~
0 f 100
~
calcuid volhv 20950 mob
set kvest 0
remote kvest %volhv.id%
~
#20959
��������� ��������~
0 z 100
~
mteleport %kvestor% %self.realroom%
if %kvest% == 0
 ���� %kvestor.iname% �� ����%kvestor.y% ��������� !
else
 ���� %kvestor.iname% ����%kvestor.y% ��������� !
end
wait 1s
msend %kvestor% �� ������.
wait 1s
msend %kvestor% ������� ��� ��� �����-���� ���.
wait 1s
if %kvest% == 0
 msend %kvestor% �� ����� ������ �� ���� ?
else
 if %actor.haveobj(20972)%
  msend %kvestor% �� ����� ������ �� ��� ���� ?
  mforce %kvestor% ���� ����.������
 elseif %actor.haveobj(20971)%
  msend %kvestor% �� ����� ������ �� ��� �������� ?
  mforce %kvestor% ���� ������.�����
 end
end
rdelete kvest %self.id%
rdelete kvestor %self.id%
~
#20960
������ ���������~
2 e 100
~
wait 1
if (%actor.realroom% == 20992)
calcuid volhv 20950 mob
eval kvestor %actor%
remote kvestor %volhv.id%
run 20959 %volhv.id%
end
~
#20961
����� �� ������~
0 p 100
~
if (%self.hitp% < 500)
msend %damager% ����� �������� �����, ��������� �� ��� � ������.
mechoaround %damager% ����� �������� �����, �������� �� %damager.rname% � ������.
calcuid room %self.realroom% room
run 20965 %room.id%
halt
end
return 0
~
#20962
����� ������ ���~
2 d 0
*~
if (%speech.contains(������ ���)% || %speech.contains(����� ���)%)
 if (%speech.contains(����)% || %speech.contains(�����)% || %speech.contains(�����)%)
 %send% %actor% ����� �� ������ ������ �� ��� ���:
 %send% %actor% - ������ ������ � ������� �������.
 %send% %actor% - ���� ������ � ����.
 end
end
~
#20963
[������� ������]~
0 c 0
������� ���������~
if (%actor.vnum% != -1)
%echo% %actor.iname% �����%actor.g% ������ �� ���� � ������� ����-��, �� ������ �� ���%actor.y%.
halt
end
if !%arg.contains(�����)%
%send% %actor% ���� �� ��������� ������� ?
halt
end
if %self.fighting%
%send% %actor% �� ����� ����������� � ���.
halt
end
wait 1s
%send% %actor% _����������� � �����, �� ���������� ��������� ������� ����,
%send% %actor% _����� ������������ ����� ������ ��������.
wait 4s
%send% %actor% ����� ������� �������, �� ��������� ������ ������� ����.
wait 4s
%send% %actor% ������� ����� ���� ���� �� ����� ���.
wait 5s
%send% %actor% ������� ����� ���� ����.
%send% %actor% ������� ����� ����� � ������� ����-�� � ���. �� ��� ������ ������ ��������� ����.
calcuid roomper %self.realroom% room
attach 20964 %roomper.id%
exec 20964 %roomper.id%
wait 1
%force% %actor% �� ����
wait 1
%send% %actor% �� ��������� ������ �� ������� �������.
wait 5s
%send% %actor% ����� ��������� ����� �� �������� ��� ��� ����� ������...
eval temppaus %actor.wait(1)%
wait 3s
calcuid volhv 20950 mob
set kvest 1
remote kvest %volhv.id%
%door% 20993 north room 20992 flags a
%force% %actor% �����
wait 1s
%door% 20993 north purge
~
#20964
������ ���� ������~
2 z 0
~
wload obj 20972
detach 20964 %self.id%
~
#20965
����� ������~
2 z 0
~
%purge% �����.�����
%send% %damager% ��� ������ ��������� ���.
%send% %damager% �� ������� �� �����, �� � ����� ��������������.
eval temppaus %damager.wait(3)%
wait 3s
%send% %damager% ����� �� ������ ������������, �� ���������� � ������� ������ � ���� �� �����.
calcuid volhv 20950 mob
set kvest 0
remote kvest %volhv.id%
%door% 20993 north room 20992 flags a
%force% %damager% �����
wait 1s
%door% 20993 north purge
~
$~
