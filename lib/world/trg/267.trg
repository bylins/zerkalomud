#26700
�������� ������� �����~
2 c 0
�������� ������ ������ ���� ������� ��������~
if !%arg.contains(������)%
  wsend %actor% ��, � ���� ����-� ?
  halt       
end
if %actor.str% < 27 
  wsend %actor% ��������� �� ���, ����� ������� ��� �����.
  wechoaround %actor% %actor.name% � ������ ��������� �� �����, �� ������ �� ���� �������.
else if 
  wdoor 26700 east purge
  wdoor 26700 east room 26701
  wsend %actor% ������ ������ ����� �� ������ �����.   
  wechoaround %actor% %actor.name% ������ ������ ����� ����� �����.  
  detach 26700 %self.id%
end
~
#26701
����������� �� ��������~
2 e 100
~
if %actor.move% > 100 
  wsend %actor% � ������, �� �� ��������� �� ��������.
  %actor.move(-99)%
else
  wait 1s
  wsend %actor% ������ ������ � ��� ��������� ����, �� ��, ��� � �� ��������� 
  wsend %actor% �����, ����� � ������ ���������.
  wechoaround  %actor% %actor.name% �� ��������� ��� ��������� �� ��������,   
  wechoaround  �� �� ���� ���������� � ���� ����.      
  
  wteleport %actor% 26716
  %actor.move(-99)%
  mdamage %actor% 100
end
~
#26702
������� ������ � ���~
0 k 100
~
dg cast !������� �����!  %target.name%
~
#26703
������ ����� � ���~
0 k 100
~
������ ������
~
#26704
������ ����� ����~
0 f 100
~
if (%world.curobjs(26701)% < 3) && (%random.100% <= 5)
  mload obj 26701
  mecho ����� ����������� � �������, ������� �� ����� ����.        
end
if (%world.curobjs(26700)% < 3) && (%random.100% <= 5)
  mload obj 26700
  mecho ����� ����������� � �������, ������� �� ����� ��������� ���.        
end
~
#26705
���� � ���~
0 k 100
~
dg cast !��� ����!
if %random.100% < 10 
  mecho ��� ����� ������ ����, �������� � ��������� ����.
  mdamage %actor.name% 75
end
~
#26706
����������� ��������~
0 c 0
���������~
say  ������� ����, %actor.name% �� ������������.
say ����� ��� �� ���� � ���.
%actor.gold(+300)%   
mecho ������� ��� ��� 300 ���.
say � � ������ ������ ������.
mecho ������� ����� � ������� �� �������.
%purge% %self%
end
~
#26707
������� �����  1~
2 c 0
������� ������ ������ ��������~
if !%arg.contains(�����)%
  wsend %actor% ��� �� ������ �������?
  halt
end
if %random.4% < 4
  wsend %actor% �� ������ �������� �� ����� � ����� ������������, ���������    
  wsend %actor% ������.
  wechoaround %actor% %actor.name% ������ ������� �� ����� � ����� ������������ 
  wechoaround %actor% � �������.  
  wdoor 26727 south purge  
  wdoor 26727 south room 26729  
  detach 26707 %self.id%
else       
  wsend %actor% �� ������������ � ������ ����, �������� �������� � ������ � 
  wsend %actor% ���� � �����-�� ������ ������ �������.
  wechoaround %actor% %actor.name% ����������� � ������ ���� � ����� �� ����.   
  wteleport %actor.name% 26780
end
~
#26708
���� ����������� �����~
0 f 100
~
mecho ����� �������� �� ��������� ��������.
mload  mob 26706
mload  mob 26706
mload  mob 26706  
if (%world.curobjs(26705)% < 4) && (%random.100% <= 5)
  mload obj 26705
end
~
#26709
������� ����� 2~
2 c 0
������� ������ ������ ��������~
if !%arg.contains(�����)%
  wsend %actor% ��� �� ������ ������� ?
  halt
end
if %random.3% < 3    
  wsend %actor% �� ������ �������� �� ����� � ����� ������������,
  wsend %actor% ��������� ������.
  wdoor 26734 south purge
  wdoor 26734 south room 26736
  mechoaround %actor% %actor.name% ������ ������� �� �����, ����� ���� �����
  mechoaround %actor%������������, ��������� ������.
  detach 26709 %self.id%
else 
  wsend %actor% �� �������� ��������, ������������ � ������� ���� � �������� � 
  wsend %actor% �����-�� ������ �������. 
  mechoaround %actor% %actor.name% ����������� � ������� ���� � �����.
  wteleport %actor.name% 26780
end
~
#26710
�������� ����� ����.~
0 f 100
~
mecho ����� �������� �� ��������� ������ �����.
mload mob 26709
mload mob 26709  
mload mob 26709     
if  (%world.curobjs(26704)% < 4) && (%random.100% <= 5)
  mload obj 26704
end
~
#26711
���� �� �����~
2 c 0
���� ������ ��������~
wsend %actor% �� ������ �� �����, �� ����� ��� ������ � ���� ���������.
wechoaround %actor% %actor.name% ����� ���� �� �����, �� ���� ��� 
wechoaround %actor% ������������.
wdoor    26744 up purge       
wdoor 26744 up room  26753
detach 26711 %self.id%
~
#26712
��� ���� � �������~
2 e 100
~
wsend  %actor.name%  ����� ������� �������� ���� ����
wdamage %actor% 100
~
#26713
����� �����~
0 f 100
~
if (%world.curobjs(26703)% < 2) && (%random.100% <= 5)
  mload obj 26703
end
~
#26714
���� ��� ����~
0 f 100
~
if (%world.curobjs(26706)% < 5) && (%random.100% <= 5)
  mload obj 26706
end
~
#26715
������ ����������~
0 f 100
~
if (%world.curobjs(26707)% < 4) && (%random.100% <= 5)
  mload obj 26707
end
~
#26716
������ ��� � ���~
0 k 100
~
if %random.2% < 2
  dg cast !����!
elseif 
  dg cast !��� �����!
end
~
#26717
������� �����~
0 n 100
~
if (%world.curobjs(26708)% < 3) && (%random.100% <= 5)
  mload obj 26708
  ������ �����
end
~
#26718
������ 2~
0 d 1
�� ���� ����~
if %actor.level% >=28
  switch %actor.class%
    *������
    case 0
      if (!%actor.spelltype(������ �����)%) && (%actor.level%>=28) && (%random.100% <= 2)
        mspellturn %actor.name% ������.����� set
        %send% %actor% ������ ������ �� ������� ��� �����.
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      end
    break
    *������
    case 1
      if (!%actor.spelltype(�������������)%) & (%actor.level%>=28) & (%random.100% <= 2)
        mspellturn %actor.name% ������������� set
        say �, %actor.name%. �� ������� �� ������������� � ������� ��� �����!
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      end            
    break
    *����
    case 2
      if (%actor.skill(��������)% < 141) && (%actor.level%>28)
        mskilladd %actor.name% �������� 10 
        msend %actor.name% ������ �� ������ �������� ����� ������ � ����� � ������� ����������.
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      end
    break
    *��������
    case 3
      if %actor.skill(������)% < 141
        mskilladd %actor.name% ������ 10
        msend %actor.name% ������ ���� �� ����������..
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      end
    break
    *�������
    case 4
      if %actor.skill(�������)% < 141
        mskilladd %actor.name% ������� 10
        msend %actor.name% ������� ���� ����� ������ ������ ����� ����.
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      end
    break
    *����
    case 5
      if %actor.skill(����������)% < 141
        mskilladd %actor.name% ���������� 10
        msend %actor.name% ������� ������ ��������� �� ���� ����� �� �����������.
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      end
    break
    *��������
    case 6
      if (!%actor.spelltype(�������)%) && (%actor.level%>=28) && (%random.100% <= 2)
        mspellturn %actor.name% ������� set
        msend %actor.name%  ������ ������ ���������� ����� ��� ����.
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      break
      *����������!
      case 7
        if (!%actor.spelltype(�������� ����)%) && (%actor.level%>=28) && (%random.100% <= 2)
          mspellturn %actor.name% ����.���� set
          say �, %actor.name%.  ������� ���������� ���� ������ �����
        else
          msend %actor.name% �� ����� ������� �������.
          %actor.exp(+1000000)%
        end
      break
      *������
      case 8
        if (!%actor.spelltype(���� ���%) && (%actor.level%>=28) && (%random.100% <= 2)
          mspellturn %actor.name% ����.��� set
          say �, %actor.name%. ������ ������������� ���� ����� ���� ����������.
        else
          msend %actor.name% �� ����� ������� �������.
          %actor.exp(+1000000)%
        end
      break
      *������
      case 9
        if %actor.skill(������)% < 141
          mskilladd %actor.name% ������ 10
          msend %actor.name% ����� ���� ����� ��� �������!
        else
          msend %actor.name% �� ����� ������� �������.
          %actor.exp(+1000000)%
        end
      break
      *���
      case 10
        if %actor.skill(�����)% < 141
          mskilladd %actor.name% ����� 10
          msend %actor.name% ������ � ���� ���� ����� �� �������
        else
          msend %actor.name% �� ����� ������� �������.
          %actor.exp(+2000000)%
          
        end
      break
      *������
      case 11
        if %actor.skill(��������)% < 141
          mskilladd %actor.name% �������� 10
          msend %actor.name% ������ ������� ������� ������� �� �����!
        else
          msend %actor.name% �� ����� ������� �������.
          %actor.exp(+1000000)%
          �����
        end
      break
      *�����
      if %actor.skill(����������)% < 141
        mskilladd %actor.name% ���������� 10
        msend %actor.name% ������ ������ ��������� �� ���� �� ���������.
      else
        msend %actor.name% �� ����� ������� �������.
        %actor.exp(+1000000)%
      end
    break
    *�����
    case 13
      mload obj 226
      say ������ �������... ��� ����� ������ ����, � ������� � ������ ��� ���� �������
      ���� ���� %actor.name%
    break   
    default
      %self.gold(+100000)%
      ���� 100000 ��� %actor.name%
    break
  done
else
  say ��� �� ��� �������, ����� ������
  %actor.exp(+500000)
end
detach 99198 %self.id%
%purge% %self%
~
#26719
������� �����~
0 n 100
~
if (%world.curobjs(26709)% < 3) && (%random.100% <= 5)     
  mload obj 26709
  ���� ���
end
~
#26720
������ ������~
0 f 100
~
if (%world.curobjs(26710)% < 2) && (%random.100% <= 8)
  mload obj 26710
end
~
#26721
������������� ��� � ����~
0 k 100
~
if %random.100% < 60 
  dg_affect %actor.name% ���������� ���������� 2 2
  mdamage %actor.name% 100
  msend  %actor.name% �� ������� �� ����� �� ������� ����� ������
  mechoaround %actor% %actor.name% ����� �� ����� �� ������� ����� ������
end
~
#26722
����������������� ����� ����.~
0 f 100
~
mecho ����� �������� �� ��������� ������ �������.
if  (%world.curobjs(26711)% < 3) && (%random.100% <= 4)
  mload obj 26711
end
~
#26723
������� ����������1~
2 c 0
�������~
wsend %actor% ��� ���������� �� �����, ������ ��� ��������
wechoaround %actor% ��� ���������� �� �����, ������ %actor.rname% ��������.
wait 2
%actor.exp(+800000)
wechoaround %actor% �� ������ �������� �������.
wload mob 26718
%echo% ������ ���� ���, ����.
%echo% ��������� ����� ������ � ������ ������ � ���� ����.
%echo% � ��������� ������ ������ � � ����
%echo%   ���� �� �������, � �������� � ���� ����� ��������.
makeuid trg 26718 
detach 26723 %self.id%
~
#26724
reset~
2 f 100
~
calcuid room 26700 room 
attach 26700 %room.id%
calcuid room 26727 room
attach 26707 %room.id%
calcuid room 26734 room
attach 26709 %room.id% 
calcuid room 26744 room
attach  26711 %room.id%
calcuid room 26779 room
attach 26723 %room.id%
~
#26725
������ ������� �����~
0 f 100
~
if (%world.curobjs(26712)% < 4) && (%random.100% <= 4)
  mload obj 26712
end
~
#26726
������ ����������~
0 f 100
*~
if (%world.curobjs(26713)% < 4) && (%random.100% <= 4)
  mload obj 26713
end
if (%world.curobjs(26714)% < 4) && (%random.100% <= 4)
  mload obj 26714
end
~
#26727
������ ���������~
0 f 100
~
if (%world.curobjs(26715)% < 4) && (%random.100% <= 5)
  mload obj 26715
end
~
#26728
������ ������~
0 f 100
~
if (%world.curobjs(26716)% < 5) && (%random.100% <= 5)
  mload obj 26716
end
~
#26729
������ ������~
0 f 100
~
if (%world.curobjs(26717)% < 4) && (%random.100% <= 5)
  mload obj 26717
end
~
$~
