#60400
����������� �������~
0 r 100
~
���� ��������� ��������� �� ����� � ���� ���������� ������
����
say ������ ����, %actor.name%
wait 2
~
#60401
������ �� ������~
2 e 80
~
wait 1
eval dmg 100+%random.100%
*set actor %random.pc% 
wechoaround %actor% ������������� ������ ��������� � ����� %actor.rname%.
wsend %actor% ������������� ������ ��������� � ���� �����.
wdamage %actor% %dmg%
wait 2s
halt
~
$~
