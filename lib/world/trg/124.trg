#12400
���� � ����������~
2 c 1
������~
wait 1
if !%arg.contains(�������)%
halt
end
if %actor.rentable% & (%actor.clan% == �� )
wsend %actor% _�� ������ ������ ������� �� ������.
wechoaround %actor% %actor.name% ������� ����� ������.
wteleport %actor% 12645
wechoaround %actor% %actor.name% �������� ������-�� ������.
halt
end
wsend %actor.name% � ������ �� ���������...
~
#12401
���� �� �����~
2 c 0
��������~
if !(%arg.contains(����)%)
wechoaround %actor% %actor.name% ����� �������%actor.u% ��������� ����.
wsend %actor.name% ��� ������ ������ ������ ����������.
   halt
end
   wechoaround %actor% %actor.name% �������%actor.u% �� �������.
   wsend %actor.name% �� ������ ����������� ����, ������������ ������� ����  - ���� ������� ������,
   wsend %actor.name% �������, ���� ��������� ��� �� ������������ �����, ������ �������� ��������������
   wsend %actor.name% ���������� ������.
~
#12402
���� �� ����~
2 e 55
����~
wait 1
wechoaround %actor% %actor.name% ��������� � � ������ ����� ��������%actor.u% ����.
wsend %actor% ����� �� ���������� � � ������ ����� �������� ����.
wsend %actor% ���... ����!!!
wsend %actor% ������ ���� �����, �� ����������� � ������ ������ �����, ������� �������
wsend %actor% ��� ���� �� �������. �� �� ����� - �� ����� ������� �� ��������� � ��������
wsend %actor% ������. �� ��� ������... 
wteleport %actor% 12429
     eval    dam124  %actor.hitp%*17/20
wait 1
     wdamage %actor% %dam124%
wsend %actor% �� ���� �� ������� � ������� ���� ���, � �������� ����.
wsend %actor% � ����������, �� ������� �� �������� �� ����.
wat 12429 wechoaround %actor% %actor.name% �� ������� � ������������ ����������%actor.u% �� �����.
~
#12403
����� � ������~
2 c 0
��������~
if !(%arg.contains(����)%)
wechoaround %actor% %actor.name% ����� �������%actor.u% ��������� ����.
wsend %actor.name% ��� ������ ������ ������ ����������.
   halt
end
   wechoaround %actor% %actor.name% �������%actor.u% �� ����.
   wsend %actor.name% �� ����� ������� ���� ��������. ����� � ��� ������ ��������� :
   wsend %actor.name% "�� ������ ����� ���� �������� ����������".
~
#12404
���� � ����~
2 c 0
�����~
if !(%arg.contains(����)%)
wechoaround %actor% %actor.name% ���������� �������%actor.u% ������� ����-��.
wsend %actor.name% �� ����������, � �� ����� ����, ���� ��������� �������.
   halt
end
   wechoaround %actor% %actor.name% �����%actor.g% �� ����������� � ������%actor.q% � ����.
   wsend %actor.name% ����������, �� �� ������������ ������� � ����.
wait 1s
wteleport %actor.name% 12444
   wsend %actor.name% ����� ��������� ����� ���� ����� ���������� �������,
   wsend %actor.name% � �� ����������� �� ���� ����.
~
#12405
���� ����~
0 j 100
~
if (%object.vnum% == 12400)
  wait 1
  mpurge %object%
  wait 1s
  tell %actor.name% ��, ��� ��� ����� ����������� ������� �����, %actor.name%!
  tell %actor.name% ������ � �������-�� ����� ������� ������!
  wait 10
  mecho ������ ����� ��������� ��� ������� ������. 
  mecho ���� ������� ����������� � �� ���.
  �����
  tell %actor.name% �������� ��� �� ������� ���� ������, ���� ������������ �� ��������.
  ���
    if %world.curobjs(12404)% < 10
      mload obj 12404
      tell %actor.name% ����� ���� ���� ����, ����� �� ���� ����������.
      ���� ���� %actor.name%
    end
  
else
  wait 1
  tell %actor.name% ������� ��� �������, ��� �� ������ ��� ������ � ���� �������.
  tell %actor.name% �� ��� �� �� ��� � ���.
  eval getobject %object.name%
  ����� %getobject.car%.%getobject.cdr%
  halt
end
~
#12406
���� � ������~
2 c 0
����������~
if !(%actor.haveobj(12404)%)
%send% %actor.name% - ��� ���?
   halt
end
if !(%arg.contains(����)%)
wechoaround %actor% %actor.name% ���������� �������%actor.u% ���-�� �������.
wsend %actor.name% �� ��� � �� ������ ��� ���� ������.
   halt
end
   wechoaround %actor% %actor.name% ���������%actor.g% ���� � ���������� �����.
   wsend %actor.name% �� ���������� ����, ����� �� ��������� ������ � �������, �������� ������. 
   wdoor 12403 north flags a
   wdoor 12403 north room 12470
wait 5s                        
wecho ����� �������� ������ �� �����.
   wdoor 12403 north purge
   
   
~
#12407
�����~
0 r 100
~
if (%actor.clan%==��) 
halt
end
mkill %actor.name% 
~
$~
