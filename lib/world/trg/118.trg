#11800
����������� ��������~
0 r 100
~
wait 1
����� 
wait 1
say ����������, ��� �������
wait 1
say �������� ���� �������� ���� �������!
wait 1
say ������ �������� ������ ����� ������ � ������ ���� �� ����!
wait 1
say ���� �� ������ ����� ������ - "�������� �����" ���� ������
say ��������� �������� � ������ ��������� ����� �� ������������ ��������.
wait 1
say ���� �� ������� �������� �������� � ����� ������ ������� � �����
wait 1
say � ����� ����������� ����.
~
#11801
� ������� ��������~
2 e 100
~
wait 1
wecho ��� ������ ����� ���� ����������
~
#11802
�������� ����� ����������~
2 e 100
~
wait 1
wecho ������������ � ���� �����!
~
#11803
������ �� ������.~
1 t 100
~
wait 1
if (%self.vnum%==11800)
  oecho ��������� ����� �����������...
end
if (%self.vnum%==11801)
  oecho ����� ��� ������� �����������...
end
if (%self.vnum%==11802)
  oecho ���������� ������ ������� � �������...
end
switch %random.3%
case 1
  oload mob 11802
  oecho ...� ������� ����� ����� �� ������������ ���.
  break
case 2
  oload mob 11803
  oecho ...� ������ ������� �������� �� �����������.
  break
default
  oload mob 11804
  oecho ...� �������� ������ ������� �� �������.
  break
done
opurge %self%
~
#11804
���� ������� � ���� �����~
0 f 100
~
eval svzem %world.curobjs(11800)%+%world.curobjs(11801)%+%world.curobjs(11800)%+%world.curmobs(11802)%+%world.curmobs(11803)%+%world.curmobs(11804)%
if (%svzem% < 2)
mload obj 11888
end
if (%random.20%==1)
 switch %random.11%
 case 1
   if (%world.curobjs(11810)%<3)
     mload obj 11810
   end
   break
 case 2
   if (%world.curobjs(11811)%<3)
     mload obj 11811
   end
   break
 case 3
   if (%world.curobjs(11812)%<2)
     mload obj 11812
   end
   break
 case 4
   if (%world.curobjs(11813)%<1)
     mload obj 11813
   end
   break
 case 5
   if (%world.curobjs(11814)%<1)
     mload obj 11814
   end
   break
 case 6
   mload obj 11815
   break
 case 7
   if (%world.curobjs(11816)%<2)
     mload obj 11816
   end
   break
 case 8
   if (%world.curobjs(11817)%<1)
     mload obj 11817
   end
   break
 case 9
   if (%world.curobjs(11818)%<2)
     mload obj 11818
   end
   break
 case 10
   if (%world.curobjs(11819)%<1)
     mload obj 11819
   end
   break
 default
   if (%world.curobjs(11820)%<1)
     mload obj 11820
   end
   break
 done
end
~
#11805
�� ������ ������~
0 f 100
~
mecho ����� ����� �� ����� ��� ���.
mecho ���� �������� �� �� ���������� ���� � �������� �����.
mecho ���� ������ ������� �� �����.
mecho ������ ����� �������� �� �������� ����� �� ���.
mecho %actor.name%, ���� ������ �������%actor.g% �� ������������ 
mecho � �� �����%actor.g% ������� ���������.
mdamage %actor% 200
calcuid monah 11800 mob
detach 11800 %monah.id%
attach 11808 %monah.id%
if %world.curobjs(1242)% < 1
if %world.curobjs(1241)% < 1
if %random.100% < 3
mload obj 1242
end
end
end
~
#11806
����� ��������~
0 p 100
~
mecho �� ��������� ����� ��� ������ ������ �������� ��� ����, ������� ����������� ������!
%self.hitp% = %self.maxhitp%
~
#11807
�������� �����~
1 h 100
~
if (%actor.realroom% == 11819)
 oecho ������ ����� ��������� ����� ������ � ���� ����������.
 calcuid voron 11801 mob
 detach 11806 %voron.id%
end
wait 1
opurge %self%
~
#11808
����������� �������� 2~
0 r 100
~
wait 1
switch %random.20%
case 1
  say ������� ������, ������ � ����������� ����!
  mload obj 11804
  ���� ��� %actor.name%
  break
case 2
  say ������� ������, ������ � ����������� ����!
  mload obj 11805
  ���� ��� %actor.name%
  break
case 3
  say ������� ������, ������ � ����������� ����!
  mload obj 548
  ���� ��� %actor.name%
  break
case 4
if %world.curobjs(3305)% < 2  
mload obj 3305
���� ����� %actor.name%
end
case 5
case 6
case 7
 if (%world.curobjs(218)%<50)
  say ������� ������, ��� ������, ��� ������� �� �� ��������!
  mload obj 218
  ���� ��� %actor.name%
  break
 end
default
  say ������� ������!
  wait 10
  say ������ �� � ����� ������ �������� ����� � ����� �������� �������!
  ���� 5000 ��� %actor.name%
done
detach 11808 %self.id%
~
#11809
����� ���� "�������� �����"~
2 f 100
~
calcuid graven 11801 mob
detach 11806 %graven.id%
attach 11806 %graven.id%
~
$~
