#31200
��� �������~
0 m 1
~
wait 1
��� %actor.name%
halt
wait 1
if %amount% < 20
  msend %actor% �� ����� ������ � ���� ������� �� ��������!
  halt
end
switch %amount%
  case 20
    msend %actor% ������ ��� ���� ������ � ����� � ������� ��� �� ���.
    mechoaround %actor% ������ ��� ���� ������ � ����� � ������� ��� �� %actor.rname%.
    dg_cast '��������� ���������' %actor.name%
    break
  case 90
    msend %actor% ������ ��� ���� ������ � ����� � ������� ��� �� ���.
    mechoaround %actor% ������ ��� ���� ������ � ����� � ������� ��� �� %actor.rname%.        dg_cast '������' %actor.name%
    break
  case 100
    msend %actor% ������ ��� ���� ������ � ����� � ������� ��� �� ���.
    mechoaround %actor% ������ ��� ���� ������ � ����� � ������� ��� �� %actor.rname%.        dg_cast '�������������' %actor.name%
    break
  case 110
    msend %actor% ������ ��� ���� ������ � ����� � ������� ��� �� ���.
    mechoaround %actor% ������ ��� ���� ������ � ����� � ������� ��� �� %actor.rname%.        dg_cast '����' %actor.name%
    break
  case 120
    msend %actor% ������ ��� ���� ������ � ����� � ������� ��� �� ���.
    mechoaround %actor% ������ ��� ���� ������ � ����� � ������� ��� �� %actor.rname%.        dg_cast '�����������' %actor.name%
    break  
  case 130
    msend %actor% ������ ��� ���� ������ � ����� � ������� ��� �� ���.
    mechoaround %actor% ������ ��� ���� ������ � ����� � ������� ��� �� %actor.rname%.        dg_cast '����' %actor.name%
    break
  default
    msend %actor% ��������, �� � �� ���� ��� �� ������ �� ���� �� ��� ������.
    msend %actor% ��� ��� � ���� ������� ��� ���:
    msend %actor% 20 ��� - ��������� ���������
    msend %actor% 90 ��� - ������
    msend %actor% 100 ��� - �������������
    msend %actor% 110 ��� - ����
    msend %actor% 120 ��� - ����������� 
    msend %actor% 130 ��� - ����
done
~
#31201
����� � ������~
0 r 100
~
wait 1
msend %actor% ����� ������ : '���� ������ �����, �� �� ��������� �������, ��� ����� �� ���.'
msend %actor% ����� ������ : '������� � ������� � ���� ������� ������� ���� �� ��������� �����.'
~
#31202
���������� ���������~
0 q 100
~
wait 1
if (%actor.clan%==��)
  if (%actor.clanrank% >= 8)
    msend %actor% ��� ���� ���, �������� ��������� � �������.
    mechoaround %actor% ��� ���� %actor.rname%, �������� ��������� � �������.
  else 
    eval rand %random.3%
     if %rand%==1
        ���� %actor.name% "������ ��� ����, %actor.name%!"
        mechoaround %actor% �������� �������������� %actor.rname%.
      elseif %rand%==2
        ������ %actor.name% 
     else
        ������ %actor.name%
    end
  end
else
msend %actor% �������� ����� ��������� �� ���.
  mechoaround %actor% �������� ����� ��������� �� %actor.vname%.
end
~
#31203
�� ���������~
0 c 100
������~
wait 1
msend %actor% �� ������������ ��������� �� ������ ��� ������.
msend %actor% ��������� ����� ������ ��� ������ �������� ���� � �������.
%actor.wait(5s)%
wait 7s
*dg_cast '�����' %actor.name%
~
#31204
����� �� �����~
2 c 100
�����~
wait 1
if (%actor.vnum% != -1)
   halt
end
if (%arg.contains(�����)%)
  wait 1
  wsend %actor% �� ������� � ���� � ����� ����������� �� ����� �����.
  wechoaround %actor% %actor.iname% ����� � ���� � ����� �� ����. 
  wteleport %actor% 31245
  halt
end
~
#31205
�������� �������~
1 g 100
~
wait 1
if (%actor.clan%==��)
  halt
else
  osend %actor.name% �������� ����� ������ %self.iname% �����%self.q%...
  %purge% %self% 
end
~
#31206
�������� ������~
0 b 100
~
wait 5
eval rnd %random.10%
if %rnd%==1
  ��������
elseif %rnd%==2
  ������
elseif %rnd%==3
  �����
elseif %rnd%==4
  �����
elseif %rnd%==5
  ����������
elseif %rnd%==6
  �������
elseif %rnd%==7
  �����
elseif %rnd%==8
  ������
elseif %rnd%==9
  ������
else
  �����
end
~
#31207
��� ������ �������~
2 e 100
~
wait 1
if (%actor.clanrank% < 8)
 wait 2
 wsend %actor% ����� ����� ���� ��������� ������ � ����������� ����� ���������.
 wsend %actor% �������� ������� ��� � ���� : '������ ����� �� ���������� ����� ������� � ���� ���!'                           
 wsend %actor% �������� �������� ��� �� �����.
 wechoaround %actor% � ��� � ������� ������ �������� � � ������� ���������� �� %actor.rname%.
 wechoaround %actor% ������� ������ ��� ��� �������� �������� %actor.rname% �� �����.
 wait 2
 wteleport %actor% 31210
else
 halt
end
~
#31208
������ � ����~
0 c 0
�������~
wait 1
if (%actor.clan%==��)  
 mechoaround %actor% %actor.name% ������%actor.g% �����.
 msend %actor% �� ������ ������� �����.
 mload obj 31233              
 ���� ������� %actor.iname%
 msend %actor% �������� ���� �������, ��������� ������ � ������.
 mechoaround %actor% �������� ���� �������, ��������� ������ � ������.
 calcuid target 31203 mob 
 mpurge %target%
else
 mechoaround %actor% %actor.name% ������%actor.g% ����� � ��������, ��� ������ �� ����������.    
 mechoaround %actor% �������� ���� ��������� �� %actor.rname% � ������: '���� ��������%actor.u% �� ����������?'
 msend %actor% �������� ���� ��������� �� ��� � ������: '���� ��������%actor.u% �� ����������?'
end
~
#31209
������� � �������~
1 c 3
���������~
wait 1
if (%actor.clan%==��)
  osend %actor% B� ��������� � ����-�������.
  oechoaround %actor% �������� ������� �����! �� ��� %actor.rname% �������� �������� ������� � �����.
  osend %actor% �������� ������� �����! ������� �������� �� ����� ��� � �����.
  oecho � ������ �� �������� �������� �������� ����.
  oload mob 31203
  oforce %actor% �������� ����      
  calcuid svistok 31233 obj 
  opurge %svistok%
else
  oechoaround %actor% %actor.iname% �������%actor.u% ��������� � ����-�������, �� � ���� ������ �� ����������.
  osend %actor% B� ���������� ��������� � ����-�������, �� � ��� ������ �� ����������.
end
~
#31210
���� ������ ������~
0 m 1
~
wait 1
if (%amount% < 100)
 msend %actor% �� �� ����� ������ ���� ������ �� ������, �� �� ����� ��� � ������� ����������.
msend %actor% ��� ��, ��� �� ������. ����� 100 ��� � ���� ������.
else
 msend %actor% ����� ������������! ������� � ������� � ���-�� ������.
 msend %actor% ����� ������ : '��� ����� ���� �������! ������� � ���� � ���� ��� �������� � ����.'
 mload obj 31233
 ���� ������� %actor.name% 
end
~
#31212
�������� ��������� ������~
0 qr 100
~
wait 1
if (%actor.clan%==��)||(%actor.haveobj(31244)%)
 halt
else
������� �������! ����� � �������� !
msend %actor% �������� ��� �� ����� �� ������� �� ���.
 if %actor.class%==1
   ��������� .%actor.iname%
 else
   ��������� %actor.iname%
 end
end
~
#31213
������� ��������� ������~
0 qr 100
~
wait 1
if (%actor.clan%==��)||(%actor.haveobj(31244)%)
 halt
else
mecho ������� ������ ���������� ���� ������ ������� �� ������������ "����!".
msend %actor% ������� ����� ������ � ������ ������ ��� ���� ���� ��� �������� �� ���.
 if %actor.class%==1
   ��������� .%actor.iname%
 else
   ��������� %actor.iname%
 end
end
~
#31214
��������~
2 c 0
������~
halt
wait 1
if %arg.contains(����)%
  eval roomtel 49900+%random.70%
  wait 1
  wsend %actor% ��� ��������� ��������� ����� � ����� ��� � ��������� �����������.
  wechoaround %actor% %actor.name% ����� � ��������� �����.
  *wteleport %actor% %roomtel%
  wdamage %actor.name% %actor.exp%-10
  halt
end
wsend %actor% ��������� ����� ���� �� ������� ��� � �����!
wsend %actor% ���������� ���� ����! ��� � ������� �������.
wechoaround %actor% ��������� ����� ���� �� ������� %actor.rname% � �����.
~
#31215
����� � �����~
2 c 100
�����~
wait 1
if (%actor.vnum%!=-1)
   halt
end
if (%arg.contains(����)%)
  wait 1
  wsend %actor% �� ��������� ����� ���������� � ����� �����.
  wechoaround %actor% %actor.iname% ����� ����, ������ �����. 
  wteleport %actor% 31244
  halt
end
~
#31216
����� ���� ����~
0 c 100
����~
wait 1
if (%actor.clan%==��)
  if (%actor.clanrank%>=8)
  mload obj 31244
  ���� ���� %actor.iname%
  end
end
~
#31217
����� ������~
1 c 4
�����~
wait 1
if !%arg.contains(�����)% && !%arg.contains(������)%
return 1
halt
end
return 0
osend %actor% �� ������ ����� ������ ������.
oechoaround %actor% %actor.name% ������ ����%actor.g% ������ ������.
if (%actor.gold% < 10 ) || (%actor.hitp% < 51 )
oecho __...� ������ �� ���������!
halt
end
osend %actor% &R��� ������������� ������!&n
odamage %actor% 50
eval bufer %actor.gold(-10)%
wait 3
oecho ������ ������ ���������, � �� ��� �������� ������ �������.
if %world.corobjs(31269) < 3
oload obj 31269
end
~
#31218
���� ����~
1 n 100
~
wait 120s
oecho ������ �������� ����������.
wait 1
opurge %self%
~
$~
