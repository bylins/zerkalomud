#61800
������ ������������~
0 r 100
~
if (%quest618.name% == %nil%)
wait 1s
����   
� ������� %actor.iname%.
����� %actor.iname%
wait 1s
� ��, � ������ �� ���� � �����, ���� ����� ������� � ����, ��� ������ ����.
� ��, �� �������� �� �������, ����� �� ������ ��� �� �������.
� �� ���� ������ �� ����, ������, � ������� �� ����������.
wait 2s
�����
� �� ��� ���������, ��� ����� �� ����� ������, �������� ��� ��� ?
end
~
#61801
������ ���� �����~
0 d 100
������~
if (%quest618.name% = %nil%)
set quest618 %actor%
global quest618
wait 1s
���
� ��� � �������, � �� ����� ������� � ������ ��� ������.
� ��� ���� ���������� ������. 
� � ��� ������� �� ������� �������...��� � ���� �� ���������, ��� ���� ������� ���� ?
wait 1s
� ��� �������� ����.
���� %actor.iname%
wait 1s
� �� ������...
� ���� ��� ������.
� � ���������� ��� ������� ���������� ����� ������ ��� �����, �� ���� �������...
� � ������ ��� ������ ����, �� �� ������ ��, � ��� ��������-���������.
� ���� ���, �� ������� ������ ��� ���, � � �� ����� ��� �� ������� ��������.
� �, ��, ��� �� �����, ��� ���� ����� ����� ��, �� ��� ���� ��� ���� ��� ������.
���
wait 1s 
mload obj 61800
���� ������ %actor.iname%
wait 1s
� �� ��� ����������, �� ������ ��� �����, � �� ���� � ���� ��������.
calcuid for_mb 61800 mob
detach 61800 %for_mb.id%
detach 61801 %for_mb.id%
end
~
#61802
���� � ������~
2 h 100
~
if (%object.iname% == ������ ������)
wait 1s
wecho   ����� �������� ��� ����� �������, �� ��� ������ ���������� ����� ������������ � ���.
wecho � �� ���� ����� ������, �� �������, ��� ���� ���������� ������.
wdoor 61808 east room 61809
wdoor 61809 west room 61808 
wait 2s
wecho ��� �� ��� ���� � ����, ������ � ������ ������ �����.
end
~
#61803
���� �������~
0 f 100
~
if %world.curobjs(1262)% < 1
if %random.100% <= 2
mload obj 1262
end
end
msend %actor.name% ��� � ������� ������ ������� ����� ������.
mechoaround %actor.name% %actor.iname% � ������� ����� ������� ������ ��� ������.
mload obj 61802
~
#61804
������� �����~
0 j 100
~
if (%actor.name% != %quest618.name%)
������� %object.iname%
�����
halt
end
if (%actor.name% == %quest618.name%)
if (%object.iname% == ������ ������� ������)
wait 1s
� �, �� � ������ �� ������ � ����!
� �� ��� ���� ���� ���!
���
wait 1s
� �� �����, ������ ������ ���� �������, � � ��� ������ �� ������ �� ���� ������!
set vnum 61812
set Get_item %random.30%
eval vnum %vnum% + %Get_Item%
if (%vnum% > 61817)  || (%world.curobjs(%vnum%)% > 1)
mload obj 61818
���� ����� %actor.name%
halt
else 
wait 1s
mload obj %vnum%
���� ����  %actor.name%
end
end
~
#61805
���� ���� �������~
0 f 100
~
msend %actor.name% ��������� ������� ��������� ������������ � �������.
mechoaround %actor.name% %actor.iname% ��� ������ ��������� ��������� ��� ��������.
mload obj 61822
~
#61806
���� �������� ����~
0 f 100
~
msend %actor.name% �������� ������� ��������� ������������ � �������.
mechoaround %actor.name% %actor.iname% ��� ������ ��������� ��������� ��� ��������.
mload obj 61824
~
#61807
���� ���� �� �����~
0 f 100
~
msend %actor.name% �� �������� ����� ���� ���������� � ��� �����.
mechoaround %actor.name% %actor.iname% � ������� � ������ ����� ����� �� ����� � �����.
mload obj 61826
~
#61808
������� ������ �� ������ �������~
2 h 100
~
if (%object.iname% == ������ �� ������ ������� �� ������)
wait 1s
wecho ���, ������, �������...�������...
wecho ��, ����� ��� �������� �� ���� �����.
wdoor 61836 west room 61827
wdoor 61827 east room 61836         
wait 2s
wecho ��, � ����! �������� ������.
end
~
#61809
������� ������ �� ������ ����~
2 h 100
~
if (%object.iname% == ������ �� ������ ���� �� ������)
wait 1s
wecho   ����� ������� ����� �� ��� �������� ���.
wdoor 61848 south room 61849
wdoor 61849 north room 61848                    
wait 1s
wecho �� ��� ������ ����� ������������ � ���, � �� ������ �� ��������� ��� ������, ���
����� ��� � �� ����� ������.
end
~
#61810
������� ������ �� ������ �����~
2 h 100
~
if (%object.iname% == ������ �� ������ ����� �� ������)
wait 1s
wecho �������� �������� ��� �� ���� �������. 
wdoor 61820 east room 61822
wdoor 61822 west room 61820                  
wait 2s
wecho � ��� ��� �� ���������� �� � ������.
end
~
#61811
���� ����~
0 f 100
~
if (%world.curobjs(61801)% < 10) && (%random.100% <= 8)
   mload obj 61801
end
~
#61812
�����~
2 f 100
~
set quest618 %nil%
global quest618
calcuid delete_trig 61800 mob
attach 61800 %delete_trig.id% 
attach 61801 %delete_trig.id%
attach 61804 %delete_trig.id%
calcuid delete_trig 61802 mob
attach 61803 %delete_trig.id% 
calcuid delete_trig 61807 mob
attach 61805 %delete_trig.id%
calcuid delete_trig 61808 mob
attach 61806 %delete_trig.id% 
calcuid delete_trig 61809 mob
attach 61807 %delete_trig.id%
calcuid delete_trig 61801 mob
attach 61811 %delete_trig.id%
wdoor 61808 east purge room 61809
wdoor 61809 west purge room 61808
wdoor 61836 west purge room 61827
wdoor 61827 east purge room 61836 
wdoor 61848 south purge room 61849
wdoor 61849 north purge room 61848 
wdoor 61820 east purge room 61822
wdoor 61822 west purge room 61820
~
$~
