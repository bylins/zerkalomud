* BRusMUD trigger file v1.0
#50100
��� ��������~
0 c0 0
�����~
if !(%arg.contains(�����������)%)
   wsend        %actor%         _��� ����� ����?
   return 0
   halt
else
wait 1s
����
� ��� ��� ���� �����������?
wait 2s
� � �� �� ����� ������, �����!
� ��� ������ �����!
����
wait 1s
� ������
���
wait 1s
� ����..
wait 1s
���
�����
wait 2s
����
wait 9s
����������
������
���� ������������ ��������� �� ��������
end




















~
#50101
����� � ������~
2 c0 0
����� ~
if !(%arg.contains(� ������)%)
   wsend       %actor% _���� �����-�� ���������?
   return 0
   halt
 else
 %actor.wait(2)%
 wsend %actor% _�������� � ������� ������ �����, �� ������� � ������.
 wechoaround %actor% _%actor.name% �����%actor.q% � ������.
 wait 2s
 wteleport %actor% 50180
 end





























~
#50102
�������� �� �������~
2 c0 0
�������~
if !(%arg.contains(�� �������)%)
   wsend       %actor% _���� �����-�� ���������?
   return 0
   halt
 else
 %actor.wait(2)%
 wsend %actor% _����������� ������ ������� � �����, �� ������� �� �������.
 wechoaround %actor% _%actor.name% �����%actor.q% �� �������.
 wait 2s
 wteleport %actor% %number%
 end

~
#50103
C������ ������~
0 r0 100
~
wait 1
if (%actor.dex% < 22) && (%random.10% <= 5)
�������
���
��� �� �� ������! ������!!!
mkill %random.pc%
wait 4s
if %world.curmobs(50133)% < 10
mecho _�������� �������� ���������� �������� ��-�� ����.
mload mob 50133
end
*detach 50106 %self.id%
end
if (%actor.dex% < 25) && (%random.10% <= 3)
�������
���
��� �� �� ������! ������!!!
mkill %random.pc%
wait 4s
if %world.curmobs(50133)% < 10
mecho _�������� �������� ���������� �������� ��-�� ����.
mload mob 50133
end
*detach 50106 %self.id%
end
if (%actor.dex% < 30) && (%random.10% <= 2)
�������
���
��� �� �� ������! ������!!!
mkill %random.pc%
wait 4s
if %world.curmobs(50133)% < 10
mecho _�������� �������� ���������� �������� ��-�� ����.
mload mob 50133
end
*detach 50106 %self.id%
end
end

~
#50104
������ ������ �����.~
0 a0b0 25
~
wait 1
if %self.fighting% || (%self.position% == 4 )
halt
end
switch %random.3%
case 3
emot �������%self.q% ���� � �������%self.g% �������� � ������.
break
case 2
emot �������%self.q% ���� � �������%self.g% �������� � ������.
wait 1s
mload obj 50115
���� ������
done

~
#50105
�������� ����~
0 b0 7
~
��� �������! ������� �������, ������� - ��������!


~
#50106
������� �����~
0 b0 30
~
eval pian %random.5%
eval pl %random.npc%
switch %pian%
case 5
��� %pl.name%
� ��, �-�-���, ��� �� �, �-��, ������ ���� ������..
wait 1s
��� �������� �� ���
break
case 4
eval mo %random.npc%
��� %mo.name%
break
case 3
��
wait 1s
eval mo2 %random.npc%
��� %mo2.name%
break
case 2
���
break
case 1
����
end


~
#50107
����� ����~
2 f0 100
~
if %exit%
detach 50101 %exit.id%
end
switch %number.8%
case 1
set number 50161
break
case 2
set number 50162
break
case 3
set number 50156
break
case 4
set number 50158
break
case 5
set number 50100
break
case 6
set number 50179
break
case 7
set number 50169
break
case 8
set number 50173
done
calcuid exit %number% room
attach 50101 %exit.id%
calcuid cavern 50180 room
remote number %cavern.id%

~
$
$
