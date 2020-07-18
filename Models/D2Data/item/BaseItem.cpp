#include "BaseItem.h"
#include "BaseMiscItem.h"
#include "BaseArmor.h"
#include "BaseWeapon.h"
#include <iostream>
#include <fstream>
#include <sstream>

int BaseItem::ItemCount = 0;
std::vector<BaseItem *> BaseItem::items;

void BaseItem::init()
{
	std::string line;


	std::ifstream ifs1("data\\BaseMiscItem.csv", std::ifstream::in);

	if (!ifs1)
	{
		std::cout << "can not find data\\BaseMiscItem.csv" << std::endl;
		exit(1);
	}

	while (std::getline(ifs1, line, '\n'))
	{
		items.push_back(new BaseMiscItem(line));
	}

	ifs1.close();


	std::ifstream ifs2("data\\BaseArmor.csv", std::ifstream::in);

	if (!ifs2)
	{
		std::cout << "can not find data\\BaseArmor.csv" << std::endl;
		exit(1);
	}

	while (std::getline(ifs2, line, '\n'))
	{
		items.push_back(new BaseArmor(line));
	}

	ifs2.close();


	std::ifstream ifs3("data\\BaseWeapon.csv", std::ifstream::in);

	if (!ifs3)
	{
		std::cout << "can not find data\\BaseWeapon.csv" << std::endl;
		exit(1);
	}

	while (std::getline(ifs3, line, '\n'))
	{
		items.push_back(new BaseWeapon(line));
	}

	ifs3.close();

	return;
}
