* ������������� BZEditor
* ���������� ��������� : 7
* ��������� 04.08.2008 16:24:03
#21200
������� ������ ����������~
0 d 100
����������~
wait 1
if %actor.vnum% != -1
halt
end 
say ������-������...
wait 1
if %actor.bank% < 360
���
say �, �� � ���� ���-�� ���� �� ������� �� ������!
halt
end
eval buffer %actor.bank(-360)%
mload obj 21245
mload obj 21246
mload obj 21248
mload obj 21249
mload obj 21249
mload obj 21249
mload obj 21249
mload obj 21249
mload obj 21249
mload obj 21249
mload obj 21249
mload obj 21249
mload obj 139
mload obj 139
put all ���
say ��� ������� ����.
give all .%actor.name%
drop all
wait 2
say ������ ���������� ���� ��������...
if %actor.bank% < 600
���
say ��... � ��� �� ���� � ���� ����� ���.
halt
end
eval buffer %actor.bank(-600)%
switch %actor.class%
*������ ���
case 1
*���
case 2
*�������
case 4
*���-��������
case 6
*���-���������
case 7
*���-���������
case 8
*�������
case 10
*�����
case 12
*�����
case 13
mload obj 21211
mload obj 21212
mload obj 21211
mload obj 21212
mload obj 21214
mload obj 21215
mload obj 21216
mload obj 21217
mload obj 21218
mload obj 21219
mload obj 21221
mload obj 21221
break
*������
case 0
*��������
case 3
*���������
case 5
*������
case 9
*������
case 11
mload obj 21200
mload obj 21201
mload obj 21200
mload obj 21201
mload obj 21203
mload obj 21204
mload obj 21205
mload obj 21206
mload obj 21207
mload obj 21208
mload obj 21209
mload obj 21210
mload obj 21210
done
mload obj 21222
mload obj 21223
give all .%actor.name% 
drop all
wait 1
say ������ ���%actor.g% ���� � ����� ���� - �� ������%actor.w% ���.






~
#21201
�������� �����~
1 gj 100
~
if %actor.clan% == ���
halt
end
osend %actor% %self.name% ��������%self.u% � ����� �����.
wait 1
opurge %self%






~
#21202
���� ����~
1 c 2
����������~
wait 1
if !%arg.contains(�������)%
osend %actor% ��� �� �������?
halt
end
osend %actor% �� ���������� ������� ������� � ����� ������������� ����� ������.
if %actor.clan% != ���
halt
end
if %world.curmobs(21208)% > 50
osend %actor% ...� ������ �� ���������!
halt
end
oechoaround %actor% %actor.name% ���������%actor.g% ������� ������� � �� ����� ������������� ����� ������.
oload mob 21208
oecho ���������� ������, ��� ���������� � ����� ���� ��-��� ����� ����� �������� �������� ���.
oforce %actor% �������� ���
wait 1
opurge %self%



~
#21203
���������� ����~
0 c 100
����������~
wait 1
if %self.fighting%
halt
end
if !%arg.contains(����)%
msend %actor%  ���� �� ������ ����������?
halt
end
if (%actor.clan% != ���) || (%actor% != %self.leader%)
msend %actor% ��� �� ��� ������.
halt
end
msend %actor% �� ���������� ��������� ��������� ����.
mechoaround %actor% %actor.name% ���������%actor.g% ��������� ��������� ����.
mload obj 21252
give ������� .%actor.name%
wait 1
mecho �������� �������� ��� ���������� � ������, ������ ����� ������ ���.
wait 1
mpurge %self%


~
#21204
������ �������~
0 m 1
~
* ���� ����� �� ������
wait 1
if %amount% < 50
wait 2
say �������� �����!
give %amount% ��� .%actor.name%
halt
end
if %world.curobjs(21252)% >= 9400
wait 2
say ���� �������� �������, ���������!
give %amount% ��� .%actor.name%
halt
end
wait 2
say ���, ��������.
mload obj 21252
give ������� .%actor.name%













~
#21205
����� ���~
0 q 30
~
���� %actor.iname%
wait 1
msend %actor% ����� ��� �������� � ���, ������� ������ � ������ ������ ����.
mechoaround %actor% ����� ��� �������� � %actor.dname%, ������� ������ � ������ ������ ����.
wait 3
msend %actor% ����� ��� �������� ������ �����, �������� �� ����� ����� ���� ������, � ������ ������� ������.
mechoaround %actor% ����� ��� �������� ������ �����, �������� �� ����� ����� ������ %actor.rname%, � ������ ������� ������.
���� ���



~
#21206
����� ���~
0 q 30
~
���� %actor.iname%
wait 1
msend %actor% ������ ����� ��� �������� � ������� �� ���� ����, ���������� �������.
mechoaround %actor% ������ ����� ��� �������� � %actor.dname%, � ������� �� ����, ���������� �������.
wait 5
msend %actor% ������ ����� ���, ���� �� ����� ����, ����� ������ ������� �������� ����� ������ � �����, ������ ��������.
mechoaround %actor% ������ ������ ���, ���� �� ���� %actor.rname%, ����� ������ ������� �������� ����� ������ � �����, ������ ��������.
wait 5
msend %actor% ������ ����� ��� ����� ������������� �������� ���� � �������� � ����� ����, ������� ������ ���� �� �������.
mechoaround %actor% ������ ����� ��� ����� ������������� �������� ���� � �������� � ���� %actor.rname%, ������� ������ ���� �� �������.
���� ���



~
$
$
