# Recovery Template - Standard Recovery Procedures

**Status:** Reusable template for all phases  
**Created:** November 3, 2025  
**Criticality:** ESSENTIAL for operations safety  
**Applicable Phases:** All phases (Phase I through VII)

## Overview

This template defines standard recovery procedures for each development phase. Each phase README should include a customized "Abort & Recover" section based on this template, with phase-specific failure scenarios and recovery commands.

---

## Template Structure

Each Phase README should include:

```markdown
## Abort & Recover

**CRITICAL:** If something fails during this phase, follow these steps immediately.
```

---

## Template - Phase I: Hardware Baseline Establishment

### Failure Scenarios

#### Scenario 1.1: ADB Connection Lost
**Symptoms:** Device becomes unresponsive to ADB commands  
**Severity:** 🟡 MEDIUM (can reconnect)  
**Recovery Time:** < 5 minutes

**Root Causes:**
- USB cable disconnected
- ADB daemon crashed
- Device overheated

**Recovery Steps:**

```bash
# Step 1: Check physical connection
lsusb | grep -i "hy300\|sunxi"  # Should show device

# Step 2: Restart ADB daemon
adb kill-server
adb start-server
adb devices  # Should re-enumerate device

# Step 3: Verify connectivity
adb shell "echo 'Device responding'" || echo "Still offline"

# Step 4: If still offline - restart device
adb reboot
# Wait 30 seconds for boot
adb devices  # Should re-appear
```

**Prevention:**
- Use powered USB hub (device requires significant current)
- Avoid untethering during backup operations
- Monitor device temperature (should stay < 50°C)

---

#### Scenario 1.2: Backup Corruption Detected
**Symptoms:** Backup file size < 2GB or checksum mismatch  
**Severity:** 🔴 HIGH (backup unusable)  
**Recovery Time:** 30-60 minutes (restart backup)

**Root Causes:**
- Insufficient storage space
- Partial write before USB disconnect
- Storage device failure

**Recovery Steps:**

```bash
# Step 1: Verify checksum
cd /backup
sha256sum -c hy300-complete-backup.sha256

# If FAILED or mismatch:
ls -lah *.bin *.sha256

# Step 2: Check storage space
df -h /backup
# Must have > 5GB free for retry

# Step 3: If space insufficient - delete partial backup
rm -f hy300-complete-backup.bin.incomplete
rm -f hy300-*.partial

# Step 4: Restart backup process
adb shell "dd if=/dev/mmcblk0 bs=1M" | \
  tee /backup/hy300-complete-backup.bin | \
  sha256sum > /backup/hy300-complete-backup.sha256

# Step 5: Verify new backup
sha256sum -c /backup/hy300-complete-backup.sha256
```

**Prevention:**
- Check free storage: `df -h` before starting
- Use `-p` (progress) flag: `dd if=/dev/mmcblk0 bs=1M | pv > backup.bin`
- Verify checksums immediately after backup
- Keep backup on multiple storage devices

---

#### Scenario 1.3: Critical Data Extraction Failed
**Symptoms:** Essential files not extracted (dmesg, device tree, modules)  
**Severity:** 🟡 MEDIUM (can re-extract)  
**Recovery Time:** 5-10 minutes per file

**Root Causes:**
- Permission denied on extracted file
- ADB crash mid-transfer
- Filesystem read error on device

**Recovery Steps:**

```bash
# Step 1: Identify missing file
ls -la phase1-data/
# Compare with Phase I checklist

# Step 2: Re-extract single file
adb pull /path/on/device /local/path

# Example: If dmesg extraction failed:
adb shell dmesg > phase1-data/hy300-dmesg-retry.txt
adb shell dmesg | gzip > phase1-data/hy300-dmesg-retry.txt.gz

# Step 3: If permission denied:
adb shell su -c "cat /file > /sdcard/temp-file"
adb pull /sdcard/temp-file ./phase1-data/
adb shell su -c "rm /sdcard/temp-file"

# Step 4: Verify extraction
file phase1-data/hy300-dmesg-retry.txt
wc -l phase1-data/hy300-dmesg-retry.txt  # Should have > 1000 lines
```

**Prevention:**
- Test permissions before bulk extraction: `adb shell ls /proc/device-tree/`
- Create extraction log: `adb shell "ls -R /proc/device-tree" > extraction.log`
- Extract critical files multiple times for redundancy

---

### Recovery: Data Loss Prevention

**If Phase I data is lost but device still running:**

```bash
# IMMEDIATE: Re-backup everything
mkdir -p /backup/emergency-recovery-$(date +%Y%m%d_%H%M%S)
cd /backup/emergency-recovery-*/

# Core backups
adb shell "dd if=/dev/mmcblk0 bs=1M" | pv > complete-dump-RETRY.bin
adb pull / complete-android-filesystem/ 2>/dev/null &
adb pull /proc/device-tree/ device-tree-retry/

# Wait for all to complete
wait

# Verify
sha256sum complete-dump-RETRY.bin > complete-dump-RETRY.sha256
du -sh complete-android-filesystem/
```

---

### Recovery: Device Won't Boot Android

**If device becomes unbootable:**

```bash
# Step 1: Check if stuck in UART bootloader
minicom -D /dev/ttyUSB0 -b 115200
# If U-Boot prompt appears (=>), proceed to UART recovery

# Step 2: Restore backup via UART (Phase I complete, no modifications yet)
# See UART Recovery section below

# Step 3: If UART inaccessible, force BROM mode
# Power OFF device
# Hold recovery button
# Apply power
# Wait 1 second, release button
# Device should enumerate in BROM mode
lsusb -d 1f3a:efe8
# If found, use sunxi-fel to restore
```

---

## Template - Phase II: UART Access & Boot Analysis

### Failure Scenarios

#### Scenario 2.1: UART Connection Unstable
**Symptoms:** Garbled characters, frequent disconnects, no output  
**Severity:** 🟡 MEDIUM (can re-establish)  
**Recovery Time:** < 10 minutes

**Root Causes:**
- Loose cable connection
- Wrong baud rate (not 115200)
- Intermittent RX/TX lines
- Power supply noise

**Recovery Steps:**

```bash
# Step 1: Verify baud rate
minicom -D /dev/ttyUSB0 -b 115200
# Press Ctrl+A, O to open settings
# Verify: 115200, 8 bits, No parity, 1 stop bit

# Step 2: Check signal integrity
# Measure with oscilloscope if available:
# TX line: Should show clean 3.3V → 0V transitions
# RX line: Should show data from device

# Step 3: Inspect cable
# Visually: No bent connectors, clean contacts
# Test: Swap RX/TX, try again (if still no output, pins were swapped)

# Step 4: Verify device power
# Multimeter on VDD: Should read 5.0V ±0.2V
# Multimeter on GND: Should show 0V with VDD reference

# Step 5: Retry connection
sudo killall minicom 2>/dev/null
sleep 1
minicom -D /dev/ttyUSB0 -b 115200
# Should show UART output from running Android kernel
```

**Prevention:**
- Use industrial-grade UART-to-USB adapter (not cheap knockoffs)
- Solder connections instead of jumper wires (more reliable)
- Document working cable configuration (labels, pinout)
- Keep backup UART adapter on hand

---

#### Scenario 2.2: U-Boot SRAM Load Failed
**Symptoms:** sunxi-fel times out, device not detected, "Cannot open device"  
**Severity:** 🟡 MEDIUM (may require power cycle)  
**Recovery Time:** 5-15 minutes

**Root Causes:**
- Device not in BROM mode
- USB connectivity issue
- sunxi-fel H713 support missing
- BROM firmware bug (H713 crash on device open)

**Recovery Steps:**

```bash
# Step 1: Force BROM mode
# Power OFF device completely
# Hold recovery button (small button on device)
# While holding button, apply power
# Wait 1 second, then release button

# Step 2: Verify BROM enumeration
lsusb -v -d 1f3a:efe8
# Should show device: "ife8" = BROM mode

# Step 3: Check sunxi-fel H713 support
/path/to/sunxi-fel -l
# Output should include: "H6" or "H713"
# If not: recompile sunxi-tools with H713 patch

# Step 4: Attempt sunxi-fel load with verbose output
/path/to/sunxi-fel -v \
  --spl u-boot-with-spl.bin \
  --spl-addr 0x20000000

# Step 5: If "Device crashed" or "Timeout":
# This is the known H713 BROM bug
# Fall back to UART manual loading:
minicom -D /dev/ttyUSB0 -b 115200
# (Send binary via xmodem, see UART_BOOTLOADER_SAFETY_PROTOCOL.md)

# Step 6: If still failing
# Power cycle device multiple times (resets BROM state)
for i in {1..5}; do
  echo "Attempt $i: Power OFF"
  # Manually unplug power
  sleep 2
  echo "Attempt $i: Power ON + BROM mode"
  # Plug power while holding button
  sleep 1
  lsusb -d 1f3a:efe8 && echo "Found!" && break
done
```

**Prevention:**
- Document exact BROM button sequence (take photo/video)
- Verify BROM mode before each sunxi-fel attempt
- Use UART as primary method, FEL as secondary

---

#### Scenario 2.3: Bootloader Test Damaged Android Boot
**Symptoms:** Device hangs at boot, doesn't reach Android, stuck in U-Boot  
**Severity:** 🔴 HIGH (device unbootable)  
**Recovery Time:** 30-60 minutes via UART

**Root Causes:**
- Modified U-Boot loaded permanently (flashed to storage)
- Corrupted boot partition
- Invalid device tree

**CRITICAL: This should NOT happen if SRAM testing used correctly!**

**Recovery Steps:**

```bash
# Step 1: Access UART bootloader
minicom -D /dev/ttyUSB0 -b 115200
# Should show U-Boot prompt (=>)

# Step 2: If at U-Boot prompt:
=> mmc list
=> mmc info
=> mmc read 0x40000000 0x1000 0x10

# If MMC not recognized: Bootloader issue (fixable)
# If MMC works: Boot partition corrupted (needs restore)

# Step 3: Restore from backup via UART
# Option A: TFTP boot from network
=> setenv ipaddr 192.168.1.100
=> setenv serverip 192.168.1.1
=> tftpboot 0x40000000 backup-kernel.bin
=> bootz 0x40000000

# Option B: Load recovery image from SD card
# (Pre-prepare recovery image on FAT-formatted SD card)
=> load mmc 1 0x40000000 recovery.bin
=> bootz 0x40000000

# Step 4: Boot into recovery/backup Android
# Use running Android to restore full backup:
adb shell "dd if=/backup/hy300-complete-backup.bin of=/dev/mmcblk0 bs=1M"
adb reboot

# Step 5: Verify Android boots normally
# Device should show "HY300" splash screen, then Android
```

**Prevention:**
- **ONLY use SRAM loading in Phase II** (never flash bootloader)
- Verify SRAM address config: CONFIG_SYS_TEXT_BASE=0x20000000
- Test bootloader only, do not modify storage
- Keep Device B for testing, Device A for reference

---

### Recovery: UART Communication Lost

**If UART becomes inaccessible during bootloader test:**

```bash
# Step 1: Try alternative UART adapters
ls /dev/ttyUSB* /dev/ttyACM*
# Use different USB port

# Step 2: Power cycle device
# (May restore normal boot if hung in bootloader)

# Step 3: If device doesn't boot Android:
# Use Device B (control device) to test recovery procedures
# Document findings, apply to Device A

# Step 4: Access via sunxi-fel fallback
lsusb -d 1f3a:efe8
# If device in BROM: sunxi-fel can restore
```

---

## Template - Phase III: U-Boot Replacement

### Failure Scenarios

#### Scenario 3.1: Bootloader Flash Failed - Device Unbootable
**Symptoms:** Device stuck in bootloader, cannot reach Android, no UART output  
**Severity:** 🔴 CRITICAL (device may be bricked)  
**Recovery Time:** 1-3 hours via FEL/UART

**Root Causes:**
- Incomplete flash (power loss mid-write)
- Corrupted bootloader binary
- Wrong flash address
- SPI/NAND write failure

**Recovery Steps:**

```bash
# Step 1: FIRST - Do NOT power cycle
# Keep device ON and connected

# Step 2: Access via UART bootloader (if available)
minicom -D /dev/ttyUSB0 -b 115200
# If U-Boot prompt (=>), go to Step 5
# If stuck in BROM, go to Step 3
# If no output, go to Step 4

# Step 3: Force BROM mode (if stuck in bad U-Boot)
# Power OFF, hold button, power ON
# Verify BROM enumeration:
lsusb -d 1f3a:efe8

# Step 4: Restore via UART + sunxi-fel
# Load bootloader binary from RAM:
/path/to/sunxi-fel \
  --spl u-boot-working.bin \
  --spl-addr 0x20000000

# Step 5: Access U-Boot console
=> # Verify console available

# Step 6: Read corrupted bootloader for analysis
=> md 0x00000000 0x100  # Read first 256 bytes of SPI
# Save hex dump for analysis

# Step 7: Flash known-good bootloader
=> load mmc 0 0x40000000 bootloader-backup.bin
=> mw.b 0x40000000 0xFF 0x400000  # Clear target area
=> mmc write 0x40000000 0x0 0x2000  # Write to boot sector

# OR use sunxi-fel from running system:
/path/to/sunxi-fel write 0x00000000 bootloader-backup.bin

# Step 8: Verify write
=> md 0x00000000 0x100
# Compare with hexdump of known-good binary

# Step 9: Power cycle and boot
=> reset
# Device should boot normally
```

**Prevention:**
- **Test on Device B first** (never Device A without Device B validation)
- Use SRAM bootloader testing first (validate before flash)
- Never power off during flash (use UPS or battery backup)
- Verify bootloader checksum before and after flash
- Keep multiple bootloader backups

**CRITICAL - Device B Testing Protocol:**

```bash
# Phase III should proceed as:
# 1. SRAM test on Device B (safe, reverts on power cycle)
# 2. Permanent flash on Device B (with recovery plan ready)
# 3. Monitor Device B for 24 hours (stability check)
# 4. Only then: Proceed to Device A (same steps)

# This gives 2x safety margin before affecting production device
```

---

#### Scenario 3.2: Bootloader Incompatible with Android
**Symptoms:** Bootloader works, but Android won't boot after U-Boot replacement  
**Severity:** 🟡 MEDIUM (can revert via UART)  
**Recovery Time:** 30-60 minutes

**Root Causes:**
- U-Boot environment mismatch (bootcmd incorrect)
- Device tree incompatibility
- Memory map changed
- Missing kernel image

**Recovery Steps:**

```bash
# Step 1: Check U-Boot environment
minicom -D /dev/ttyUSB0 -b 115200
=> printenv
=> printenv bootcmd
# Verify bootcmd loads Android kernel correctly

# Step 2: Fix bootcmd if needed
=> setenv bootcmd "mmc read 0x40000000 0x1000 0x2000; bootz 0x40000000"
=> saveenv

# Step 3: Try boot
=> boot
# If still hangs, capture UART output:
# Press Ctrl+A, C to log in minicom

# Step 4: Manually trace boot process
=> load mmc 0 0x40000000 /boot/zImage
=> load mmc 0 0x48000000 /boot/sun50i-h6.dtb
=> setenv bootargs "console=ttyS0,115200n8"
=> bootz 0x40000000 - 0x48000000
# Observe where boot fails

# Step 5: Restore factory bootloader
# Via UART/sunxi-fel:
/path/to/sunxi-fel write 0x00000000 factory-uboot-backup.bin
=> reset
```

**Prevention:**
- Export/document factory U-Boot environment before replacement
- Create validated bootcmd that works with both U-Boot and Android
- Test bootcmd in SRAM environment before permanent flash

---

### Recovery: Device A Emergency Revert

**If Device A bootloader fails:**

```bash
# ASSUME: Device B successfully running modified U-Boot
# Use Device B to help recover Device A

# Step 1: If Device A UART accessible
# Same steps as above, use Device B as reference

# Step 2: If Device A UART fails
# Last resort: Use sunxi-fel + Device B reference
# Device A: BROM mode, restored via FEL
# Device B: Reference for correct bootloader binary

# Procedure:
lsusb -d 1f3a:efe8  # Device A in BROM
/path/to/sunxi-fel write 0x00000000 factory-uboot-backup.bin
# Device A should boot normally after power cycle
```

---

## Template - Phase IV and Beyond

For Phases IV-VII (Kernel, Drivers, ROM), failure recovery follows similar patterns:

### Standard Recovery Procedure (Phases IV-VII)

```markdown
## Abort & Recover

### Bootloader Available (Device accessible via UART/U-Boot console)

1. Access UART: `minicom -D /dev/ttyUSB0 -b 115200`
2. Boot recovery image: `=> load mmc 0 0x40000000 recovery.bin; bootz 0x40000000`
3. Restore from backup: `adb shell "dd if=/backup/phase-X-backup.bin of=/dev/mmcblk0 bs=1M"`
4. Reboot: `adb reboot`

### Bootloader Inaccessible (Device won't boot)

1. Force BROM mode: Power OFF, hold button, power ON
2. Restore bootloader: `/path/to/sunxi-fel write 0x00000000 factory-uboot.bin`
3. Boot into Android recovery
4. Restore full system: `adb shell "dd if=/backup/complete-backup.bin of=/dev/mmcblk0 bs=1M"`

### Multiple Devices Strategy

- **Device B:** Test recovery procedures first
- **Device A:** Only apply proven procedures from Device B
- **Device C (if available):** Parallel testing of alternatives
```

---

## A/B Testing Fallback Strategy

### Device Assignment

- **Device A:** Primary development (start with Phase III changes)
- **Device B:** Control/Reference (verify recovery with Device A state)
- **Backup Strategy:** Full system snapshot before each phase transition

### Fallback Protocol

```bash
# Before each major phase transition:

# Step 1: Snapshot current state on Device A
adb shell "dd if=/dev/mmcblk0 bs=1M" | \
  pv | gzip > /backup/device-a-phase-X.bin.gz

# Step 2: Document Device A configuration
adb shell "getprop" > /backup/device-a-props.txt
adb shell "cat /proc/cpuinfo" > /backup/device-a-cpuinfo.txt

# Step 3: Prepare Device B with same state (optional, for A/B comparison)
adb -s <device-b-serial> shell "dd if=/dev/mmcblk0 bs=1M" | \
  pv | gzip > /backup/device-b-phase-X.bin.gz

# Step 4: If Device A fails during phase X
# Restore Device A from phase X-1:
adb shell "gunzip -c /backup/device-a-phase-$(($X-1)).bin.gz | \
  dd of=/dev/mmcblk0 bs=1M"
adb reboot
```

---

## Critical Backup Structure

```
/backup/
├── hy300-complete-factory-backup.bin       # Original complete dump (immutable)
├── hy300-complete-factory-backup.sha256    # Verification checksum
│
├── device-a-phase-1.bin.gz                 # Phase I snapshot (after Phase I)
├── device-a-phase-2.bin.gz                 # Phase II snapshot
├── device-a-phase-3-pre.bin.gz             # Pre-Phase III
├── device-a-phase-3-post.bin.gz            # Post-Phase III
│
├── device-b-phase-1.bin.gz                 # Device B parallel tracking
├── device-b-phase-2.bin.gz
├── device-b-phase-3-pre.bin.gz
├── device-b-phase-3-post.bin.gz
│
└── emergency/                               # Latest recovery points
    ├── latest-bootloader.bin                # Current working bootloader
    ├── latest-kernel.bin                    # Current working kernel
    └── latest-dtb.bin                       # Current working device tree
```

---

## Recovery Decision Tree

```
Device Won't Boot
├── Has UART Output?
│   ├── YES → U-Boot prompt (=>)
│   │   └── → Follow bootloader recovery (restore from UART)
│   │
│   ├── YES → Kernel panic / error messages
│   │   └── → Document error, follow Phase IV-VII recovery
│   │
│   ├── NO → Force BROM mode
│   │   ├── BROM found (lsusb)?
│   │   │   └── → Restore bootloader via sunxi-fel
│   │   │
│   │   └── No BROM → Hardware issue
│   │       └── → Use Device B as reference, check power/UART
│   │
│   └── NO → UART inaccessible
│       └── → Power cycle, try sunxi-fel FEL mode
```

---

## Contact/Escalation

If recovery steps don't work:

1. **Document exact failure:** Screenshot UART output, save logs
2. **Compare with Device B:** Verify Device B exhibits same issue
3. **Consult research:** Check `research/H713_FEL_PROTOCOL_ANALYSIS.md`
4. **Review UART logs:** `phases/phase2-uart-access/UART_BOOTLOADER_SAFETY_PROTOCOL.md`
5. **Escalate:** If both devices fail, hardware issue likely (check power, UART cable)

---

## References

- `phases/phase2-uart-access/UART_BOOTLOADER_SAFETY_PROTOCOL.md` - Bootloader testing
- `phases/phase1-hardware-baseline/README.md` - UART pinout
- `research/H713_FEL_PROTOCOL_ANALYSIS.md` - FEL mode technical details
- `research/USING_H713_FEL_MODE.md` - Workaround strategies

---

**Last Updated:** November 3, 2025  
**Template Version:** 1.0  
**Applicable to Phases:** I through VII
