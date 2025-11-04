# Sunxi-Tools H713 Support - Validation and Completion Plan

**Phase:** II - UART Access & Boot Analysis  
**Priority:** CRITICAL (Required for Phase III safety)  
**Status:** Software implementation complete, hardware validation pending  
**Created:** November 3, 2025

## Executive Summary

Previous research implemented H713 support in sunxi-tools (patch created, software working), but encountered **H713 BROM firmware bugs** that prevent FEL mode USB access. This plan outlines a comprehensive validation and workaround strategy using UART as the primary recovery method.

## Critical Finding from Previous Research

### ⚠️ H713 BROM USB Issue (CONFIRMED)

**Source:** `H713_FEL_PROTOCOL_ANALYSIS.md`

**Problem:** H713 BROM crashes when USB device is opened by libusb
- Device enumerates correctly (VID/PID: 1f3a:efe8)
- Crashes immediately on device open
- Enters reset loop until power cycle
- Affects ALL tools: sunxi-fel, lsusb -v, custom programs

**Root Cause:** BROM firmware bug (not sunxi-tools issue)

**Implication:** **FEL mode USB cannot be used reliably** for H713

### ✅ Software Implementation Status

**What was completed:**
1. H713 entry added to sunxi-tools `soc_info.c`
2. Patch file created: `sunxi-tools-h713-support.patch`
3. Memory addresses configured (from boot0.bin analysis)
4. Compiled and verified SoC recognition

**What remains:**
- Hardware validation (blocked by BROM bug)
- Alternative access method required

---

## Strategic Approach: UART-First Recovery

Since FEL mode USB is unreliable, we adopt a **UART-primary** strategy:

### Phase II.A: UART Console Establishment (MANDATORY)
**Before any risky operations, establish UART access.**

### Phase II.B: Sunxi-Tools Alternative Testing
**Test sunxi-tools via alternative methods.**

### Phase II.C: FEL Mode Workarounds (Optional)
**Explore potential FEL mode workarounds if time permits.**

---

## Phase II.A: UART Console Establishment

**Priority:** CRITICAL  
**Duration:** 1-2 days  
**Risk Level:** 🟢 LOW (hardware connection only)

### Objectives

1. **Establish reliable UART serial console**
2. **Validate U-Boot access via UART**
3. **Document recovery procedures**
4. **Test bootloader commands**

### Task Breakdown

#### Task 009: UART Hardware Connection ⏱️ 1-2 hours
**Status:** pending  
**Prerequisites:** Phase I complete

**Hardware Required:**
- USB-UART adapter (3.3V TTL)
- Female-to-female jumper wires
- Multimeter (for voltage verification)

**Steps:**
1. **Identify UART pins on HY300:**
   ```
   Expected (from Android kernel logs):
   - UART0: Debug console (115200n8)
   - Pins: TX, RX, GND (likely on header/test points)
   ```

2. **Verify voltage levels:**
   ```bash
   # Use multimeter to verify 3.3V logic levels
   # DO NOT connect if >3.3V
   ```

3. **Connect USB-UART adapter:**
   ```
   HY300     USB-UART
   TX    →   RX
   RX    →   TX  
   GND   →   GND
   ```

4. **Test connection:**
   ```bash
   # On development machine
   screen /dev/ttyUSB0 115200
   # or
   minicom -D /dev/ttyUSB0 -b 115200
   
   # Power cycle HY300, should see boot messages
   ```

**Success Criteria:**
- [ ] UART pins identified
- [ ] Voltage verified (3.3V)
- [ ] Connection made
- [ ] Boot messages visible
- [ ] U-Boot console accessible

**Deliverables:**
- `hardware-access/uart-pinout.md` with photos/diagrams
- `hardware-access/uart-connection-guide.md`

---

#### Task 010: U-Boot Console Access ⏱️ 1 hour
**Status:** pending  
**Prerequisites:** Task 009

**Objectives:**
- Access U-Boot console by interrupting boot
- Extract complete U-Boot environment
- Test critical commands
- Validate recovery capabilities

**Steps:**

1. **Interrupt boot sequence:**
   ```bash
   # Power on device while watching serial console
   # Press spacebar/enter repeatedly during U-Boot countdown
   # Should see: "Hit any key to stop autoboot:"
   ```

2. **Extract U-Boot environment:**
   ```bash
   U-Boot> printenv
   # Save complete output to file
   
   U-Boot> env print -a > /sdcard/uboot_env.txt
   ```

3. **Test critical commands:**
   ```bash
   # Display info
   U-Boot> version
   U-Boot> bdinfo
   
   # Storage access
   U-Boot> mmc list
   U-Boot> mmc dev 0
   U-Boot> mmc info
   
   # Memory operations
   U-Boot> md.l 0x40000000 0x100    # Read DRAM
   
   # Network (if available)
   U-Boot> dhcp
   ```

4. **Test boot commands:**
   ```bash
   # Boot factory Android (should work)
   U-Boot> boot
   
   # Or manually:
   U-Boot> load mmc 0:4 0x40080000 boot.img
   U-Boot> bootm 0x40080000
   ```

**Success Criteria:**
- [ ] U-Boot console accessible
- [ ] Environment extracted
- [ ] MMC commands working
- [ ] Factory Android boots via UART commands
- [ ] Can interrupt and control boot process

**Deliverables:**
- `backup/uboot_environment.txt`
- `hardware-access/uboot-commands.md`
- Boot command documentation

---

#### Task 011: UART Recovery Procedures ⏱️ 2-3 hours
**Status:** pending  
**Prerequisites:** Task 009-010

**Objectives:**
- Document complete recovery procedures
- Test bootloader re-flash via UART
- Validate factory restore
- Create emergency recovery guide

**Steps:**

1. **Document boot recovery:**
   ```bash
   # Via U-Boot UART console:
   
   # 1. Boot from SD card (if prepared)
   U-Boot> setenv bootcmd 'load mmc 1:1 0x40080000 boot.img; bootm'
   U-Boot> saveenv
   U-Boot> boot
   
   # 2. Boot over TFTP (network recovery)
   U-Boot> setenv serverip 192.168.1.100
   U-Boot> setenv ipaddr 192.168.1.200
   U-Boot> tftp 0x40080000 recovery.img
   U-Boot> bootm 0x40080000
   ```

2. **Test eMMC access:**
   ```bash
   # Read eMMC partitions
   U-Boot> mmc dev 0
   U-Boot> mmc part
   
   # Read boot partition
   U-Boot> mmc read 0x40080000 0x8000 0x8000
   U-Boot> md.b 0x40080000 0x100
   ```

3. **Prepare SD card recovery:**
   ```bash
   # On development machine:
   # Create bootable SD card with U-Boot and recovery kernel
   dd if=u-boot-sunxi-with-spl.bin of=/dev/sdX bs=1024 seek=8
   
   # Test booting from SD
   # Power off, insert SD, power on
   # Should boot from SD automatically
   ```

4. **Test factory restore:**
   ```bash
   # From U-Boot, restore factory boot
   U-Boot> env default -a
   U-Boot> saveenv
   U-Boot> boot
   # Should boot factory Android
   ```

**Success Criteria:**
- [ ] Can boot from SD card via UART
- [ ] Can boot over network (TFTP) via UART
- [ ] eMMC accessible and readable
- [ ] Factory boot commands documented
- [ ] Recovery procedures tested and validated

**Deliverables:**
- `hardware-access/uart-recovery-procedures.md`
- `hardware-access/sd-card-recovery-guide.md`
- Emergency recovery checklist

---

## Phase II.B: Sunxi-Tools Alternative Testing

**Priority:** HIGH  
**Duration:** 2-3 hours  
**Risk Level:** 🟢 LOW (non-invasive testing)

### Objectives

Test sunxi-tools H713 support via alternative methods that don't rely on FEL mode USB.

### Task 012: Validate H713 Configuration ⏱️ 1 hour
**Status:** pending  
**Prerequisites:** Phase II.A complete

**Approach:** Validate memory addresses against live hardware

**Steps:**

1. **From UART U-Boot console, verify SRAM addresses:**
   ```bash
   # Our sunxi-tools configuration:
   # SPL Address:     0x20000
   # Scratch Address: 0x21000
   # Thunk Address:   0x53a00
   
   # Validate via U-Boot:
   U-Boot> md.l 0x20000 0x100    # Read SPL region
   U-Boot> md.l 0x21000 0x100    # Read scratch region
   ```

2. **Verify SID (Security ID) registers:**
   ```bash
   # Our config: SID Base: 0x03006000
   U-Boot> md.l 0x03006000 0x10
   # Should show chip ID and other security info
   ```

3. **Compare with boot0.bin analysis:**
   ```bash
   # From Phase I, we have boot0.bin DRAM parameters
   # Verify these match what U-Boot reports
   U-Boot> bdinfo
   # Check DRAM base, size matches our analysis
   ```

4. **Cross-reference device tree:**
   ```bash
   # From running Android (ADB):
   adb shell su -c "cat /proc/device-tree/memory/reg"
   # Verify against our sunxi-tools memory map
   ```

**Success Criteria:**
- [ ] SPL address confirmed (0x20000)
- [ ] SID base address confirmed (0x03006000)
- [ ] SRAM sizes validated
- [ ] Memory map matches boot0.bin analysis

**Deliverables:**
- `phases/phase2-uart-access/h713-memory-validation.md`
- Updated sunxi-tools configuration if needed

---

### Task 013: Document Sunxi-Tools Status ⏱️ 1-2 hours
**Status:** pending  
**Prerequisites:** Task 012

**Objectives:**
- Consolidate all sunxi-tools work
- Document what works, what doesn't
- Provide upstream submission plan

**Content:**

1. **Implementation summary:**
   - Patch file location and content
   - Build instructions
   - Configuration rationale

2. **Validation results:**
   - Memory addresses confirmed
   - SID configuration validated
   - What was tested, what wasn't

3. **Known issues:**
   - FEL mode USB BROM bug
   - Workaround strategies
   - Alternative access methods

4. **Upstream submission plan:**
   - Patch ready for linux-sunxi project
   - Test results documentation
   - Community communication plan

**Deliverables:**
- `phases/phase2-uart-access/SUNXI_TOOLS_H713_STATUS.md`
- Patch ready for upstream submission

---

## Phase II.C: FEL Mode Workarounds (Optional)

**Priority:** LOW (Nice-to-have)  
**Duration:** 2-4 hours  
**Risk Level:** 🟡 MEDIUM (experimental)

### Why Optional?

**UART provides sufficient recovery capability.** FEL mode is a convenience feature, not a requirement. With UART:
- We can reflash bootloader
- We can boot recovery kernels
- We can access eMMC
- We have complete device control

**FEL mode would be nice for:**
- Quick SPL testing without SD card
- Automated testing workflows
- Development convenience

### Potential Workarounds to Explore

#### Workaround 1: Android FEL Trigger (Safest)

**Approach:** Boot Android, then trigger FEL mode from userspace

```bash
# From Android ADB:
adb shell su -c "reboot fel"
# or
adb shell su -c "echo fel > /sys/power/reboot_mode"
adb shell su -c "reboot"
```

**Advantage:**
- Avoids BROM USB bug (device already initialized)
- May work if BROM crash is only on cold boot

**Testing:**
1. Boot Android normally
2. Trigger FEL mode via ADB
3. Check if USB enumeration is stable
4. Test sunxi-fel commands

#### Workaround 2: Different USB Host

**Approach:** Test on different hardware/OS combinations

**Variations to try:**
- Windows PC (different USB stack)
- USB 2.0 hub (slower enumeration)
- Raspberry Pi (different USB controller)
- macOS (different libusb behavior)

**Rationale:** BROM bug might be timing-sensitive

#### Workaround 3: USB Protocol Tuning

**Approach:** Modify sunxi-tools USB initialization

**Areas to explore:**
1. **Slower enumeration:**
   ```c
   // Add delays during USB initialization
   libusb_open_device_with_vid_pid(...);
   usleep(1000000);  // 1 second delay
   ```

2. **Different descriptor reads:**
   ```c
   // Request only minimal descriptors
   // Avoid full configuration descriptor
   ```

3. **Reset before open:**
   ```c
   // Try USB reset before opening
   libusb_reset_device(handle);
   ```

**Note:** This is speculative - BROM bug may not be workaroundable

---

## Integration with Main Roadmap

### Phase II Updates

**Original Phase II:** UART Access & Boot Analysis (1-2 days)

**Enhanced Phase II:** UART Access & Sunxi-Tools Validation

**Updated Tasks:**
- Task 009: UART Hardware Connection (Phase II.A)
- Task 010: U-Boot Console Access (Phase II.A)
- Task 011: UART Recovery Procedures (Phase II.A)
- Task 012: Validate H713 Configuration (Phase II.B)
- Task 013: Document Sunxi-Tools Status (Phase II.B)
- Task 014: FEL Workarounds (Phase II.C - optional)

**Timeline:**
- Phase II.A (CRITICAL): 1-2 days
- Phase II.B (HIGH): 2-3 hours
- Phase II.C (OPTIONAL): 2-4 hours if time permits

**Total Phase II Duration:** Still 1-2 days (unchanged)

---

## Success Criteria

### Minimum Success (MANDATORY)
- [ ] UART console working reliably
- [ ] U-Boot accessible via UART
- [ ] Recovery procedures documented and tested
- [ ] Can reflash bootloader via UART
- [ ] Sunxi-tools memory map validated against hardware
- [ ] H713 configuration confirmed correct

### Optimal Success (DESIRED)
- [ ] Above minimum criteria PLUS:
- [ ] Sunxi-tools patch ready for upstream
- [ ] FEL mode workaround identified (if possible)
- [ ] Complete documentation for community

### Known Limitations (ACCEPTABLE)
- [ ] FEL mode USB may remain unreliable
- [ ] BROM bug documented as known issue
- [ ] UART-based recovery is primary method

---

## Risk Assessment

### Risks with Mitigation

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| UART pins damaged | Low | Critical | Verify voltage, careful connection |
| Wrong UART pins identified | Medium | Low | Test systematically, use multimeter |
| U-Boot password protected | Low | Medium | Unlikely on consumer device |
| FEL mode permanently broken | High | Low | UART recovery sufficient |
| Bootloader corruption during test | Low | Critical | Test on Device B first, UART recovery ready |

### Safety Protocol

**Device A vs B Strategy:**
1. **UART testing:** Start on Device B (safer)
2. **Recovery procedures:** Validate on Device B
3. **Risky operations:** Only on Device A after Device B success

**Before Any Testing:**
- [ ] Complete Phase I backup exists
- [ ] UART connection verified
- [ ] Recovery procedures documented
- [ ] Second device (Device B) available

---

## Documentation Structure

```
rebuild/phases/phase2-uart-access/
├── README.md                              # Phase II overview (existing)
├── SUNXI_TOOLS_H713_VALIDATION_PLAN.md   # This document
├── SUNXI_TOOLS_H713_STATUS.md            # Final status (Task 013)
└── h713-memory-validation.md             # Validation results (Task 012)

rebuild/hardware-access/
├── uart-pinout.md                         # UART hardware details
├── uart-connection-guide.md               # Connection procedures
├── uart-recovery-procedures.md            # Emergency recovery
├── sd-card-recovery-guide.md              # SD card boot
└── uboot-commands.md                      # U-Boot command reference
```

---

## Previous Research References

### What to Validate

**From `/docs/H713_BROM_MEMORY_MAP.md`:**
- ✅ SPL address: 0x104000 vs 0x20000 (CONFLICT - needs resolution)
- ✅ Stack pointer: 0x124000
- ⚠️ Previous research used boot0.bin disassembly (might be different)

**Resolution needed:** Compare boot0.bin memory layout vs H616 layout

**From `SUNXI_TOOLS_H713_SUMMARY.md`:**
- ✅ Patch created and ready
- ⚠️ Based on H616 memory layout
- ⚠️ Never hardware tested

**From `H713_FEL_PROTOCOL_ANALYSIS.md`:**
- ❌ FEL mode USB BROM bug confirmed
- ✅ Multiple fix attempts documented
- ⚠️ All USB-based approaches failed

### What to Keep

**Patch file:** `sunxi-tools-h713-support.patch` - Keep as baseline  
**Memory analysis:** Boot0.bin DRAM parameters - Cross-reference with U-Boot  
**Build system:** Nix flake configuration - Already working  

### What to Revise

**Memory addresses:** Validate 0x20000 vs 0x104000 SPL address conflict  
**Testing strategy:** Switch from FEL-first to UART-first  
**Recovery approach:** UART as primary, FEL as optional  

---

## Next Steps After Phase II

**Phase III Prerequisites:**
- ✅ UART recovery validated
- ✅ U-Boot console accessible
- ✅ Sunxi-tools memory map confirmed
- ✅ Emergency recovery procedures tested

**Phase III Will Use:**
- UART for bootloader flashing
- UART for boot monitoring
- SD card for safe testing
- Device B for validation first

**Sunxi-Tools Role in Later Phases:**
- Nice-to-have for development convenience
- Not blocking any critical operations
- Community contribution ready (patch + docs)

---

## Abort & Recover

**CRITICAL:** If something fails during this phase, follow these steps immediately.

### Failure Scenario: UART Connection Unstable

**Symptoms:** Garbled characters, frequent disconnects, no output  
**Recovery Time:** < 10 minutes

**Steps:**
```bash
# Verify baud rate (must be 115200)
minicom -D /dev/ttyUSB0 -b 115200
# Press Ctrl+A, O for settings

# Check signal integrity with multimeter:
# VDD: Should read 5.0V ±0.2V
# GND: Should show 0V

# Inspect cable: No bent connectors, clean contacts
# Try swapping RX/TX if no output

# Retry connection
sudo killall minicom 2>/dev/null
sleep 1
minicom -D /dev/ttyUSB0 -b 115200
```

### Failure Scenario: U-Boot SRAM Load Failed

**Symptoms:** sunxi-fel times out, device not detected  
**Recovery Time:** 5-15 minutes

**Steps:**
```bash
# Force BROM mode
# 1. Power OFF device completely
# 2. Hold recovery button
# 3. Apply power while holding button
# 4. Wait 1 second, release button

# Verify BROM enumeration
lsusb -v -d 1f3a:efe8

# Check sunxi-fel H713 support
/path/to/sunxi-fel -l

# Attempt load with verbose output
/path/to/sunxi-fel -v \
  --spl u-boot-with-spl.bin \
  --spl-addr 0x20000000

# If fails (known H713 BROM bug), use UART manual loading:
minicom -D /dev/ttyUSB0 -b 115200
# Send binary via xmodem (Ctrl+A, S in minicom)
```

### Failure Scenario: Bootloader Test Damaged Android

**Symptoms:** Device hangs at boot, won't reach Android  
**Recovery Time:** 30-60 minutes via UART

**Steps:**
```bash
# Access UART bootloader
minicom -D /dev/ttyUSB0 -b 115200
=> # Should see prompt

# Restore from backup
=> load mmc 0 0x40000000 /boot/android-kernel.bin
=> bootz 0x40000000

# If that fails: Use factory backup
=> load mmc 0 0x40000000 factory-backup.bin
=> mmc write 0x40000000 0x0 0x2000

=> reset  # Reboot
```

**CRITICAL: SRAM testing should not damage anything!**  
If Android won't boot after SRAM-only testing: Report as bug, check for accidental flashing.

### Emergency Device Brick Recovery

**If device completely unbootable:**

```bash
# Force BROM mode (may require physical reset)
# Power OFF, remove battery if possible
# Hold recovery button during power-on
# Device should enumerate in BROM mode

# Restore via sunxi-fel
lsusb -d 1f3a:efe8  # Verify BROM mode
/path/to/sunxi-fel write 0x00000000 factory-bootloader-backup.bin

# Reboot
# Device should boot Android normally
```

**For detailed recovery procedures:** See `phases/RECOVERY_TEMPLATE.md`

---

## Community Contribution Plan

After validation complete:

### 1. Upstream Submission to linux-sunxi

**Materials to provide:**
- Patch file with H713 support
- Hardware validation results
- Known issues documentation (BROM USB bug)
- Test results from UART validation

### 2. Documentation

**Wiki page:** "Allwinner H713 Support Status"
- Working: Memory map, configuration
- Not working: FEL mode USB (BROM bug)
- Workaround: UART-based recovery

### 3. Hardware Database

**Add H713 to sunxi hardware database:**
- SoC ID: 0x1860
- Board: HY300 Projector
- Status: Partially working (UART yes, FEL no)

---

**Created:** November 3, 2025  
**Status:** Planning document - ready for Phase II execution  
**Next Action:** Execute Task 009 (UART Hardware Connection) when Phase I complete
