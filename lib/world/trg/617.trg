#61700
������ �� ������~
2 e 100
~
wait 1
set fchar %random.pc%
if %fchar.vnum% != 0
  wsend %fchar% ����������� ����� � � ���� ����������� ���� �����.
  wsend %fchar% ���� ����� ��������� �� ��� �������� �������� ����.
  wechoaround %fchar% ���� ����� ��������� ������ ���.
  wechoaround %fchar% ��������� �� ��� ������ � %fchar.vname%.
  wdamage %fchar% 150
end
~
$~
