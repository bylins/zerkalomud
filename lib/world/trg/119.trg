#11900
�������~
0 j 100
~
wait 1
if %object.vnum% == 11907
wait 1s
mjunk all
mecho ������ ������ ���� �����, ��������� ���� ����, ������� ������� ����� ����.
switch %random.10%
 case 1
 if (%world.gameobs(11901)% < 2)
 mecho ������ ������ ���-�� �������!
 mload obj 11901 
���� ���.������� %actor.name%
 end
 break
case 2
 if (%world.gameobs(11902)% < 2)
 mecho ������ ������ ���-�� �������!
mload obj 11902
���� ���.��� %actor.name%
end
break
case 3
 if (%world.gameobs(11903)% < 2)
 mecho ������ ������ ���-�� �������!
 mload obj 11903 
���� ���.������ %actor.name%
 end
 break
case 4
 if (%world.gameobs(11904)% < 2)
 mecho ������ ������ ���-�� �������!
 mload obj 11904 
���� ���.����� %actor.name%
 end
break
case 5
 if (%world.gameobs(11905)% < 2)
 mecho ������ ������ ���-�� �������!
 mload obj 11905 
���� ���.������� %actor.name%
 end
 break
default
 mecho ������ ���������� ��������������, �� ������ �� �����.
done
elseif %object.vnum% == 11908
wait 1
mjunk all
  say ���!!! �� ������ ����!
  say �� ���� ������ �� �� ��� ��� ������? ���� ���� ����... 
  say ������ �� �������� �� ���� ����������. � �� �� �����.   
  wait  1s
if %random.3% == 1 && %world.curobjs(11906)% < 3
  mecho ������ ���� ������ ����� �, ��������� ��������� ���������, ���� ������ ������!
  mload obj 11906
  ���� ��� %actor.name%
  break
else 
 mecho ������ ���� ������ ����� �, ��������� ��������� ��������� ����, ������ ���-��, � ������� ���� � ������. 
  wait 1s
  say ��� ��� ������ ����������, � ���� � ������� �� �������.
 *��������� ��� �����
  if %actor.class% == 3 && !%actor.skill(������)%
  mskillturn %actor.name% ������ set
  else  
  %self.gold(500)%
  ���� 500 ��� %actor.name%
  end
end  
detach 11900 %self.id%
end
~
#11901
�����������~
0 q 100
~
wait 1
say ��� ���� ��������!
say � �� ���� ��?
say ������ �� �������� �� ����... 
say ������ � ��������� �������� �� ������������...    
if %actor.class% == 3
wait 2s                                               
���� %actor% 
say �� ��� ���? �� ���� �����, ���� ������...����� ������.
say ����� ������... �� ��� ����������� ����� ��������. 
say � � ����, ��� � ��� ���� ���������  ������ ������, ������ ��� ��� ������.
say ���� ���������� ������� � � ����� �� ��������...
say ���� �� ���������� ��������� ����� ������� ��� �����.
say �� ��� ���, ��������, ������� ���� ��������? 
attach 11902 %self.id%
else
say � �� ���� ����� � ���� � ���� ��� ������ �� �����.
wait 3s
end
~
#11902
���� ���� ������~
0 d 1
�� ������~
if %actor.class% != 3
   say ���� � ���� ����� �����������, ������ ��� ������ �������.
   halt
end
wait 1
say ����� �� �� ��� �� ����������.
say � � ���� �������� ������ �� ������� ����.
say � ���� ���������� �� ������... 
mteleport %actor% 11909
attach 11900 %self.id%
detach 11901 %self.id%
detach 11902 %self.id%
~
#11903
�������~
2 c 1
������� ��������� ���������~
if !%arg.contains(�����)% 
   return 0
   halt
end
wait 1
wdoor 11953 north room 11960
wsend %actor.name% ������ ������ �� ������� � ����� ������� ������.
wechoaround %actor% %actor.name% ������ ������ �����%actor.q% �����, ������ ������� ������.
detach 11903 %self.id%
~
#11905
����� �������~
2 f 100
~
wdoor 11953 s purge
attach 11903 %self.id%
~
#11906
����� ������� �������~
2 f 100
~
attach 11903 %self.id%
~
#11907
����� ������~
0 n 100
~
attach 11901 %self.id%
attach 11902 %self.id%
~
#11908
����� �������� ����~
0 f 100
~
mload obj 11908
~
#11909
������ �����~
0 f 100
~
mload obj 11907
~
$~
