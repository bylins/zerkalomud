* BRusMUD trigger file v1.0
#68700
���� ������ ������~
0 j0 100
~
if %object.vnum% == 68611
wait 1s
mecho %self.name% ����������: ��� ������!
mecho %self.name% ����������: ������ � �������� �� ������ ���� ������!
wait 1s
mecho ���������� ������� �������� �����������...
calcuid demid 68613 mob
attach 68701 %demid.id%
set hero %actor%
remote hero %demid.id% 
detach 68605 %demid.id%
mteleport %self% 68760
wait 1s                 
calcuid demrum 68656 room
attach 68607 %demrum.id%
mpurge %self%

end




~
#68701
������� ������  �� ����������~
0 q0 100
~
if %actor% != %hero%
wait 2s
halt
end 
say ������� ���� ��� ������� ?
say ������� �� ����� ��� � ���� ������ ��������� �������� ����� ������.
say �����, ������ ��������!
msend %actor% %self.name% ��� ��� ��������� �������.
mechoaround %actor% %self.name% ��� %actor.tname% ��������� �������.
%actor.gold(+10000)%                                
msend %actor% ��� ��������� 10000 ���.
detach 68701 %self.id%




~
#68702
�������~
0 r0 100
~
wait 1s
say ��������� ������, ��� ������� ���� ?
wait 10s




~
#68703
������� �����~
0 d0 0
����� ����~

wait 1s
mecho ������� ������� ��������.
wait 1s
say ��� �, ������ ���� %actor.name%, �� ������ � �� ���.
say ������� ����� � ����� ����, ������� ����� ��� �� ����, ������� ������
say ����������. ������ ��� ���������� �� ����, ��� ������ � �������� ���� �����.
wait 2t
halt




~
#68704
����� ���� -  ������ ������� ��������~
0 f0 100
~
calcuid vodan 68718 mob
detach 68702 %vodan.id%    
detach 68703 %vodan.id% 
set killer %actor%
remote killer %vodan.id%
attach 68705 %vodan.id%




~
#68705
������� ���� ����� ����~
0 r0 100
~
if %actor% != %killer%
wait 1s
halt
end
say ���� ����� ������� �� ��������%actor.g%.
wait 1s
mecho ������� ������ ������ � ����� �����.
wait 1s
say ��� ������ %killer%, ��� � ������.
mload obj 68702
���� ������ %actor.name%
wait 1s
��� ������
detach 68705 %self.id%




~
#68706
�����  ���~
0 f0 100
~

mecho %self.name% ����������: �� ������� ����, �� �  ��� �������!!!
mecho ���������� ������� �������� �����������... 
if %random.4% ==  1 && %world.curobjs(68713)% < 2
mload obj 68713
end




~
$
$
