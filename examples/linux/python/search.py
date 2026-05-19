import defs
import sys

from lw14_class import lw14

if __name__ == "__main__":

    dalibus = lw14()
    dalibus.SetI2cBus(defs.LW14_I2C_ADDRESS_1)

    # --- full scan 0..63 ---
    for i in range(0, defs.LW14_MAX_SINGLE):
        dalibus.SetDaliAddress(i, defs.LW14_ADR_SINGLE, defs.LW14_MODE_CMD)
        res = dalibus.ReadQuery(defs.DA_QUERY_CONTROL_GEAR)
        dalibus.WaitForReady()
    
        if res >= 0:
            print("ADR %02d: 0x%02X" % (i, res))
        else:
            print("ADR %02d: err %d" % (i, res))
