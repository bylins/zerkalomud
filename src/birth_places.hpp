//  $RCSfile$     $Date$     $Revision$
//  Part of Bylins http://www.mud.ru
// ������ �������� ��� ����������� ��� �������� � ������ �� ������� ����� ����� � ����

#ifndef BIRTH_PLACES_HPP_INCLUDED
#define BIRTH_PLACES_HPP_INCLUDED

//��� ���, � ���� ��� ����������� ����� ���, � ��� ���.
//���� ��-�������� ������ � ���� ������ ������ �����
#define DEFAULT_LOADROOM 4056
#define BIRTH_PLACES_FILE "birthplaces.xml"
#define BIRTH_PLACE_UNDEFINED    -1
#define BIRTH_PLACE_NAME_UNDEFINED "Undefined: � ������ �����-�� ��������"
#define BIRTH_PLACE_MAIN_TAG "birthplaces"
#define BIRTH_PLACE_ERROR_STR "...birth places reading fail"

#include <vector>
#include <boost/shared_ptr.hpp>
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"

class BirthPlace;

typedef boost::shared_ptr<BirthPlace> BirthPlacePtr;
typedef std::vector<BirthPlacePtr> BirthPlaceListType;

class BirthPlace
{
public:
    //static void Load(const char *PathToFile);               // �������� ����� ��������
    static void Load(pugi::xml_node XMLBirthPlaceList);
    static int GetLoadRoom(short Id);                       // ��������� ����� ����������� ������� �� ID
    static std::string GetMenuStr(short Id);                // ��������� ������� ��� ���� �� ID
    static std::vector<int> GetItemList(short Id);          // ��������� ������ ���������� ������ �� ID
    static std::string ShowMenu(std::vector<int> BPList);   // ��������� ���� ������ ����� ����� �������
    static short ParseSelect(char *arg);                    // ����� ����� �� ���������� ����� � �������� (description)
    static bool CheckId(short Id);                          // �������� ������� ����� � ��������� ID

    // ������ � ��������� ������.
    // �� �������� �����, �� ����� �����
    short Id() const {return this->_Id;}
    std::string Name() {return this->_Name;}
    std::string Description() {return this->_Description;}
    std::string MenuStr() {return this->_MenuStr;}
    int LoadRoom() {return this->_LoadRoom;}
    std::vector<int> ItemsList() const {return this->_ItemsList;}

private:
    short _Id;                                  // ������������� - ����� �����
    std::string _Name;                          // �������� �����
    std::string _Description;                   // �������� �������� ��� ��������
    std::string _MenuStr;                       // �������� � ����
    int _LoadRoom;                              // ����� ������� �����
    std::vector<int> _ItemsList;                // ������ ���������, ������� ������������� �������� �� ���� � ���� �����
    static BirthPlaceListType BirthPlaceList;   // ������ ����� ����� � ���� ����� ����������

    static void LoadBirthPlace(pugi::xml_node BirthPlaceNode);  // ������� �������� ����� ����� �����
    static BirthPlacePtr GetBirthPlaceById(short Id);           // ��������� ������ �� ����� ���� �� �� ID
};

#endif // BIRTH_PLACES_HPP_INCLUDED
