* BRusMUD trigger file v1.0
#81000
�������-������ ����������� ������~
0 q0 10
~
wait 1
msend %actor% _������� ������������ ��������� �� ���.

~
#81001
������� ������������ ������~
0 q0 100
~
wait 1
if !%exist.mob(81002)%
halt
end
wait 3
say ������ ���� %actor.iname%!
mecho _- ���� �� �����%actor.g% ����� ������, � �����.
mecho _- �� ������� �� �� ��� ��������� ������?
mecho _- � � �� �� ������ �� ��������.
������ %actor.iname% 

~
#81002
������� ���� ����� ������~
0 d0 1
�� ������ ������~
wait 3
detach 81001 %self.id%
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
attach 81003 %self.id%
detach 81002 %self.id%

~
#81003
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
mload obj 81010
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
detach 81003 %self.id%
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
#81004
�����~
2 f0 100
~
calcuid from 81009 mob
detach 81001 %from.id%
attach 81001 %from.id%
detach 81002 %from.id% 
detach 81003 %from.id%
attach 81003 %from.id%

~
#81005
����� � ������� ����~
0 q0 100
~
wait 1
msend %actor% _������ ��� ������ ������� � �������� �� ���!
mkill %actor%
wait 2
mechoaround %actor% _������ ��� ������ ������� � ���������� �� %actor.rname%!  

~
#81006
����� ������������~
0 q0 15
~
wait 1
������ %actor.iname%
msend %actor% _- ���������� %actor.iname%!
msend %actor% _- ������� � ������� ���, ��� ������ ���������!

~
#81007
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
#81008
����� ������� ������ ������~
0 j0 100
~
wait 5
switch %object.vnum%
  case 700
    msend %actor% _- ��� �����. ���� ����� �� ���� ���� ������� �� ����� � ������!
    mload obj 81014
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
 
  case 701
    msend %actor% _- ��� �����. ���� ����� ��������� ���� ���������.!
    mload obj 81011
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
 
  case 705
    msend %actor% _- ��� �����. ���� ����� �� ���� ���� ������� �� ����� � ������!
    mload obj 81014
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
  
  case 751
    msend %actor% _- ��� �����. ���� ����� ������� ���� �� ������!
    mload obj 81012
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
  
  case 752
    msend %actor% _- ��� �����. ���� ����� ������� ����, ���� �� ���������.
    mload obj 81013
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
         
  case 754
    msend %actor% _- ��� �����. ���� ����� ������� ����.
    mload obj 81015
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
  
  case 755
    msend %actor% _- ��� �����. ���� ����� �������� ���� ������!
    mload obj 81016
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
  
  case 756
    msend %actor% _- ��� �����. ���� ����� ������� ���� � ������� ������.
    mload obj 81017
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break   
  
  case 757
    msend %actor% _- ��� �����. ���� ����� �������� ���� �������� �� ���� ������ �����!
    mload obj 81020
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break            
  
  case 758
    msend %actor% _- ��� �����. ���� ����� ������� ���� ���������� ��������.
    mload obj 81018
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break  
  
  case 721
    msend %actor% _- ��� �����. ���� ����� ������ � ���� ��������� �����!
    mload obj 81019
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break 
  
  case 722
    msend %actor% _- ��� �����. ���� ����� ������� ����, ���� �� ���������.
    mload obj 81013
    ���� ����� %actor.iname%
    mpurge %object.iname%
  break   
  
  case 724
    msend %actor% _- ��� �����. ���� ����� ������� ���� �� ��������!
    mload obj 81012
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
#81009
� ��������� ���~
2 e0 25
~
wait 1
wecho _�������� �� ����������� ���������� ����� ������ ������� ������!
wecho _�� ���-��� ������!

~
#81010
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
#81011
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
#81012
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
#81013
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
#81014
����� ������ � ����� ����������~
2 e0 20
~
wait 1
wecho _�� ����� ���������� ��������� ������� �����.

~
#81015
��������� ����� �������~
0 g0 20
~
wait 1
msend %actor% _- ��! ��! ����� ������� ������� �� ��� ���������!
msend %actor% _- ������, �������, ���� ���� ������.

~
#81016
������ ������ ���������~
0 q0t0 15
~
wait 1
msend %actor% _������ ������ ����������� ��������� �� ���.
msend %actor% _������ �� �� ����� ������� ���, ��� �� ������ �� ��� ����.
say � ������!

~
#81017
������������ ������� � ����� �����~
2 e0 15
~
wait 1
msend %actor% _�������, ������� �� �������� � ����� ������� ����������.

~
#81018
������������ ������ �� ������~
1 t0 10
~
wait 1
msend %actor% _������ ����, ��������� �� ������� ����������� �� ������.

~
#81019
��������� ������ 1 ����~
0 r0t0 20
~
wait 5
msend %actor% _- ����� ����!
msend %actor% _- �� ����� ����!
msend %actor% _- �� ��� ���, ����� ��� �� ?!

~
#81020
���� ��������� 1 ����~
0 m0 1
~
wait 1
msend %actor% _- ������� ���, ������ �������!

~
#81021
������� ���� �����~
0 r0t0 15
~
wait 1
����� %actor.iname%
msend %actor% _- �� ��! ������� ������� �� ��� � �����. ������ �� ������� �����!

~
#81022
������ ������ �� ����~
2 e0 20
~
wait 1
msend %actor% _����� � ��� ��� ������ ������ ������������ ����������.

~
$
$
