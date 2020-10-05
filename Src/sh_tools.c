#include "sh_tools.h"
#include "mks_cfg.h"
#include "fastio_RobinNano.h"

unsigned char is_filament_fail(void) {
	if (gCfgItems.feature_mask & MASK_DETECTOR_FILAMENT)
	    return 0;
	if (gCfgItems.filament_det0_level_flg == MKS_MT_DET1_OP)
		return 1;
	if ((mksCfg.extruders == 2) && (gCfgItems.filament_det1_level_flg == MKS_MT_DET2_OP))
		return 1;
	return 0;
}
