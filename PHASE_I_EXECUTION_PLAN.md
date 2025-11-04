# Phase I Execution Plan - Tasks 004-009

**Status:** Planning  
**Current:** Tasks 001-003 ✅ Completed  
**Blocker:** CP2102 in transit (needed for Phase II UART work)  
**Strategy:** Continue Phase I without UART, prepare Phase II infrastructure

## Situation Analysis

### What We Have
✅ Complete eMMC dump (7.3 GB) with all partitions extracted
✅ Boot partitions (bootloader, boot, vendor_boot, dtbo)
✅ System filesystems mounted (system, vendor, product)
✅ FEX configuration analyzed
✅ Kernel source extracted (boot_fs/)

### What We Need for Remaining Tasks
- Task 004: Kernel modules from vendor_boot.bin (LZ4 decompression)
- Task 005: Device tree conversion (DTB→DTS via dtc)
- Task 006: Calibration data search in system_mount/vendor_mount
- Task 007: Boot analysis from dmesg (in system_mount)
- Task 008: WiFi firmware files from vendor_mount
- Task 009: Phase I summary & Phase II prep

### Dependencies
- **No UART needed** ✅ All tasks can use extracted data
- **No ADB needed** ✅ Device already crashed, using eMMC dump
- **No special hardware** ✅ Just analysis of extracted files

## Task Execution Strategy

### Task 004: Kernel Module Documentation (2-3 hours)
**Source:** `vendor_boot_a.bin` contains LZ4-compressed vendor modules

**Steps:**
1. Extract vendor_boot_a.bin via binwalk
2. Decompress LZ4 sections
3. Find and list all .ko files
4. Document each module:
   - File name and path
   - Size and compression
   - Section markers
   - Likely hardware association

**Commands:**
```bash
cd /home/luca/Desktop/hy300-linux-porting/backup/dumps/partitions

# Analyze vendor_boot_a.bin
file vendor_boot_a.bin
binwalk vendor_boot_a.bin | head -20

# Extract with binwalk
mkdir -p vendor_boot_extracted
binwalk -e -C vendor_boot_extracted vendor_boot_a.bin

# Find kernel modules
find vendor_boot_extracted -name "*.ko" -type f

# Analyze LZ4 sections
strings vendor_boot_a.bin | grep -i "module\|driver" | head -20
```

**Deliverables:**
- `phases/phase1-hardware-baseline/kernel_modules_inventory.md`
- `phases/phase1-hardware-baseline/vendor_boot_analysis.md`
- `backup/vendor_boot_extracted/` (modules)

---

### Task 005: Hardware Register Mapping (4-6 hours)
**Source:** Device tree in vendor_boot_a.bin (69.3 KB DTB)

**Steps:**
1. Extract DTB from vendor_boot_a.bin (binwalk finds at 0xD000)
2. Convert DTB to DTS (human-readable)
3. Parse device tree for:
   - GPIO definitions
   - Interrupt mappings
   - Clock tree
   - I2C/SPI devices
   - Memory regions
4. Cross-reference with FEX analysis

**Commands:**
```bash
# Extract DTB
dd if=vendor_boot_a.bin of=device_tree.dtb bs=1 skip=53248

# Install device tree compiler if needed
sudo apt-get install device-tree-compiler

# Convert DTB to DTS
dtc -I dtb -O dts device_tree.dtb -o device_tree.dts

# Search for GPIO configs
grep -n "gpio\|pin" device_tree.dts | head -50

# Search for interrupt definitions
grep -n "interrupt" device_tree.dts | head -30

# Search for clock definitions
grep -n "clock" device_tree.dts | head -20
```

**Deliverables:**
- `phases/phase1-hardware-baseline/device_tree.dts` (full)
- `phases/phase1-hardware-baseline/gpio_mappings.md`
- `phases/phase1-hardware-baseline/interrupt_assignments.md`
- `phases/phase1-hardware-baseline/clock_tree.md`

---

### Task 006: Calibration Data Extraction (2-3 hours)
**Source:** system_mount/ and vendor_mount/ filesystems

**Steps:**
1. Search for calibration-related files
2. Extract keystone motor calibration
3. Find display/color calibration
4. Locate thermal parameters
5. Preserve MAC addresses
6. Document all findings

**Search Patterns:**
```bash
cd /home/luca/Desktop/hy300-linux-porting/backup/dumps/partitions

# Calibration file search
sudo find system_mount -type f -name "*calib*"
sudo find vendor_mount -type f -name "*calib*"

# Configuration files
sudo find vendor_mount/etc -type f -name "*.conf"
sudo find vendor_mount/etc -type f -name "*.ini"

# Keystone/motor files
sudo find system_mount -type f -name "*keystone*"
sudo find system_mount -type f -name "*motor*"

# Display config
sudo find vendor_mount -path "*/display*"
sudo find vendor_mount -type f -name "*lcd*"

# Thermal
sudo find vendor_mount -name "*thermal*" -o -name "*ths*"

# MAC addresses
sudo grep -r "mac\|MAC" vendor_mount/etc/ 2>/dev/null | head -10
```

**Deliverables:**
- `backup/calibration_data/` (extracted files)
- `phases/phase1-hardware-baseline/calibration_inventory.md`
- `phases/phase1-hardware-baseline/display_config.md`
- `phases/phase1-hardware-baseline/thermal_parameters.txt`

---

### Task 007: Boot Process Analysis (2-3 hours)
**Source:** system_mount filesystem and boot_a.bin

**Steps:**
1. Parse boot_a.bin Android bootimg
2. Extract init.rc files
3. Analyze service startup
4. Extract kernel command line
5. Document boot timing

**Commands:**
```bash
cd /home/luca/Desktop/hy300-linux-porting/backup/dumps/partitions

# Extract Android bootimg
tools/abootimg -x boot_a.bin
# (or use manual DD to extract zImage + ramdisk)

# List init files in system mount
sudo find system_mount -name "init*.rc" -type f

# Read init configurations
sudo cat system_mount/init.sun50iw12p1.rc | head -100

# Search for services
sudo grep -n "^service\|^import" system_mount/init.sun50iw12p1.rc

# Extract kernel command line from boot partition
strings boot_a.bin | grep -i "buildprop\|cmdline" | head -20

# Check build properties
sudo cat system_mount/system/build.prop | grep -i "version\|board" | head -20
```

**Deliverables:**
- `phases/phase1-hardware-baseline/boot_process_documentation.md`
- `phases/phase1-hardware-baseline/init_rc_analysis.md`
- `phases/phase1-hardware-baseline/kernel_command_line.txt`
- `backup/boot_analysis/` (extracted components)

---

### Task 008: Network Configuration Baseline (1-2 hours)
**Source:** vendor_mount/etc/wifi/ and vendor_mount/firmware/

**Steps:**
1. Locate WiFi driver and firmware
2. Identify WiFi chip model
3. Extract firmware files
4. Document WiFi configuration
5. Analyze Bluetooth setup
6. Extract network capabilities

**Commands:**
```bash
cd /home/luca/Desktop/hy300-linux-porting/backup/dumps/partitions

# WiFi firmware location
sudo find vendor_mount/firmware -name "*wifi*" -o -name "*aic*" -o -name "*rt*"

# WiFi config files
sudo ls -la vendor_mount/etc/wifi/
sudo cat vendor_mount/etc/wifi/wifi_board_config.ini | head -50

# Bluetooth config
sudo find vendor_mount -name "*bt*.ini" -o -name "*bluetooth*"
sudo cat vendor_mount/etc/bt_configure*.ini 2>/dev/null

# Check for WiFi module
sudo find system_mount -name "*wifi*.ko" -o -name "*aic*.ko"

# Network interfaces config
sudo cat vendor_mount/etc/fstab.sun50iw12p1 | grep -i "mount\|network"
```

**Deliverables:**
- `backup/wifi_firmware/` (extracted firmware)
- `phases/phase1-hardware-baseline/network_baseline.md`
- `phases/phase1-hardware-baseline/wifi_analysis.md`
- `phases/phase1-hardware-baseline/bluetooth_config.md`

---

### Task 009: Phase I Summary & Phase II Preparation (2-3 hours)
**Dependencies:** Tasks 001-008 complete

**Steps:**
1. Verify all Phase I deliverables
2. Create completion report
3. Document findings summary
4. Prepare Phase II infrastructure
5. Document UART connection procedure
6. Plan bootloader analysis

**Deliverables:**
- `phases/phase1-hardware-baseline/PHASE_I_COMPLETION_REPORT.md`
- `phases/phase2-uart-access/README_PREPARED.md` (scaffolding)
- `phases/phase2-uart-access/UART_HARDWARE_SETUP.md`
- `phases/phase2-uart-access/BOOTLOADER_ANALYSIS_PLAN.md`

---

## Timeline

| Task | Duration | Start | Est. End | Status |
|------|----------|-------|----------|--------|
| 001 | 30 min | Nov 3 | Nov 3 | ✅ Done |
| 002 | 2-3 hrs | Nov 3 | Nov 4 | ✅ Done |
| 003 | 2-3 hrs | Nov 4 | Nov 4 | ✅ Done |
| 004 | 2-3 hrs | Nov 4 | Nov 4 | 🔄 Ready |
| 005 | 4-6 hrs | Nov 4 | Nov 5 | ⏳ Next |
| 006 | 2-3 hrs | Nov 5 | Nov 5 | ⏳ Soon |
| 007 | 2-3 hrs | Nov 5 | Nov 5 | ⏳ Soon |
| 008 | 1-2 hrs | Nov 5 | Nov 5 | ⏳ Soon |
| 009 | 2-3 hrs | Nov 6 | Nov 6 | ⏳ Final |

**Total:** ~19-25 hours (distributed Nov 4-6)

---

## Phase II Preparation (During Phase I)

While waiting for CP2102, we can prepare Phase II infrastructure:

### Phase II Requirements
1. **UART Hardware Setup Documentation**
   - CP2102 pin diagram
   - HY300 UART pins (PH00/PH01)
   - Connection diagram
   - Safety procedures

2. **Bootloader Analysis Framework**
   - U-Boot source analysis
   - Binary structure documentation
   - SPL vs U-Boot separation
   - Environment variable mapping

3. **Recovery Procedures**
   - FEL mode activation
   - UART download protocol
   - Bootloader restore procedure
   - Testing framework

### Phase II File Structure
```
phases/phase2-uart-access/
├── README.md (scaffolding)
├── UART_HARDWARE_SETUP.md
├── BOOTLOADER_ANALYSIS_PLAN.md
├── RECOVERY_PROCEDURES.md
└── tools/
    ├── uart_connection_test.sh
    ├── bootloader_analysis.sh
    └── fel_mode_utils.sh
```

---

## Resources Available Now

✅ **Kernel Source:** `build/kernel/boot_fs/` (108 KB extracted tar)
✅ **Device Tree:** `backup/dumps/partitions/vendor_boot_a.bin` (32 MB)
✅ **System Filesystem:** `backup/dumps/partitions/system_mount/` (mounted)
✅ **Vendor Filesystem:** `backup/dumps/partitions/vendor_mount/` (mounted)
✅ **Boot Image:** `backup/dumps/partitions/boot_a.bin` (64 MB)
✅ **Bootloader:** `backup/dumps/partitions/bootloader_a.bin` (32 MB)

---

## Risk Assessment

| Risk | Severity | Mitigation |
|------|----------|-----------|
| Mount point read-only issues | Low | Use sudo for access |
| LZ4 decompress failure | Low | Have alternative analysis methods |
| DTB parse failure | Low | Use strings/hexdump fallback |
| File permission errors | Low | Change file ownership if needed |
| Storage exhaustion | Low | Have 6GB free, tasks ~2GB max |

**Overall Phase I Risk:** 🟢 **LOW** - All data available, no hardware operations

---

## Success Criteria

Phase I completion requires:
- ✅ Task 001: Root access documented
- ✅ Task 002: Hardware configuration analyzed (FEX)
- ✅ Task 003: Complete system backup created
- ⏳ Task 004: Kernel modules extracted & documented
- ⏳ Task 005: Device tree analyzed & documented
- ⏳ Task 006: Calibration data located & archived
- ⏳ Task 007: Boot process analyzed & documented
- ⏳ Task 008: Network baseline established
- ⏳ Task 009: Phase I summary + Phase II prep complete

**Expected:** Phase I 100% complete by Nov 6 EOD
**Then:** Phase II (UART) starts when CP2102 arrives

---

**Last Updated:** November 4, 2025  
**Next Review:** Start Task 004
