#31201
����� � ������~
0 r 100
~
wait 1
msend %actor% ����� ������ : '���� ������ �����, �� �� ��������� �������, ��� ����� �� ���.'
msend %actor% ����� ������ : '������� � ������� � ���� ������� ������� ���� �� ��������� �����.., ��� ��� �� 50'
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
#31205
�������� �������~
1 gh 100
~
wait 1
if (%actor.clan%==��)
  halt
else
  osend %actor.name% �������� ����� ������ %self.iname% �����%self.q%...
  wait 1
  %purge% %self% 
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
2 c 100
���~
wait 1
if ( %actor.clanrank% >= 2 )
  wait 1
  wsend %actor% ����� ����� ���� ��������� ������ � ����������� ����� ���������.
  wsend %actor% �������� ������� ��� � ���� : '������ ����� �� ���������� ����� ������� � ���� ���!'                          
  wsend %actor% �������� ��������� ����� ���� �����.
  halt
end
wechoaround %actor% %actor.name% ��%actor.y% �� ������.
wteleport %actor% 31225
wechoaround %actor% %actor.name% ����%actor.y% � ������.
~
#31208
������ � ����~
0 c 0
�������~
wait 1
if ((%actor.clan%==��) && (%self.leader% == %actor%))
  mechoaround %actor% %actor.name% ������%actor.g% �����.
  msend %actor% �� ������ ������� �����.
  mload obj 31233              
  ���� ������� %actor.iname%
  msend %actor% �������� ���� �������, ��������� ������ � ������.
  mechoaround %actor% �������� ���� �������, ��������� ������ � ������.
  wait 1 
  mpurge %self%
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
  wait 1 
  opurge %self%
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
if (%amount% < 50)
  msend %actor% �� �� ����� ������ ���� ������ �� ������, �� �� ����� ��� � ������� ����������.
  msend %actor% ��� ��, ��� �� ������. ����� 50 ��� � ���� ������.
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
#31219
����� ������~
2 g 100
~
wait 1
if %actor.vnum% == 31221
  wforce %actor% drop all
  wforce %actor% south
end
~
#31220
���� �����~
0 b 18
~
get all
set victim %random.pc%
switch %random.5%
  case 1
    ����
  break
  case 2
    � ��������� ���... � �� ������...
  break
  case 3
    ������� %victim.name%
  break
  case 4
    ������ %victim.name%
  break
  case 5
    if %victim%
      say ����� ��� ������, ������!
      say ����, ��, ���� �����!
      emot �������� ������ �� ����� %victim.rname%
    end
  break
done
if %random.10% != 7
  halt
end
while %self.realroom% != 31250
  switch
  case 31228
    north
  break
  case 31218
    case 31217
      case 31246
        case 31222
          east
        break
        case 31223
          case 31248
            case 31220
              case 31216
                case 31221
                  case 31203
                    west
                  break
                  case 31224
                    west
                    case 31207
                      west
                    break
                    case 31226
                      east
                      case 31205
                        east
                      break
                      case 31225
                        west
                        case 31210
                          west
                        break
                        case 31249
                          east
                          case 31245
                            east
                            case 31244
                              south
                              case 31243
                                up
                                case 31206
                                  east
                                break
                                case 31247
                                  south
                                  case 31219
                                    south
                                    case 31215
                                      ������� �����
                                      south
                                      ������� �����
                                      case 31214
                                        down
                                      break
                                      case 31227
                                        west
                                        case 31212
                                          west
                                          case 31204
                                            north
                                            case 31213
                                              west
                                            break
                                            case 31229
                                              south
                                              case 31200
                                                south
                                                case 31201
                                                  south
                                                  case 31202
                                                    south
                                                    case 31208
                                                      south
                                                      case 31209
                                                        west
                                                        case 31211
                                                          north
                                                        done
                                                        wait 2
                                                      done
                                                      south
                                                      west
                                                      west
~
#31221
������� �� ��������~
0 z 100
~
while %self.realroom% != 31250
  switch
  case 31228
    north
  break
  case 31218
    case 31217
      case 31246
        case 31222
          east
        break
        case 31223
          case 31248
            case 31220
              case 31216
                case 31221
                  case 31203
                    west
                  break
                  case 31224
                    west
                    case 31207
                      west
                    break
                    case 31226
                      east
                      case 31205
                        east
                      break
                      case 31225
                        west
                        case 31210
                          west
                        break
                        case 31249
                          east
                          case 31245
                            east
                            case 31244
                              south
                              case 31243
                                up
                                case 31206
                                  east
                                break
                                case 31247
                                  south
                                  case 31219
                                    south
                                    case 31215
                                      ������� �����
                                      south
                                      ������� �����
                                      case 31214
                                        down
                                      break
                                      case 31227
                                        west
                                        case 31212
                                          west
                                          case 31204
                                            north
                                            case 31213
                                              west
                                            break
                                            case 31229
                                              south
                                              case 31200
                                                south
                                                case 31201
                                                  south
                                                  case 31202
                                                    south
                                                    case 31208
                                                      south
                                                      case 31209
                                                        west
                                                        case 31211
                                                          north
                                                        done
                                                        wait 1s
                                                      done
~
#31222
�� ������ �����~
2 g 100
~
if %actor.vnum%  == 31221
  return 0
  halt
end
~
#31223
���� � �����~
2 c 1
���������~
if (!%arg.contains(� �����)% || ( %actor.name% != �������� ))
  return 0
  halt
end
wait 1
wsend %actor% �� �������� �������� ����� �������� ������ � ��������� ����� �� �������� ��������.
wteleport %actor% 31253
~
#31224
����� �� �����~
2 c 1
����������~
if !%arg.contains(� �����)%
  return 0
  halt
end
wait 1
wsend %actor% �� ���������� ���� �� �������� � ���� ������������ � �������� ������.
wteleport %actor% 31211
set exit 1
global exit
calcuid panther 31222 mob
remote exit %panther.id%
~
#31225
������� �������~
0 c 1
��������� ������~
if ((%cmd% == ���������) && !%arg.contains(�������)%)
  return 0
  halt
end
if ((%cmd% == ������) && !%arg.contains(� ��������)%)
  return 0
  halt
end
wait 1
if (%cmd% == ���������)
  mechoaround %actor% %actor.name% ������� ��������%actor.g% �������.
  msend %actor% �� ��������� ������� �� ����������� �������.
  wait 1s
  switch %random.5%
    case 1
      msend %actor% ������� �������� �������� � �������� ��������� � ���� ����.
      mechoaround %actor% ������� �������� �������� � �������� ��������� � ���� %actor.rname%.
    break
    case 2
      mecho ������� ���������� �� ������������, ���������� �������� ������ ��� ������.
    break
    case 3
      msend %actor% ������� ��������� �� ����� � �������� ������ ���� ���� ������� ������.
      mechoaround %actor% ������� ��������� �� ����� � �������� ������ ���� %actor.rname% ������.
    break
    case 4
      msend %actor% ������� ������� ������� ����� � ���� ����, ���� �� ��������� �� �����.
      mechoaround %actor% ������� ������� ������� � ���� %actor.rname%.
    break
    case 5
      msend %actor% ������� ������� ���� ���� ������, ���������� ������ ��� ��� �����.
      mechoaround %actor% ������� ������� ���� %actor.rname% ������, ���������� ��� ��� �����.
    break
  done
end
if (%cmd% == ������)
  mechoaround %actor% %actor.name% ������%actor.u% ������ � ��������.
  msend %actor% �� ��������� ������ � �������� �������.
  wait 1s
  switch %random.4%
    case 1
      msend %actor% ������� ������ �� �������� ����, � ������� ������, ������� ������� ���� ����.
      mechoaround %actor% ������� ������ �� �������� ����, � ������� ������, ������� ������� ���� %actor.rname%.
    break
    case 2
      mecho ������� � ���������� ����� ��������� � �������.
    break
    case 3
      msend %actor% ������� ������ ������� � ����� ���� ����.
      mechoaround %actor% ������� ������ ������� � ����� ���� %actor.rname%.
    break
    case 4
      msend %actor% ������� �������� �� �����, ������������ �� ����� ����.
      mechoaround %actor% ������� �������� �� �����, ������������ �� ���� %actor.rname%.
    break
  done
end
~
#31226
������ �������~
0 j 100
~
wait 1
set str %object.name%
set food %str.car%.%str.cdr%
if (%object.type% != 19) || (!%food.contains(���)% && !%food.contains(���)% && !%food.contains(�����)% && !%food.contains(�����)%))
  wait 1
  mecho ������� �������� �������.
  drop %food%
  halt
end
mecho ������� � ��������� ������ %object.vname%.
wait 1
mpurge %object%
wait 2s
mecho ������� ���������� � �������� ���������� �������.
~
#31227
���� ������~
2 c 1
������� ����������~
wait 1
if %cmd% == �������
  if !%arg.contains(�����)%
    wsend %actor%  ��� �� ������ �������?
    halt
  end
  if %exist.obj(31277)%
    wsend %actor%  ����� ��� ����� �����!
    halt
  end
  wsend %actor% �� �������� �����.
  wechoaround %actor% %actor.name% �������� �����.
  wload obj 31277
  halt
end
if %cmd% == ����������
  if !%arg.contains(�����)%
    wsend %actor%  ��� �� ������ ����������? �������? :)
    halt
  end
  if !%exist.obj(31277)%
    wsend %actor% ������ ��� ������������ ����� - ���� �� ��������� �����. :)
    halt
  end
  wsend %actor% �� ���������� ��������� ������� � �����.
  wsend %actor% ����� �������� � ����� �����.
  calcuid flame 31277 obj
  wpurge %flame%
  wload obj 31277
end
~
#31228
����� �����~
1 nz 100
~
oecho &r����� ���� ��������� � ������, �������� ������� � ������ ����� ��� �������.&n
wait 20s
oecho &r����� ������� �������� ������ � ������.&n
wait 40s
oecho &r�� ������ ������� ������� ����� �����.&n
wait 40s
oecho &r��������� �� ������ ����� ����� ������� ����.&n
wait 30s
oecho &r����� � ������ ������������, �������� ������� ����.&n
wait 45s
oecho &r����� � ������ ����-���� �������.&n
wait 50s
oecho &r������� ������ � ������� ������� ����������� �� ��������� ��������.&n
wait 40s
oecho &r����� � ������ �������� �������� �������.&n
wait 50s
oecho &r����� ������� ���������� �� ������� ���� � �������� � �������.&n
wait 50s
oecho &r����� ��������, � ��� ����� �� �������� �������.&n
wait 45s
oecho &r���� � ������ �����������, ���������� � ����.&n
wait 50s
oecho &r����� ����� ����.&n
wait 50s
oecho &r���� ���� ����� � ���������� ����� ������.&n
wait 50s
oecho &r����� �����.&n
wait 1
opurge %self%
~
#31229
������� ���������~
0 q 100
~
wait 1
if ((%actor.name% == ��������) && (%exit% == 1))
  set exit 0
  global exit
  �������
  ��� �������
  wait 2s
  emot �������� �� ����� �� �������
end
~
#31230
�������� �������~
0 n 100
~
set exit 1
global exit
~
$~
