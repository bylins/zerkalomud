#75000
�������� �������� ��������~
0 g 100
~
switch %actor.sex%
  case 1
    ��� %actor.iname%
    wait 10
    if %actor.eq(6)%
      � ��� � ��, %actor.iname%, ��� �������� �����...
      � � ����� ���� � ����� �������!
      � ����� �� ��������� - ��� ���� ���� ���������.
      if %direction% == west
        wait 1
        msend %actor% ��������� ������� ��������, �� �������� ����.
        mechoaround %actor% ��������� ������� ��������, ~~%actor.iname% ������� ����.
        mteleport %actor% 75044
        mechoaround %actor% ~~%actor.iname% ����� �� �����, ���������.
      end
    end
    break
  case 2
    ��� %actor.iname%
    wait 10
    if !%actor.eq(6)%
      � ��� � ��, %actor.iname%, ��� �������� �����...
      � � ����� ���� � ����������� �������� �������!
      � ����� �� ��������� - ��� ���� ���� ���������.
      if %direction% == west
        wait 1
        msend %actor% ��������� ������� ��������, �� �������� ����.
        mechoaround %actor% ��������� ������� ��������, %actor.iname% �������� ����.
        mteleport %actor% 75044
        mechoaround %actor% %actor.iname% ����� �� �����, ���������.
      end
    end
    break
  default
    ��� %actor.name%
    wait 10
    :������� ����������
done
~
#75001
����� �������� ��������~
0 g 100
~
switch %actor.sex%
  case 1
    ��� %actor.iname%
    wait 1
    if %actor.eq(6)%
      � ��� � ��, %actor.iname%, ��� �������� �����...
      � � ����� ���� � ����� �������!
      � ����� �� ��������� - ��� ���� ���� ���������.
      if %direction% == south
        wait 1
        msend %actor% ��������� ������� ��������, �� �������� ����.
        mechoaround %actor% ��������� ������� ��������, ~~%actor.iname% ������� ����.
        mteleport %actor% 75046
        mechoaround %actor% ~~%actor.iname% ����� �� �����, ���������.
      end
    end
    break
  case 2
    ��� %actor.iname%
    wait 1
    if !%actor.eq(6)%
      � ��� � ��, %actor.iname%, ��� �������� �����...
      � � ����� ���� � ����������� �������� �������!
      � ����� �� ��������� - ��� ���� ���� ���������.
      if %direction% == south
        wait 1
        msend %actor% ��������� ������� ��������, �� �������� ����.
        mechoaround %actor% ��������� ������� ��������, %actor.iname% �������� ����.
        mteleport %actor% 75046
        mechoaround %actor% %actor.iname% ����� �� �����, ���������.
      end
    end
    break
  default
    ��� %actor.name%
    wait 1
    :������� ����������
done
~
#75002
������� �����~
1 c 3
������~
if %arg% != �������
   osend %actor% ��� ��� �� ������ ���������?
else                  
   osend %actor% �� ������� �������. ���-����!
   oechoaround %actor% ~~%actor.iname% � ��������� ������%actor.g% �������.
   eval lizalki %lizalki%-1 
   global lizalki
   if %lizalki% == 0
      osend %actor% �� ����� ������� � �� ������� ��������� ������ �������.
      oechoaround %actor% ~~%actor.iname% ����%actor.g% ������� � �� ������� ��������%actor.g% ������ �������.
      oload obj 75009
      opurge �������
   end
end
~
#75003
������� ��������~
1 n 100
~
set lizalki 6
global lizalki
            
~
$~
