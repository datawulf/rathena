// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "naturerage.hpp"

#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"
#include "skill_factory_druid.hpp"

SkillNatureRage::SkillNatureRage() : SkillImpl(AT_NATURE_RAGE) {
}

void SkillNatureRage::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	skillratio += -100 + 700 * skill_lv;
}

void SkillNatureRage::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	const status_change* sc = status_get_sc(&src);

	if (sc == nullptr)
		return;

	if (sc->hasSCE(SC_TRUTH_OF_ICE))
		element = ELE_WATER;
	else if (sc->hasSCE(SC_TRUTH_OF_WIND))
		element = ELE_WIND;
	else if (sc->hasSCE(SC_TRUTH_OF_EARTH))
		element = ELE_EARTH;
}

void SkillNatureRage::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST(BL_PC, src);
	status_change* sc = status_get_sc(src);

	if (sc != nullptr && sc->hasSCE(SC_TRUTH_OF_WIND)) {
		// Grants one charge; never fires the overcharged follow-up itself.
		uint16 lv = std::max<uint16>(1, sd != nullptr ? static_cast<uint16>(pc_checkskill(sd, AT_ROARING_PIERCER)) : 1);
		SkillFactoryDruid::addThunderingCharge(src, AT_ROARING_PIERCER, lv, 1);
	} else if (sc != nullptr && sc->hasSCE(SC_TRUTH_OF_EARTH)) {
		// Can complete growth but must not trigger Ground Bloom or consume it.
		int32 stacks = 2;
		if (sc->hasSCE(SC_GROUND_GROW))
			stacks += sc->getSCE(SC_GROUND_GROW)->val3;
		stacks = std::min(13, stacks);
		status_change_start(src, src, SC_GROUND_GROW, 10000, 0, 0, stacks, 0, 10000, SCSTART_NOAVOID);
	}

	skill_attack(BF_MAGIC, src, src, target, getSkillId(), skill_lv, tick, flag);
}
