#3700
������� ����� - ������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
  halt
end
if ((%actor.name% != ������) && (%actor.name% != �����))
  osend %actor% _������ ����� ������ �������� � ���� �� �������� ��� �����! 
  return 0
  halt
else
  wait 1
  oecho _����� ������ ����� ����������� �������� ������.
end
~
#3701
������� ����� - ������~
1 gjp 100
~
if (%actor.level% > 30 ) & (%actor.vnum% == -1 )
  halt
end
if %actor.name% != ������
  osend %actor%  _������������ �� ����� ����� � �������� ������ ���. 
  return 0
  halt
else
  wait 1
  oecho _����� ��������� ������ � ������� � �����.
end
~
#3702
������� ���� - ��������~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
  halt
end
if %actor.name% != ��������
  osend %actor% _���� ������� ���� ���� ����� � �� �� ������ �������� ��.
  return 0
  halt
else
  wait 1
end
~
#3703
������� ���� - �����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
  halt
end
if %actor.name% != �����
  osend %actor% &K����� ������ �������&n ��������� ������� ������ ����������� ������� � ���� ����-���� ����� �������.
  return 0
  halt
else
  wait 1
  oecho ��������� ������������ ��� �������� �� &K����� ������ �������&n � ����������� � ������ �������.
end
~
#3704
������� ���� - ���� (��������)~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.level% == -1))
  halt
end
if %actor.name% != ����
  osend %actor% &R�&Y�&G�&C�&B� &M�&R�&Y� &G�&G�&B�&M�&R�&Y�&n ��������� �� ����� ���!
  return 0
  halt
else
  wait 1
  oecho �� &R�&Y�&G�&C�&B� &M�&R�&Y� &G�&G�&B�&M�&R�&Y�&n ���������� �������� ����!
end
~
#3705
������� ���� - �����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
  halt
end
if ((%actor.name% != �����) && (%actor.name% != ������))
  osend %actor% _������� ������ ������������ � ��� ����� ������� � ���� �� �����.
  return 0
  halt
end
set owner %self.worn_by%
if ((%owner.name% == ����� ) || (%owner.name% == ������ ))
  halt
end
osend %actor% ������� ������ ����� ������������ ���������� ����.
oechoaround %actor% ������� ������ ����� ������������ ���������� ����.
~
#3706
������� ���� -- ��������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ��������)
  otransform 3706
  osend %actor%  ����� �� ����� ������� ���������� ����� ����-������� ����� � ������ �������� ������.
  osend %actor%  �� �������� ��������� ����.
  return 0
  halt
end
set char %self.worn_by%
if (%random.1000% <= 40 || (!%char% && %random.1000% <= 40))
  wait 1
  oecho _&W����� �� ����� &K������ ������� &n�������� ������������, ���������
  oecho ___������� &G�������&n ����� � ���������� ������� ��� ������ ������� ���������.
end
~
#3707
������� ����� - ��������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ��������)
  otransform 3707
  osend %actor% _����� ���������� ����� ��������� � ������� �� ����� ���.
  return 0
  halt
end
set char %self.worn_by%
if (%random.1000% <= 40 || (!%char% && %random.1000% <= 40))
  wait 1
  oecho _�������� �� ����� ���������� ������� ���� � ��������� ���� �������.
end
~
#3708
������� ���� - ������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if %actor.name% != ������
  otransform 3708
  osend %actor% ����������� &c�������&n ������������ � � ��� �� ������� �������� �������! � ����� �� ���������!
  return 0
  halt
else
  wait 1
  if %random.num(10)% > 8
    osend %actor% &c������� ���������� �������&n �������� � ����� ������.
    oechoaround %actor% ��������� %actor.rname% � ������� ���������� ������� �������� ��� �������.
  else
    set selfnum 0
    set num 0
    foreach i %self.pc%
      eval num %num%+1
    done
    set ranofself %random.num(%num%)%
    foreach n %self.pc%
      eval selfnum %selfnum%+1
      if %ranofself% == %selfnum%
        set char %n%
      break
    end
  done
  if %char.name% != %actor.name%
    oechoaround %char% %char.iname% ��������%char.g% ��������� ������ � &c������� ���������� �������&n, ������� �����-�� ����� � ����� %actor.name%.
    osend %char% ��� ������, ���������� � &c������� ���������� �������&n %actor.rname%, ������� ���!
  else
    oechoaround %actor% %actor.iname% �����%actor.g% ��������������� ����� &c������� ���������� ��������&n.
    osend %actor% �� ������ ��������������� ����� &c������� ���������� ��������&n.
  end
end
end
~
#3709
������� ����� - ������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ������)
  otransform 3709
  osend %actor% _����� ���� ������������� � ��� ����� ������� � ����� �� �����.
  return 0
  halt
end
~
#3710
������� ���� - �������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if %actor.name% != ������� && %actor.name% != ������ && %actor.name% != �����
  otransform 3710
  osend %actor% �� ���������� ����� &R�������&n-&M����������&n ����, ���� �������� �� ���� ������� &R�����&n � �� ��� �� ��������� ���� � ������.
  oechoaround %actor% %actor.name% �������%actor.u% ����� &R�������&n-&M�������&n ����, ���� ������� ��������� ������� &R������&n � ������� %actor.rname%.
  return 0
  halt
else 
  wait 1
  oecho &R�������&n-&M����������&n ���� ������ �������� ���� ����� � ����������� �������� �� ���� ���� ������������ ��� ����� : &Y����&n
~
#3711
������� ����� - �������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if (( %actor.name% != ������� ) && (%actor.clan% != �� ))
  otransform 3711
  osend %actor% _���� ���� ��� ������ ������ ������ ������ ����� � �������� ������������ �������.
  return 0
  halt
end
*set char %self.worn_by%
*if (%random.1000% <= 120 || (!%char% && %random.1000% <= 40))
* wait 1
* oecho  ��� ����������� �� ����� ����� ��������� ����� �� ����� ������.
*end
~
#3712
������� �����  - ������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ������)
  otransform 3712
  osend %actor% _���� ����������� �� ����� ���, ������ � ����� ������.
  return 0
  halt
end
set char %self.worn_by%
if ((%random.1000% <= 450) || (!%char% && %random.1000% <= 450))
  wait 1
  switch %random.7%
    case 1
      set word �����
    break
    case 2
      set word �����
    break
    case 3
      set word ������
    break
    case 4
      set word ����
    break
    case 5
      set word ������
    break
    case 6
      set word ��������
    break
    default
      set word ����
    done
    oechoaround %actor% _���� ������������� � �������� ������ ������� �� %word%.
    osend %actor% _���� ������������� � �������� ������ ��� �� %word%.
  end
~
#3713
������� ����� - �������~
1 gjlp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if %actor% && ((%actor.name% != �������) && (%actor.name% != �����) && (%actor.name% != �����) && (%actor.name% != ����))
  osend %actor%  __���� � ���� &Y�������&n �� �������� &W����� ������&n � ��������: "������� ��
  osend %actor%  _������: ��� ������, ����� ���, �������."
  oechoaround %actor%  �� &Y�������&n ����� &W����� ������&n � ��������: '������� �� ������: ��� ������, ����� ���, �������.'
  return 0
  halt
end
set master %self.worn_by%
wait 1
*����������� ������ ��� ������ �����
if (%self.carried_by% && %actor% && !%master%)
  osend %actor% &Y�������&n ������������, ������ ���������� ��������
  osend %actor% __(� ��� ��� ������� &W��������� ������&n) ��������� ��� � ���.
  oechoaround %actor% &Y�������&n ������������, ������ ���������� ��������
  oechoaround %actor%   (� ��� ��� ������� &W��������� ������&n) ��������� � ��� %actor.dname%.
  halt
end
*��� ������ �����
if (!%self.worn_by% && %actor% && %self.carried_by% && %master%)
  oecho _&Y�������&n ���������� ��������.
  halt
end
*��� ��������� ����� � �� ����
oecho _&W��������� ������&n �������� �� &Y�������&n � ������ �������� ��������� �����!
~
#3714
������� ����� - �����~
1 gjp 100
~
if (%actor.level% > 30 ) && (%actor.vnum% == -1 )
  halt
end
if ((%actor.name% != �����) && (%actor.name% != ����) && (%actor.name% != �����) && (%actor.name% != ����))
  osend %actor% ����� �������� � ��� ����� �������� ��������� ���� � �������� �� �����.
  return 0
  halt
else
  wait 1
end
~
#3715
������� ����� -- ��������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.level% == -1))
  halt
end
if ( %actor.name% != ��������)
  otransform 3715
  osend %actor% _���� ���������, ������ �������, � ��������� �� ����� ���.
  return 0
  halt
end
wait 1
set owner %self.worn_by%
if (%owner.name% == �������� )
  halt
end
oecho ���� ����������� ������������ ������ � ��� ����� ������ �� ����.
~
#3716
������� ����� - ������~
1 gjp 100
~
if ((%actor.level% > 30) & (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ������)
  otransform 3716
  osend %actor% �� ���������� ����� �����, �� ��� �����������, ������������� � ������, ��� ��������.
  return 0
  halt
end
wait 1
set owner %self.worn_by%
if (%owner.name% == �������� )
  halt
end
oecho ����� ���������� � ������ ���������.
~
#3717
������� ���� - ������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ������)
  otransform 3717
  osend %actor% _�� ���������� ����� ����� �� ������������.
  return 0
  halt
end
set char %self.carried_by%
wait 1
if (%char% && %self.worn_by%)
  oechoaround %actor%   %actor.name% �������� �������%actor.g% � �������.
  osend %actor% �� ������ ���������������...
end
~
#3718
������� ����� - ��������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ��������)
  otransform 3718
  osend %actor% _�� ������ ����� ����� ������� ������� ���������, �� ������� ��������,
  osend %actor% ___��� ������� � ���� �� ������, � ������ �� ������� ����� �����.
  oechoaround %actor% %actor.name% �������%actor.u% �������� ����� ������� �������, �� ������� ���������%actor.g%.
  return 0
  halt
end
~
#3719
������� ���� - ������~
1 gjp 100
~
if ((%actor.level% > 30)  (%actor.vnum% == -1))
  halt
end
if %actor.name% != ������ && %actor.name% != ������ && %actor.name% != �����
  osend %actor% ���������� ������ �� ���� ������� ������ ������� � �������� ��� � ����.
  oechoaround %actor% ���������� ������ �� ���� ������� ������ ������� �� %actor.vname%.
  return 0
  halt
else
  wait 1
  oecho ���������� ������ �� ���� ������� ��������� ������� ������ �����.
end
~
#3720
������� ���� -- ������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if %actor.clan% == �� || %actor.clan% == ��
  halt
end
if (%actor.name% != ������)
  otransform 3720
  osend %actor% �� &W������� �������&n ������� ������� ��������� �������, �� � ������ ����������� �� ����!
  oechoaround %actor% %actor.name% ��������%actor.u% �� &W������� �������&n, �������� ������%actor.g% ���-�� ��������.
  return 0
  halt
else
  wait 1
  osend %actor% &W������ �������&n ����� ��� �������� ������.
  oechoaround %actor% &W������ ������� &n����� ��� ������� �������, ��� ����� �������.
end
~
#3721
������� ����� -- ��������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ��������)
  otransform 3721
  osend %actor% _�� ���������� �������� ������� �� ������� �����, �� ��� ����������� � ��� � ����� �� ������ �������.
  return 0
  halt
end
~
#3722
������� ����� -- ����~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ����)
  otransform 3722
  osend %actor% _�� ������� �������� ������� � ���������� ������� ������.
  osend %actor% __�� ��������� ��������� ����.
  return 0
  halt
end
~
#3723
������� �����  -- �������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ((%actor.name% != ��������) && (%actor.name% != �������) && (%actor.name% != �����) && (%actor.clan% != ���))
  otransform 3723
  osend %actor% _���� ������� �� ��������� ��� ����� ����� ����� ���������� ��������.
  return 0
  halt
end
~
#3724
������� ����� -- ����~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ( %actor.name% != ����)
  otransform 3724
  osend %actor% _��������� �������, �� ������� ����������������� ������������ ������.
  osend %actor% _��������� ������� �� �� �����, �� ������� ��������� ����� � �������.
  osend %actor% _�� �� �����.
  return 0
  halt
end
set char %self.worn_by%
if (%random.1000% <= 40 || (!%char% && %random.1000% <= 40))
  wait 1
  oecho _��������� ������ ����� ����� ��� �� ������� �� �������� �������.
end
~
#3725
������� ����� -- ����~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ((%actor.name% != ����) && (%actor.clan% != ��))
  otransform 3725
  osend %actor% _������ ������ ������ ������ ��� �� �����.
  return 0
  halt
end
~
#3726
������� ����� -- ������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ((%actor.name% != ������) && (%actor.name% != ������))
  otransform 3726
  osend %actor% �� ���������� ��������� � ������ ����, �� � ��� ����������� ������ � �� ���� �� ���������� �� ���.
  return 0
  halt
end
set char %self.worn_by%
if (%random.1000% <= 40 || (!%char% && %random.1000% <= 40))
  wait 1
  oecho %actor.name% ��������%actor.g% � ������ ����, ������� �������%actor.u% ���, � �������%actor.w% ��������%actor.g% �������.
end
~
#3727
������� ����� -- �����~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ((%actor.name% != �����) && (%actor.clan% != ��))
  otransform 3727
  osend %actor% ��̣��� ޣ���� �������� �� �����, ������ ������� ��� � ������: "�� ��ϣ, �� �����!"
  return 0
  halt
end
~
#3728
������� ����� --  ������~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if (%actor.name% != ������)
  otransform 3728
  osend %actor% _�������� ������ ������� ������ ���� �� ������� ��� ����.
  return 0
  halt
end
~
#3729
������� ����� -- �����~
1 gjp 100
~
if ((%actor.level% > 30) && (%actor.vnum% == -1))
  halt
end
if ((%actor.name% != �����) && (%actor.name% != �����) && (%actor.name% != �����))
  otransform 3729
  osend %actor% _������ ���������� ��������� � ������ ��� �� ����.
  osend %actor% __�� ����� ��������� &K��ڣ��� ����� � &R��������.&n
  return 0
  halt
end
set char %self.worn_by%
set char2 %self.carried_by%
if (%random.1000% <= 40 || (!%char% && %random.1000% <= 40) || (!%char% && !%char2%))
  wait 1
  oecho _������ ���������� ���������,������ ����������� � ���������.
end
~
#3730
������� ���� - �����~
1 gjp 100
~
if %actor.level% > 30 && %actor.level% == -1
  halt
end
if %actor.name% != ����� 
  otransform 3730
  osend %actor% ��������� �� &Y��&y���&Y��� &Y��&y��&n &b��&B��&c�&C� �&c�&b��&B���&n �� ������ ��� ������� ��� �� �� ����� �� �������.
  oechoaround %actor% ��������� �� &Y��&y���&Y��� &Y��&y��&n &b��&B��&c�&C� �&c�&b��&B���&n %actor.iname% �����%actor.g% ��� ������� ��� �� � ���������.
  return 0
  halt
else
  wait 1
  oecho �������� ���� �� &Y��&y���&Y��� &Y��&y���&n &b��&B��&c�&C� �&c�&b��&B���&n ��������� ����� ��������!
end
~
#3731
������� ���� - �����~
1 jp 100
~
if %actor.level% > 30 && %actor.level% == -1
  halt
end
if %actor.name% != ����� 
  otransform 3731
  oechoaround %actor% ������� ���������� ���� ������� � ��� ���������� ���� ���� ������� %actor.vname%.
  osend %actor% ������� ���������� ���� ������� � ��� ���������� ���� ���� ������� ���.
  return 0
  halt
else
  wait 1
  osend %actor% ����� �������� �� ����, ������� ���� ������ ��� � ������� �������.
  oechoaround %actor% ����� �������� �� ����, ������� ���� ������ %actor.rname% � ������� �������.
end
~
#3732
������� ���� - �����~
1 gjp 100
~
if %actor.level% > 30 && %actor.level% == -1
  halt
end
if %actor.name% != ����� 
  otransform 3732
  oechoaround %actor% ������ ������������ �� ��� %actor.rname%.
  osend %actor% ������� ������������ � ��� �� ���.
  return 0
  halt
else
  wait 1
  osend %actor% �������� � ������������ ������ ����������� ���������� ���. �� ������������� �����������.
  oechoaround %actor% �������� � ������������ ������, ������������ �� ������� �����, ������ ���������� ���.
end
~
#3733
������� ���� - ������~
1 gjp 100
~
if %actor.level% > 30 && %actor.level% == -1
  halt
end
if %actor.name% != ������� && %actor.name% != ������ && %actor.name% != ��������
  otransform 3733
  oechoaround %actor% %actor.name% �������%actor.g% ���� � &W�&n��&K���&r�&R� �&r�&K���&n�&W� &y������&n, �� � &R������&n ��������%actor.g% ��.
  osend %actor% �� �������� ���� � &W�&n��&K���&r�&R� �&r�&K���&n�&W� &y������&n, �� � &R������ &n��������� ��.
  return 0
  halt
else
  wait 1
  osend %actor% &W�&n��&K���&r�&R� �&r�&K���&n�&W� &y������&n �������� � ��� �� ����.
  oechoaround %actor% �� ��������, ��� ���� ������ ����� ������� �� &W�&n��&K���&r�&R� �&r�&K���&n�&W� &y������&n
end
~
#3734
������� ���� - �����~
1 gjp 100
~
if %actor.level% > 30 && %actor.level% == -1
  halt
end
if %actor.name% != ����� && %actor.name% != ���� 
  otransform 3734
  oechoaround %actor% ���� &Y��������� &K��������&n ���������� ������� %actor.rname% �� �����!
  osend %actor% ���� &Y��������� &K��������&n ���������� ������� ��� �� �����!
  return 0
  halt
else
  wait 1
  osend %actor% ���� &Y��������� &K��������&n ��������� ����� ����������.
  oechoaround %actor% ���� &Y��������� &K��������&n ��������� � ��� ������.
end
~
#3798
������ �������� �����~
0 q 100
~
return 1
wait 1
if ((%actor.name% == ������) || (%actor.name% == �����))
  if (%world.curobjs(3700)% == 0)
    say �����, �����, �������!
    ����
    mload obj 3700
    ���� ������ .%actor.name%
    ����� .������
  end
elseif %actor.name% == ������
  if (%world.curobjs(3701)% == 0)
    say ���, %actor.name%! ��� ���� ����, �� �������.
    mload obj 3701
    ���� ���� .%actor.name%
  end
elseif %actor.name% == ��������
  if (%world.curobjs(3702)% == 0)
    say �, ��������... ��� ���� ����, �������, � �� ����������� �������� ������� ����������.
    mload obj 3702
    ���� ���� .%actor.name%
  end
elseif %actor.name% == �����
  if (%world.curobjs(3703)% == 0)
    emote �����
    say � ��� �� %actor.name%, ��� ����� ���� �����!
    mload obj 3703
    ���� ����� .%actor.name%
  end
elseif %actor.name% == ����
  if (%world.curobjs(3704)% == 0)
    ���
    mload obj 3704
    ���� ����� .%actor.name%
    ���� .�����
  end
elseif ((%actor.name% == �����) || (%actor.name% == ������))
  if (%world.curobjs(3705)% == 0)
    ������ .�����
    emot ������ ������ ������� � ����� �� ���������
    mload obj 3705
    ���� ������� .%actor.name%
  end
elseif %actor.name% == ��������
  if (%world.curobjs(3706)% == 0)
    say �, �������� �������������! ������� ���� �����, ��� ��� ��� ����� ������� ���������� � ������� �������.
    mload obj 3706
    ���� ��� .%actor.name%
  end
elseif %actor.name% == ��������
  if (%world.curobjs(3707)% == 0)
    ���
    emot ������������ ��������� ���
    mload obj 3707
    ���� ���� .%actor.name%
  end
elseif %actor.name% == ������
  if (%world.curobjs(3708)% == 0)
    mload obj 3708
    ���� ������� .%actor.name%
  end
elseif %actor.name% == ������
  if (%world.curobjs(3709)% == 0)
    mload obj 3709
    ������
    ���� ��� .%actor.name%
  end
elseif %actor.name% == ������� || %actor.name% == ������ || %actor.name% == �����
  if (%world.curobjs(3710)% == 0)
    mload obj 3710
    ����
    ���� ��� .%actor.name%
  end
elseif %actor.name% == �������
  if (%world.curobjs(3711)% == 0)
    mload obj 3711
    ���� ��� .%actor.name%
  end
elseif %actor.name% == ������
  if (%world.curobjs(3712)% == 0)
    mload obj 3712
    ���� ��� .%actor.name%
    emot ����� ���� ��������
  end
elseif ((%actor.name% == �������) || (%actor.name% == �����) || (%actor.name% == �����) || (%actor.name% == ����))
  if (%world.curobjs(3713)% == 0)
    mload obj 3713
    ���� ����� .%actor.name%
    ��� .%actor.name%
  end
elseif %actor.name% == �����
  if (%world.curobjs(3714)% == 0)
    mload obj 3714
    ���� ��� .%actor.name%
  end
elseif %actor.name% == ��������
  if (%world.curobjs(3715)% == 0)
    mload obj 3715
    ���� ���� .%actor.name%
  end
elseif %actor.name% == ��������
  if (%world.curobjs(3718)% == 0)
    mload obj 3718
    ���� ��� .%actor.name%
  end
elseif %actor.name% == ������ || %actor.name% == ������ || %actor.name% == �����
  if (%world.curobjs(3719)% == 0)
    mload obj 3719
    ���� ������ .%actor.name%
  end
elseif %actor.name% == ������
  if (%world.curobjs(3720)% == 0)
    mload obj 3720
    ���� ������ .%actor.name%
  end
elseif %actor.name% == ��������
  if (%world.curobjs(3721)% == 0)
    mload obj 3721
    ���� ����� .%actor.name%
  end
elseif %actor.name% == ����
  if (%world.curobjs(3722)% == 0)
    mload obj 3722
    ���� ������ .%actor.name%
  end
elseif %actor.name% == ��������
  if (%world.curobjs(3723)% == 0)
    mload obj 3723
    ���� ������ .%actor.name%
  end
elseif %actor.name% == ����
  if (%world.curobjs(3724)% == 0)
    mload obj 3724
    ���� ����� .%actor.name%
  end
elseif %actor.name% == ����
  if (%world.curobjs(3725)% == 0)
    mload obj 3725
    ���� ����� .%actor.name%
  end
elseif %actor.name% == ������
  if (%world.curobjs(3726)% == 0)
    mload obj 3726
    ���� ���� .%actor.name%
  end
elseif %actor.name% == �����
  if (%world.curobjs(3727)% == 0)
    mload obj 3727
    ���� ��� .%actor.name%
  end
elseif %actor.name% == ������
  if (%world.curobjs(3728)% == 0)
    mload obj 3728
    ���� ���� .%actor.name%
  end
elseif ((%actor.name% == �����) || (%actor.name% == �����) || (%actor.name% == �����))
  if (%world.curobjs(3729)% == 0)
    mload obj 3729
    ���� ��� .%actor.name%
    halt
  end
elseif %actor.name% == �����
  if (%world.curobjs(3730)% == 0)
    mload obj 3730
    ���� ���� .%actor.name%
  end
end
~
#3799
����-�������~
1 b 5
~
eval objname %self.name%
eval owner %self.carried_by%
if !%owner%
  halt
end
switch %self.vnum%
  case 3700
    if ((%owner.name% != ������) && (%owner.name% != �����))
      wait 1
      oforce %owner% ������� %objname.car%.%objname.cdr%
      halt
    end
  break
  case 3701
    if %owner.name% != ������
      wait 1
      oforce %owner% ������� %objname.car%.%objname.cdr%
      halt
    end
  break
  case 3702
    if %owner.name% != ��������
      wait 1
      oforce %owner% ������� %objname.car%.%objname.cdr%
      halt
    end
  break
  case 3703
    if %owner.name% != �����
      wait 1
      oforce %owner% ������� %objname.car%.%objname.cdr%
      halt
    end
  break
  case 3704
    ________if %owner.name% != ����
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3705
  if ((%owner.name% != �����) && (%owner.name% != ������))
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3706
  if %owner.name% != ��������
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3707
  if %owner.name% != ��������
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3708
  ____if %owner.name% != ������
  oforce %owner% ������� %objname.car%.%objname.cdr%
  halt
end
break
case 3709
  if %owner.name% != ������
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3710
  if %owner.name% != ������� && %owner.name% != ������ && %owner.name% =! �����
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3711
  if %owner.name% != �������
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3712
  if %owner.name% != ������
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3713
  if ((%owner.name% != �������) && (%owner.name% != �����) && (%owner.name% != �����) && (%owner.name% != ����))
    oforce %owner% ������� %objname.car%.%objname.cdr%
    halt
  end
break
case 3714
  if ((%owner.name% != �����) && (%owner.name% != ����) && (%owner.name% != �����) && (%owner.name% != ����))
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3715
  if %owner.name% != ��������
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3716
  if %owner.name% != ������
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3717
  if %owner.name% != ������
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3718
  if %owner.name% != ��������
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3719
  if %owner.name% != ������ && %owner.name% != ������ && %owner.name% != �����
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3720
  if %owner.name% != ������ && %owner.clan% != �� && %owner.clan% != ��
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3721
  if %owner.name% != ��������
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3722
  if %owner.name% != ����
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3723
  if ((%owner.name% != ��������) && (%owner.name% != �������) && (%owner.name% != �����) && (%owner.clan% != ���))
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3724
  if %owner.name% != ����
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3725
  if ((%owner.name% != ����) && (%actor.clan% != ��))
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3726
  if ((%owner.name% != ������) && (%owner.name% != ������))
    wait 1
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3727
  if (%owner.name% != �����)
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3728
  if (%owner.name% != ������)
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3729
  if ((%owner.name% != �����) && (%owner.name% != �����) && (%owner.name% != �����))
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
case 3730
  if %owner.name% != �����
    oforce %owner% ������� %objname.car%.%objname.cdr%
  end
  halt
break
done
~
$~
