#33000
���� � ������� (33024)~
0 q 100
~
  wait 1s
  say ����� ��� ������, � ����� ���� ���������. ��� ������� �����-�� ������ ������ ��� �����������.
  �����
  wait 5s
say ��� �� � �� ����� �� ���.
~
#33001
�������� ������������ (33027)~
0 q 100
~
  wait 1
  say ������ ������! ��� ����, �����, ������ �������. � � ���� ���� ����������� �������� �������, �� ����������� �� ��������� ��������������. 
  wait 1
  ���� %actor.name%
~
#33002
���� 100 ����� �������� (33027)~
0 m 1
~
wait 1
if %amount% < 100 then
   say �������� �����,  � ��� ������ ��� ����������.
   halt
end
switch %myvar%
  case 1
    say - �� �������  ������ ��������, ����� � ��� � ��� ������ ������!!! ��..
    wait 1
    ���
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 2
    global myvar
  break
  case 2
    say - ���� ������ ���� �� �����, �� �� ����� ����� � ����� ������������� � ��������...
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 3
    global myvar
  break
  case 3
    say - �� �������� �������� ������� ���� ����, ������� � �����������, ���� ����� �� ����� � ������� ��� ����� �������...
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 4
    global myvar
  break
  case 4
    say - ������� ���� ���� �������� ����� �������� �������...
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 5
    global myvar
  break
  case 5
    say - C����� ����� �� ��� - ������� ����, ����� �� �������� ������� ��������, ��� �������� ������� ���� �� �������...
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 6
    global myvar
  break
  case 6
    say - � ���������� � �������, �������, ����� ����!!! ��� ��� ��������... �� �����...
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 7
    global myvar
  break
  case 7
    say - ������ �������, � ����� ����, ��� �������� ����������� �� ���� ������� �����!!!
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 8
    global myvar
  break
  case 8
    say - �������, ��� � �������, �������� ����������, ������� ��� ��� ������!!! � ��, ��������, ��� ���� �� � �����, ��� ��� ����!!!
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 9
    global myvar
  break
  case 9
    say - ���, ��� ���������? ������, ��� �����, ������� � ����� ���� ������...�� � �� � ���...
    wait 1
    mecho �������� ���������� �� ��� � ������� ������ ������.
    set myvar 0
    global myvar
  break
  default
    say - ������ ����, ������ � ��� �����, �� ���� ��������, ����� ���...
    wait 1
    mecho �������� ���������, ��������� � �������� �� �������.
    set myvar 1
    global myvar
  break
done
~
#33003
� ����� ~
0 q 100
~
  wait 1
  say ������� ������ �� ������ � �� �������. ��� ��� ������, �������!!!
  wait 1s
  mecho  _����� ���������� ���� ���������.
  say ��� �� ����, ����������!!!.
  msend  %actor%  ����� ��������� �������, �������� ��� � ���� ����� � ������ ������ ���� ����� �������!
  mechoaround %actor% %actor.name%, ������� ���� ��������, � ������ �������%actor.g% �� ����!!! 
  wait 1s
  msend %actor.name% .- ������� ���, �� ������ � ����������� ����.
  mteleport %actor% 33043
  calcuid zellroom 33043 room
  attach 33018 %zellroom.id%
  exec  33018 %zellroom.id%
  detach 33018 %zellroom.id%
  calcuid kupez 33013 mob
  attach 33013 %kupez.id%
  calcuid kupez 33013 mob
  detach 33003 %kupez.id%
~
#33004
���� � ���������� ~
0 r 100
~
wait 10
  say ��, ��������, ������%actor.g% ���� ��������!!!
  say �� ������!!!.
  ������
 kill %actor.name%
end
~
#33005
���� � ������� �������������� ������~
0 r 100
~
  wait 1s
  say ���������� ������....
  wait 1s
  say ��� ��������� ������� �������?
~
#33006
���� � ����������� �������~
0 q 100
~
  wait 1
  msend %actor%  ������ �������� �������� �� ���.
  mechoaround %actor% ������ �������� �������� �� %actor.vname%
~
#33007
��� ����� ��-��~
0 j 100
~
if %object.vnum% == 33019 then
  wait 2s
  say �������. �������%actor.g%!
  wait 2s
  if (%world.curobjs(242)% < 50) && (%random.3% == 2)
   mecho _������ ������ ���� � �����.
   mload obj 242
   wait 2s
   ��� ��� %actor.name%
  else
   mecho _������ ���� ������� ���� � ����.
   mload obj 33000
   wait 2s
   ��� ���� %actor.name%
  end
  calcuid var 33019 obj
  mpurge %var.name%
  end
  else
  say  ����� ��� ���? 
  eval getobject %object.name%
  ���� %getobject.car%.%getobject.cdr%
end
~
#33008
��� ������� �������~
0 j 100
~
if %object.vnum% == 33017 then
  wait 2s
  say ��. ����� ��� ��������... �� �����, ������ ���� ��������, ������ ������ �� ���� �� ����! 
  mpurge �������
  wait 2s
  mecho ������ ����� ��������� �������..
  wait 2s
  mload obj 33018
��� ���� .%actor.name%
end
~
#33009
���� � �������� �������~
0 g 100
~
  wait 2s
mecho  ������� ���������� ��� � ���������� �������� � ��� ������������
~
#33010
����� 10 ����� ������~
0 m 10
~
if %amount% < 10 then
  wait 1
  say - �� ����� ������ ����� ��� ������ �� ������!!!.
else
  wait 1s
  say - ������� ����� ������.
  wait 1s
  mecho _������ ������� ����-��.
  wait 1s
  mecho _������ ��������� � �������� ����, ������� ����� ����������.
  mload obj 33003
  ��� ��� %actor.name%
end
~
#33011
��� ����� �������~
0 j 100
~
wait 1s
set i 0
if %object.vnum% == 33310
wait 1s
if %world.curobjs(33307)% < 4 && %random.3%==1
say �������!
wait 1s
mload obj 33307
wait 10
say ����� ��� ��� ������!
���� ����.���� %actor.name%
else
set i 1
end
end
if %object.vnum% == 33311
wait 1s
if %world.curobjs(33308)% < 4 && %random.3%==1
say �������!
wait 1s
mload obj 33308
wait 10
say ����� ��� ��� ������!
���� ����.��� %actor.name%
else
set i 1
end
end
if %object.vnum% == 33309
set i 1
end
if %i%
say �������!
%self.gold(+20000)%
���� 20000 ��� %actor.name%
end
mpurge %object%
~
#33012
� ����������~
2 e 100
~
wait 3s
wecho _������ ���� ������ ��� � � ��� ����������� ������.
wait 5s
wecho _������ ���� ������ ��� � � ��� ����������� ������.
wait 10s
wecho _������ ���� ������ ��� � � ��� ����������� ������.
wait 15s
wecho _������ ���� ������ ��� � � ��� ����������� ������.
~
#33013
���� � ����� 2~
0 q 100
~
  wait 1
  say � �� ���� ������������ ���� �� ����� �� ��� �������!!!
  ������
  mkill %actor%
~
#33014
����� ��������~
2 f 100
~
calcuid kupez 33013 mob
detach 33013 %kupez.id%
calcuid kupez 33013 mob
attach 33003 %kupez.id%
 calcuid udochkki 33004 room
attach 33023 %udochkki.id% 
calcuid udochkl 33004 room
exec 33026  %udochkl.id%
~
#33015
�������������~
0 f 20
~
if %world.curobjs(501)% == 0
mload obj 501
end
~
#33016
���������~
0 f 20
~
switch %random.2%
  case 1
    if %world.curobjs(504)% == 0
      mload obj 504
    end
    break
  case 2
    if ((%world.curobjs(219)%<50) && (%random.5% == 1))
      mload obj 219
    end
  break
  default
  break
done
~
#33017
��������������~
0 f 20
~
if %world.curobjs(1271)% < 1
if %random.100% < 3
mload obj 1271
end
end
if %world.curobjs(521)% == 0
mload obj 521
end
~
#33018
����� ����� �������� �� �����~
2 z 100
~
wechoaround %actor% ���-�� � ������ ������ �������� ���� �� ����������� ������ � ��������� � �����.
~
#33019
����� � ������ � ����������~
2 e 100
~
wait 2s
wecho ���������� ��� �������� ���� �� ���.
wload mob 33022
wait 5s
wecho ���������� ��� �������� ���� �� ���.
wload mob 33022
detach 33019 %self.id%
~
#33020
���� ����������~
0 f 100
~
if %world.curobjs(522)% == 0
mload obj 522
end
~
#33021
�������� ����� ������ ����~
2 c 0
�������� ������������ ��������~
if !(%arg.contains(������)%) 
   wsend       %actor% ��� �� ������ ������������???
   return 0
   halt
end
  wsend       %actor% ������������, �� �������� ����� ������ �����.
  wechoaround %actor% ������������, %actor.name% �������%actor.g% ����� ������.
  wait 1s
  wsend %actor% .- �� �� �� ������� �������.
wteleport %actor% 33015
 wechoaround %actor% ���-�� ������� ����.
~
#33022
�������� ����� ������ �����~
2 c 0
�������� ������������ ��������~
if !(%arg.contains(������)%) 
   wsend       %actor% ��� �� ������ ������������???
   return 0
   halt
end
  wsend       %actor% ������������, �� �������� ����� ������ �����.
  wechoaround %actor% ������������, %actor.name% �������%actor.g% ����� ������.
  wait 1s
  wsend %actor% .- �� �� �� ������� �������.
  wteleport %actor.name% 33012
 wechoaround %actor% ���-�� ������� ����.
~
#33023
�������� ������~
2 c 0
�������� ��������� ������ ��������~
if !(%arg.contains(������)%) 
   wsend       %actor% ��� �� ������ ��������???
   return 0
   halt
end
wait 1
  wsend       %actor% �������� ������� � ���������� �������������, �� �������� ������.
  wechoaround %actor% �������� ������� � �������� �� ���� �������� ������, %actor.name% �������%actor.g% ������ � ����.
   wpurge  ������
   wload obj 33033
 calcuid udorchkk 33004 room
  attach 33024 %udorchkk.id% 
  exec  33024 %udorchkk.id% 
 calcuid urdochkki 33004 room
detach 33023 %urdochkki.id% 
~
#33024
�����~
2 z 100
~
switch %random.3%
  case 1
 wait 15s
wecho _�����. ����� ����� �������.
 calcuid uqdochkki 33004 room
 attach 33025 %uqdochkki.id% 
end
  break
  case 2
 wait 10s
wecho _�����. ����� ����� �������.
 calcuid uwdochkkii 33004 room
  attach 33025 %uwdochkkii.id% 
end
  break
  case 3
 wait 5s
wecho _�����. ����� ����� �������.
 calcuid uedochkkk 33004 room
  attach 33025 %uedochkkk.id% 
end
  break
done
~
#33025
������� ������~
2 c 0
������� ������� �������~
if !(%arg.contains(������)%) 
   wsend       %actor% ��� �� ������ �������???
   return 0
   halt
end
  wsend       %actor% �� ��������� ����� ������� � ����� ��������.
  wechoaround %actor% ������ ��� �����, %actor.name%, ��������� ������ ������%actor.g% �������.
 calcuid uddochkk 33004 room
 detach 33024 %uddochkk.id% 
switch %random.3%
  case 1
 wait 1s
wsend       %actor% ����������� ������ ��������� � ������... ������.
  wechoaround %actor%  %actor.name% �������%actor.g% �� ������, ����� �������� ������ ��������� � ������.
 calcuid udfochkki 33004 room
attach 33023 %udfochkki.id% 
   wpurge  ������
   wload obj 33028
 calcuid uhdochkkk 33004 room
  detach 33025 %uhdochkkk.id% 
end
  break
  case 2
 wait 1s
wsend       %actor% �� ������� �������� �������.
  wechoaround %actor%  %actor.name% � ������� ������� �������%actor.g% ��������� �������.
 calcuid ujdochkki 33004 room
attach 33023 %ujdochkki.id% 
   wpurge  ������
wload obj 33003
   wload obj 33028
 calcuid uldochkkk 33004 room
  detach 33025 %uldochkkk.id% 
end
  break
  case 3
 wait 1s
wsend       %actor% �� ������� �������� ������� ������.
  wechoaround %actor%  %actor.name%, ��������� �� ������, �������%actor.g% ������� ������.
 calcuid uxdochkki 33004 room
attach 33023 %uxdochkki.id% 
   wpurge  ������
wload obj 33034
   wload obj 33028
 calcuid udoqwchkkk 33004 room
  detach 33025 %udoqwchkkk.id% 
end
  break
done
~
#33026
������� ��� � ������~
2 z 100
~
   wpurge  ������
wload obj 33028
~
#33027
�����������~
0 r 100
~
wait 1
msend %actor% �������� ������ ����:
msend %actor% - ������ ����, %actor.name%!
msend %actor% - � ���� ��������� ���� ����� ���� � ����������� �� 100 ���.
~
#33028
�������~
0 m 1
~
wait 1
switch %amount%
 case 100
  msend %actor% �������� ����������� ����� � �����, ����� ��� ����� ������������ � ���.
  wait 1s
  msend %actor% �� ������� �� �����.
  msend %actor% ������� ����� �����, � ����� �� ��������� ����� ��������� � ������� � �����.
 if %actor.sex%==1
  mechoaround %actor% %actor.name% ����� � ����� � ����������.
  mechoaround %actor% %actor.name% ����� �� ����� � ����������.
 else
  mechoaround %actor% %actor.name% ������� � ����� � ����������.
  mechoaround %actor% %actor.name% ������ �� ����� � ����������.
 end
  wait 1s
  msend %actor% �� �������� � ����������� � ������� �� �����.
  msend %actor% �������� ��� � ����� � ������ �����.
  mteleport %actor.name% 25065
  wait 1s
* if %actor.sex%==1
*  mat 25065 mechoaround %actor% %actor.name% ������� � ����������.
*  mat 25065 mechoaround %actor% %actor.name% ����� �� �����.
* else
*  mat 25065 mechoaround %actor% %actor.name% �������� � ����������.
*  mat 25065 mechoaround %actor% %actor.name% ������� �� �����.
* end
*  mat 25065 mechoaround %actor% �������� ��� � ����� � ������ �����.
*  wait 1s
*  mat 33027 mecho �������� ������� �����.
 break
 default
 msend %actor% ���-�� �� ��.
 msend %actor% � ������� ����������� �����, � ��� ��������� ���. 
 ���� %amount% %actor.name%
 break	
done
~
#33029
���� ���� ����� �������� � �����~
0 f 100
~
if %random.100% <= 30
if %world.curobjs(562)% < 2
mload obj 562
end
end
~
$~
