#!/usr/bin/perl

# (C) Pereplut 2009.
# (������ �������� � ������� *.mob ��������, ������ �� �������)

## 1. �������� �� ��������� ���� � ������������� ������ (�������� �� ������ ��������� �� �� �������� ������)
## 2. ����� ������������ �������� ����������� ���� "Class:" � ����� ����: "Race:"
#
# ����� ����������� Class: � Race:
# Class: -> Race:
#  100 -> 100 - ������� ���
#  101 -> 110 - ������ (����� ����)
#  102 -> 101 - �������
#  103 -> 104 - ��������
#

## �� �������� � ������� ���������� ������ ������� ��������� ���� done_class_check.txt

use locale;
use POSIX qw (locale_h);
setlocale(LC_CTYPE, 'ru_RU.KOI8-R');

$file_name=undef;
@files=();

$mob_count      = 0; # ������� ������� ����� � ���� � ������������ �����
$class_detects  = 0; # ������� ������� ������� ������� ����������� �� �����
$class_transfer = 0; # ������� ������� ������� ���� ���������� �� ���� Class: � Race:
$words_in_base  = 0; # ������� ������� ���� � ��� � ����� "���� ����"

## ���� �������:
# 1) �������
@class_chelovek = qw(�������� ������ ���� ������ ����� ����� ���������� �������� ������� ������ ����-������ ����� �������� ������ ����� ���� ������� ������� ����������� ������ ������� ������ ������ ����� ����� ������ ���������� ������ ����� ������� ���� ������� ������-��� ������� ������� ������� �����-����������� �������� �������� ������� ������������ ������������ ���������� ������ �������� ������� ������ ������ ������ ������� ���� ������-������� ���� ������� ����� ����� ����� ������� ������� �������� �������� ��� �������� ������� ������� ������ �������� ������� ������ ����� �������� �������� ���� ���� ������� �������� ����� ���� ������� �������� ��������� ��������� ������� ������ ������ ������� ������� ������ ���� ������ ������� ���� ������ ������� ������� ����� ����� ����� �������� ������� ����� ������� ����� ������ ������ ������ ������� ������� ����-������ ���� ����� ������� ���� ������ ��������� ������� �������� ������� �������� ����� ���� ���� ��������� �������� �������� �������� ���������� �������� ������ ������� ���������� �������� ������� ������ �������-��������� �������� ���� �������� ����������� ��������� ������� ����� �������� ������ ���� ����� ���������� ������ ������ �������� �������� ������� �������� ��������� ���� ������ ������� ����� ���� ��������� ������ �������� �������� �������� �������� �����-�������� ��� ���� �������� ������� ������� ���������-��������� ���������-��������� ��������� ������� ������ ������ ������ ������� ������ ����� ������ ����� ������ ������-�������� ������� ������ ������ ������ ��������� ���� ����� ���� ������� ������� ������� ������� ����� ������� �������� ������� ������� ������ ��� �������� �������� ������� ������ ��� ������ ����� ������ �������� ��������� ������������� ������� ������� ���������� ������ �������� ��������-������� ���������� ������� ���������� ��������-������� ������� ��������� ����� ����� ������ ����� �������� ��� ������� ������ �������� �������� ������ �������� ���������� ������ ��������� ���� ����� ������ �������� ���������� ������� ��������� ���������� �������� ��������� ��������������� ��������������� ������������� ���� �������� ��������� ������� ��������� ������� ����� ������ ������� ������ ������ �������� ����������� ������ ������� ������ ��������� ���������� ����������� ��������� ������ ��������� ������� ��������� ������ ������ ����������� �������� �������-������� ����� �������� ����� ����� ����� ����� ��������� ������� ������� ����� ���� ����� ����� ������ ������ ���������� �������� �������� ������� ���� ������ ������������� ������ ������ ������ ������ ���� ������ ����� ������� ���� ������ ���������� ��������� ������� ����-������� ������� �������� �������� ������� ������� �������� ���� ������ ���������� ����� ���� ���� ���� ������ ����������� �������� ������� ������� ������ ����� �������� ������ ������� �������� ����� ����� �����-������� ����� ���� ������ �������� ����������� ������� ����� ���� ����� ���� ���� �������� ���� ����� ������� ������ ��������� �������� �������� �����-��� ���-��� ��� ����� �������� ������� ����� ���� ������� ������ �������� �������� ������� ���� ����� ������ ������� �������� ����� ��������� ������� ������� �������� ������ ��������� ��������� ����� ��� ���������� ������ ����� �������� ����� ����� ���� ������� �������� ������� ������� ������ ��������� �������� ������������ ������������� ���������� ���������� �������� ������������ �������� �������� ������� ������� ����� ���� ����-��������� ������ ������� ����� ����� ���������� ����� ������� ��� ������ ������ ��������� ����� ������ ������ ������ ������ ���� ������� ����� ���� �������� �������� ������� ������� ������� ����� ��� ������� ����� ����);
# 2) ��������-�����
@class_chelovekozver = qw(�������� �������-��������� ���� ����� �������� ��������� ������� �������-��������� ������);
# 3) �����
@class_ptitcha = qw(������� ���� ������� ����� ���������� ������ ����� ����� ������ ����� ����� ������ ��������� ������� ������ �������� ������ ���� ��������� ������� �������� ������ ������� ������� ����� ������� ���� ������ ����� ������ ��������� ����� ������ ������� ������ ����� ���� ����� ����� ������ ����� �������� ����� ����� �����-��������� ����� ������� ������);
# 4) ��������
@class_jivotnoe = qw(�������� ����� ���� ������� ����� ���������� ������� ����� ����� ����� ����� ������� �������� �������� �� ���� �������� ���������� ������ ����� ������� ����� ���� ���� ����� ��� ������ ��� ������� ���� ������� ������ ������ �������� ��������� ������� ������� ���������� ����� ������ ���� ����� ������� ���� ����� ����� ����������� ������ ��� ����� ��� �������� ���� �������� ������ ������� ���� ��������� ������ ��������� ���������� ����� ������� ������� ����� ���� ����� ����� ������� ������ ��);


# 5) ��������������
@class_presmikayusheesya = qw(���� �������� ����������� �������� ���� ������ ������� ������ ������� ������� ���� �� ����� ����� ����� ������ ���� ������);
# 6) ����
@class_riba = qw(����� ���� ��� ����� ��� ����� ������� ������ ���� ������� ����� �������� ������� ����� ����);
# 7) ���������
@class_nasekomoe = qw(������� ����� �������� ��� ��������� ������� ������� ���� ������ ������� ������� ��������� ���������� ���� ����� ���� ������� ������ ���� ��� ������� ��������� �������� ������ ��� ������ ������ ������� �����);
# 8) ��������
@class_rastenie = qw(������ ������� �������� ���-�������� ��� ������ ��������-���� ������� ��������);
# 9) �������
@class_predmet = qw(���� ������� ����� ������ ����� ������ ������� ������� �������� ����� �������� ���� ����� ���� ������ �-100 ���������� ����� ����� ��������� ������� ����� ��������� ������ ������ ���);
#10) ����� ����
@class_jivoitrup = qw(����� ������� ���������� �������� ������� ������� ������ ������ ������ �������� ����� ���������� ������� �����);
#11) �������
@class_prizrak = qw(���);
#12) �������
@class_nechist = qw(������� ������ ����� ������� ������� ������-���������� ������� ������� ������������� ������ ��������� ���� ������ �������� ���������� ����� ���� ������ ����� �������� ����� ����� ��������� ����� ����� ����� ������� ����-������ �������� ������������ �������� ����� ����� ����� ��������� ���� ������ ������� ������� ���� �����);
#13) �������������� ���
@class_nematerialniiduh = qw(����� ���� ������� ��������� ����� �������� ������-������ ������ �������� ����� ����-����� �������� �������� ����� ����� ����� ���� ����� ������ ��� ��������);
#14) ���������� ��������
@class_magicheskoesozdanie = qw(��������� ������� ������� ����� ��������� ������� ������ �������� ���-����� ������ ��������� ���������� ���������� ����� ������ ������� ��������-�������� ������ ��� ����� ����� ������ ������� ������ ����� ���������� ���);

# ������� ���� � ����
$words_in_base += $#class_chelovek + 1;
$words_in_base += $#class_chelovekozver + 1;
$words_in_base += $#class_ptitcha + 1;
$words_in_base += $#class_jivotnoe + 1;
$words_in_base += $#class_presmikayusheesya + 1;
$words_in_base += $#class_riba + 1;
$words_in_base += $#class_nasekomoe + 1;
$words_in_base += $#class_rastenie + 1;
$words_in_base += $#class_predmet + 1;
$words_in_base += $#class_jivoitrup + 1;
$words_in_base += $#class_prizrak + 1;
$words_in_base += $#class_nechist + 1;
$words_in_base += $#class_nematerialniiduh + 1;
$words_in_base += $#class_magicheskoesozdanie + 1;

# �������� �������� ������ �� ��� ������
%type_name_by_number = (0 => "UNDEF",
		       1 => "�������",
		       2 => "��������-�����",
		       3 => "�����",
		       4 => "��������",
		       5 => "��������������",
		       6 => "����",
		       7 => "���������",
		       8 => "��������",
		       9 => "�������",
		       10 => "����� ����",
		       11 => "�������",
		       12 => "�������",
		       13 => "�������������� ���",
		       14 => "���������� ��������");

# �������������� ����� � �����
%convert_class_to_race = (100 => 100, 101 => 110, 102 => 101, 103 => 104);

# ���������, ������������ ����� ����
sub detect_mob_class {
	# �������� ����� � �������� ����
	my @args = split(/\s/, shift);
	my $detected_type = 0;
	my $now_num = -1;

	foreach $keyword (@args) {
		$now_num = 1;
		foreach $classmember (@class_chelovek) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_chelovek{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 2;
		foreach $classmember (@class_chelovekozver) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_chelovekozver{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 3;
		foreach $classmember (@class_ptitcha) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_ptitcha{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 4;
		foreach $classmember (@class_jivotnoe) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_jivotnoe{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 5;
		foreach $classmember (@class_presmikayusheesya) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_presmikayusheesya{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 6;
		foreach $classmember (@class_riba) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_riba{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 7;
		foreach $classmember (@class_nasekomoe) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_nasekomoe{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 8;
		foreach $classmember (@class_rastenie) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_rastenie{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 9;
		foreach $classmember (@class_predmet) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_predmet{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 10;
		foreach $classmember (@class_jivoitrup) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_jivoitrup{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 11;
		foreach $classmember (@class_prizrak) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_prizrak{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 12;
		foreach $classmember (@class_nechist) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_nechist{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 13;
		foreach $classmember (@class_nematerialniiduh) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_nematerialniiduh{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
		$now_num = 14;
		foreach $classmember (@class_magicheskoesozdanie) {
			if (length($keyword) > 0 && length($classmember) > 0 && index($keyword, $classmember) == 0) {
				print "CONFLICT types: [".$type_name_by_number{$detected_type}.",".$type_name_by_number{$now_num}."] ".$mob_vnum." ".$mob_name if ($detected_type != 0 && $detected_type != $now_num);
				$addedclass_magicheskoesozdanie{$mob_name." [".$mob_vnum."]"}++;
				$detected_type = $now_num;
				return $detected_type;
			}
		}
	}
#DEBUG	print "TYPE '$type_name_by_number{$detected_type}' ADDED $mob_name" if $detected_type != 0;
	
	return $detected_type;
}

open INDEX, "index";
	$\ = "\n";
	$/ = "\n";
	@index_data = <INDEX>;
close INDEX;


if (@index_data) {
	#initialize
	$heroes_count = 0;
	$null_levels_mobs = 0;

	foreach $file_name (@index_data) {
		chomp $file_name;
		if ($file_name =~ m/^\d{1,4}\.mob$/) { # WORK MODE
#		if ($file_name =~ m/^\d{1}\.mob$/) { # DEBUG MODE

#			rename $file_name.".old_m", $file_name if (-e "$file_name.old_m"); #DEBUG

			open SOURCE, "$file_name";
#READONLY			open OUTPUT, ">$file_name.new";
#			print $file_name; #DEBUG

			
			while ($line=<SOURCE>) {
				chop $line;
				
				# ���������� ����
				if ($line =~ m/^\#(\d+?)$/) {
					$mob_vnum = $1;

					#initialize
					$mob_count ++;
					$mob_name  = "";
					$mob_race  = -1;
					$mob_class = -1;
					$mob_detected_class = -1;

					# ���������� ��� ���� (������������ �����)
#READONLY					print OUTPUT $line;
					foreach(1..2) {
						$line=<SOURCE>;
						chop $line;
#READONLY						print OUTPUT $line;
					}
					chop $line;
					$mob_name = $line;
					$mob_detected_class = detect_mob_class(lc($mob_name));
					#print "$mob_vnum = $mob_name";
				} else {

					# ���������� ����� ���� ���� ���� � ����� ����������� ��� �����
					if ($line =~ m/^Class\: (\d+)$/) {
						$mob_class = $1;
#READONLY						print OUTPUT "Class: $mob_class";
						if ($mob_detected_class > 0) {
							$class_detects++;
#READONLY						print OUTPUT "Race: ".(100 + $mob_detected_class);
						} else {
							$class_transfer++;
#READONLY						print OUTPUT "Race: ".$convert_class_to_race{$mob_class};
						}
					}
				
#READONLY					print OUTPUT $line if ($line !~ m/^(Class|Race)\:.+?/)
				}
			}
			close SOURCE;
#READONLY			close OUTPUT;
#			rename "$file_name", $file_name.".old_m" if (-e "$file_name"); # DEBUG MODE
#			rename "$file_name", $file_name.".cl_backup" if (-e "$file_name"); # DEBUG MODE
#READONLY			rename $file_name.".new", $file_name if (-e "$file_name.new") # WORKMODE
			
		}
	}

}

## ������� ����������
open STATISTIK, ">race_class_checker_stat.txt";
print STATISTIK "Generated by $0 at ".localtime(time)."\n";

print STATISTIK "Words in base: $words_in_base\nMobs in MUD: $mob_count\nAuto detects: $class_detects\nTransfers: $class_transfer";

@stat_ar = sort keys(%addedclass_chelovek);
print STATISTIK "\n== ".$type_name_by_number{1}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_chelovekozver);
print STATISTIK "\n== ".$type_name_by_number{2}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_ptitcha);
print STATISTIK "\n== ".$type_name_by_number{3}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_jivotnoe);
print STATISTIK "\n== ".$type_name_by_number{4}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_presmikayusheesya);
print STATISTIK "\n== ".$type_name_by_number{5}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_riba);
print STATISTIK "\n== ".$type_name_by_number{6}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_nasekomoe);
print STATISTIK "\n== ".$type_name_by_number{7}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_rastenie);
print STATISTIK "\n== ".$type_name_by_number{8}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_predmet);
print STATISTIK "\n== ".$type_name_by_number{9}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_jivoitrup);
print STATISTIK "\n== ".$type_name_by_number{10}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_prizrak);
print STATISTIK "\n== ".$type_name_by_number{11}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_nechist);
print STATISTIK "\n== ".$type_name_by_number{12}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_nematerialniiduh);
print STATISTIK "\n== ".$type_name_by_number{13}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;
@stat_ar = sort keys(%addedclass_magicheskoesozdanie);
print STATISTIK "\n== ".$type_name_by_number{14}." [".($#stat_ar + 1)."] ==";
print STATISTIK $_ foreach @stat_ar;

close STATISTIK;

print "\nDone mob files convertation. (words in base: $words_in_base mobs: $mob_count class detects: $class_detects transfers: $class_transfer)\n";

