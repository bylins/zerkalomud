#12300
������ ������~
0 q 50
~
wait 1
������ %actor.name%
~
#12301
�������� �������.~
0 b 25
~
%echo% �������� ���-�� ������� ����� � ����� ���������� ������������
~
#12302
���~
0 b 25
~
���
~
#12303
�����~
0 b 25
~
������
~
#12304
����~
0 b 25
~
����
~
#12305
���~
0 b 15
~
���
~
#12306
������� ������~
0 b 25
~
%echo% ������� �������� ������� � ���������� �� ���� ������� ����� ����� ������.
~
#12307
���� �� �����~
2 e 100
~
wait 2
wsend %actor.name% �� �� ���������! 
wsend %actor.name% ��� ��� ����� ����������� ���� �������, � ��� ���������. 
~
#12308
����� � �����~
2 c 100
�����~
wait 1
wechoaround %actor% %actor.name% ���� �� ��.
wsend %actor.name% _��� ��������� � ���������.
wsend %actor.name% _�� ����� �� ��.
wteleport %actor% 12338
wat 12338 wechoaround %actor% %actor.name% ������ � ������.
~
#12309
�������� ������� �� ���.~
0 r 25
~
if %actor.name%==�����
  ������ %actor.name%
else
  ����� %actor.name%
end
~
#12310
�������� ����~
2 c 100
��������~
if ((%actor.clan% != ���) || (%actor.clanrank% < 9))
  return 1
  halt
end
wait 1
if (%arg.contains(�����)% && (%world.curobjs(12320)% < 25 ))
  wecho ������� �������� � ������ ���� � ������ ����� ��������� �����.
  wload obj 12320
  halt
elseif (%arg.contains(������)% && (%world.curobjs(12321)% < 25 ))
  wecho ������� �������� � ������ ���� � ������ ������ ��������� �����.
  wload obj 12321
  halt
elseif (%arg.contains(�����)% && (%world.curobjs(12322)% < 25 ))
  wecho ������� �������� � ������ ���� � ������ ����� ��������� �����.
  wload obj 12322
  halt
elseif (%arg.contains(��������)% && (%world.curobjs(12323)% < 25 ))
  wecho ������� �������� � ������ ���� � ������ �������� � �������� ��������� �����.
  wload obj 12323
  halt
end
wait 1s
wsend %actor% ...� ������ �� ���������.
~
#12311
�������� �����.~
0 m 0
500~
if (%world.curobjs(12305)% <= 300)
  �� %actor.name% ������ �������.
  �� %actor.name% �� ������ ��� ��� ����� ���������� � ��������.
  mload obj 12305
  ���� ������ %actor.name%   
else
  say ���� ������ - ���������!
end
~
#12312
���������� �����.~
1 c 3
����������~
if ((%actor.clan% == ��� ) && (%world.curmobs(12311)% <= 50))
  oechoaround %actor% %actor.name% ��������� ���������%actor.g% �����.
  osend %actor% �� ��������� ���������� �����.
  oload mob 12311
  wait 1
  opurge %self%
else
  oechoaround %actor% %actor.name% �������%actor.u% ���������� �����, �� ������ �� ����������.
  osend %actor% �� ���������� ���������� �����, �� ������ �� ����������.
end
~
#12313
�������� �����.~
0 c 0
��������~
if (%actor.clan%==���)
  mechoaround %actor% %actor.name% ��������� ������%actor.g% �����.
  msend %actor% �� ��������� ������� �����.
  mload obj 12305
  calcuid target 12311 mob 
  mpurge %target%
else
  mechoaround %actor% %actor.name% �������%actor.u% ������� �����, �� ��� �������� �� ���.
  msend %actor% �� ���������� ������� �����, �� �� �������� �� ����� ���.
end
~
#12314
���� �����~
0 m 1
~
wait 1
��� %actor.name%
halt
if %amount% < 20
  msend %actor.name% �� ����� ������ � ���� ������� �� ��������!
  halt
endif
switch %amount%
  case 20
    mecho ����� �������� ������.
    dg_cast '��������� ���������' %actor.name%
  break
  case 50
    mecho ����� �������� ������.
    dg_cast '������' %actor.name%
  break
  case 60
    mecho ����� �������� ������.
    dg_cast '�������������' %actor.name%
  break
  case 70
    mecho ����� �������� ������.
    dg_cast '����' %actor.name%
  break
  case 80
    mecho ����� �������� ������.
    dg_cast '�����' %actor.name%
  break
  default
    msend %actor% ��������, �� � �� ���� ��� �� ������ �� ���� �� ��� ������.
    msend %actor% ��� ��� � ���� ������� ��� ���:
    msend %actor% 20 ��� - ��������� ���������.
    msend %actor% 50 ��� - ������.
    msend %actor% 60 ��� - �������������.
    msend %actor% 70 ��� - ����.
    msend %actor% 80 ��� - �����.
  done
~
#12315
�� �����~
2 c 100
������~
wait 1
halt
if (%actor.vnum% != -1)
  halt
end
if %arg.contains(�������)%
  eval roomtel 4000+%random.61%
  wait 1
  wsend %actor% ��� ��������� ��������� ����� � ����� ��� � ��������� �����������.
  wechoaround %actor% %actor.name% ����� � ��������� �����.
  wteleport %actor.name% %roomtel%
  wforce %actor.name% ��������
  wsend %actor% ������ ���������, ����� ��������� ���������� ������������.
  wechoaround %actor% ���������� ������ ������� %actor.rname% � ������.
  eval hit %actor.hitp% + 15
  wdamage %actor% %hit%
  halt
end
if %arg.contains(�������)%
  eval roomtel 27000+%random.69%
  wait 1
  wsend %actor% ��� ��������� ��������� ����� � ����� ��� � ��������� �����������.
  wechoaround %actor% %actor.name% ����� � ��������� �����.
  wteleport %actor.name% %roomtel%
  wforce %actor.name% ��������
  wsend %actor% ������ ���������, ����� ��������� ���������� ������������.
  wechoaround %actor% ���������� ������ ������� %actor.rname% � ������.
  eval hit %actor.hitp% + 15
  wdamage %actor% %hit%
  halt
end
if %arg.contains(��������)%
  eval roomtel 33060+%random.6%
  wait 1
  wsend %actor% ��� ��������� ��������� ����� � ����� ��� � ��������� �����������.
  wechoaround %actor% %actor.name% ����� � ��������� �����.
  wteleport %actor.name% %roomtel%
  wforce %actor.name% ��������
  wsend %actor% ������ ���������, ����� ��������� ���������� ������������.
  wechoaround %actor% ���������� ������ ������� %actor.rname% � ������.
  eval hit %actor.hitp% + 15
  wdamage %actor% %hit%
  halt
end
wsend %actor% ��������� ����� ���� �� ������� ��� � �����!
wsend %actor% ���������� ���� ����! ��� � ������� �������.
wechoaround %actor% ��������� ����� ���� �� ������� %actor.name% � �����.
~
#12316
�������� ���� ��� ���� 12397~
1 ghjlt 100
~
wait 1
if %actor.sex% == 2
  halt
endif
oecho ���� ������������� � ������.
wait 1
opurge %self%
~
#12317
check DSS item~
1 ghi 100
~
wait 1
if %actor.clan% == ���
  halt
endif
oecho �������� ������ ������ �������.
wait 1
%purge% %self%
~
#12318
������� ������~
1 gj 100
���������~
wait 1
if %actor.level%>30
  halt
end
if %actor.name% == �������
  foreach i %self.pc%
    if %i.name% != �������
      osend %i% �������� ����������� �� ������ �������� �������� �����������.
    end
    * comm 10
  done
  osend %actor% ����������� �� ������ �������� �����������.
  halt
endif
osend %actor% ����� � �������� ������������ �������� �� ����� ���.
osend %actor% ����� ������� �������� ��� �� ���������.
wait 1
%purge% %self%
~
#12319
�������� ���������~
0 z 100
~
foreach pers %self.pc%
  if %pers.name% == �������
    if %pers.canbeseen%
      msend %actor% ����������� �� ������ � ����� �������� �������� �����������.
    end
  end
done
~
#12320
����� ��� �� ������� �����������~
1 g 100
~
if ((%actor.name% == ����) && (%actor.clan% == ���))
  *if %actor.clan% == ���
  halt
end
return 0
osend %actor% _��� ������� ��� ������� ����� ���� ��������� �����!
oechoaround %actor%  %actor.name% �������%actor.u% ����� ���� ��������� ����� - � ����� �� ������%actor.g%!
~
#12321
��������� � �����~
1 t 100
~
wait 1
if %actor.clan% != ���
  osend %actor% &C�� �������� ������������� �� ����� ���� ��������� �����.&n
end
~
$~
