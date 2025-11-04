# UART Bootloader Safety Protocol

**Phase:** II - UART Access & Boot Analysis  
**Created:** November 3, 2025  
**Criticality:** ESSENTIAL for Phase III safe bootloader replacement  
**Risk Level:** 🟢 LOW (read-only operations only)

## Executive Summary

This protocol defines **safe bootloader testing procedures** using UART console access without risking device modification. We use **SRAM-based U-Boot loading** to validate bootloader functionality on real hardware before any permanent flashing.

**Key Principle:** *Test everything via SRAM first, only flash to permanent storage after validation.*

---

## Why SRAM-Based Testing is Safer

### Traditional Bootloader Flashing (High Risk ❌)
```
1. Compile U-Boot
2. Flash directly to SPI/NAND storage
3. Reboot to test
4. If broken → device unbootable → recovery required
5. Multiple iterations = multiple risk points
```

**Risk:** Each flash cycle risks permanent damage (bad erase, power loss, etc.)

### SRAM-Based Testing (Safe ✅)
```
1. Compile U-Boot for SRAM execution
2. Load via UART (sunxi-fel or manual)
3. Execute in RAM (no storage touched)
4. Test all functions
5. Power cycle to revert (clean)
6. Iterate safely until validated
7. Only flash final, tested version
```

**Safety:** Any failure reverts on power cycle, zero permanent damage risk.

---

## Part I: Preparation

### Prerequisites

✅ UART serial console connection established  
✅ H713 device accessible (power + UART cable)  
✅ sunxi-tools compiled with H713 support  
✅ U-Boot source configured for H713  
✅ Bootloader analysis completed (Phase I)  
✅ Complete device backup verified  

### Required Tools

```bash
# sunxi-fel with H713 support
/path/to/sunxi-tools/sunxi-fel

# U-Boot compiled for SRAM loading
u-boot-with-spl.bin  (built with CONFIG_SYS_TEXT_BASE=0x20000000)

# Serial console terminal
minicom / picocom / screen

# Diagnostic tools
hexdump, xxd, od
```

### Backup Validation

Before ANY bootloader testing:

```bash
# Verify backup integrity
sha256sum /backup/hy300-complete-backup.bin > /backup/hy300.sha256
cat /backup/hy300.sha256

# Store backup on multiple locations (NAS, USB, external SSD)
# Never test without verified, multi-copy backup
```

---

## Part II: U-Boot Compilation for SRAM

### Step 1: Configure U-Boot for SRAM Execution

```bash
# Download/prepare U-Boot source for H713/H6 compatible board
cd u-boot/
git log --oneline | head -5  # Verify version

# Configure for SRAM-based execution
# CRITICAL: Use SRAM address range that doesn't conflict with kernel
cat >> .config << 'EOF'
CONFIG_SYS_TEXT_BASE=0x20000000
CONFIG_SYS_SPL_START=0x20000000
CONFIG_SYS_SPL_MALLOC_START=0x20040000
CONFIG_SYS_LOAD_ADDR=0x40000000
EOF

# Build U-Boot with SPL
make H6_defconfig  # Or H713 if available
make -j$(nproc)

# Verify output files
ls -lah:
  u-boot-with-spl.bin  (main bootloader binary)
  u-boot.bin           (U-Boot binary only)
  spl/u-boot-spl.bin   (SPL secondary program loader)
```

### Step 2: Validate Binary Size for SRAM

```bash
# H713 SRAM layout (from Phase I analysis):
# 0x00000000-0x1FFFF: 128 KB (reserved for BROM/firmware)
# 0x20000000-0x3FFFF: 128 KB (safe for U-Boot SPL)
# 0x40000000-0x4FFFF: 64 KB (U-Boot main)

BINARY_SIZE=$(stat -c%s u-boot-with-spl.bin)
MAX_SRAM=131072  # 128 KB

if (( BINARY_SIZE > MAX_SRAM )); then
    echo "❌ Binary too large: $BINARY_SIZE > $MAX_SRAM bytes"
    echo "Reduce U-Boot config or use smaller variant"
    exit 1
fi

echo "✅ Binary fits SRAM: $BINARY_SIZE / $MAX_SRAM bytes"
hexdump -C u-boot-with-spl.bin | head -20
```

---

## Part III: SRAM Loading via UART

### Method A: Using sunxi-fel (Recommended for H713)

**Prerequisites:**
- H713 device in BROM mode (power + hold button, or cold boot)
- UART terminal open (secondary connection)
- sunxi-tools compiled with H713 support

```bash
# Step 1: Identify device in BROM mode
lsusb -v -d 1f3a:efe8  # Should show "ife8 usb device"

# Step 2: Load U-Boot binary to SRAM
/path/to/sunxi-tools/sunxi-fel \
  --spl u-boot-with-spl.bin \
  --spl-addr 0x20000000 \
  write 0x40000000 u-boot.bin

# Step 3: Execute bootloader
# (Device should boot U-Boot, output visible on UART terminal)

# Step 4: Verify execution
# Expected UART output:
#   U-Boot SPL ...
#   U-Boot ...
#   H6 board...
#   Hit any key to stop...
```

### Method B: Manual UART Loading (Fallback)

If sunxi-fel has connectivity issues:

```bash
# Step 1: Connect via UART terminal
minicom -D /dev/ttyUSB0 -b 115200

# Step 2: Interrupt BROM boot (press key during boot)
# (BROM console should appear)

# Step 3: Send binary via xmodem
# From minicom: Ctrl+A, S, xmodem
# Select file: u-boot-with-spl.bin

# Step 4: Execute from loaded address
# BROM> go 0x20000000
# (Should execute U-Boot)
```

---

## Part IV: Bootloader Validation Sequence

### Safety Checkpoint 1: SPL Initialization

Expected UART output:
```
U-Boot SPL 2024.01 (Nov 03 2025 - 12:34:56 +0000)
DRAM: 2048 MiB (DDR3, 533 MHz, 32-bit)
Loading Environment...
Loading Kernel...

```

**Validation:**
- ✅ SPL initializes DRAM correctly
- ✅ DRAM size matches known configuration (2GB)
- ✅ No memory errors reported
- ✅ SPL can locate kernel image

### Safety Checkpoint 2: U-Boot Console

Expected behavior:
```
U-Boot 2024.01 (Nov 03 2025 - 12:34:56 +0000)

CPU: Allwinner H6 (SID: xxxxxxxx)
Model: H6 generic board
DRAM: 2 GiB (DDR3, ...

Hit any key to stop autoboot:  0
=>
```

**Validation:**
- ✅ U-Boot prompt available (=>)
- ✅ CPU identification correct
- ✅ DRAM detection working
- ✅ Accepts commands

### Safety Checkpoint 3: Command Execution Tests

```bash
# Test 1: Print environment variables
=> printenv
=> echo $baudrate  # Should show 115200

# Test 2: Memory access (read-only)
=> md 0x40000000 0x10  # Read 16 bytes from kernel load address
=> md 0x20000000 0x10  # Read SPL area

# Test 3: Device listing
=> mmc list
=> mmc info         # Should show MMC device info

# Test 4: Boot script validation
=> load mmc 0 0x40000000 /boot/boot.scr
=> source 0x40000000  # Execute boot script (SAFE in SRAM)

# Test 5: Ethernet (if applicable)
=> net list
=> dhcp            # Test network boot capability
=> tftpboot 0x40000000 uImage  # Load kernel from TFTP
```

**CRITICAL:** Stop here if any errors occur. Do NOT proceed to flashing.

### Safety Checkpoint 4: Kernel Boot Attempt (Optional)

If all above tests pass, attempt factory kernel boot:

```bash
=> load mmc 0 0x40000000 /boot/zImage
=> load mmc 0 0x48000000 /boot/sun50i-h6-hy300.dtb
=> bootz 0x40000000 - 0x48000000
```

**Expected Result:**
- Kernel starts booting
- Eventually hangs (expected - no mainline rootfs)
- **POWER CYCLE TO REVERT** (all in SRAM, unchanged storage)

**DO NOT interrupt kernel boot with ctrl+C** - let it complete or timeout.

---

## Part V: Issue Diagnosis and Recovery

### Issue: UART Output Garbled

```
Symptoms: Output shows ???, ▬▬▬, or random characters

Solution:
1. Check baud rate: 115200 bps (8N1)
   minicom -D /dev/ttyUSB0 -b 115200

2. Check cable: Swap RX/TX if necessary
   TX (device) → RX (terminal)
   RX (device) → TX (terminal)
   GND → GND

3. Check power: Device receiving stable 5V
   Multimeter: Measure VDD_USB (should be ~5.0V)
```

### Issue: sunxi-fel Connection Fails

```
Symptoms: "Cannot open device" or timeout

Solution:
1. Verify BROM mode:
   Device power OFF
   Hold recovery button
   Apply power
   Wait 1 second
   Release button
   
2. Check USB:
   lsusb -v -d 1f3a:efe8  # Should appear
   
3. Verify H713 support:
   /path/to/sunxi-fel -l
   # Should list "H6" or "H713" as supported
   
4. Run with sudo:
   sudo /path/to/sunxi-fel [commands]
   # USB access may require elevated privileges
```

### Issue: SPL Hangs at DRAM Initialization

```
Symptoms: Output stops after "DRAM: 2048 MiB"

Solution:
1. Reduce DRAM speed in U-Boot config:
   CONFIG_DRAM_CLK_MHZ=300  (reduced from 533)
   Recompile and retry

2. Check memory voltage:
   Factory firmware should show VDD_DDR in UART logs
   Ensure power supply stable

3. Manual memory test:
   Wait 30 seconds for timeout
   Power cycle
   Try again
   
If consistent: Memory module may be faulty
Fallback: Use Device B for testing
```

### Issue: Command "write" or "load" Fails

```
Symptoms: "Error: MMC not initialized" or similar

Solution:
1. Verify Device Tree includes storage:
   => fdt list /mmc@1c0f000
   
2. Try manual initialization:
   => mmc rescan
   => mmc list
   => mmc info
   
3. Check for storage corruption:
   Power cycle, try again
   If persistent: storage may need recovery image rewrite
```

### Issue: U-Boot Refuses to Boot Kernel

```
Symptoms: "Bad CRC" or "Image not found"

Solution:
1. Verify kernel image location:
   => load mmc 0 0x40000000 /boot/zImage
   # Check address returned
   
2. List available boot files:
   => fatls mmc 0 /boot
   
3. If no images: factory bootloader may use different partition
   Analyze boot device:
   => mmc dev 0 0  (main partition)
   => mmc dev 0 1  (boot partition 1)
   => mmc dev 0 2  (boot partition 2)
```

---

## Part VI: Documentation and Decision Points

### Decision Point: Proceed to Permanent Flash?

**Only after ALL of the following:**

- [ ] SPL initializes DRAM without errors
- [ ] U-Boot console responsive to commands
- [ ] Memory read-write tests successful (non-destructive)
- [ ] Boot script loads and executes
- [ ] Kernel boot attempted (tolerates hangup)
- [ ] Device recovered cleanly after power cycle
- [ ] **Backup verified on 2+ storage devices**
- [ ] A/B device strategy planned (Device B tested first)

**If ANY check fails:**
- [ ] Document the specific failure
- [ ] Update UART_BOOTLOADER_SAFETY_PROTOCOL.md with findings
- [ ] Investigate root cause (see Part V: Diagnosis)
- [ ] DO NOT PROCEED TO FLASHING

### Documenting Test Results

Create test report:

```markdown
# UART Bootloader Validation Report
**Date:** [Date]
**Device:** HY300 Device [A/B]
**U-Boot Version:** [Version]
**UART Connection:** [Port, Baud rate]

## Test Results

| Checkpoint | Status | Notes |
|-----------|--------|-------|
| SPL Init | ✅ Pass | DRAM 2GB detected |
| U-Boot Prompt | ✅ Pass | Console responsive |
| Memory R/W | ✅ Pass | 16 reads successful |
| MMC Detection | ✅ Pass | /dev/mmcblk0 found |
| Kernel Load | ⚠️ Timeout | Expected on SRAM boot |
| Power Cycle Recovery | ✅ Pass | Clean revert to Android |

## Issues Encountered
[Document any errors, solutions, and workarounds]

## Recommendations
[Proceed to flashing / Additional testing required / Hardware investigation needed]
```

---

## Part VII: Moving to Phase 2.B Sunxi-Tools Alternative Testing

After UART bootloader validation succeeds, proceed with:

1. **FEL Mode Testing** (if H713 BROM issue permits)
   - Use UART as fallback if FEL fails
   - Document success/failure

2. **UART + sunxi-fel Hybrid**
   - Load bootloader via UART (reliable)
   - Test commands via both channels

3. **Permanent Flash Decision**
   - Only after both Device B and Device A UART tests pass
   - Flash to backup SPI/NAND with immediate UART recovery available

---

## Part VIII: Critical Safety Rules

**ABSOLUTE NON-NEGOTIABLE:**

1. **NEVER test on Device A first**
   - Device A = production test device
   - Validate on Device B first
   - Only apply proven procedure to Device A

2. **ALWAYS keep UART connection during bootloader work**
   - Recovery via UART must be possible
   - If UART fails → device may be unrecoverable

3. **NEVER skip power cycle recovery test**
   - Verify device reverts cleanly
   - If not → SRAM testing is not safe

4. **ALWAYS store backup in multiple locations**
   - Local backup on development machine
   - Network backup (NAS)
   - USB backup
   - Cloud backup (if privacy acceptable)

5. **NEVER flash without SRAM validation passing**
   - Each flash is irreversible (until recovery firmware available)
   - SRAM testing is free and safe
   - No shortcuts

---

## Next Steps After Validation

1. ✅ Phase II.A: UART bootloader validation complete
2. → Phase II.B: Sunxi-tools alternative testing (FEL mode retry)
3. → Phase III: Permanent bootloader replacement (Device B first)
4. → Phase IV: Kernel bootstrap with validated U-Boot

---

## References

- `phases/phase1-hardware-baseline/README.md` - UART pinout identification
- `research/H713_FEL_PROTOCOL_ANALYSIS.md` - FEL mode technical details
- `research/USING_H713_FEL_MODE.md` - Workaround strategies
- `phases/RECOVERY_TEMPLATE.md` - Recovery procedures (see related docs)
- U-Boot documentation: https://u-boot.readthedocs.io/

---

## Appendix: Command Reference

### sunxi-fel Command Examples

```bash
# Load and execute U-Boot from SRAM
sunxi-fel --spl u-boot-with-spl.bin --spl-addr 0x20000000

# Load kernel and DTB for TFTP boot
sunxi-fel write 0x40000000 zImage
sunxi-fel write 0x48000000 sun50i-h6.dtb

# Read memory for debugging
sunxi-fel read 0x20000000 0x1000 > sram_dump.bin
hexdump -C sram_dump.bin

# Write diagnostics
sunxi-fel write 0x40000000 diagnostic.bin
sunxi-fel exe 0x40000000

# Dump BROM for analysis
sunxi-fel readext 0x00000000 0x08000 > brom_dump.bin
hexdump -C brom_dump.bin | head -50
```

### U-Boot UART Commands

```bash
# Environment
=> printenv
=> setenv bootdelay 5
=> setenv bootcmd 'mmc read 0x40000000 0x1000 0x2000; bootz 0x40000000'

# Memory
=> md 0x40000000  # Memory display
=> mw 0x40000000 0xDEADBEEF  # Memory write (use with CARE)
=> cmp 0x40000000 0x50000000 0x1000  # Memory compare

# MMC/Storage
=> mmc list
=> mmc info
=> mmc read 0x40000000 0x1000 0x100
=> mmc write 0x40000000 0x1000 0x100

# Boot
=> boot
=> reset
=> poweroff
```

---

**Last Updated:** November 3, 2025  
**Status:** Ready for Phase II implementation  
**Next Review:** After Device B UART validation completes
