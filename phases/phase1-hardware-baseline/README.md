# Phase I: Hardware Baseline Establishment

**Status:** 🎯 **ACTIVE**  
**Start Date:** November 3, 2025  
**Estimated Duration:** 2-3 days  
**Prerequisites:** Root access (✅ Available)

## Overview

This phase establishes a complete baseline of the current working Android system before any modifications. We leverage root access to document every aspect of the hardware, extract all configuration data, and create comprehensive backups.

**Why This Phase Matters:**
- Creates safety net for all future work
- Documents working hardware configuration
- Preserves calibration and settings
- Enables evidence-based development
- Provides rollback capability

## Objectives

### 1. Complete System Documentation ✅
Document the running Android system in its current working state:
- All running processes and services
- Loaded kernel modules with parameters
- Hardware component addresses and mappings
- Device tree from running kernel
- Boot process and timing

### 2. Comprehensive Backups 💾
Create multiple backup levels:
- Complete filesystem dump
- Partition table and layout
- Individual critical partitions
- Calibration and configuration files
- Checksums for validation

### 3. Hardware Inventory 🔍
Identify and document all hardware:
- SoC, CPU, GPU, memory specifications
- Display subsystem architecture
- Input devices (IR, motors, sensors)
- Connectivity hardware (WiFi, Bluetooth, HDMI)
- Storage layout and usage

### 4. Driver Analysis 🔬
Extract and analyze all drivers:
- List all loaded kernel modules
- Document module parameters
- Map driver dependencies
- Extract module binaries
- Identify mainline equivalents

## Task List

### Critical Path Tasks

```
Task 001: Root Access Verification ─────┐
                                         ├──► Task 002: System Dump ─────┐
                                         │                                │
                                         ├──► Task 003: Module Docs ─────┤
                                         │                                ├──► Task 008: Phase Summary
                                         ├──► Task 004: Register Map ────┤
                                         │                                │
                                         ├──► Task 005: Calibration ─────┤
                                         │                                │
                                         ├──► Task 006: Boot Analysis ───┤
                                         │                                │
                                         └──► Task 007: Network Config ──┘
```

### Task 001: Root Access Verification ⏱️ 30 min
**Status:** pending  
**Priority:** CRITICAL  
**Dependencies:** None

Verify and document root access methods.

**Steps:**
1. Test ADB root access
2. Check SSH access (if available)
3. Verify available disk space
4. Setup backup storage location
5. Document access procedures

**Success Criteria:**
- [ ] Root access confirmed via ADB
- [ ] Available storage documented (min 16GB needed)
- [ ] Backup location established
- [ ] Access procedure documented

**Deliverables:**
- `hardware-access/root-access-verification.md`
- `hardware-access/storage-assessment.txt`

---

### Task 002: Complete System Dump 📦 2-3 hours
**Status:** pending  
**Priority:** CRITICAL  
**Dependencies:** Task 001

Create comprehensive backup of entire system.

**Steps:**
1. Dump partition table with `fdisk` or `parted`
2. Create complete filesystem tarball
3. Dump boot partition separately
4. Extract important partitions individually
5. Generate SHA256 checksums
6. Verify backup integrity

**Commands:**
```bash
# Partition layout
adb shell su -c "cat /proc/partitions > /sdcard/partition_table.txt"
adb shell su -c "fdisk -l > /sdcard/fdisk_output.txt"

# System dump (WARNING: Large file)
adb shell su -c "tar czf /sdcard/system_dump.tar.gz /system /vendor /data"

# Boot partition
adb shell su -c "dd if=/dev/block/by-name/boot of=/sdcard/boot.img"

# Pull to local
adb pull /sdcard/system_dump.tar.gz ./backup/
adb pull /sdcard/boot.img ./backup/
```

**Success Criteria:**
- [ ] Partition table documented
- [ ] Complete system dump created
- [ ] Boot partition backed up
- [ ] All checksums generated
- [ ] Backup integrity verified

**Deliverables:**
- `backup/partition_table.txt`
- `backup/system_dump_YYYYMMDD.tar.gz`
- `backup/boot_partition.img`
- `backup/checksums.sha256`
- `phases/phase1-hardware-baseline/backup-inventory.md`

---

### Task 003: Kernel Module Documentation 📋 2-3 hours
**Status:** pending  
**Priority:** HIGH  
**Dependencies:** Task 001

Extract and document all loaded kernel modules.

**Steps:**
1. List all loaded modules
2. Get detailed info for each module
3. Document module parameters
4. Map module dependencies
5. Extract module binaries
6. Identify driver functions

**Commands:**
```bash
# List modules
adb shell su -c "lsmod > /sdcard/lsmod_output.txt"

# Module details (for each module)
adb shell su -c "modinfo module_name > /sdcard/modinfo_module_name.txt"

# Module parameters
adb shell su -c "find /sys/module -name parameters -type d" | while read dir; do
    echo "=== $dir ===" >> /sdcard/module_parameters.txt
    adb shell su -c "ls -la $dir" >> /sdcard/module_parameters.txt
done

# Extract module binaries
adb shell su -c "find /system /vendor -name '*.ko' -type f" > module_paths.txt
# Pull each .ko file
```

**Success Criteria:**
- [ ] All modules listed with versions
- [ ] Module info extracted for each
- [ ] Parameters documented
- [ ] Dependencies mapped
- [ ] Module binaries extracted
- [ ] Driver categories identified

**Deliverables:**
- `phases/phase1-hardware-baseline/kernel_modules_inventory.md`
- `phases/phase1-hardware-baseline/module_dependency_graph.txt`
- `backup/kernel_modules/` directory with all `.ko` files
- `phases/phase1-hardware-baseline/driver_analysis.md`

---

### Task 004: Hardware Register Mapping 🔬 4-6 hours
**Status:** pending  
**Priority:** HIGH  
**Dependencies:** Task 001

Document all hardware registers and memory mappings.

**Steps:**
1. Extract device tree from running system
2. Parse /proc/iomem for memory regions
3. Document GPIO configurations
4. Map interrupt assignments
5. Extract clock tree information
6. Document power management

**Commands:**
```bash
# Extract device tree
adb shell su -c "cat /proc/device-tree/model > /sdcard/device_model.txt"
adb shell su -c "dtc -I fs -O dts /proc/device-tree > /sdcard/running_system.dts"

# Memory regions
adb shell su -c "cat /proc/iomem > /sdcard/iomem.txt"

# GPIO configurations
adb shell su -c "cat /sys/kernel/debug/gpio > /sdcard/gpio_state.txt"

# Interrupts
adb shell su -c "cat /proc/interrupts > /sdcard/interrupts.txt"

# Clock tree
adb shell su -c "cat /sys/kernel/debug/clk/clk_summary > /sdcard/clocks.txt"
```

**Success Criteria:**
- [ ] Device tree extracted and parsed
- [ ] Memory regions documented
- [ ] GPIO mappings complete
- [ ] Interrupt assignments documented
- [ ] Clock configurations extracted

**Deliverables:**
- `phases/phase1-hardware-baseline/hardware_register_map.md`
- `backup/device_tree/running_system.dts`
- `phases/phase1-hardware-baseline/gpio_mappings.txt`
- `phases/phase1-hardware-baseline/interrupt_map.txt`
- `phases/phase1-hardware-baseline/clock_tree.txt`

---

### Task 005: Calibration Data Extraction 📊 2-3 hours
**Status:** pending  
**Priority:** HIGH  
**Dependencies:** Task 002

Locate and extract all calibration and configuration data.

**Steps:**
1. Search for calibration files
2. Extract keystone correction data
3. Extract color/display calibration
4. Document thermal parameters
5. Preserve MAC addresses
6. Backup configuration files

**Search Locations:**
```bash
# Common calibration locations
/data/misc/
/data/vendor/
/persist/
/mnt/vendor/persist/
/system/etc/
/vendor/etc/

# Search for specific patterns
adb shell su -c "find /data -name '*calib*' -o -name '*keystone*'"
adb shell su -c "find /persist -type f"
```

**Success Criteria:**
- [ ] Calibration files located
- [ ] Keystone data extracted
- [ ] Display calibration preserved
- [ ] Thermal parameters documented
- [ ] MAC addresses recorded
- [ ] All configs backed up

**Deliverables:**
- `backup/calibration_data/` directory
- `phases/phase1-hardware-baseline/calibration_inventory.md`
- `phases/phase1-hardware-baseline/thermal_parameters.txt`
- `phases/phase1-hardware-baseline/mac_addresses.txt`

---

### Task 006: Boot Process Analysis 🔍 2-3 hours
**Status:** pending  
**Priority:** MEDIUM  
**Dependencies:** Task 001

Document complete boot process and initialization.

**Steps:**
1. Extract kernel boot logs
2. Analyze init system configuration
3. Document service startup sequence
4. Map critical dependencies
5. Extract boot timing information

**Commands:**
```bash
# Kernel logs
adb shell su -c "dmesg > /sdcard/dmesg.txt"

# Init configuration
adb shell su -c "find /system /vendor -name 'init*.rc' -type f"
# Pull all init.rc files

# Running services
adb shell su -c "ps -A > /sdcard/running_processes.txt"

# Boot timing
adb shell su -c "cat /proc/uptime > /sdcard/uptime.txt"
```

**Success Criteria:**
- [ ] Complete dmesg log captured
- [ ] Init configurations extracted
- [ ] Service dependencies mapped
- [ ] Boot timing documented
- [ ] Critical path identified

**Deliverables:**
- `phases/phase1-hardware-baseline/boot_process_documentation.md`
- `backup/dmesg_output.txt`
- `backup/init_rc_files/` directory
- `phases/phase1-hardware-baseline/service_startup_sequence.md`

---

### Task 007: Network Configuration Baseline 🌐 1-2 hours
**Status:** pending  
**Priority:** MEDIUM  
**Dependencies:** Task 001

Document network hardware and configuration.

**Steps:**
1. Identify WiFi chip and driver
2. Extract WiFi firmware files
3. Document Bluetooth configuration
4. Capture network interface config
5. Extract MAC addresses
6. Document HDMI/network features

**Commands:**
```bash
# Network interfaces
adb shell su -c "ip addr > /sdcard/ip_config.txt"
adb shell su -c "ifconfig > /sdcard/ifconfig.txt"

# WiFi information
adb shell su -c "cat /proc/net/wireless > /sdcard/wifi_status.txt"
adb shell su -c "iw list > /sdcard/wifi_capabilities.txt"

# Firmware search
adb shell su -c "find /vendor/firmware -name '*wifi*' -o -name '*bt*'"
```

**Success Criteria:**
- [ ] WiFi chip identified
- [ ] WiFi driver documented
- [ ] Firmware files extracted
- [ ] Bluetooth config documented
- [ ] Network baseline established

**Deliverables:**
- `phases/phase1-hardware-baseline/network_baseline.md`
- `backup/wifi_firmware/` directory
- `phases/phase1-hardware-baseline/bluetooth_config.txt`
- `phases/phase1-hardware-baseline/network_interfaces.txt`

---

### Task 008: Phase I Summary and Validation ✅ 2-3 hours
**Status:** pending  
**Priority:** CRITICAL  
**Dependencies:** Tasks 001-007

Validate all Phase I work and prepare Phase II.

**Steps:**
1. Verify all backup integrity
2. Validate checksums
3. Review documentation completeness
4. Create Phase I completion report
5. Identify Phase II prerequisites
6. Document any issues/blockers

**Validation Checklist:**
- [ ] All backups created successfully
- [ ] Checksums match for all files
- [ ] Documentation complete and organized
- [ ] No missing critical information
- [ ] Phase II prerequisites identified
- [ ] Known issues documented

**Deliverables:**
- `phases/phase1-hardware-baseline/PHASE_I_COMPLETION_REPORT.md`
- `phases/phase1-hardware-baseline/backup_verification_log.txt`
- `phases/phase1-hardware-baseline/phase2_prerequisites.md`
- Updated `rebuild/README.md` with Phase I results

---

## Directory Structure

```
rebuild/phases/phase1-hardware-baseline/
├── README.md                           # This file
├── backup-inventory.md                 # Backup catalog
├── kernel_modules_inventory.md         # Module documentation
├── module_dependency_graph.txt         # Dependency mapping
├── driver_analysis.md                  # Driver categorization
├── hardware_register_map.md            # Register documentation
├── gpio_mappings.txt                   # GPIO assignments
├── interrupt_map.txt                   # Interrupt assignments
├── clock_tree.txt                      # Clock configurations
├── calibration_inventory.md            # Calibration files
├── thermal_parameters.txt              # Thermal settings
├── mac_addresses.txt                   # Network identifiers
├── boot_process_documentation.md       # Boot analysis
├── service_startup_sequence.md         # Init process
├── network_baseline.md                 # Network config
├── bluetooth_config.txt                # BT settings
├── network_interfaces.txt              # Interface config
├── PHASE_I_COMPLETION_REPORT.md        # Final report
├── backup_verification_log.txt         # Validation log
└── phase2_prerequisites.md             # Next phase prep
```

## Hardware Access Methods

### ADB Root Access
```bash
# Connect device
adb devices

# Root shell
adb shell
su

# Or direct commands
adb shell su -c "command"
```

### File Transfer
```bash
# Device to computer
adb pull /path/on/device ./local/path

# Computer to device
adb push ./local/file /path/on/device
```

### Storage Considerations
- System dump: ~4-8 GB
- Module binaries: ~100-500 MB
- Logs and configs: ~50-100 MB
- Total space needed: ~10-15 GB

## Safety Protocols

### Before Any Task
1. Verify backup storage available
2. Check device battery (>50%)
3. Ensure stable ADB connection
4. Document current system state

### During Tasks
1. Monitor device temperature
2. Check for error messages
3. Verify commands before execution
4. Save intermediate results

### After Tasks
1. Verify file integrity (checksums)
2. Document any anomalies
3. Update task status
4. Commit documentation

## Common Issues and Solutions

### ADB Connection Lost
**Solution:** Reconnect USB, restart ADB server
```bash
adb kill-server
adb start-server
adb devices
```

### Insufficient Storage
**Solution:** Use external storage or streaming
```bash
# Use SD card or USB storage
adb shell su -c "df -h"
# Find mounted external storage
```

### Permission Denied
**Solution:** Verify root access
```bash
adb shell su -c "id"
# Should show uid=0(root)
```

### Large File Transfer
**Solution:** Compress or split files
```bash
# Compress on device
adb shell su -c "tar czf /sdcard/backup.tar.gz /path"
adb pull /sdcard/backup.tar.gz
```

## Success Metrics

### Phase I Completion
- [ ] All 8 tasks completed
- [ ] 100% backup verification passed
- [ ] All documentation complete
- [ ] No critical issues unresolved
- [ ] Phase II prerequisites ready

### Quality Standards
- Complete documentation (no TODOs)
- Verified backups (checksums match)
- Organized file structure
- Clear next steps identified

## Timeline

| Task | Duration | Dependencies | Status |
|------|----------|--------------|--------|
| 001 | 30 min | None | pending |
| 002 | 2-3 hrs | 001 | pending |
| 003 | 2-3 hrs | 001 | pending |
| 004 | 4-6 hrs | 001 | pending |
| 005 | 2-3 hrs | 002 | pending |
| 006 | 2-3 hrs | 001 | pending |
| 007 | 1-2 hrs | 001 | pending |
| 008 | 2-3 hrs | 001-007 | pending |

**Estimated Total:** 15-23 hours (2-3 days with validation)

## Next Phase Preview

After Phase I completion, Phase II will focus on:
- UART serial console access
- U-Boot environment inspection
- Boot sequence monitoring via serial
- Bootloader backup and analysis

**Prerequisites for Phase II:**
- UART hardware connection established
- Phase I backups verified
- Complete hardware baseline documented
- Recovery procedures prepared

---

**Last Updated:** November 3, 2025  
**Phase Status:** Active  
**Current Task:** 001 (Ready to start)
