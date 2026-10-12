// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "plumepiercer.hpp"

#include "map/skill.hpp"
#include "map/status.hpp"

SkillPlumePiercer::SkillPlumePiercer() : SkillImplRecursiveDamageSplash(AT_PLUME_PIERCER) {
}

void SkillPlumePiercer::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& skillratio, int32 mflag) const {
	const status_data* sstatus = status_get_status_data(*src);

	skillratio += -100 + 900 * skill_lv;
	// TODO: Confirm official CON scaling; the coefficient 5 is provisional.
	skillratio += 5 * sstatus->con;
	RE_LVL_DMOD(100);
}
