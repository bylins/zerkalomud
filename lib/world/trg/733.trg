* BRusMUD trigger file v1.0
#73300
���� � �������~
0 d0 2
�����~
if ((%actor.class% != 4) && (%actor.class% != 10) && (%actor.dex% > 40))
mskilladd %actor.name% �������.���� 200
else 
msend %actor% - ����������, ����������!
halt
end


~
#73301
���� �� �������� �����~
0 f0 100
~
if (%random.1000% > 900)
eval loadok 5
eval anticycle 1
while ((%anticycle% != 20) && (%loadok% == 5))
eval stfnum (73299 + %random.20%)
if %exist.obj(%stfnum%)% != 1
%load% obj %stfnum%
eval loadok 1
else
eval anticycle (%anticycle% + 1)
end
done
end


~
#73302
����������� � �������~
0 h0 100
~
wait 2
mecho - ������ ����, ������!
wait 10
���
mecho - ���� ��� ��������� ����.
mecho - �� ��� �� �� - ���� �� ���� ��������...
mecho - �� ������ �� ������� �������� ������ ������?
mecho - ���� ������, ��� ��� ������������ � ��������� �� ��������� ����!


~
$
$
