* BRusMUD trigger file v1.0
#61400
1� ������� ���������~
0 0 100
~
wait 4
makeuid questor614 %actor.id%
worlds questor614
%actor.setquest(61400)%
� ���� ���, ������, ������ � ������. �� � ��� ��� � ������ ��� �� ����.
wait 3
� ������ ������. �� ������ ������ ������� � ������.
wait 3
�  ����� �� ��� �����. �� ������, ��� �� �������� ������� ����; � ����� ����.
calcuid room61446 61446 room
attach 61410 %room61446.id%
exec 61410 %room61446.id%
attach 61402 61408 mob
wait 3
� ��� ������� ������ �����, ������ �� ���, � ���� ������ �����.
detach 61400 %self.id% 








~
#61401
������������� �������~
0 q0 100
~
 if !(%questor614.id% == %actor.id%) then
   msend %actor% �������� �������� ���������.   
halt 
end
if  !(%killer614.id% == %questor614.id%) then
    msend %actor% �������� ��������.
halt
end
wait 4
say ������� ���� �������� ������ �������. ������� ���� ���������,
say ������ �� ���� ���� � �� ��.
say ��� ���� ���, ������, ���� �������?  �� �� �������, � � ��� ����, ��� ��.
wait 1s
say ������ � ���������� �� �������, ����� ��� � ������������� ��� ��� ����������.
 say � �� ������ �� �������� ����������� �����.
wait 4
say �������� ��� ��� ��������� ������� � ��������.
%actor.gold(+500)%
makeuid questor614 %actor.id%
worlds questor614
calcuid mob61401 61401 mob
attach 61403  %mob61401.id%
detach 61401 %self.id% 









~
#61402
������ �������~
0 f0 100
~
makeuid killer614 %actor.id%
calcuid questor 61408 mob
worlds killer614
calcuid slepoy 61400 mob
attach 61401 %slepoy.id%
detach 61402 %self.id%









~
#61403
2� ������ � ���������~
0 q0 100
~
if !(%questor614.id% == %actor.id%) then
   wsend %actor% ������!!!
halt
end
wait   2
emote ������� ����������� ��� ����� ��������� 
say  ������� ����, ��� ������ ����� �������... ���...  ���...
wait 3
say ��� ����� �������� ������� ���� ��������, �� ������ � ���� �� �����������.
wait 2
say  ���� � ���� � ���� ��� ���� �������. ��������� �������� � ������ ����� ����� 
wait 2
say ���������� ��������� �����. �� ������� � ���� ��������, �� ���� ����� � ����������� ����� �������� �� �������.
wait 2
say ��� � ������� ��� ���� ������� ��������� ���� ����� ������.
wait 2
say �� � ������ ������, ��� ��� ��� �������� ������ ������, ������ �����- ����� ���. 
wait 1s
say ����� �������� ��� ������� ��� �������? � �� �� �� ������� �� �������.
attach 61404 %self.id%
detach 61403 %self.id%







~
#61404
2� ������ � ���������(����� 2)~
0 d0 0
������� ������ ��~
 if  !(%questor614.id% == %actor.id%)
   msend %actor% - ���� �� ������? �� � ����� �������������!!
   halt 
end
wait   4
say �������. ������  ����� ���-�� � ����� �����. �� ������ ����� �� ������� ����.
wait 2
say ���� � ������� ��� ������ �� �������. ��� ����� ���������� �����.
wait 2
say ����� ��� ��������� ���� ���� ��� ��� ����� ��� ��� ����, � ������ �������� � ������� ������.
wait 2
say �� ���� ��������� ����� ������� � ����, ����� ��� �����, �� � ��������� � ����������� 
wait 2
say ������ �� �������, ��� ��� ����� ��� � �����.
wait 2
say ������� ����� ���������� �� ������, �� �� ������ �� �����.
wait 2
say ������ ������ �� ���������� �������, � ����� ��� ��������� � ������, �� ������ ���� ��� 
wait 2
say ���� � ������� � ����� ����� ������.
wait 2
say ���� ��� ������ �� ���� �� ���, ���� ��� �������� � - ������ ���� ����� ������� ����.
wait 2
say ���� ������ ��� ������� ������...
wait 2
say �� �����, ����� ����. � ���� ����� ���� � ������ � �����.
wait 1s
calcuid mob61411 61411 mob
attach 61405 %mob61411.id%
calcuid room61447 61447 room
attach 61411 %room61447.id%
exec  61411 %room61447.id%
mecho �������� ��������, ������ ����� ���������, ����� �� ������.
mpurge ������.���
detach 61404 %self.id% 











~
#61405
������ � ��������~
0 q0 100
~
 if !(%questor614.id% == %actor.id%))
   halt 
end
� �� ���-�� ����� ����?
attach 61406 %self.id%
detach 61405 %self.id%









~
#61406
�������� � ���������(1)~
0 d0 0
�� ������� ���~

 if  !(%questor614.id% == %actor.id%)
   msend %actor% - ���� �� ������? �� � ����� �������������!!
   halt 
end
say  � ��� �� �����? ������ ������ ���-��?
say ����� ���������� ����� �����...
wait 2
say � ����� �� 100 ��� � ����� ����� ����� �� ����� ���� ������.
wait 2
attach 61407 %self.id%
detach 61406 %self.id%









~
#61407
�������� � ���������(2)~
0 m0 1
~
if !(%questor614.id% == %actor.id%)
   msend %actor% - ������� ���� ������� ����������, ������ �� �������.
   halt 
end
if  %amount% < 100 then
switch %random.6% 
case 1
���
� ������ ������ - ������ ������.
mecho �������� ���������� � �������� ��������� ������.
break
case 2
���� ����������
� ����� �� ���� ������ ���� �� ������.
mecho �������� ���������� � �������� ��������� ������.
break
case 3
����
� �� �������, ������ ��� ������.
mecho �������� ���������� � �������� ��������� ������.
break
case 4
mecho ������� ����� ������������
� �... ���... ��� ... ���... "��.. � ...."
mecho �������� ���������� � �������� ��������� ������.
break
case 5
� ������ �����, ����� ������.
mecho �������� ���������� � �������� ��������� ������.
break
case 6
� ����� ���� ��� � ����� ������?
mecho �������� ���������� � �������� ��������� ������.
break
else
 if %amount% > 99 then
 switch %random.2% 
case 1
� ������� ���� � ������ �� ����, ��������� �� �� ���������.
� ����� � ���� ��� �� ������� ������
� ��� ������� �� ������� ���������, ��� ����� ����� ����� ��� �����.
mload obj 61414
���� ������� %actor.name%
calcuid room61460 61460 room
attach 61408 %room61460.id%
wsend %actor% %room61460.id%
attach 61408 %room61460.name%
detach 61407 %self.id%
break
case 2
� ���������� - �������� ����. ���  ������� �������� ������ ����������, ��� ������ �����.
� ������� �� �� ��� �� ������ ����� � ����� ��� 100 ���.
halt
break
 end
end


~
#61408
������ �������~
2 c0 0
������ ��������� �������� �������������~
if !(%arg.contains(�������)) then
wsend %actor.name% ��� �� ������ ���������������� ?  
 return 1
   halt
end
 if  !(%questor614.id% == %actor.id%) then
   msend %actor% �� ������ ������ ������ ����� �� �������, ��  ������ ������� ��-�� �������������� ��������. 
mechoaround %actor% %actor.name% �����%actor.g% ������ ������ ����� �� �������, �� ����%actor.u% ��
mechoaround %actor% ������� ����� � ���������� ��� �� ����%actor.q%.
return 1
   halt 
end
wsend %actor.name% �� ���������� ������� �������� � ������ ������ ������.
wait 1s
wforce %actor.name% � �������, �������,
wait 1s
wforce %actor.name% � ������ �� ��� �������,
wait 1s
wforce %actor.name% � � ���� �����.
wait 3s
wecho ������� �������: '�� ��� ��, � �� ��� ������ �� ������.'
wait 4s
wforce %actor.name% � �������, �������
wait 1s
wforce %actor.name% � ������ �� ��� �������
wait 1s
wforce %actor.name% � � ���� �����.
wait 3s
wecho ������� �������: '������� �� ����, ���� ��������.'
wait 1s
wecho ������� �������: '� ������ ���� ��� �����, �����-�� �������� �� ������ ���� �������?'
wait 4s
wforce %actor.name% � �������, �������,
wait 1s
wforce %actor.name% � ������ �� ��� �������,
wait 1s
wforce %actor.name% � � ���� �����.
wait 3s
wecho ������� �������: '�� �� - ���� �������!'
wecho ������� �������: '�� ����� ��� �� � ����.'
wait 1s
wecho ������� ������ ������� ��������������.
wait 2
wforce %actor.name% ���� ������.�����.����
wecho ������� ����� �� ����� � ���������� � �����. 
wpurge ������.�����.����
wpurge 61414
wait 1s

wdoor  61460  east room 61461 
wdoor 61461 west room 61440

detach 61408 %self.id%







~
#61409
������� �� ����~
2 c0 0
�������� ��������� ��������~
if !%arg.contains(����)
wsend %actor.name% ��� �� ������ ��� ��������� ?  
 return 1
   halt
end
wsend %actor% ����� � ���� �� ������, ������ �� � �������� �������, �� ��� �� ���� �������?
wechoaround %actor% %actor.name% ����������� �������%actor.g% ����.









~
#61410
���� �������~
2 0 100
~
wload mob 61408










~
#61411
RE���� ���������~
2 0 100
~
wait 2s
calcuid mob61401 61401 mob
wload mob 61401
wait 2s
attach 61427 %mob61401.id%
detach 61400 %mob61401.id%
detach  61411 %self.id%







~
#61412
����� ����~
2 f0 100
~
unset questor614
unset killer614








~
#61413
������ ������~
0 q0 100
~
if  !(%questor614.id% == %actor.id%)
   attach 61400 %self.id%
exec 61400 %self.id%
 detach 61413 %self.id%
end
wait 3
����� %questor614.name%
attach 61427 %self.id%
detach 61413 %self.id%






~
#61414
������ ���� ���~
0 f0 100
70~
calcuid room61464 61464 room
attach 61415 %room61464.id%
exec 61415 %room61464.id%
detach 61414 %self.id%







~
#61415
������ ���~
2 0 100
~
wait 2s
wecho ���� ���� ��� ������ �����, ������ ��� ��� �� ������.
wait 1s
wload mob 61417
mload mob 61417
wecho  ���� ��� ������������ � ������ ����������� �� �����������.
wait 3
wecho ��� �������� ���!
wait 2
wecho ���� ��� ������ ����� � ������ ���������� �� ���.
wait 4
wecho ���� ��� ������ ����� ����� ������ � ������
wait 2
wecho ���� ��� �������: '�������� ����, � ��� ��� ��� ��� ������! ��������� ������ ��������!
calcuid mob61417 61417 mob
attach 61416 %mob61417.id%
exec 61416 %mob61417.id%
detach 61415 %self.id%







~
#61416
����������� ���~
0 0 100
~
wait 1s
�� ����� �����������
wait 2
� ���� � �������� ������.
wait 4
� ��� ������� ��������� �����, � ���� � ��� �� �������, � ���������:-)
wait 4
� ���� ��������� ����, �� � ������ ���� ��� �����.
wait 2
������
attach 61417 %self.id%
detach 61416 %self.id%  







~
#61417
����������� ���(2)~
0 d0 0
���� ������ ������ ���������~
wait 3
msend %actor%  ��� �������� ��� �� ���� � �������� �� ����� �� ����� �� �������.
mechoaround %actor% ���� ��� �������� %actor.rname% �� ���� � �������� �� ����� � ������.
wait 1s
mecho �� ������������ ������ ������ ���� ����� �� ����� ����.
wait 6 
mecho �� ����� �� ��������� ������ �������.
mteleport all 61477
calcuid room61477 61477 room
attach 61418 %room61477.id%
exec 61418 %room61477.id%
mpurge ����.���
detach 61417 %self.id%






~
#61418
(!!!� ������� ��� � �����!!!)~
2 0 100
~
wload mob 61417
calcuid mob61417 61417 mob
attach 61419 %mob61417.id%
exec 61419 %mob61417.id%
detach 61418 %self.id%







~
#61419
��������� ����� ���~
0 0 0
~
wait 1s
� ��� �� ������� ���������.
wait 3
� ����� ��������� � ������, �� ��� �� ����� � ������� � ����� �������.
mecho ���� ��� ������ ������� �������� � ���� ����.
mpurge ����.���
detach 61418 %self.id%







~
#61420
����� � ����~
2 c0 0
����� �������~
if !%arg.contains(����)%
wsend %actor.name% ���� �� ������ ������� ?  
 return 1
   halt
end
wait 5
wsend   %actor.name% �� ������� �������� ���� � ������� ������.
wechoaround %actor% %actor.name% ������%actor.g% �������� ���� � ������ �����%actor.q% � ����.
wait 1s
wteleport %actor% 61462






~
#61421
����� � ����~
2 c0 0
������� ~
if !%arg.contains(����)%
wsend %actor.name% ��� �� ������ ������� ?  
 return 1
   halt
end
wait 4
wsend   %actor.name% �� ������ ��������� ����� � ������� ����� �� ������� ����� ��������� � ������� ���.
wechoaround %actor% %actor.name% � ����� ������������� �����%actor.g% ������� ����.
wait 4
wechoaround %actor% %actor.name% ��������%actor.g%, ��� ������ ����� ������ � ...
wsend   %actor.name% ����� ����� �� �������� ��������� ��� � � ������ �������� ���������� ���.
wechoaround %actor% ... �������%actor.g% ���������� ���.
wdoor 61452 down room 61470 
wdoor 61470 up room 61452
detach 61421 %self.id%






~
#61422
������� �������~
0 q0 100
~
wait 4
��� %actor.name%

~
#61423
����� ��������~
0 r0 100
~
mload mob 61424
mload mob 61424
wait 3
� ���� ���� !! ��� �� ��������?!?
wait 3
� �� ��� ���� ��������� ������ �������?!!
wait 2
� ������! ������ ! �������� ����.
wait 2
mecho �������� �������� �� ����� ����������.
wait 1
mecho �������� �������� �� ����� ����������.
detach 61423 %self.id%

~
#61424
������� ������~
0 q0 100
~
wait 2
� ����, ��� ����?!?
wait 3
� ����������� �����, ���������� ������ ��� �� ����������.




~
#61425
��������~
0 b0 100
~
wait 3
�� ������ ����� � ����� ������������� ����� ��� ����.






~
#61426
������ �����~
0 b0 100
~
� �����! ���� �����.
���






~
#61427
������ ������~
0 j0 100
~
if !(%object.vnum%==61400) then
wait 4
� ����� ��� ���??
wait 2
�  � � ���� ����� ��������.
wait 2
���
halt
end
if %object.vnum%==61400 then
  if !(%questor614.id% == %actor.id%) then
   msend %actor% �������� �������� ���������.   
wait 1s
� ������� ���� ������ �������.
wait 3
mecho �������� �������� ��������� ���� ��� �� ��������� �����.
wait 4
mecho ����� �������� ���������� � ����� � ��� �� ������ � ���� ������� ����.
wait 3
mecho �������� ����� �� ���� � ��������.
wait 3
mecho "��������" �������� ������� �������� ����� ������� �����.
wait 4
mecho ������ �������� � ��������� ������ � �����.
wait 4
mecho "������" �������� ������� � ���������� �� �������.
halt 
end
wait 3
say �� ����� ���� !!
wait 4
�  � �� ���� ����� ������.
wait 1
����� %questor614.name%
wait 4
� �� ������ ��� ?!?!!
wait 4
� ������ ��������, ��� �� ��� ������ ...
wait 2
mecho �������� �������� ��������� ���� ��� �� ��������� �����.
wait 1s
� �-�...
wait 3
� ������ �� ������� ���� ��������!!!?!!!
wait 4
� ��� ��� �� ��� ��������, ��� ��� ����� ����???!!!
wait 3
mecho ����� �������� ���������� � ����� � ��� �� ������ � ���� ������� ����.
wait 4
mecho �������� ����� �� ���� � ��������.
wait 3
mecho �������� ���������� ���������.
wait 5
� ������, ��� �� ������� ���� %questor614.name%, ������� ���� ��������.
wait 4
mecho "��������" �������� ������� �������� ����� ������� �����.
wait 4
mecho ������ �������� � ��������� ������ � �����.
wait 3
mecho "������" �������� ������� � ���������� �� �������.
wait 4
� ������ ���� ������� %questor614.name%, � �� ���� �� ������ �����-������ � ������ ���� ������������� ����.
wait 5
�  ������ ���������� ��� ���� ���������� � ���� ������������� � ��������.
wait 2
%self.gold(+2000)%
���� 2000 ��� %questor614.name%
end
detach 61427 %self.id%

~
#61428
����� ����~
2 c0 0
������� �������~
if !%arg.contains(����� ����)%
wsend %actor.name% ��� �� ������ ������� ?  
 return 1
   halt
end
wait 4
wsend %actor% ����� �� ������� �������� �� ������ � ������� ����� � �������� ������� �� ��� �� ������� ������ ����� ����� ����.
wechoaround %actor% %actor.name% ������%actor.g% ����� ���� �� ������� �������.
wload obj 61400
wforce %actor.name% ����� �����.






~
#61429
������~
0 b0 100
~
wait 4
� ��� - ��� !






~
#61430
����� � ����(� ���� �����)~
2 c0 0
����� ������� ��������~
if !%arg.contains(����)%
wsend %actor.name% ���� �� ������ ������� ?  
 return 1
   halt
end
wait 5
wsend   %actor.name% �� ���������� �� ���� ���������� ���� � ����� �������� �����.
wechoaround %actor% %actor.name% �������%actor.u% �� ���� ���������� ���� � ������%actor.u% ������.
wait 1s
wteleport %actor% 61462



~
#61431
����� �� ����~
2 c0 0
������� ����� ������~
if !%arg.contains(�����)%
wsend %actor.name% ���� �� ������ ������� ?  
 return 1
   halt
end
wait 5
wsend   %actor.name% �� ������� �������� ���� � ������� ������.
wait 1s
wteleport %actor% 61463
wechoaround %actor% %actor.name% ������%actor.g% �������� ���� � ������ �����%actor.q% �� ����.



~
#61432
����� �������~
1 j0 100
~
wait 3
oechoaround %actor% %actor.name% ������%actor.u% ������� �� ������� ������� ������ ������� �� ��� ������.
osend %actor% ������ ������ ����������� � ��� �� ������, ������ ��� ��� ������.



~
#61433
������� �� ����~
2 c0 0
�������� ���������~
if !%arg.contains(����)%
wsend %actor.name% ���� �� ������ ��������� ?  
 return 1
   halt
end
wait 5
wsend   %actor.name% �� ��������� ����.
wait 1s
wteleport %actor% 61475
wechoaround %actor% %actor.name% �������%actor.u% ������.



~
#61434
����� ������~
0 b0q0 100
~
wait 4
���



~
#61435
����������� ������ �������~
0 q0 100
~
makeuid proklyatiy614 %actor.id%
worlds proklyatiy614 
attach 61437 %proklyatiy614.id%
mecho �����614    %proklyatiy614.id%
mecho  �����     %actor.id%
wait 2
� ������ ���� ������ �������.
wait 1
� ������ ��� �������, ������ � ���� � ���������.
wait 2
� ������ ���, ������ � �� �����.
����
���
� �� ������ � ������, ��� ��� 50 ���, ���� � ���� � ���� ������.
attach 61436 %self.id%
detach 61435 %self.id%

~
#61436
���� ����� �������~
0 m0 1
~
if  %amount% > 49  then
wait 3
� ������� �����, ������ ������ ����� �������, � � ����� �����.
mecho ������ �������� �����.
detach 61437 %actor.id%
dg_cast '����' %actor.name% 
dg_cast '������' %actor.name% 
dg_cast '�����' %actor.name% 
����
unset proklyatiy614
attach 61435 %self.id%
detach 61436 %self.id%
else
if  %amount% < 50  then
wait 2
���
wait 3
� ���������� ��� �������?!? 
wait 2
� �� ���� �� �������, ����� �� ���!
attach 61437 %actor.id%
attach 61435 %self.id%
detach 61436 %self.id%
end
end

~
#61437
���������(�� ����������)~
0 b0 100
~
switch %random.6%
case 1
wait 20s
mechoaround %self.id% %self.name% ������������%self.u% � ����%self.g%.
� ���� ������!!!
 �����
break
case2
wait 20s
� �� ����� �� ��� �� ���� !!!
 ������
 ������
 ������
break
case 3
 wait 20s
���
break
case 4
wait 20s
���
�����
����
break
case 5
wait 20s
����� ����
break
case 6
wait 20s
����� ���
break
end

~
#61438
������ �������~
0 f0 100
~
detach 61437 %actor.id%


~
#61439
��� � �����~
2 c0 100
����������� ����������~
wait 5
wsend %actor% � ����� ����������� ���������� ��� �� ����������.
wsend %actor% �� ����� ������������ ������ ���.
wsend %actor% �� ������ � ��� �� ����� ����� ����, � �� ����� ��� ������� ������������ �������.
wsend %actor% �� ��� ����� ���������, � �� ��� ������ �������, �������������� �� ��.

~
#61440
������ �������~
0 f0 100
~
if (%random.100% < 20) && (%world.curobjs(61403)% < 5)  then
mload obj 61403
end

~
#61441
������ �����~
0 f0 100
~
if (%random.100% < 15) && (%world.curobjs(61412)% < 3)  then
mload obj 61412
end

~
#61442
������ �����~
0 f0 100
~

if (%random.100% < 50) && (%world.curobjs(61405)% < 8)  then
mload obj 61405
end

~
#61443
������ ����~
0 f0 100
~
if (%random.100% < 25) && (%world.curobjs(61407)% < 5)  then
mload obj 61407
end

~
#61444
������ ������~
0 f0 100
~
if (%random.100% < 30) && (%world.curobjs(61415)% < 5)  then
mload obj 61415
end
if (%random.100% < 10) && (%world.curobjs(61419)% < 3)  then
mload obj 61419
end

~
#61445
������ ������~
0 f0 100
~
if (%random.100% < 40) && (%world.curobjs(61411)% < 6)  then
mload obj 61411
end

~
#61446
���� ��������~
0 n0 100
~
if (%random.100% < 50) && (%world.curobjs(61409)% < 5)  then
mload obj 61409
end
if (%random.100% < 15) && (%world.curobjs(61416)% < 5)  then
mload obj 61416
end
if (%random.100% < 15) && (%world.curobjs(61413)% < 5)  then
mload obj 61413
end

~
#61447
���� ���������~
0 n0 100
~
if (%random.100% < 10) && (%world.curobjs(61406)% < 5)  then
mload obj 61406
end

~
#61448
���� ������~
0 n0 100
~
if (%random.100% < 20) && (%world.curobjs(61420)% < 5)  then
mload obj 61420
end

~
#61449
���� ����������~
0 n0 100
~
if (%random.100% < 15) && (%world.curobjs(61410)% < 5)  then
mload obj 61410
end

~
#61450
���� �������~
0 n0 100
~
if (%random.100% < 10) && (%world.curobjs(61408)% < 3)  then
mload obj 61408
end

~
#61451
���� ������~
0 0 100
~

if (%random.100% < 30) && (%world.curobjs(61417)% < 10)  then
mload obj 61417
end

~
#61452
���� �������~
0 n0 100
~
if (%random.100% < 20) && (%world.curobjs(61404)% < 6)  then
mload obj 61404
end

~
#61453
���� ���~
0 n0 100
~
if (%random.100% < 15) && (%world.curobjs(61418)% < 5)  then
mload obj 61418
end

~
#61454
���� �������~
0 n0 100
~
if (%random.100% < 5) && (%world.curobjs(61421)% < 5)  then
mload obj 61421
end

~
$
$
