#33600
� ������~
0 b 50
~
wait 1
foreach fchar %self.pc%
if !%fchar%
halt
end
switch %random.15%
case 1
mecho ������ ������ ������� � � ������� ����������� ������� ������.
mecho ���� �� ����� �������� �� %fchar.rname% � �������.
mdamage %fchar% 5
break
case 2
mecho ������ ������ ������ ������� � � ������� ����������� ������� ������.
mecho ���� �� ����� �������� �� %fchar.rname% � �������.
mdamage %fchare% 15
break
case 3
say ��������. ����������� ��.
break
case 4
mecho ������ ����� ������ ������ ������� � � ������� ����������� ������� ������.
mecho ���� �� ����� �������� �� %fchar.rname% � �������.
mdamage %fchar% 25
break
case 5
mecho ������ ��� ���� ��� ������ ������� � � ������� ����������� ������� ������.
mecho ���� �� ����� �������� �� %fchar.rname% � �������.
mdamage %fchar% 55
done
done
~
#33601
� ������~
0 q 100
~
wait 3s
mecho _���-�������� ����������� ������ �����.
~
#33602
� �����~
0 q 100
~
wait 1
if      %actor.sex% == 1
   msend       %actor% ������ ���������� ��� ��������� �����.
   mechoaround %actor% ������� ������� %actor.dname% ������ ���������� �����������.
else
msend       %actor% ������ ��������� �� ��� � ������, ��� ����� �� � ���, ��� � ����� ������.
mechoaround %actor% ������� ������ ���� %actor.rname%, ���� �� �������� �� ���...
end
~
#33603
� ��������~
2 b 100
~
return 0
wait 1
set fchar %self.people%
while %fchar%
      set pc %fchar.next_in_room%
      if %fchar.vnum%==-1
      switch %random.8%
case 1
      wecho ������� ����� �������� � � ���� ������ ����������� ��� � ����� ������.
      wdamage %fchar.name% 5
break
case 2
      wecho �� ���������� ������ ������� ������� � �����  %fchar.dname% � �����.
      wdamage %fchar.name% 15
break
case 3
      wecho �� ���������� ������ ������� ������� � �����  %fchar.dname% � ����.
      wdamage %fchar.name% 35
break
case 4
      wecho ������� ������������ � ������ ������� %fchar.rname%.
      wdamage %fchar.name% 55
break
default
break
done
      end
      if %pc%
         makeuid fchar %pc.id%
      else
         set fchar 0
      end
done
~
#33604
����� ������~
0 f 100
~
*if (%world.curobjs(608)% < 40) && (%random.3% == 1)
*   mload obj 608
*end
~
#33605
�������� � ������~
0 h 100
~
wait 1
if %actor.vnum% == 33833
mshou �����! ������ � �����! � ������!
end
wait 1
if %actor.vnum% == 33834
mshou  ��������! ��������� ���� ��������� �� ����-����! �� ������!
end
~
$~
