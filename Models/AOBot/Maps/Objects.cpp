#include "Objects.h"
#include <cmath>

Objects::Objects() {
}

Objects::~Objects() {
}

void Objects::add(OBJECT object) throw(...) {
	cs.Lock();
	list.push_back(object);
	cs.Unlock();
}

void Objects::add_owner(unsigned int uid, unsigned int ownerUID) throw(...) {
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->uid == uid) {
			iter->ownerUID = ownerUID;
		}
	}

	cs.Unlock();
}

void Objects::remove(unsigned int uid) throw(...) {
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->uid == uid) {
			iter = list.erase(iter);
			break;
		}
	}

	cs.Unlock();
}

OBJECT Objects::find(unsigned int uid) throw(...) {
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->uid == uid) {
			object = *iter;
			break;
		}
	}

	cs.Unlock();
	return object;
}

OBJECT Objects::find(GameObjectID::GameObjectID objectID) throw(...) {
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectID == objectID) {
			object = *iter;
			break;
		}
	}

	cs.Unlock();
	return object;
}

std::vector<OBJECT> Objects::findMultiple(GameObjectID::GameObjectID objectID) throw(...) {
	std::vector<OBJECT> result;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectID == objectID) {
			result.push_back(*iter);
		}
	}

	cs.Unlock();
	return result;
}

std::vector<OBJECT> Objects::findInRadius(short x, short y, int radius) throw(...) {
	std::vector<OBJECT> result;

	OBJECT object;

	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		object = *iter;

		//is this chest within the given radius?
		if (object.getDistance(x, y) <= radius)
			result.push_back(object);
	}

	cs.Unlock();
	return result;
}

void Objects::update(unsigned int uid, short x, short y) throw(...) {
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->uid == uid) {
			iter->x = x;
			iter->y = y;
		}
	}

	cs.Unlock();
}

int Objects::distance(OBJECT object, short x, short y) throw(...) {
	return (int) ::sqrt((double) ((object.x - x) * (object.x - x) + (object.y - y) * (object.y - y)));
}

int Objects::distance(unsigned int uid, short x, short y) throw(...) {
	int result = -1;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->uid == uid) {
			result = (int) ::sqrt((double) ((iter->x - x) * (iter->x - x) + (iter->y - y) * (iter->y - y)));
		}
	}

	cs.Unlock();
	return result;
}

OBJECT Objects::FindWaypoint() throw(...) {
	//int result = -1;
	OBJECT object;
	object.uid = 0;

	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if ((iter->objectID == GameObjectID::Act1WildernessWaypoint) ||
			(iter->objectID == GameObjectID::Act2CellerWaypoint) ||
			(iter->objectID == GameObjectID::Act2SewerWaypoint) ||
			(iter->objectID == GameObjectID::Act2Waypoint) ||
			(iter->objectID == GameObjectID::Act3TownWaypoint) ||
			(iter->objectID == GameObjectID::Act3TravincalWaypoint) ||
			(iter->objectID == GameObjectID::ExpansionWaypoint) ||
			(iter->objectID == GameObjectID::ExpansionWildernessWaypoint) ||
			(iter->objectID == GameObjectID::IceCaveWaypoint) ||
			(iter->objectID == GameObjectID::InnerHellWaypoint) ||
			(iter->objectID == GameObjectID::PandamoniumFortressWaypoint) ||
			(iter->objectID == GameObjectID::TempleWaypoint) ||
			(iter->objectID == GameObjectID::ValleyWaypoint) ||
			(iter->objectID == GameObjectID::WaypointH) ||
			(iter->objectID == GameObjectID::WaypointPortal) ||
			(iter->objectID == GameObjectID::WorldstoneWaypoint)) {
			//result = iter->uid;
			object = *iter;
		}
	}

	cs.Unlock();
	//return result;
	return object;
}

bool Objects::isDoor(OBJECT object) {
	return (
			object.objectID == GameObjectID::DoorGateLeft ||
			object.objectID == GameObjectID::DoorGateRight ||
			object.objectID == GameObjectID::DoorWoodenLeft ||
			object.objectID == GameObjectID::DoorWoodenRight ||
			object.objectID == GameObjectID::DoorCourtyardLeft ||
			object.objectID == GameObjectID::DoorCourtyardRight ||
			object.objectID == GameObjectID::DoorCathedralDouble ||
			object.objectID == GameObjectID::DoorMonasteryDoubleRight ||
			object.objectID == GameObjectID::AndarielDoor ||
			object.objectID == GameObjectID::DoorCathedralLeft ||
			object.objectID == GameObjectID::DoorCathedralRight ||
			object.objectID == GameObjectID::DoorWoodenLeft2 ||
			object.objectID == GameObjectID::TrappDoor ||
			object.objectID == GameObjectID::DoorByAct2Dock ||
			object.objectID == GameObjectID::TombDoorLeft ||
			object.objectID == GameObjectID::TombDoorRight ||
			object.objectID == GameObjectID::TombDoorLeft2 ||
			object.objectID == GameObjectID::TombDoorRight2 ||
			object.objectID == GameObjectID::SecretDoor1 ||
			object.objectID == GameObjectID::TyraelsDoor ||
			object.objectID == GameObjectID::SlimeDoor1 ||
			object.objectID == GameObjectID::SlimeDoor2 ||
			object.objectID == GameObjectID::IronGrateDoorLeft ||
			object.objectID == GameObjectID::IronGrateDoorRight ||
			object.objectID == GameObjectID::WoodenGrateDoorLeft ||
			object.objectID == GameObjectID::WoodenGrateDoorRight ||
			object.objectID == GameObjectID::WoodenDoorLeft ||
			object.objectID == GameObjectID::WoodenDoorRight ||
			object.objectID == GameObjectID::TeleportationPad1 ||
			object.objectID == GameObjectID::TeleportationPad2 ||
			object.objectID == GameObjectID::TeleportationPad3 ||
			object.objectID == GameObjectID::TeleportationPad4 ||
			object.objectID == GameObjectID::DummyArcaneThing1 ||
			object.objectID == GameObjectID::DummyArcaneThing2 ||
			object.objectID == GameObjectID::DummyArcaneThing3 ||
			object.objectID == GameObjectID::DummyArcaneThing4 ||
			object.objectID == GameObjectID::DummyArcaneThing5 ||
			object.objectID == GameObjectID::DummyArcaneThing6 ||
			object.objectID == GameObjectID::DummyArcaneThing7 ||
			object.objectID == GameObjectID::Act3SewerStairs ||
			object.objectID == GameObjectID::Act3SewerStairsToLevel3 ||
			object.objectID == GameObjectID::ExpansionTownGate ||
			object.objectID == GameObjectID::SiegeHellGate ||
			object.objectID == GameObjectID::PenBreakableDoor ||
			object.objectID == GameObjectID::ArreatSummitDoorToWorldstone
	);
}

bool Objects::isRack(OBJECT object) {
	return (
			object.objectID == GameObjectID::ArmorStandLeft ||
			object.objectID == GameObjectID::ArmorStandRight ||
			object.objectID == GameObjectID::WeaponRackLeft ||
			object.objectID == GameObjectID::WeaponRackRight ||
			object.objectID == GameObjectID::ExpansionArmorStandLeft ||
			object.objectID == GameObjectID::ExpansionArmorStandRight ||
			object.objectID == GameObjectID::ExpansionWeaponRackLeft ||
			object.objectID == GameObjectID::ExpansionWeaponRackRight
	);
}


bool Objects::isChest(OBJECT object) {
	return (
			(
					//(object.objectID == GameObjectID::LargeChestRight) ||
					//(object.objectID == GameObjectID::LargeChestLeft) ||
					(object.objectID == GameObjectID::TombLargeChestL) ||
					(object.objectID == GameObjectID::TombLargeChestR) ||
					//(object.objectID == GameObjectID::Act1LargeChestRight) ||
					(object.objectID == GameObjectID::Act1TallChestRight) ||
					(object.objectID == GameObjectID::Act1MediumChestRight) ||
					(object.objectID == GameObjectID::Act1LargeChest1) ||
					(object.objectID == GameObjectID::Act2MediumChestRight) ||
					(object.objectID == GameObjectID::Act2LargeChestRight) ||
					(object.objectID == GameObjectID::Act2LargeChestLeft) ||
					(object.objectID == GameObjectID::MediumChestLeft) ||
					(object.objectID == GameObjectID::LargeChestLeft2) ||
					(object.objectID == GameObjectID::JungleChest) ||
					(object.objectID == GameObjectID::JungleMediumChestLeft) ||
					(object.objectID == GameObjectID::TallChestLeft) ||
					(object.objectID == GameObjectID::Gchest1L) ||
					(object.objectID == GameObjectID::Gchest2R) ||
					(object.objectID == GameObjectID::Gchest3R) ||
					(object.objectID == GameObjectID::GLchest3L) ||
					(object.objectID == GameObjectID::MafistoLargeChestLeft) ||
					(object.objectID == GameObjectID::MafistoLargeChestRight) ||
					(object.objectID == GameObjectID::MafistoMediumChestLeft) ||
					(object.objectID == GameObjectID::MafistoMediumChestRight) ||
					(object.objectID == GameObjectID::SpiderLairLargeChestLeft) ||
					(object.objectID == GameObjectID::SpiderLairTallChestLeft) ||
					(object.objectID == GameObjectID::SpiderLairMediumChestRight) ||
					(object.objectID == GameObjectID::SpiderLairTallChestRight) ||
					(object.objectID == GameObjectID::HoradricCubeChest) ||
					(object.objectID == GameObjectID::HoradricScrollChest) ||
					(object.objectID == GameObjectID::StaffOfKingsChest) ||
					(object.objectID == GameObjectID::LargeChestR) ||
					(object.objectID == GameObjectID::InnerHellBoneChest) ||
					(object.objectID == GameObjectID::ArcaneLargeChestLeft) ||
					(object.objectID == GameObjectID::ArcaneLargeChestRight) ||
					(object.objectID == GameObjectID::ArcaneSmallChestLeft) ||
					(object.objectID == GameObjectID::ArcaneSmallChestRight) ||
					(object.objectID == GameObjectID::SparklyChest) ||
					(object.objectID == GameObjectID::KhalimChest1) ||
					(object.objectID == GameObjectID::KhalimChest2) ||
					(object.objectID == GameObjectID::KhalimChest3) ||
					(object.objectID == GameObjectID::ExpansionChestRight) ||
					(object.objectID == GameObjectID::ExpansionWoodChestLeft) ||
					(object.objectID == GameObjectID::BurialChestLeft) ||
					(object.objectID == GameObjectID::BurialChestRight) ||
					(object.objectID == GameObjectID::ExpansionChestLeft) ||
					(object.objectID == GameObjectID::ExpansionWoodChestRight) ||
					(object.objectID == GameObjectID::ExpansionSmallChestLeft) ||
					(object.objectID == GameObjectID::ExpansionSmallChestRight) ||
					(object.objectID == GameObjectID::ExpansionExplodingChest) ||
					(object.objectID == GameObjectID::ExpansionSpecialChest) ||
					(object.objectID == GameObjectID::ExpansionSnowyWoodChestLeft) ||
					(object.objectID == GameObjectID::ExpansionSnowyWoodChestRight) ||
					(object.objectID == GameObjectID::ExpansionSnowyWoodChest2Left) ||
					(object.objectID == GameObjectID::ExpansionSnowyWoodChest2Right)
			) &&
			(object.objectMode == GameObjectMode::Neutral) &&
			!(object.interactType & GameObjectInteractType::LockedChest) &&
			(object.interactType != GameObjectInteractType::DisabledChest)
	);
}

OBJECT Objects::FindChest(short x, short y) throw(...) {
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (isChest(*iter)) {
			object = *iter;
			break;
		}
	}

	cs.Unlock();
	return object;
}

std::vector<OBJECT> Objects::FindDoors(short x, short y, int radius) throw(...) {
	std::vector<OBJECT> doors;
	int maxCount = 50;

	OBJECT object;

	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		object = *iter;

		//is this a door?
		if (isDoor(object)) {
			//is within radius?
			if (object.getDistance(x, y) > radius)
				continue;

			doors.push_back(object);

			if (doors.size() >= maxCount) {
				break;
			}
		}
	}

	cs.Unlock();
	return doors;
}

std::vector<OBJECT> Objects::FindRacks(short x, short y, int radius) throw(...) {
	std::vector<OBJECT> racks;
	int maxCount = 50;

	OBJECT object;

	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		object = *iter;

		//is this object a chest?
		if (isRack(object)) {
			//is this chest within the given radius?
			if (object.getDistance(x, y) > radius)
				continue;

			//push this chest into the returned vector
			racks.push_back(object);

			if (racks.size() >= maxCount) {
				break;
			}
		}
	}

	cs.Unlock();
	return racks;
}

std::vector<OBJECT> Objects::FindChests(short x, short y, int radius) throw(...) {
	std::vector<OBJECT> chests;
	int maxCount = 50;

	OBJECT object;

	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		object = *iter;

		//is this object a chest?
		if (isChest(object)) {
			//is this chest within the given radius?
			if (object.getDistance(x, y) > radius)
				continue;

			//push this chest into the returned vector
			chests.push_back(object);

			if (chests.size() >= maxCount) {
				break;
			}
		}
	}

	cs.Unlock();
	return chests;
}


OBJECT Objects::FindShrine(GameObjectType::GameObjectType type) {
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectMode == GameObjectMode::Neutral && iter->interactType == type) {
			object = *iter;
			break;
		}
	}

	cs.Unlock();
	return object;
}

OBJECT Objects::FindPortal() throw(...) {
	//int result = -1;
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectID == GameObjectID::ArcaneSanctuaryPortal) {
			//result = iter->uid;
			object = *iter;
		}
	}

	cs.Unlock();
	//return result;
	return object;
}

OBJECT Objects::FindBank() throw(...) {
	//int result = -1;
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectID == GameObjectID::Bank) {
			//result = iter->uid;
			object = *iter;
		}
	}

	cs.Unlock();
	//return result;
	return object;
}

OBJECT Objects::FindTownPortal() throw(...) {
	//int result = 0;
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectID == GameObjectID::PermanentTownPortal) {
			//result = iter->uid;
			object = *iter;
		}
	}

	cs.Unlock();
	//return result;
	return object;
}

OBJECT Objects::FindTownPortalByOwner(unsigned int ownerUID) throw(...) {
	//int result = 0;
	OBJECT object;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectID == GameObjectID::TownPortal && iter->ownerUID == ownerUID) {
			//result = iter->uid;
			object = *iter;
		}
	}

	cs.Unlock();
	//return result;
	return object;
}

OBJECT Objects::FindPermanentPortalByDestination(AreaLevel::AreaLevel destination) throw(...) {
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		// Yes, it doesnt check the destination on baal, meph and arcane portals. Not needed.
		if ((iter->objectID == GameObjectID::BaalsPortal || iter->objectID == GameObjectID::HellGate ||
			 iter->objectID == GameObjectID::ArcaneSanctuaryPortal) ||
			(iter->objectID == GameObjectID::PermanentTownPortal && iter->destination == destination)) {
			object = *iter;
			break;
		}
	}

	cs.Unlock();
	return object;
}

void
Objects::SetGameObjectMode(unsigned int uid, bool canChangeBack, GameObjectMode::GameObjectMode objectMode) throw(...) {
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->uid == uid) {
			iter->canChangeBack = canChangeBack;
			iter->objectMode = objectMode;
		}
	}

	cs.Unlock();
}

OBJECT Objects::FindTownPortalByDestination(AreaLevel::AreaLevel destination) throw(...) {
	OBJECT object;
	object.uid = 0;
	cs.Lock();

	for (std::list<OBJECT>::iterator iter = list.begin(); iter != list.end(); iter++) {
		if (iter->objectID == GameObjectID::TownPortal && iter->destination == destination) {
			object = *iter;
		}
	}

	cs.Unlock();
	return object;
}
