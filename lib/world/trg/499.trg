#49900
�����������~
2 e 100
~
wait 1
wsend %actor% ����� ������ ����:
wsend %actor% - ������ ����, %actor.name%!
wsend %actor% - � ���� ��������� ���� � ����� �������, ����������� �����������.
wsend %actor% - ��������� ��� 1050 ��� - � ��������� ��� � ��������� ���.
~
#49901
���� ��������� � ��������� ���������~
2 d 1
�� ������� ���~
wait 1
wsend %actor% ����� ������ ����:
*wsend %actor% - H� ��� � ���������� ������� ����� ��� 200 ���.
wsend %actor% - � ������� ����� �������, �������� ������, ������� ��������� ��������� ��� ��� ������ ������, ��� ��� ����� 1050 ���.
~
#49902
���������� � ����~
2 z 0
~
wait 1
switch %amount%
*case 200
*wecho ����� ����� ������ ����������, ������� ������ � ����� �������� ����.
*wechoaround %actor% %actor.name% ����� � ��������� �����������.
*wsend %actor% ��� ���������� �������� � �� �� ��� �������� ��������.
*wteleport %actor% 90047 horse
*wat 90047 wechoaround %actor% %actor.name% ������%actor.u% ��� � ������ ����.
*break
 case 1050
  wecho ����� ����� ������ ����������, ������� ������ � ����� �������� ����.
  wechoaround %actor% %actor.name% ����� � ��������� �����������.
  wsend %actor% ��� ���������� �������� � �� �� ��� �������� ��������.
  eval rand 27000+%random.60%
  wteleport %actor% %rand% horse
  wat %rand% wechoaround %actor% %actor.name% ������%actor.u% ��� � ������ ����.
  unset rand
 break
 default
 wsend %actor% ����� ������ ����:
 wsend %actor% - ������� �� ������, �� � �� ����, ���� ���� ����������� �� ����� �����. 
 wsend %actor% - H� ���� �� �����... ���� ��� �������.
 break	
done
~
#49903
������ ������������� ��������~
0 m 0
~
calcuid roomrun 49934 room
run 49902 %roomrun.id%
~
#49904
���� ����~
0 z 100
~
mecho ������������ ��������, ������ ����������� � ������.
mpurge %self%
~
#49905
� ������� ������� ����~
0 z 100
~
wait 1
if %actor.vnum% == 34632
mshou �������� ��� �����! ���� � �����....
end
wait 1
if %actor.vnum% == 34631
mshou  �������! ���� � ���������! ������...
end
~
#49906
���� ��������~
2 g 100
~
wait 1
if %actor.vnum% == 34632
calcuid ug34632r 34632 mob
while  %ug34632r%
exec 34607 %ug34632r.id%
eval ug34632r 
calcuid ug34632r  34632 mob
done
wecho ���� ���������� � ������ �������.
end
wait 1
if %actor.vnum% == 34631
calcuid ug34631r 34631 mob
while  %ug34631r%
exec 34607 %ug34631r.id%
eval ug34631r 
calcuid ug34631r  34631 mob
done
wecho ���� ���������� � ������ �������.
end
wait 1
if %actor.vnum% == 34630
calcuid ug34630r 34630 mob
while  %ug34630r%
exec 34607 %ug34630r.id%
eval ug34630r 
calcuid ug34630r  34630 mob
done
wecho ���� ���������� � ������ �������.
end
wait 1
if %actor.vnum% == 34629
calcuid ug34629r 34629 mob
while  %ug34629r%
exec 34607 %ug34629r.id%
eval ug34629r 
calcuid ug34629r  34629 mob
done
end
~
#49907
�������� ������������� � ������ ������� (������)~
0 ab 100
~
if ( (%reddragonraid915% == 1) || (%whitedragonraid915% == 1) )
mshou ���� ������� �������� � ����! ����� ������ ����������� ����� ����� � ��� �� �������!
wait 1
mshou ��-��� ����� �������, �� ����� ����� ��������� ����� ����� �� ���� ������.
switch %random.2%
case 1
wait 120s
break
case 2
default
wait 250s
done
end
~
#49908
�������� ������������� � ������ (�������� �������)~
0 ab 100
~
if (%reddragonraid915% == 1)
wait 5s
mshou �� ������ ������ � ����� ��� �����! ������, �������� ����! ��� ��� ����� ������� - �� �����!
switch %random.2%
case 1
wait 60s
break
case 2
default
wait 150s
done
end
~
#49909
�������� ������������� � ������ (������ �������)~
0 ab 100
~
if (%whitedragonraid915% == 1)
wait 5s
mshou �� ������ ������ � ����� ��� �����! ������, �������� ����! ��� ��� ����� ������� - �� �����!
switch %random.2%
case 1
wait 60s
break
case 2
default
wait 150s
done
end
~
#49910
���������������� ����� ����� ������ (!!!)~
2 abf 100
~
if (%whitedragonraid915% == 0) && (%reddragonraid915% == 0) && (%posessedraid814% == 0)
unset raid499
end
~
#49911
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
if (%self.haveobj(60064)% && %self.haveobj(60065)%)
  say �������! ������ � ������� ������ � ������� ��������!
  say � ��� ������� � �������� ��� � �������
  mecho ������� ���-�� ��������� ��� ������������� � �������� �� � �����.
  ���� ���
  mteleport all 27016
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
#49912
����������� �������~
0 r 100
~
wait 1
msend %actor% ������� ������� ����:
msend %actor% - �� ������� ���� ������� �� ������� ���, %actor.name%!
msend %actor% - ����, ��� �� ������ ��� �� ���� ���%actor.y%! ���� ���� ������!
msend %actor% - ���� �� ���� ��� ������� ��� ��� ������ �����
msend %actor% - � ������ ���� ����� ��������� �� ������� �������� ������.
~
#49913
���� ������ �������~
0 m 1
~
 if (%amount% < 200)
say �� ����� ����� ���%actor.g% ����� ���������. � � �� ������!
halt
end
wait 1
msend %actor% - ������� ������� ����:
msend %actor% - ���� � ������� ������ - ������� ��� ������� � ������.
msend %actor% - ���� �������� ������� � ����� ��, ����� ��������� ���� � �����.
msend %actor% - � �������� � ����������� ������� - ��� ��� ������ � �����.
~
#49914
���������� �����~
0 qr 50
~
wait 1
context 915
if %target1%
set target2 %actor%
worlds target2
elseif %target2%
set target3 %actor%
worlds target3
elseif %target3%
set target4 %actor%
worlds target4
elseif %target4%
set target5 %actor%
worlds target5
else %target2%
set target1 %actor%
worlds target1
end
if %actor.id% == %agressor.id%
halt
end
context 0
wait 1
��
follow .%actor.name%
set victim %actor%
global victim
~
#49915
������� �������~
0 k 70
~
if %actor.vnum% != -1
set agressor %actor.leader%
global agressor
else
set agressor %actor%
global agressor
end
wait 4
follow �
flee
flee
~
#49916
������ ���������~
0 b 9
~
switch %random.5%
case 1
if %victim.sex% == 1
say ��������, ����� �������, �� ��������, ����� �������! 
else
say ��������, ����� �������, �� ��������, ����� �������!
end
break
case 2
say �� �������������! � �����-�� ����������� ���� ����!
break
case 3
say �� �� ��������! ��� ����� ����! ��� 100 ������� ��������!
break
case 4
emot ������������� ��������� �� ��� � ����������� ��������
break
case 5
say �� �� ������, ���� ��� ���������� ����� �� ���, ��� ����������� ������ �������?
������
break
done
����� ��� �����
����� ��� �����
����� ��� �����
����� ��� ����
����� ��� ����
����� ��� �����
��� ��� �����
~
#49917
�� ���� � ������� ��������~
0 q 100
~
wait 1
���
say ���������, ���� ������, ���� � ������� �������� ��������� ������� ������!
emot ����������� ��������� ��� ������
~
#49918
���� ������� ������ ����~
0 j 100
~
wait 1
if %object.vnum% == 91517
 say ����� �� ��� ��� ������%actor.q%?
 ������� %object.name%
 halt
elseif %object.vnum%  == 91522
remote actor %self.id%
remote object %self.id%
exec 91536 %self.id%
wait 1s
halt
 end
wait 1
mpurge %object%
say ��...!
say �� �������, ���������� � ������� �������.
say ��� ���� � �������.
switch %random.6%
case 2
mload obj 1170
give ������ %actor.name%
shou �� ������ ��� �����, ��������� � %actor.name% ������� �������!
break
case 3
mload obj 1171
give ������ %actor.name%
shou �� ������ ��� �����, ��������� � %actor.name% ���������� �������!
break
case 4
mload obj 1172
give ������ %actor.name%
shou �� ������ ��� �����, ��������� � %actor.name% ��������� �������!
break
default
eval buffer %self.gold(+5000)%
give 5000 ��� %actor.name%
shou �� ������ ��� �����, ��������� � %actor.name% �������!
done
~
$~
