#27600
������ ����~
1 c 100
������~
context 276
if (%arg%==����) && (%actor.name%==%hero276%)
  osend %actor% �� ����� ��������� ������ � ����� ����. ��������� ���.
  oechoaround %actor% %actor.name% ����%actor.g% ��������� ������ � ����� ����. ��������� ���.
  oload obj 27609
  oforce %actor.name% ����� ������
  calcuid mpast 27610 mob
  detach 27603 %mpast.id%
  detach 27600 %self.id%
else
  osend %actor% � ��� ������?
end
~
#27601
��������~
0 q 100
~
wait 1
if %time.month% > 11 
  halt
end
if %time.month% < 3
  halt
end
if %world.curobjs(27610)%>0
  tell %actor.name% ������ ���� �������, �� ����� �� � ���� ���� �����?
  tell %actor.name% � ���� ��������� ���� ��� � ���.
end
~
#27602
������~
0 d 0
�� ������� yes~
wait 1
if %time.month% > 11 
  halt
end
if %time.month% < 3
  halt
end
wait 1
context 276
eval hero276 %actor.name%
if %exist.mob(27609)%
  calcuid z276 27609 mob
  remote hero276 %z276.id%
end
if %exist.mob(27613)%
  calcuid u276 27613 mob
  remote hero276 %u276.id%
end
if %exist.mob(27610)%
  calcuid p276 27610 mob
  attach 27603 %p276.id%
  remote hero276 %p276.id%
end
tell %actor.name% �������, � ������� ����� ������ �������.
tell %actor.name% �� �������� �������, ��� ��������� ����� �������� �� �����
tell %actor.name% ������ �� �����������.
tell %actor.name% � ��� ��� ������ ��� �������� ������, ��������.
tell %actor.name% � ����� �� ������� ���� � ������� ���� ���� ������.
tell %actor.name% ����� � ��������, ������ � ���, ����� �� ��� ���� ����
tell %actor.name% ���������� ��� �����.
detach 27601 %self.id%
detach 27602 %self.id%
~
#27603
����������~
0 q 100
~
wait 1
context 276
if (%actor.name% == %hero276%) && (%actor.realroom%==27645)
  wait 1
  tell %actor.name% ����! ���������, ������ ��� ����!
  tell %actor.name% � ������ ��� �� ��� �������, �������� ���������.
  tell %actor.name% ���, ��� �� ��� ���������?
  tell %actor.name% ���� ���� ����? � ��� ����� ��� ��.
  tell %actor.name% ��, ���������� ������?
  tell %actor.name% �� ���� ����� ��������.
  wait 2
  ���
  wait 2
  tell %actor.name% �� �������� ������ ����!
  tell %actor.name% ���������� ������ ����, ��������� �����.
  tell %actor.name% �� ������ � �� ���, � ���-�� ������� ������ �� �����.
  tell %actor.name% ������ ���� ������ �� �������. �����, ���� ����.
  tell %actor.name% ����� � �������.
  calcuid hobj 27608 obj
  attach 27600 %hobj.id%
  attach 27605 %self.id%
  remote hero276 %hobj.id%
  detach 27603 %self.id%
end
~
#27604
���������~
2 f 100
~
context 276
unset hero276
unset s276
unset u276
unset p276
unset z276
if %world.curobjs(27610)%>0
  calcuid hmob 27609 mob
  attach 27601 %hmob.id%
  attach 27602 %hmob.id%
  detach 27606 %hmob.id%
  calcuid hmob 27613 mob
  detach 27607 %hmob.id%
  calcuid mpast 27610 mob
  detach 27603 %mpast.id%
  detach 27605 %mpast.id%
  calcuid hobj 27608 obj
  detach 27600 %hobj.id%
else
  calcuid hmob 27609 mob
  dettach 27601 %hmob.id%
  dettach 27602 %hmob.id%
end
~
#27605
����������������~
0 j 100
~
context 276
wait 1
if (%object.vnum%!=27609)
  ���� %object.name% %actor.name%
  tell %actor.name% ����� ��� ���?
  halt
end
if (%object.vnum%==27609) && (%actor.name% != %hero276%)
  mpurge ������
  tell %actor.name% ������� �� ������!
  tell %actor.name% ������ �� ������ ��� ��� ���? � ���� ����� ������� �� ����?
  �����
  mkill %actor.name%
  detach 27605 %self.id%
  halt
end
if (%object.vnum%==27609) && (%actor.name% == %hero276%)
  mpurge ������
  tell %actor.name% �� ��� ����, �������!
  tell %actor.name% ������� ���� �����, � ������ ������ ������ ����� �������.
  wait 1s
  tell %actor.name% �� ���� ����� �����, ������ ���� ��� ������!
  tell %actor.name% � ����� ����� ���������, ���� ���� �������� ��� ������ ���.
  tell %actor.name% ��� ����� �������.
  ����
  calcuid hmob 27609 mob
  attach 27606 %hmob.id%
  remote hero276 %hmob.id%
  detach 27605 %self.id%
end
~
#27606
�����������������~
0 q 100
~
wait 1
context 276
if %actor.name% == %hero276%
  wait 1
  tell %actor.name% �������! �� �������� ��� �������, ����� ��� ���������
  tell %actor.name% ����� ���� ����������. �� � � ��������� ��� ��������.
  tell %actor.name% ��� ������, ��� ������, ��� � ����.
  %self.gold(+50)%
  ���� 50 ��� %actor.name%
  wait 1
  if %world.curobj(27614)%==0
    ���
    tell %actor.name% ��� �� ���� ������ ���� ��� ������ �� � ������.
    tell %actor.name% �� ������� ��� �� ��� ��� ����� ����������, �� ���� ��������.
    tell %actor.name% ����� � ���� ����� ���� �������� �������� �����������.
    tell %actor.name% ��� ��� ������� ����� ����� ���������, ���� �����������.
    tell %actor.name% �� ������ ���� ������ ����� ������, ���������� ��������
    tell %actor.name% ��������, �����, �� ������� � ������� ������������ ������.
    tell %actor.name% �� ��� �������� ��� ��� ����������� �����.
    wait 1
    tell %actor.name% � �����, ��� ���� ����, �����, �������� ��� ���.
    tell %actor.name% �� � ���� ���� �������, � �� ����, ���� ����� �� ������.
    tell %actor.name% ���� ������� ����� � �������� ������ � �� ���������.
    mload obj 27614
    ���� ���� %actor.name%
    calcuid hmob 27613 mob
    attach 27607 %hmob.id%
    remote hero276 %hmob.id%
    ������
  end
  detach 27606 %self.id%
end
~
#27607
�������������~
0 q 100
~
wait 1
context 276
if (%actor.name% == %hero276%) & (%actor.realroom%==27663)
  wait 1
  ���� 150 ��� %actor.name%
  � ������ ����!
  � �� ������, ��� ������ ��� ���� ������ ����?
  � ������� �� ������������!
  � � ��� ������� �������������, ��� ���� �� ���� �����,
  � �� ����, ��� ��� ����� ������� ������.
  � �� ���� ��� ���������� ���� �����.
  � ������� � ���� � �����:
  if %world.curobjs(517)%==0
    � 100  ��� : ������� �������������
  end
  if %world.curobjs(516)%==0
    � 500  ��� : ��������
  end
  if %world.curobjs(512)%==0
    � 2000 ��� : �����
  end
  if %world.curobjs(527)%==0
    � 200  ��� : ��������� ���������
  end
  if %world.curobjs(518)%==0
    � 400  ��� : ��������
  end
  if %world.curobjs(538)%==0
    � 2500 ��� : �����
  end
  if %world.curobjs(500)%==0
    � 1500 ��� : �������
  end
  if %world.curobjs(543)%==0
    � 1000 ��� : ��������� ����
  end
  if %world.curobjs(537)%==0
    � 5000 ��� : �������� �����
  end
  � ������� ������, �� � ������.
  � ���������, ��� ����� ������, �� � ������ ������� �������� �������.
  attach 27611 %self.id%
  calcuid hmob 27613 mob
  remote hero276 %hmob.id%
  detach 27605 %self.id%
end
~
#27608
����������~
0 f 100
~
if (%world.curmobs(27614)%==1) && (%world.curobjs(27614)%==0)
  context 276
  set hero276 %actor.name%
  calcuid u276 27613 mob
  attach 27607 %u276.id%
  remote hero276 %u276.id%
  mload obj 27614
end
~
#27609
�����������~
0 f 100
~
if (%world.curobjs(535)%==0) && (%random.5%==1)
  mload obj 535
end
~
#27610
�������~
1 g 100
~
if %world.curmobs(27614)%>0 && %world.curmobs(27617)%==0
  calcuid hmob 27614 mob
  opurge %hmob.name%
  oload mob 27617
  oechoaround %actor.name% ���� ����� ��������� � ����� �� %actor.name%.
  osend %actor.name% ���� ����� ��������� � ����� �� ���.
end
~
#27611
��������������~
0 m 100
~
context 276
wait 1
switch %amount%
  if (%actor.name% == %hero276%) && (%actor.realroom%==27663)
    case 100
      if %world.curobjs(517)%==0
        mload obj 517
        ���� �������� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 100 ��� %actor.name%
      end
    break
    case 200
      if %world.curobjs(527)%==0
        mload obj 527
        ���� ����� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 200 ��� %actor.name%
      end
    break
    case 400
      if %world.curobjs(518)%==0
        mload obj 518
        ���� ����� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 400 ��� %actor.name%
      end
    break
    case 500
      if %world.curobjs(516)%==0
        mload obj 516
        ���� ������� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 500 ��� %actor.name%
      end
    break
    case 1000
      if %world.curobjs(543)%==0
        mload obj 543
        ���� ����� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 1000 ��� %actor.name%
      end
    break
    case 1500
      if %world.curobjs(500)%==0
        mload obj 500
        ���� ����� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 1500 ��� %actor.name%
      end
    break
    case 2000
      if %world.curobjs(512)%==0
        mload obj 512
        ���� ����� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 2000 ��� %actor.name%
      end
    break
    case 2500
      if %world.curobjs(538)%==0
        mload obj 538
        ���� ����� %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 2500 ��� %actor.name%
      end
    break
    case 5000
      if %world.curobjs(537)%==0
        mload obj 537
        ���� ������ %actor.name%
      else
        � ������, ����� ������ ������� ����� � ������� ��� ����� ������.
        ���
        ���� 5000 ��� %actor.name%
      end
    break
    default
      � �������������.
      ���
    break
  done
  � ���, � ����� �������� ������ ����.
  calcuid hmob 27613 mob
  mpurge %hmob.name%
  detach 27611 %hmob.id%
else
  say ����� �� ������������� �� ����� �� ����.
  ���� %amount% ��� %actor.name%
end
~
#27612
����������������~
1 g 100
~
wait 1
if (%random.5%<2) && (%world.curobjs(27619)%<51
  osend %actor.name% ������, � ������� ������� ����� ��� ���������� � �������.
  osend %actor.name% �� ������ ������� ������, ����� ������� ��� ����� ������
  osend %actor.name% ����������� � ������� ������ ��������.
  oload obj 27619
  oforce %actor.name% ����� ��������
  calcuid hobj 27613 obj
  opurge %hobj.name%
else
  osend %actor.name% �� ����������� ������� �������� �� ������, �� ������ ��������� ��.
  calcuid hobj 27613 obj
  opurge %hobj.name%
end
~
#27613
����� ��������~
0 f 100
~
if %world.curobjs(542)% < 1
  if %random.100% < 41
    mload obj 542
  end
end
~
$~
