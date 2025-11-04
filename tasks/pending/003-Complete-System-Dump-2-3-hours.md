# Task 003: Complete System Dump 📦 2-3 hours

**Status:** pending
**Priority:** CRITICAL
**Phase:** Phase I - Hardware Baseline Establishment
**Created:** 2025-11-04
**Dependencies:** Task 001

## Objective

Create comprehensive backup of entire HY300 running system. This task dumps all critical system data, filesystem contents, partitions, and boot configuration to enable complete recovery and forensic analysis.

## Context

With root access verified (Task 001) and hardware baseline documented (Task 002), we now capture a complete snapshot of the running Android system before any modifications. This serves as:
- Safety net for recovery
- Reference for driver extraction
- Validation of FEX parameters against running system
- Evidence base for Phase II bootloader planning

## Steps

### 1. Dump Partition Table (5 min)
```bash
# Get partition layout from kernel
adb shell su -c "cat /proc/partitions > /sdcard/partition_table.txt"
adb shell su -c "fdisk -l > /sdcard/fdisk_output.txt"

# Pull to development machine
adb pull /sdcard/partition_table.txt backup/
adb pull /sdcard/fdisk_output.txt backup/
```

### 2. Create Filesystem Dump (45-60 min)
```bash
# Create system dump (may be 4-8 GB)
adb shell su -c "tar czf /sdcard/system_dump.tar.gz /system /vendor /data"

# Pull to local machine
adb pull /sdcard/system_dump.tar.gz ./backup/
```

### 3. Extract Boot Partition (10 min)
```bash
# Dump boot partition raw image
adb shell su -c "dd if=/dev/block/by-name/boot of=/sdcard/boot.img"

# Pull to local
adb pull /sdcard/boot.img backup/
```

### 4. Extract Device Tree (5 min)
```bash
# Get running device tree
adb shell su -c "dtc -I fs -O dts /proc/device-tree > /sdcard/running_device_tree.dts"
adb pull /sdcard/running_device_tree.dts backup/
```

### 5. Generate Checksums (5 min)
```bash
# On development machine
cd backup/
sha256sum system_dump.tar.gz boot.img > checksums.sha256
sha256sum -c checksums.sha256
```

### 6. Verify Integrity (5 min)
```bash
# Check sizes and md5
ls -lah backup/*.tar.gz backup/*.img
tar -tzf backup/system_dump.tar.gz | head -20
```

## Success Criteria

- [ ] Partition table documented (partition_table.txt, fdisk_output.txt)
- [ ] Complete system dump created (system_dump.tar.gz ~4-8 GB)
- [ ] Boot partition backed up (boot.img ~64 MB)
- [ ] Device tree extracted (running_device_tree.dts)
- [ ] All checksums generated (checksums.sha256)
- [ ] Backup integrity verified (tar extraction test, checksum validation)

## Deliverables

- `backup/partition_table.txt` - Kernel partition info
- `backup/fdisk_output.txt` - Full fdisk partition table
- `backup/system_dump_20251104.tar.gz` - Complete filesystem (4-8 GB)
- `backup/boot.img` - Boot partition binary (64 MB)
- `backup/running_device_tree.dts` - Device tree from running system
- `backup/checksums.sha256` - SHA256 checksums for verification
- `phases/phase1-hardware-baseline/backup-inventory.md` - Inventory and verification report

## Notes

**Storage Planning:**
- System dump: 4-8 GB (compressed with gzip)
- Development machine available: 18 GB
- Will use ~50% of available space

**Timeline:**
- Partition dump: 5 min
- System dump: 45-60 min (slow on eMMC)
- Boot extraction: 10 min
- Checksum and verification: 10 min
- **Total: ~90 minutes**

**Safety Notes:**
- Read-only operations on device
- No modifications to running system
- Safe to run multiple times
- Can be interrupted and resumed
