#25000
�����������~
0 r 100
~
wait 1
msend %actor% �������� ������ ����:
msend %actor% - ������ ����, %actor.name%!
msend %actor% - � ���� ��������� ���� ����� ���� �� ����� ����� �� 10 ���.
msend %actor% - ���� � �� ������ ����� ������, ��� ������ ������ �� 100 ���.
~
#25001
�������~
0 m 1
~
wait 1
switch %amount%
 case 10
  msend %actor.name% �������� ����������� ����� � �����, ����� ��� ����� ������������ � ���.
  wait 1s
  msend %actor.name% �� ������� �� ����� ����� ���� �� ����� �����.
 if %actor.sex%==1
  mechoaround %actor% %actor.name% ����� � ����� � ����������.
  mechoaround %actor% %actor.name% ����� �� ����� � ����������.
 else
  mechoaround %actor% %actor.name% ������� � ����� � ����������.
  mechoaround %actor% %actor.name% ������ �� ����� � ����������.
 end
  wait 1s
  msend %actor.name% �� �������� � ������ ������ ����.
  msend %actor.name% �������� ��� � ����� � ������ �����.
  mteleport %actor% 25120
  wait 1s
 if %actor.sex%==1
  %echoaround% %actor% %actor.name% ������� � ����������.
  %echoaround% %actor% %actor.name% ����� �� �����.
 else
  %echoaround% %actor% %actor.name% �������� � ����������.
  %echoaround% %actor% %actor.name% ������� �� �����.
 end
  %echoaround% %actor% �������� ��� � ����� � ������ �����.
  wait 1s
mat 25065 %echo% �������� ������� �����.
 break
 case 100
  msend %actor.name% �������� ����������� ����� � �����, ����� ��� ����� ������������ � ���.
  wait 1s
  msend %actor.name% �� ������� �� �����.
  msend %actor.name% ������� ����� ������ � ����� ������, ����� � ����� �����. 
 if %actor.sex%==1
  mechoaround %actor% %actor.name% ����� � ����� � ����������.
  mechoaround %actor% %actor.name% ����� �� ����� � ����������.
 else
  mechoaround %actor% %actor.name% ������� � ����� � ����������.
  mechoaround %actor% %actor.name% ������ �� ����� � ����������.
 end
  wait 1s
  msend %actor.name% �� �������� � �������� �������.
  msend %actor.name% �������� ��� � ����� � ������ �����.
  mteleport %actor% 33027
  wait 1s
 if %actor.sex%==1
  %echoaround% %actor% %actor.name% ������� � ����������.
  %echoaround% %actor% %actor.name% ����� �� �����.
 else
  %echoaround% %actor% %actor.name% �������� � ����������.
  %echoaround% %actor% %actor.name% ������� �� �����.
 end
  %echoaround% %actor% �������� ��� � ����� � ������ �����.
  wait 1s
mat 25065 %echo% �������� ������� �����.
 break
 default
 msend %actor% ���-�� �� ��.
 msend %actor% � ������� ����������� �����, � ��� ��������� ���. 
 ���� %amount% ��� %actor.name%
 break	
done
~
#25002
�����������������~
0 r 100
~
wait 1
say ������, ������.
say �� ����� � ������� ����� �����������, ��� �������� �� ������� �������� �����.
say ����� ���� � ����� ������.
���� %actor.name% ������� ��������� ������������ ������.
���� %actor%
~
#25003
����������������~
0 r 100
~
wait 1
say ������, ������.
say �� ����� � ������� ����� �����������, ��� �������� �� ������� �������� �����.
say �������, ����� ���� � ����� ������.
~
#25004
�����������~
0 r 100
~
wait 1s
���
say ������ ����, ������.
say �� ����� � ������� ����� �����������, ��� �������� �� ������� �������� �����.
say ������������ ����� � ���, ������ ������� �������, ����� ������, ���� ������������.
wait 1s
say ��� � ������ �� ����� ������ ������� ����� ������� ���� ��������.
say �� ����� ����� � �������, �� � ���������, �� ���� �������.
wait 1s
����
say ���� �� ��������� ������������. 
say ���� ���������, �� ���� ����������� ��� ��� � � ����, ����� �������, ��������.
~
#25005
������ �� ���� ���������~
0 m 100
~
wait 1s
emot �������� ���������� � ����������
mtransform 25019
wait 5s
emot �������� ������������ ���-�� ��������������
wait 5s
emot ��������� ����� � ����� ���������� � �������
mtransform 25008
~
#25006
���� � �������-�������~
0 q 100
~
wait 1
msend %actor% ������-������ ��������� � ���� �������, ����� ���-�� ��������� ���� ��� ���.
mechoaround %actor% ������-������ ��������� �� %actor.rname%, ����� ���-�� ��������� ���� ��� ���.
* ���� �������� ������ 18 � ���� ������
if %actor.skill(���������� ���)% > 0
halt
end
if (%actor.level%>=18 && %actor.class%==10)
wait 1
if (%actor.sex%==1)
say � ���� �� �������� �������, ������ ����� �� ��� � �������?
else
say ���� �� �������� ��������, �� ������� ����� �� ��� � �������?
end
end
~
#25007
������ ������ � �������~
0 d 1
����� ���� �������� ��������~
wait 1
if %actor.skill(���������� ���)% > 0
halt
end
if %actor.id% ==  %questor270.id%
say �� � ����� ��� ��� ���� ������������. ��� ��� �������?
halt
end
if (%actor.level%>=18 && %actor.class%==10)
wait 1s
���
say �������� �� ���� � ����� ����, �� � ���� �� ���� � ������ �� ����� - � ������� � ���� � ���� �����.
say ����� ��������� �� ���� ���-������. �������, � �������.
mload obj 25007
give ������� %actor.name%
say ��� ���� ������� �� ����.
say ������� �� �������.
set questor270 %actor%
end
~
#25008
���� ��������� �������~
0 j 100
~
wait 1
if %object.vnum% == 25008
wait 1
mpurge %object%
if %actor.vnum% != -1
halt
end
if %actor.quested(25000)%
say ��... ������, �� ���� ����� ����������.
emot ������ ������� ������� � ����� ���-�� �� ��� ������.
say ����� ��� ��� ������� ������� - ��� �������, ��� �� ����� ����� ���� � �������.
mload obj 25009
give ������� %actor.name%
halt
end
end
if ( %object.vnum% == 25007 )
wait 1
mpurge %object%
if %actor.vnum% != -1
halt
end
������ %actor.name%
emot ������ �������
wait 1s
say ��... ������, ������ ������, ����� � �� ���� ���������...
if %actor.skill(���������� ���)% > 0
say ������, �� � ���� �� ����������, �� � ���� ��� ���� ������ ���...
halt
end
if (%actor.level% >= 18 && %actor.class%==10 )
wait 2s
say � ���� ���� �� ����, ���� ���� �� �� ������ ��� ���� ������...
���
wait 2s
say � �� �� ���� ���������. ���� �� ����� �������:
say ������� ������ �� ���� � ������ �� ������ ���������-��������.
say ����������� ������, �� ����� ������, �������.
say ���� ������� ��� ��������� - � �������� �� ���� ����� ��������.
%actor.setquest(25000)%
else
say ������, �� � ���� �� ����������, �� � ���� ��� ���� ������ ���...
halt
end
else
say ��� ��� �� �����, ������ ����.
give %object.name% %actor.name%
end
~
#25009
����� ������� �������~
0 j 100
~
wait 1
if %object.vnum% != 25009
���
������� %object.name%
halt
end
wait 1
mpurge %object%
if %actor.vnum% != -1
halt
end
emot �������� �������
if !%actor.quested(25000)%
say �������... 
say ������-�� ������ �������, �� ������ ��������� ��� ������ �� � ����!
���
halt
end
if (%actor.level% >= 18 && %actor.class%==10 )
wait 1s
say �� ��� ��, ��� ������� �������� �� ���� - � ������ ���� � �������.
msend %actor% ������ ��������� ��� �� ������������ �������� ������� �����.
mechoaround %actor% ������-������ �������� ���-�� ������������ %actor.name%.
mskillturn %actor.name% ����������.��� set
else
���
say ���� � ������ ������� �� �����.
end
~
#25010
�������� ���������~
0 f 100
~
if %actor.quested(25000)%
mload obj 25008
end
~
$~
