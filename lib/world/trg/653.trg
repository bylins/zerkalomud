#65300
��������������~
0 r 100
~
wait 1
�����
wait 1
say ������ ����, %actor.name%
say ��� ������ � ���� ������������! ������ � ������ ���������.
���  
end   
detach 65300 %self.id%
~
#65301
���� ������ �������~
0 j 100
~
if !(%object.vnum% == 65400)
say �� ��, ���?! �����������!?
emot ������ ����������
drop %object.name%
halt
end
wait 1
say �, ��� ��� - ��! ��� ��� �������!
say ��� �, ���� � ����� ����������. 
mechoaround %actor% _������ ���� �������� ����������� %actor.rname%.
if (%actor.level% > 23) && (%actor.level% < 27)                         
msend %actor% _������, ��� ������ ���� ��� �����������, ��� ��� �������� ��� 5000 ����� �����.
%actor.exp(+5000)%
else
msend %actor% ������ ���� ��� ������ �����������, �� ������ "��������" ��� �� ������! 
mload obj 65301
���� ����� %actor.name%
end
mecho ������ ��� ��� �������� ������������� ���� ������.
mpurge %object.iname%     
mload mob 65302
mpurge %self%
~
$~
