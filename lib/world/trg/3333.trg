#333300
����� ����~
2 f 100
~
calcuid dafu 333341 mob
attach 333306 %dafu.id%
attach 333307 %dafu.id%
~
#333301
������� �����������~
0 p 100
~
%echo% ����� �������� � ������ �����������...
������
~
#333302
�������� �����������~
1 j 0
*~
wait 2s
if (!%exist.mob(333354)%)
  %load% mob 333354
  %send% %actor% ��������� ������ �������� � ��� �� ��� � ��������� �����.
else
  %echo% ...� ������ �� ���������.
end
~
#333303
�������� �����������~
1 l 0
*~
wait 1s
if (%exist.mob(333354)%)
  %teleport% %world.mob(333354)% %self.room%
  %purge% ���������.������
  %echo% ��������� ������ ������ � ������ � ������� ����.
else
  %echo% ...� ������ �� ���������.
end
~
#333304
������� �������~
0 n 100
~
eval pcs %self.pc%
foreach pc %pcs%
  if ((%pc.eq(���)%==%world.obj(333399)%)||(%pc.eq(�����)==%world.obj(333399)%))
    ���� .%pc.name%
    ���
    halt
  end
done
�����
%echo% ������ �� ���� ���������� ������ ������� � �����...
%purge% ���������.������ 
~
#333305
����������� �������~
0 d 0
*~
switch %speech%
  case ��� �� � ��� �� �����? �� �� �� �� ��� ����!
    � %actor.name% �������: "���� ������ ��� ������? ��� ��� ��� ������!"
  break
  case ���� ��� �� ��, ��� ����!
    � %actor.name% �������: "��������� ������ �������, ������!"
  break
  case ��� �� ������, ��� ��� �� ���.
    � %actor.name% �������: "������� �������, ���� ��������, �� ������ �� ���������."
  break
  case ���, �� ����-�� �����...
    � %actor.name% �������: "��, ���� ���-�� ��������� ������� ������..."
  break
  case � �� ��� ��! �� ����!
    � %actor.name% �������: "�� �� �� ��� �����! ���, ������!"
  break
  case ��� ���� ���� ��� ��� ��, ��� ���� ����-��.
    � %actor.name% �������: "�����, ������� ������� �����, ��������, ��� ���-�� �������."
  break
  case ���! ���� ����? ���� ��� ������ �� �� ����!
    � %actor.name% �������: "��! ������� ����? ���, �������� ������ ������� �������� ����������!"
  break
  case �� �������! �� ��� �� �� ���� �� ���� ��� ��.
    � %actor.name% �������: "�� �����������! ������ ���� ������� ��� ���������."
  break
  case ����� ���?
    � %actor.name% �������: "������, ��������, �����?"
  break
  case ���� ����! ������ ������...
    � %actor.name% �������: "��� � ������! ������ ��������� �������..."
  break
  case ����! ������ ��� �� �� �� ������.
    � %actor.name% �������: "���������! ������ � ����� ������� ������� ���������."
  break
  case �� ��� ���� ��
    � %actor.name% �������: "� ������ ���� ������"
  break
  case ����
    � %actor.name% �������: "������ ������� ���"
  break
  case �� ��
    � %actor.name% �������: "� ���"
  break
  case �� ���
    � %actor.name% �������: "� �� ������"
  break
  case ���, �������.
    � %actor.name% �������: "���, �� ��� ����."
  break
  case ��� ����!
    � %actor.name% �������: "�������� ��!"
  break
  case ����-�� ����� �� ������.
    � %actor.name% �������: "���, �������� ������ ���� ����� ������������ ������� ���-��."
  break
  case �� ��� ��� ���� ��, ������� �� ���...
    � %actor.name% �������: "��� ��� ����� �������� �������� ����, ��������� ������� �����..."
  break
  case ��� �� ��� �� �� ���� �� ��� ��� ���� ��� ������...
    � %actor.name% �������: "� ���, ��� � ���� �������� �������, ������, ��� ��� ������ ����� ����� ���������������� ���..."
  break
  case �� �� �� ��� ���
    � %actor.name% �������: "��� ������ ��� �����������"
  break
  case ���!
    � %actor.name% �������: "��� � ������!"
  break
  case �������! ����, ����!
    � %actor.name% �������: "�� �� ���! �������, ��-��������, ������ �� �������!"
  break
  case ������� ����� ���!
    � %actor.name% �������: "� �� ��� ��������������� - ��!"
  break
  case ����, ������ �� ��� �� �����?
    � %actor.name% �������: "������, � �� �� �������� ��� ������� ���������� ��� �����?"
  break
  case ��� �� �������� �� �����, ��� ���� ...
    � %actor.name% �������: "������� ��������� ����� �����, ���������..."
  break
  case ��� �� ����. �� �� �����.
    � %actor.name% �������: "������� �������� ���� - � �� ���� ������ ����."
  break
  case ���� ������� �� ��, ������� �� �������,
    � %actor.name% �������: "����� ������ ������, �� �� ��������� � ���� �����,"
  break
  case ����� ����.
    � %actor.name% �������: "�������� ����� ������."
  break
  case ���?
    � %actor.name% �������: "��� �����?!"
  break
  case �� ��������� �� ����.
    � %actor.name% �������: "� ����� ������� �� �������."
  break
  case ��� ������?
    � %actor.name% �������: "���� ������?"
  break
  case ���... ��... ���... �-��-�!
    � %actor.name% �������: "���... ���.. ���... �-��-�!"
  break
  case ���, ��� �� ���� �� ���...
    � %actor.name% �������: "��, ������� ���� �������..."
  break
  case ���? ��� ��!?
    � %actor.name% �������: "��� ���? ������?!"
  break
  case ��� ���, �� �� ����� �� �� ��� �� ���.
    � %actor.name% �������: "���������, ������� ������� ��� ������� �� ������ �������� �� ������."
  break
  case ����ȣ �� ������ ��?
    � %actor.name% �������: "����� ��� ��� ���������?"
  break
  case �����... �� ����
    � %actor.name% �������: "��... ��� ����������..."
  break
  default
    ���
  break
done
~
#333306
�����������~
0 r 100
~
wait 1s
� ���?
� ��� ������?
~
#333307
���� ������~
0 j 100
~
wait 1s
if (%object.vnum%==15707)
  %echo% ���� �������� ������ ������.
  wait 1
  %purge% ������
  � ���... ��... ���... �-��-�! 
  wait 1
  ��
  wait 1
  %echo% ���� ���������� � ������������...
  wait 2s
  � ���, ��� �� ���� �� ���...
  wait 1
  � ��� ���, �� �� ����� �� �� ��� �� ���.
  � ����ȣ �� ������ ��?
  � ��� �� � ��� �� �����? �� �� �� �� ��� ����!
  wait 1
  � �����... �� ����
  attach 333308 %self.id%
  detach 333306 %self.id%
  detach 333307 %self.id%
else
  � ���? ��� ��!?
  eval vzyatka %object.iname%
  ���� %vzyatka.car%
  %send% %actor% ���� � ����� ������� %object.iname% ��� � ����.
  %echoaround% %actor% ���� � ����� ������� %object.iname% %actor.dname% � ����.
  %damage% %actor% %random.50%
end
~
#333308
�����������~
0 d 1
������� ���������� ��������� �������� �������� ������� ���������~
� �������! ����, ����!
���
Wait 1s
� ��� �� �������� �� �����, ��� ���� ...
� �� ��������� �� ����.
Wait 1s
� ����� ����.
Wait 1s
� ���� ������� �� ��, ������� �� �������,
� ��� �� ����. �� �� �����.
Wait 1s
� ����, ������ �� ��� �� �����?
� ������� ����� ���!
%echo% ���� ������ ����� � ������ ��� � ������������� ��������� �� ���.
attach 333309 %self.id%
detach 333308 %self.id%
~
#333309
�����1~
0 d 1
�� �������� ������~
wait 1s
context 3333
� ���!
eval kvestvnum 333318+%random.70%
eval kvestitem %world.obj(%kvestvnum%)%
set kvestnum 1
� �� �� �� ��� ���
� %kvestitem.iname%
context 3333
global kvestitem
global kvestnum
wait 1s
� ��� �� ��� �� �� ���� �� ��� ��� ���� ��� ������...
wait 1s
� �� ��� ��� ���� ��, ������� �� ���...
wait 1s
� ����-�� ����� �� ������.
� ��� ����!
exec 333311 333390
attach 333310 %self.id%
detach 333309 %self.id%
~
#333310
�����2������~
0 j 100
~
wait 1s
context 3333
if (%object.vnum%==%kvestitem.vnum%)
  eval kvestnum %kvestnum%+1
  global kvestnum
  � ���, �������.
else 
  � �� ���
  �              %kvestitem.vname%
  � �� ��� �� ��
  �            - %object.iname%
  halt
end
if (%kvestnum%<10)
  eval kvestvnum 333318+%random.70%
  eval kvestitem %world.obj(%kvestvnum%)%
  global kvestitem
  � ����
  �                      %kvestitem.vname%
  eval vragnum 333342+%kvestnum%
  eval trignum 333310+%kvestnum%
  exec %trignum% 333390
  eval vragid %world.mob(%vragnum%)%
  � �� ��� ���� ��
  �                                     %vragid.vname% !
else
  � ����! ������ ��� �� �� �� ������.
  wait 1
  %echo% ���� ������ ��� ���������� � ������� �����, �����, ������ 
  %echo% � ������ ���� ������������ �������� � ���������.
  wait 1
  � ���� ����! ������ ������...
  wait 2
  � ����� ���?
  wait 1
  � �� �������! �� �� ���� �� ���� ��� ��.
  ��� �����
  wait 1
  � ���! ���� ����? ���� ��� ������ �� �� ����!
  wait 1
  %echo% ���� ������ ������ �������� ������ �� ��.
  %teleport% ����� 333382
  � ��� ���� ���� ��� ��� ��, ��� ���� ����-��.
  ���
  attach 333399 %self.id%
  detach 333310 %self.id%
end
~
#333311
�����������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333343
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333312
���������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333344
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333313
��������������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333345
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333314
�������������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333346
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333315
������������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333347
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333316
���������������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333348
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333317
����������������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333349
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333318
������������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333350
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333319
����������������������~
2 z 100
~
eval i 1
while (%i%<=12)
  %load% mob 333351
  eval targetroom 333300+%random.89%
  %teleport% ������ %targetroom%
  eval i %i%+1
done
~
#333350
�������� ��������~
2 z 100
~
wait 1s
%load% mob 333352
eval targetroom 333300+%random.89%
%teleport% ������ %targetroom%
~
#333351
�������� ��������~
2 z 100
~
wait 1s
%load% mob  333353
eval targetroom 333300+%random.89%
%teleport% ������� %targetroom%
~
#333352
���� ������~
0 f 100
~
exec 333350 333390
return 1
~
#333353
������ ��������~
0 f 100
~
exec 333351 333390
return 1
~
#333354
���� ����� 00-18~
0 f 100
~
eval loadvnum 333299+%random.19%
%echo% %loadvnum%
if (%random.10%==1)
  if (%world.curobjs(%loadvnum%)%<5)
    %load% obj %loadvnum%
  end
end
~
#333355
���� ����� 93-99~
0 f 100
~
eval loadvnum 333292+%random.7%
%echo% %loadvnum%
if (%random.10%==1)
  if (%world.curobjs(%loadvnum%)%<5)
    %load% obj %loadvnum%
  end
end
~
#333370
test~
0 p 100
~
%echo% %damager.name%
%echo% %skill%
return 5
~
#333399
���������~
0 q 100
~
wait 1s
if (%exist.mob(333339)%)
  � � �� ��� ��! �� ����!
else 
  � ���, �� ����-�� �����...
  � ��� �� ������, ��� ��� �� ���.
  %load% obj 333389
  ���� ������� .%actor.name%
  wait 1s
  � ���� ��� �� ��, ��� ����!
  %teleport% all 15750 horse
  detach 333399 %self.id%
~
$~
