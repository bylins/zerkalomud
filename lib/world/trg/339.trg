#33900
��������~
0 f 10
~
if %world.curobjs(33901)% < 100
     mload obj 33901
end
if %world.curobjs(33912)% < 50
     mload obj 33912
end
~
#33901
��������� �������~
0 f 10
~
if %world.curobjs(33916)% < 50
     mload obj 33916
end
~
#33902
������ �������~
0 f 10
~
if %world.curobjs(33915)% < 10
     mload obj 33915
end
~
#33903
������ �������~
0 f 10
~
if %world.curobjs(33904)% < 10
     mload obj 33904
end
~
#33904
����� �������~
0 f 10
~
if %world.curobjs(33903)% < 5
     mload obj 33903
end
~
#33905
��������~
0 f 10
~
if %world.curobjs(33902)% < 6
     mload obj 33902
end
~
#33906
���������_���-��������~
0 q 100
~
wait 1
msend %actor% ����� ���� ����������� �������� ��� � ������ �� ���.
mechoaround %actor% ����� ���� ����������� �������� %actor.vname% � ������ �� ���.
mecho ���� �� ��������� ���������.
wait 1s
msend %actor% ����� ����� ����� �������� �� ���.
mechoaround %actor% ����� ����� ����� �������� �� %actor.vname%.
wait 1s 
���� %actor.name%
say ��, ������� �� ������� ��� ������, %actor.name%...
wait 1s
say �����%actor.g% �� �� ��� ������?
calcuid voin 33925 mob
attach 33907 %voin.id%
~
#33907
������~
0 d 0
�� ������~
wait 1
say �������!
wait 5
mechoaround %actor% ������ ����� ������� �������, ������ ���������
mechoaround %actor% � ����� ���-�� ���� �������� %actor.dname%.
msend %actor% ������ ����� ������� ������� � ������ ���������.
tell %actor.name% � ��� � ������� ���������� �������� ����.
tell %actor.name% ��� �������� ����� ������ ��������.
tell %actor.name% ��� ���� �������� ��� �����!
tell %actor.name% ��, � ���������, �� ������� ������ � ��� ���...
���
wait 10
tell %actor.name% � � ��� ��� � ��� ������ ����������� ���-�� ��������.
tell %actor.name% ������� �� ������ ���, ��� ������ �����, ������� ��� � ������.
tell %actor.name% � �� ��������� ���� �� �����...
tell %actor.name% � ����� ���, �� �� �����.
wait 5
tell %actor.name% ��������, �� ������� ���, %actor.name%, �� ��� �������� �������.
tell %actor.name% ���, � ��� ����, �������� ��� ���������� ����.
tell %actor.name% ��� ��������� ������������ ����, ���� �� ������� �� ����.
wait 1s
say ��, ���� �� �����! 
wait 5
msend %actor% ���� ���������� � ��� � ����� ������� �� ���.
mechoaround %actor% ���� ���������� � ���-�� �������� �� ��� %actor.dname%
msend %actor% �� ��� ������� � �����-�� ����������� �����.
msend %actor% �� ����� �� ����� ��� ���...
wait 1s
mecho ���� ������� ����� � ��������� ���������� � ���� ����.
eval questor339 %actor.name%
worlds questor339
detach 33906 %self.id%
calcuid gena 33926 mob
attach 33909 %gena.id%
detach 33907 %self.id%
~
#33908
�������� ~
0 q 100
~
wait 1
msend %actor% �������� ��������� ��� � ������ �� ���.
mechoaround %actor% ������� ��������� %actor.rname% � ������ �� ���.
wait 1s
msend %actor% - �� �� �������� ���� ��� �� ���� ����, %actor.name%!
msend %actor% ����� ����� ��� ������ ��� ������������ � �����������.
mechoaround %actor% ������� ������������ %actor.vname% � �����������.
msend %actor% ���... ������ ���� ���������, ��� �� �������, � ������ ����� �������� ��������.
mechoaround %actor% �� ������� %actor.vname%! �� ������ ��� ������� ������ ���!
~
#33909
�����������~
0 c 0
�������~
if (%arg.contains(������)% || %arg.contains(����)% || %arg.contains(����)%) 
 if %actor.name%==%questor339%
  wait 2
  calcuid gena 33926 mob
  msend %actor% ������� ��������� �� ��� ������� �������, ���������� � ��������� �������.
  attach 33915 %self.id%
  run 33915 %self.id%
 else
  wait 2
  msend %actor% ������� ��������� �� ��� ������� �������, ����������
  msend %actor% � ����� �����������.
 end
else
 return 0
end
~
#33910
������������~
0 q 100
~
wait 1s
msend %actor.name% ������ �������� ������, �� ��������� ��������� � ��������� ������ ������, �� ������� ���.
~
#33911
�������������~
0 q 100
~
if (%actor.name%!=%questor339%)
 halt
end
wait 2
if %exist.mob(33927)%
 msend %actor.name% �������� ����������� � ��� � ������ ���� ��������.
 tell %actor.name% %actor.name%, � ������ ���� ���� �� ���� �������� �������.
 mechoaround %actor% ������� ��������� � %actor.dname% � ����� ��� ������ ��������.
 tell %actor.name% �����, ������� �� ������� �������, ����� ��������!
 tell %actor.name% � ��� �������� ������� �� ����, � ����� �� ����� �� ������.
 tell %actor.name% ������ ��, � ���� ���� �� ������� ��� ��������.
 mecho ������� ����������� ������ � �������������� � ����� ����������.
 calcuid vdova 33910 mob
 attach 33916 %vdova.id%
end
detach 33911 %self.id%
~
#33912
����������������~
0 q 100
~
wait 1s
msend %actor% ������ �������� ������, �� ��������� ��������� � ��������� ������ ������, �� ������� ���.
if %actor.name%==%questor339%
 if %exist.mob(33927)%
  wait 1s
  msend %actor% ���������� ������ ������ �� ��� ����� � ������� � ������ �� ���.
  msend %actor% - �, �����. ����, �������, �������� ���� �������. �� ��� �...
  wait 1s
  msend %actor% - �� �������� ���������� ���� - ������ ��� �� ���, � ����� �, ��������, ������ ����.
  msend %actor% ���� ����� ������� ������ ������.
  calcuid nichiy 33915 mob
  attach 33913 %nichiy.id%
  attach 33930 %self.id%
  detach 33912 %self.id%
 end
end
~
#33913
���������������~
0 m 1
~
wait 1
if %amount% < 100
   say ������� ��������, �� ��������� ���� ��� �� ������� �����!
   halt
end
switch %myvar%
  case 1
    say �� �������� ����� ���������� ����!
    ����
    wait 1
    mecho ������ �������� ����� ������ ��� ����� ��� ���-��.
    set myvar 2
    global myvar
  break
  case 2
    say ������ ���������� ��� �� �����, ������� ���� �� �������� ���������.
    wait 1
    mecho ������ �������� ����� ������ ��� ����� ��� ���-��.
    set myvar 3
    global myvar
  break
  case 3
    say ��� ���� �������, ��� ��� ���� � �������� ���! ������� �������!
    wait 1
    mecho ������ �������� ����� ������ ��� ����� ��� ���-��.
    set myvar 4
    global myvar
  break
  case 4
    say � ��� ���������� �� � ����������. ����� ���� ����� �� ������������.
    wait 1
    mecho ������ �������� ����� ������ ��� ����� ��� ���-��.
    set myvar 5
    global myvar
  break
  case 5
    say � ��� �������, ����� ������� ���� ���� �������, ������� �� ������� � ������ ����...
    wait 1
    mecho ������ ��������, ����� ����������, ������� � ����� � ������.
    set myvar 0
    global myvar
    mpurge %self.name%
  break
  default
    say �������� �������, ����, �� �� ���������� �� ���� �����!
    wait 1
    mecho ������ �������� ����� ������ ��� ����� ���-�� ����� ������.
    set myvar 1
    global myvar
  break
done
~
#33914
������������~
0 q 100
~
%actor.wait(2)%
wait 1
mecho ������ ���� ��� � �������� � ��� ���� ������� ����.
msend %actor% �� ���������� �� ������� ��������.
wait 1s
msend %actor% �������� ���� ������, � ��� �� �������, ��� ������, ���� ������ ��� ��� ������.
msend %actor% ��� ���� �� ��� �� � ����� - ����� 100, � ����� 200, ���� ��������� �� �����.
wait 2
msend %actor% ������ ��������� �� ��� ������� � ���������.
~
#33915
����������2~
0 z 100
~
%actor.wait(3)%
wait 2
if %actor.sex% == 1
   msend %actor% ��... ��... ������� ���� ��������... ����� �� ����� ����� �� �����... 
   wait 1s
   say %actor.name%, � ���� �� �� ������ ��������. 
   say ��� ��� ����� ����������� ���������. 
   say � � ��������� ����� �� �������� � �������� �����.
   say ���� � �� ����� ��������.
   wait 1s
   say ��� ���� ��� �����. ��� ���, ��� �������, ��� ���� ����� ��� ���� ������.
   say ������, ��� ���� ������ ����������.
   wait 1s
   msend %actor% ������� ������ ��� � ����� ����������: "����� ����� ����, � �� �� ���������!"
   mechoaround %actor% ������� ������ %actor.vname% � ����� ����������.
else
   msend %actor% �� ��� �, ������� ����� ������ �������... �� ������� ��� �����.
   wait 1s
   say %actor.name%, �� ������� �������, � �� ������� �� ������ ������� �� ������.
   say ��� �������, ���� ����� ��� ���� ����� ����� ����.
   say �� ����� ������� � �����-�� �����, ����� ��� ������ �� �����������.
   say � ����� � ���� ����� �������, ������ - ��� ������������ ��� � ���� ��������.
   say ����� ���� ����� ������ � ����� ���������, �������, ��� ���� ����� ��� ������ ����.
   say ����������, ����� ��� ����� ������������.
   ����
end
 calcuid star 33917 mob
 attach 33911 %star.id%
 detach 33908 %star.id%
 calcuid diak 33916 mob
 detach 33910 %diak.id%
 attach 33912 %diak.id%
 detach 33909 %self.id%
 detach 33915 %self.id%
~
#33916
����� ������ �������~
0 q 100
~
if (%actor.name%==%questor339%) && %exist.mob(33927)%
 wait 1
 �����
 ������
 wait 1s
 mecho ����� ����� ���������� �... ����� ���������� �����������.
 wait 1s
 mecho ������������, �����, ��� �� ��������, ����������.
 wait 1s
 msend %actor% - �������� ����, �� � ���� ������� ����. ��� ��� �����-�� ����� �����
 msend %actor% � �������� ���. � �� �� ��� ������ �����.
 wait 1s      
 ����
 wait 1s
 msend %actor% - ������ �� - ���������� �������, - �� �������� �� ��� �� �����.
 msend %actor% � ������� ������� � �����������, �� ��������. ��� �� ����� �����
 msend %actor% ��� �������������. ����� ����, %actor.name%, �������� ���!
 wait 1s
 msend %actor% - � �������� ����� � ����� ���� ���-����. ����� ����, ������!
 msend %actor% ������� ������ ��������� � ����������.
 wait 1s
 msend %actor.name% - �� ���������� � ������ ��������, �� � �� ����, ��� ��� ��������.
 msend %actor.name% ����� ����! - ������� ��� � �����������.
 attach 33918 %self.id%
 detach 33916 %self.id%
end
~
#33917
������������~
2 c 0
������������~
wait 1
if !(%arg.contains(���)%) 
   return 0
   halt
end
wsend  %actor% ����� ���������, ������ ���������, �... ����� ����� ������� �������� ���!
wsend  %actor% ������ ��������� �� ����� ������ ������ � � ���������� ����������, ���
wsend  %actor% ��������� ��������� ����������� �������!
wload mob 33928
calcuid slav 33928 mob
attach 33923 %slav.id%
exec 33923 %slav.id%
wait 1s
wpurge ������
detach 33917 %self.id%
~
#33918
������ � ��������~
0 q 100
~
if (%actor.name%==%questor339%) 
 if %exist.mob(33927)%
  wait 2
  ������
  msend %actor% - ��, %actor.name%, �� ��� ��� ������ ����!
 else
  wait 2
  ��
  msend %actor% - �� ��������%actor.g% � ���?
  attach 33920 %self.id%
  detach 33918 %self.id%
 end
end
~
#33919
��������������������~
0 c 0
��������~
if (%actor.name%!=%questor339%) 
 return 0
 halt
end
say �� ��� �, � ���� �� �������%actor.u%! ������ �, �������, ������ ���� ���� ������.
mecho ���� �� ��������� ���������.
wait 8
say ����� � ������������ ������� ����� ����������� ����������� �� ����� 
mecho ��������� ������. �� ����, ��� � ��� �����������, ��������� ������ �����!
wait 1s
say � ���, ���� �� �����, ���� �� �����. ����������!
if %world.curobjs(33935)% < 1
 mload obj 33935
 say ������ ��� - ����������...
 wait 2
 ���� ���� %actor.name%
end
wait 2
mecho ���� ����� ������ �� ��� � �������� �� ���� ������� ������.
calcuid aaa 33981 room
attach 33917 %aaa.id%
detach 33919 %self.id%
~
#33920
��������������������~
0 c 0
��������~
if (%actor.name%!=%questor339%) 
 return 0
 halt
end
say �� ��� �, � ���� �� �������%actor.u%! ������ �, �������, ����������� ����.
wait 1s
mecho ����� ������ ������ � ��������.
wait 1s
mecho ���! - ������� ��� ������ �� ������� ��������� �����.
say ������ ��� ��� ���������� � ����� � ���� �������������.
mload obj 33900
mload obj 33929
���� ��� %actor.name% 
���� ��� %actor.name%
wait 1s
mecho ����� ���������� � �������� ��� �� ��������.
calcuid  vdova 33910 mob
detach 33920 %vdova.id%
~
#33921
���������������~
2 z 100
~
wait 1s
%purge% questor3391
wecho ����� ����� ������� �������� ��� �� ���������! �� �� - ������������ ��...
if %world.curobjs(33934)% == 0
wload obj 33934
end
~
#33922
�����������������������~
0 n 100
~
if %world.curobjs(33905)% < 2
mload obj 33905
end
if %world.curobjs(33913)% < 25
mload obj 33913
end
if %world.curobjs(33914)% < 10
mload obj 33914
end
if %world.curobjs(33925)% < 10
mload obj 33925
end
~
#33923
������~
0 z 0
~
wait 1
mecho ������ �������� � ������ ����!
say %actor.name%, ���, � ������� ���� � �����!
dg_cast 'san' %actor.name%
mecho ������ �����...
~
#33924
��������~
0 r 100
~
wait 1s
����
say �, ��������� ���! ������� ����!
wait 1s
mecho ������ ���������.
say � ����� ���������� ��������� � ��������� ��� �����.
say �� ��� ����, � ���-�� ������� ���� ���������� ������.
say ��� � ������ ���� ����. ����� �� � ����� � ������� �����!
detach 33924 %self.id%
~
#33925
���������������������~
0 j 100
~
wait 1
if %object.vnum% == 33934
 say �������� �������! ��� ��, ������ � ������� ������ �����!
 mecho ������ ������� ���������� � ��������� �����-�� ���������� ����� � ����� �������.
 mecho ������ ��� ���� ����! - ��������� ��� ������-�� ������.
 calcuid luks 33949 room
 exec 33921 %luks.id%
 calcuid genae 33926 mob
 attach 33927 %genae.id%
end
~
#33927
������������������~
0 j 100
~
wait 1
if !(%object.vnum% == 33934)
msend %actor% ����� ��� ���?
eval getobject %object.name%
����� %getobject.car%.%getobject.cdr%
halt
end
if !(%actor.name%==%questor339%)
msend %actor% H� ���� ������ �������, �� �� ���������� �������.
else
mpurge %object.name%
msend %actor% ������� ��������� �� ��� � �������� �������� �������.
mechoaround %actor% ������� ��������� �� %actor.rname% � �������� �������� �������.
msend %actor% ��� ��� - ������ ����� ����������� ���, - �� �����!
����
wait 2
say �� ����, ������ � ���� ���� ��� ������, ������ �� ��� ����� �������.
say �� ��� � ���� �����������!
switch %actor.class%
*������
case 0
if (!%actor.spelltype(������� ���)%) & (%actor.level%>=10)
mspellturn %actor.name% �������.��� set
say �, %actor.name%. ������ ���������� ������� ���!
else
%self.gold(+1000)%
���� 1000 ��� %actor.name%
end
break
*������
case 1
if (%random.10% < 5)
if (!%actor.spelltype(������������)%) & (%actor.level%>=20)
mspellturn %actor.name% ������������ set
say �, %actor.name%. ����� �� �� ������� ������������ ��� ������ ������!
end
end
break
*����
case 2
if (!%actor.skill(�����������)%) & (%actor.level%>9)
mskillturn %actor.name% ����������� set
say ��� ���� ������, ��� ��������������, %actor.name%.
else
say ������, �� ���� �� ���%actor.g% ���, ���� ��� ������ �����������. 
end
break
*��������
case 3
if %actor.skill(������)% < 98
if %actor.skill(������)% > 4
mskilladd %actor.name% ������ %random.3%
else
mecho ������, �� � �� ���� ���� ������� ������ ������...
end
else
��� %actor.name%
mecho ������, � ��� ������ ������ ���� �� �����...
�����
end
break
*�������
case 4
if %actor.skill(����������)% < 98
mskilladd %actor.name% ���������� %random.3%
else
��� %actor.name%
mecho ������, � ��� ������ ������ ���� �� �����...
�����
end
break
*����
case 5
if %actor.skill(�����)% < 98
mskilladd %actor.name% ����� %random.3%
else
��� %actor.name%
mecho ������, � ��� ������ ������ ���� �� �����...
�����
end
break
*��������
case 6
if (%actor.level%<18)
say �� ������� ���, ���� � ���� ���� �����������, ����� ������.
%actor.gold(+1000)%
else
if !%actor.spelltype(��������� ����)%
mspellturn %actor.name% �����.���� set
say �, %actor.name%. ������ �� ������� ���� ����������� ������ �������!
else
say �� ��� ������ ��� ����������, ���� ������������� ���� ������ ��������. 
%actor.gold(+1000)%
end
break
*����������!
case 7
if (!%actor.spelltype(��������)%) & (%actor.level%>=14)
mspellturn %actor.name% �������� set
say �, %actor.name%. ������ �� ������� ��������� ���������!
else
say ������, �� ���� �� ���%actor.g% ���, ���� ��������� ����� ������������ ����. 
end
break
*������
case 8
if (!%actor.spelltype(��������� �����)%) & (%actor.level%>=13)
mspellturn %actor.name% ���������.����� set
say �, %actor.name%. ������ �� ������ ����� ���������� �����!
else
say ������, �� ���� �� ���%actor.g% ���, ���� ���������� ����� ���� ��� ��������. 
end
break
*������
case 9
if %actor.skill(�����������)% < 98
mskilladd %actor.name% ����������� %random.3%
else
��� %actor.name%
mecho ������, � ��� ������ ������ ���� �� �����...
�����
end
break
*����
case 10
if %actor.skill(���������)% < 98
mskilladd %actor.name% ��������� %random.3%
else
��� %actor.name%
mecho ������, � ��� ������ ������ ���� �� �����...
�����
end
break
*������
case 11
if %actor.skill(��������)% < 98
mskilladd %actor.name% �������� %random.3%
else
��� %actor.name%
mecho ������, � ��� ������ ������ ���� �� �����...
�����
end
break
*�����
case 12
if (!%actor.spelltype(�������)%) & (%actor.level%>=25)
mspellturn %actor.name% ������� set
say ��� ���� ������ ���������� �������, %actor.name%.
else
say ������, �� ���� �� ���%actor.g% ���, ���� ��� ������ � ���������� ��������.
end
break
*�����
* case 13
* mload obj 621
* say ������ �������... �� ����� �������� ��....
* ���� ����� %actor.name%
* break   
default
%self.gold(+1000)%
���� 1000 ��� %actor.name%
break
done
wait 1
end
detach 33927 %self.id%
~
#33928
������������~
0 f 100
~
if (%world.curobjs(528)%) == 0 && (%random.4%==1)
  mload obj 528
end
~
#33929
����� ������~
2 f 100
~
unset questor339
calcuid voin 33925 mob
attach 33906 %voin.id%
calcuid gena 33926 mob
detach 33909 %gena.id%
detach 33915 %gena.id%
detach 33927 %gena.id%
calcuid dyak 33916 mob
detach 33912 %dyak.id%
detach 33919 %dyak.id%
detach 33930 %dyak.id%
attach 33910 %dyak.id%
calcuid star 33917 mob
detach 33911 %star.id%
attach 33908 %star.id%
calcuid ubog 33915 mob
detach 33913 %ubog.id%
attach 33914 %ubog.id%
calcuid vdov 33910 mob
detach 33916 %vdov.id%
detach 33918 %vdov.id%
detach 33920 %vdov.id%
calcuid iconroom 33981 room
detach 33917 %iconroom.id%
calcuid luch 33923 mob
attach 33924 %luch.id%
~
#33930
������ � ��������~
0 q 100
~
if %actor.name%!=%questor339%
 wait 1s
 msend %actor.name% ������ �������� ������, �� ��������� ��������� � ��������� ������ ������, �� ������� ���.
 halt
end
if %exist.mob(33927)%
 wait 1s
 msend %actor% ���� ��������� �� ����� ��� � ���-�� ��������� ���� ��� ���, ����� �� ���.
 wait 1s
 msend %actor% ���� ����� ��� ������� � ������, ����� ������, ��� ������ ��� ������� ������.
 halt
end
wait 1s
msend %actor% ���� ��������� �� ����� ��� � ������� ������� ���.
wait 2
say �, %actor.name%! ��.. ������ �������%actor.u% � ���� ��������?
say ��-��, ��-��, �������.
wait 2
msend %actor% ������ ��������� �� ��� � ���������.
attach 33910 %self.id%
attach 33919 %self.id%
detach 33930 %self.id%
~
#33931
���� ���� ����� ���������� �� �������~
0 f 100
~
if %random.100% <= 20
if %world.curobjs(561)% < 2
mload obj 561
end
end
~
$~
