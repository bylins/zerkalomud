* BRusMUD trigger file v1.0
#104000
�������-������ ����������� ������~
0 q0 10
~
wait 1
msend %actor% _������� ������������ ��������� �� ���.



~
#104001
������� ������������ ������~
0 q0 100
~
wait 1
if !%exist.mob(104002)%
halt
end
wait 3
say ������ ���� %actor.iname%!
mecho _- ���� �� �����%actor.g% ����� ������, � �����.
mecho _- �� ������� �� �� ��� ��������� ������?
mecho _- � � �� �� ������ �� ��������.
������ %actor.iname% 



~
#104002
������� ���� ����� ������~
0 d0 1
�� ������ ������~
wait 3
detach 104001 %self.id%
say ��� �������!
mecho _- ��� ����� ������ �����.
wait 8
mecho _- �����, � ������ ������� ������ ������� ���!
mecho _- �� ����� ������� � ���� ������, ������ ������� ���
mecho _- �� �� ������� ����� �������.
mecho _- ��� ������� �� ���� ������ �������� ���������� ������
mecho _- � ��� ��� ��������� ������� ����� � ����� ��� ��������!
wait 4
say  �� ���� ����� ��������, �������� ����� ��������� ������.
mecho _- � ����� � �� ���� ���������.
wait 10
say _- � ������� ����, ����� �������� �� �����.
mecho _- � ��� ���� � ����� ��� ������ �� �����!
mecho _- ���� �� ������� ��� � ������, ������� ��� ��� ����
mecho _- � � ���������� ����.
���� %actor.iname%
set quester %actor%
global quester
attach 104003 %self.id%
detach 104002 %self.id%



~
#104003
������� ��������� �����~
0 j0 100
~
wait 1
if %quester.id% == %actor.id%
if %object.iname% == ����.�������.��������.����
wait 1
mpurge %object%
wait 10 
say ��� �������!
mecho _- �������%actor.g% ����� ������� �����.
mecho _- ������ �� �� ��� �� ���� �� �����������.
mecho _- �� � ����� ����� �� ������ ���������!
wait 10
say _- ������� ����.
mecho _- � ���, ����� ��������� �������.
switch %random.3%
case 1
wait 10
msend %actor% _������ ������� ��������� ��������� ���� ����, ���� � ���� �����.
mload obj 104010
���� ���� %actor.iname%
break
default
mecho _������ ������� ������ ��������� ������� ��� �� �������� ������.
mechoaround %actor% _������ ������� ��� %actor.dname% ��������� ����� ���.
msend %actor% _������ ������� ��� ��� ��������� ����� ���.
msend %actor% _��� ��������� 100 ���.
%actor.gold(+100)%       
break
done
detach 104003 %self.id%
else              
wait 10
say _- ��� �� ������ �� ��, ��� ��� ����.
mecho _- ��� ��� ������� ���� ��� �������.
���� %object.name% %actor.name%
end
else
drop %object.name%
end 



~
#104004
�����~
2 f0 100
~
calcuid from 104009 mob
detach 104001 %from.id%
attach 104001 %from.id%
detach 104002 %from.id% 
detach 104003 %from.id%
attach 104003 %from.id%



~
#104005
����� � ������� ����~
0 q0 100
~
wait 1
msend %actor% _������ ��� ������ ������� � �������� �� ���!
mkill %actor%
wait 2
mechoaround %actor% _������ ��� ������ ������� � ���������� �� %actor.rname%!  



~
#104006
����� ������������~
0 q0 15
~
wait 1
������ %actor.iname%
msend %actor% _- ���������� %actor.iname%!
msend %actor% _- ������� � ������� ���, ��� ������ ���������!



~
#104007
����� ������� ������������~
0 r0 100
~
wait 1
�������
wait 10
say - ���������� ��������������!
mecho _- � ������� - ������������� ������.
wait 5
mecho  _- ����� ��� � ����������� �� ���� � ������ ��������
mecho  _- ������ ���������� ������������.
wait 10
mecho  _- � �����, ��� ������ �� ��� ����� �������, 
mecho  _- � ����� ������ ������, ��� ����� ��� �� �����������.
wait 10
mecho  _- ���� �� ������� �����-���� �����������, ������� �� ���.
mecho  _- � � �������� ������ ���� �� ��� ������.



~
#104008
����� ������� ������ ������~
0 j0 100
~
wait 5
switch %object.vnum%
  case 700
    msend %actor% _- ��� �����. ���� ����� �� ���� ���� ������� �� ����� � ������!
    mload obj 104014
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
 
  case 701
    msend %actor% _- ��� �����. ���� ����� ��������� ���� ���������.!
    mload obj 104011
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
 
  case 705
    msend %actor% _- ��� �����. ���� ����� �� ���� ���� ������� �� ����� � ������!
    mload obj 104014
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
  
  case 751
    msend %actor% _- ��� �����. ���� ����� ������� ���� �� ������!
    mload obj 104012
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
  
  case 752
    msend %actor% _- ��� �����. ���� ����� ������� ����, ���� �� ���������.
    mload obj 104013
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
         
  case 754
    msend %actor% _- ��� �����. ���� ����� ������� ����.
    mload obj 104015
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
  
  case 755
    msend %actor% _- ��� �����. ���� ����� �������� ���� ������!
    mload obj 104016
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
  
  case 756
    msend %actor% _- ��� �����. ���� ����� ������� ���� � ������� ������.
    mload obj 104017
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break   
  
  case 757
    msend %actor% _- ��� �����. ���� ����� �������� ���� �������� �� ���� ������ �����!
    mload obj 104020
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break            
  
  case 758
    msend %actor% _- ��� �����. ���� ����� ������� ���� ���������� ��������.
    mload obj 104018
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
  
  case 721
    msend %actor% _- ��� �����. ���� ����� ������ � ���� ��������� �����!
    mload obj 104019
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
  
  case 722
    msend %actor% _- ��� �����. ���� ����� ������� ����, ���� �� ���������.
    mload obj 104013
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break   
  
  case 724
    msend %actor% _- ��� �����. ���� ����� ������� ���� �� ��������!
    mload obj 104012
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break
  
  default
    msend %actor% _- ����� ��� ���?
    msend %actor% _- �� ����� ������ �� ���������, ��� ��� ������� �������.
    ���� %object.name% %actor.name%
  break
done



~
#104009
� ��������� ���~
2 e0 25
~
wait 1
wecho _�������� �� ����������� ���������� ����� ������ ������� ������!
wecho _�� ���-��� ������!



~
#104010
� ������ ����� ������� �����~
0 q0 100
~
wait 1
if (%actor.level% <8) && (%actor.hitp% < %actor.maxhitp%)
%actor.wait(1)%
say _- ���! ��� � ���� ���?
mecho _- �������-��, � ���� ������.
mecho _����� ������ � ������ ����� � ���� �����-�� �������.
mecho _����� ���� �� ������� � ��� � � ����� ����� �� ���� ������.
mecho _��� ��������� ������� � ��� �������� ��������.
mecho _�� ����� �� ������ � ����, �� �� ����������� �� ���� 
mecho _�� ������ ��������.
dg_cast '���������' %actor.name%
end



~
#104011
����� ������� ������������~
0 q0 100
~
wait 5
msend %actor% _- ����� ��������������.
msend %actor% _- � ���� ��������� ���� � ������ �����.
msend %actor% _- ��� ����� ����� ���������� �� ���� ����������.
msend %actor% _- ���� �� �������������%actor.g% �� ���� ���
msend %actor% _- ��������� ����� � %actor.level% ���.
 



~
#104012
���� ������ ������ ���������~
0 m0 1
~
wait 2
if (%amount% == %actor.level%) || (%amount% > %actor.level%)
msend %actor% _- �� �����. ������ � �������.
msend %actor% _- � �������� �������!
msend %actor% _������� ���� ������ ��� � ������� � �� ��� �� �������� � ����.   
mechoaround %actor% _������� ���� ������ � ������� %actor.dname% � ������ � ����
*����� ������ mteleport XXXX %actor.iname%
else
msend %actor% _- �������� ���-��.
msend %actor% _- �� ������� � ���� �� ���� �� ������!
���� %amount% ��� %actor.iname%
end



~
#104013
����� �� ����� ��� ������~
2 e0 20
~
wait 1
switch %random.2%
case 1
wecho _�� ����� �� ������� ��������� ������ �����.
break
case 2
wecho _�� ����� �� ������� ��������� ������� ������ � ��� �����.
break
done



~
#104014
����� ������ � ����� ����������~
2 e0 20
~
wait 1
wecho _�� ����� ���������� ��������� ������� �����.



~
#104015
��������� ����� �������~
0 g0 20
~
wait 1
msend %actor% _- ��! ��! ����� ������� ������� �� ��� ���������!
msend %actor% _- ������, �������, ���� ���� ������.



~
#104016
������ ������ ���������~
0 q0t0 15
~
wait 1
msend %actor% _������ ������ ����������� ��������� �� ���.
msend %actor% _������ �� �� ����� ������� ���, ��� �� ������ �� ��� ����.
say � ������!



~
#104017
������������ ������� � ����� �����~
2 e0 15
~
wait 1
msend %actor% _�������, ������� �� �������� � ����� ������� ����������.



~
#104018
������������ ������ �� ������~
1 t0 10
~
wait 1
msend %actor% _������ ����, ��������� �� ������� ����������� �� ������.



~
#104019
��������� ������ 1 ����~
0 r0t0 20
~
wait 5
msend %actor% _- ����� ����!
msend %actor% _- �� ����� ����!
msend %actor% _- �� ��� ���, ����� ��� �� ?!



~
#104020
���� ��������� 1 ����~
0 m0 1
~
wait 1
msend %actor% _- ������� ���, ������ �������!



~
#104021
������� ���� �����~
0 r0t0 15
~
wait 1
����� %actor.iname%
msend %actor% _- �� ��! ������� ������� �� ��� � �����. ������ �� ������� �����!



~
#104022
������ ������ �� ����~
2 e0 20
~
wait 1
msend %actor% _����� � ��� ��� ������ ������ ������������ ����������.



~
$
$
