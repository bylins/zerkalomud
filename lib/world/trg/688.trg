#68800
����� ������� � �������~
0 r 100
~
wait 5
msend %actor% _�����:
msend %actor% _- �� ��! 
msend %actor% _- � �� ��� ������.
msend %actor% _- � ���� � ���� ���� ����...
msend %actor% _- ������ ������������.
msend %actor% _- �� ������� ����� �����, �����������.
msend %actor% _- ���� ����� ����� ���������� � ���� ����������.
wait 10
msend %actor% _- �� ���, ������� �������������?
~
#68801
����� ������ �����~
0 d 0
�� ������ ������ ���������~
wait 10          
detach 68800 %self.id%
set quester %actor%
global quester
msend %actor% _�����:
msend %actor% _- � ����, ��� �� �����������.
msend %actor% _- �����, ������ ������ ���� �� �����������.
wait 10
msend %actor% _- ��� ��� ������, ��� ���� ��������� �������.
msend %actor% _- � �������� ���� �������.
msend %actor% _- � ���� ���� ��������� ����, ������� ����� ����� �������.
msend %actor% _- ���� �� ��������� ��� ���, � ����� ���������� ����
wait 10
msend %actor% _- �����, ��� ���, ��� ���� ����� �����.
msend %actor% _- ��� ��.  
detach 68801 %self.id%
~
#68802
����� ��������� ����~
0 j 100
~
if %actor.iname% == %quester.iname%
if %object.iname% == ��������� ����
wait 1 
mpurge ����
msend %actor% _�����:
msend %actor% _- ��� ���������!
msend %actor% _- �� ������ � �� ���� ����� �����.
msend %actor% _- ��� ���� ���� �������, �� ����� �������� ����.
switch %random.5%
case 1
if %world.curobjs(68808)% < 15 
msend %actor% _- ��� ���� �������, ������� � ������ ���� � �����.
msend %actor% _- � ������� �� ������ ����� ��������� ����, �� �������� � �������.
msend %actor% _- ����� ��� �������� ���� �����.
mload obj 68808    
���� ������ %actor.iname%
end
break
case 2
if %world.curobjs(68809)% < 15
msend %actor% _- ��� �����-�� ���� ������.
msend %actor% _- � ��� �������, ����� ����� ����������
msend %actor% _- �� ����� ��� ���� �� ��������� ����.
mload obj 68809    
���� ���� %actor.iname%
end
case 3
if %world.curobjs(68810)% < 15
msend %actor% _- ��� ���� ��� ����.
msend %actor% _- ��� ���� ������� ���� � ����� � ������� �������������� ���� ������ �����.
mload obj 68810    
���� ���� %actor.iname%
end
break
default
msend %actor% _- ���, ����� ���� ������� ���.
msend %actor% _����� ��� ��� ��������� ������ ���.
msend %actor% _��� ��������� 500 ���.
%actor.gold(+500)%
mechoaround %actor% _����� ��� ��������� ����� ��� %actor.dname%.
break
done
else
msend %actor% _- �� ��� ����� ��� ��� �����?
msend %actor% _- ���� �, ��-������, ������ �� ����?!
���� %object.iname%
end 
else
msend %actor% _- ������!
msend %actor% _- � �� ����� ����.
msend %actor% _- ����� �� ����� ��� ��� �����?
msend %actor% _- ���������� ���� ������?!
���� %object.iname%
end
~
#68803
������������ ������~
2 e 25
~
wecho _������ ������� ���������� � ������� ����������.
~
#68804
������� �������~
1 h 100
~
wait 10
oechoaround %actor% _%actor.iname% ���������%actor.g% � ��� ������ ����� �� �����. 
osend %actor% _�� ���������� ������� � ��� ������ ����� �� �����.
switch %random.2%
case 1
oecho _�� ������� ������ ������� � ������������ ��������.
break
case 2
oecho _�� ������� ������ ������� � ������������ ����������� ������������ ����.
break
done
~
#68805
�����~
2 f 100
~
calcuid fromob 68824 mob
attach 68800 %fromob.id%
attach 68801 %fromob.id%
calcuid fromob 68818 mob
attach 68806 %fromob.id%
attach 68807 %fromob.id%
calcuid fromob 68803 mob
attach 68809 %fromob.id%
attach 68810 %fromob.id%
~
#68806
��������� ������������~
0 r 100
~
wait 10
msend %actor% _���������:
msend %actor% _- ������ ��������������.
msend %actor% _- �� ������� �� ���������� ������� ���?
msend %actor% _- ��������� �� ������� � ����� ������?
msend %actor% _- �� ���, �� ��������?
~
#68807
��������� ���� �������~
0 d 0
�� ������ ������~
wait 1
detach 68806 %self.id%
msend %actor% _���������:
msend %actor% _- ��� � �������.
msend %actor% _- ��� ���� �������.  
mload obj 68817
���� ��� %actor.iname%
msend %actor% _- ������ ��� ������ � ��� �������.
msend %actor% _- �� �� ���� � �������� �� ������.
msend %actor% _- � ����������!
detach 68807 %self.id%
~
#68808
����� ��������� ������� � ���� ����~
0 j 100
~
if %object.iname% == ��������� �������
wait 1
mpurge �������
msend %actor% _������� �����:
msend %actor% _- �������!
msend %actor% _- ��� ��� �������.
msend %actor% _- � �� ����� ������, ��� ���� ������ ���� �� ���� �� ������ ����.
msend %actor% _- ������� ���� �� ������.
wait 6
msend %actor% _����� ��������� � ������ ���� � ��� ��� �������� ������.
msend %actor% _- �� ��������� ����� ���� ������.
msend %actor% _- ��� ��� ���������� - ������ ����.
mechoaround %actor% _����� ��������� � ������ ���� � %actor.dname% �������� ������.
mload obj 68813
���� ������ %actor.iname%
else  
wait 5
msend %actor% _- ��. ��� ��� �� �����.
msend %actor% _- ������� ������ ��� ����!
���� %object.iname% %actor.iname%
end
~
#68809
������� ������������~
0 r 100
~
wait 5
msend %actor% _�������:
msend %actor% _- ������ ����, ��������.
msend %actor% _- ��� ������ ����� ������� �����.
msend %actor% _- ���� �� ����� �����%actor.g% � ������.
msend %actor% _- �� ��� �� �� �������� �� �� ��� � ��������� ������?
msend %actor% _- �������� �� �� ������� ��������?
~
#68810
������� ���� �������~
0 d 0
�� ������ ������ ���������~
wait 5 
detach 68809 %self.id%
msend %actor% _�������:
msend %actor% _- ��� � �������.
msend %actor% _- �������, � �������� ����� �� ������, ������� ���� �������.
msend %actor% _- � ����� ������� � ������, ���� �� ��� ������.
msend %actor% _- �� ��� ������ �������� ������.
wait 10
msend %actor%  - ����������, ��� ��� ���-�� ����-�� ������� � ����� �������� ����.
msend %actor% _- ���� �� ������� ���, � �� ������ �� ��������.
detach 68810 %self.id%
~
#68811
������� ���� �������~
0 j 100
~
if %object.vnum% == 68818
wait 1
mpurge %object%
msend %actor% _�������:
msend %actor% _- �� ��� �� ����%actor.g% ����� ���!
msend %actor% _- � � ��� � �� ��������.
msend %actor%  - � �� ����� �����, ��� ������ ���, �� �� ��������%actor.g% �������. 
wait 5
if %exist.obj(68807)%
�� ����
���� ���� %actor.iname%
msend %actor% _- ���, ����� ��� ������ �������.
msend %actor% _- ����� �� ������� ����.
else
msend %actor% ������� ��� ��� ��������� �����.
msend %actor% ...�� ��� - ��� �� ����� ��� ��������� ������ ���� ������� :(
end
else                                   
msend %actor% _- ��� �� ��� ����!
msend %actor% _- ��� ��� �� �����.
���� %object.iname%
end
~
$~
