# HY300 eMMC Dump Analysis - Complete System Baseline

**Date:** November 4, 2025  
**Source Device:** HY300 Android Projector (Allwinner H713)  
**Image File:** `full_emmc_dump_clean_root.img` (7.3 GB)

## Directory Structure

```
dumps/
├── full_emmc_dump_clean_root.img     # Complete eMMC raw dump (7.3 GB)
├── EXTRACTION_SUMMARY.md             # Detailed extraction report
├── README.md                          # This file
│
├── partitions/                        # Extracted partition images
│   ├── bootloader_a.bin               # U-Boot SPL+loader (32 MB)
│   ├── boot_a.bin                     # Kernel + ramdisk (64 MB, Android bootimg)
│   ├── vendor_boot_a.bin              # LZ4 compressed vendor modules (32 MB)
│   ├── dtbo_a.bin                     # Device tree overlay (2 MB)
│   ├── super.bin                      # Logical partition container (2.0 GB)
│   │
│   ├── super_unpacked/                # Unpacked logical partitions
│   │   ├── system_a.img               # System filesystem (937 MB, ext4)
│   │   ├── vendor_a.img               # Vendor filesystem (110 MB, ext4)
│   │   ├── product_a.img              # Product partition (513 MB, ext4)
│   │   └── system_b.img, vendor_b.img # Redundant copies (empty)
│   │
│   ├── system_mount/                  # Read-only mount of system_a.img
│   │   └── (Android system framework, apps, libraries)
│   │
│   └── vendor_mount/                  # Read-only mount of vendor_a.img
│       └── (Hardware drivers and configs)
```

## Partition Analysis

### eMMC Layout (26 GPT Partitions)

| # | Partition | Start | Size | Type | Status |
|---|-----------|-------|------|------|--------|
| 1-2 | bootloader_a/b | 73728 | 32 MB | Bootloader | ✅ Extracted |
| 3-4 | env_a/b | 204800 | 256 KB | Environment | ℹ️ Config |
| 5-6 | boot_a/b | 205824 | 64 MB | Kernel | ✅ Extracted |
| 7-8 | vendor_boot_a/b | 467968 | 32 MB | Vendor Kernel | ✅ Extracted |
| 9 | super | 599040 | 2.0 GB | Logical | ✅ Unpacked |
| 10 | misc | 4793344 | 16 MB | Miscellaneous | ℹ️ Control |
| 11-16 | vbmeta_* | 4826112 | 896 KB | Verification | ℹ️ Security |
| 17 | frp | 4827136 | 512 KB | Factory Reset | ℹ️ Security |
| 18 | empty | 4858880 | 15 MB | Empty | ℹ️ - |
| 19 | metadata | 4891648 | 16 MB | Metadata | ℹ️ A/B |
| 20 | private | 4924416 | 2 MB | Private | ℹ️ Data |
| 21-22 | dtbo_a/b | 4924416 | 2 MB | Device Tree | ✅ Located |
| 23 | media_data | 4932608 | 272 MB | Media | ℹ️ User |
| 24-25 | Reserve0_a/b | 5489664 | 16 MB | Reserve | ℹ️ Future |
| 26 | UDISK | 5555200 | 4.6 GB | Userdata | ℹ️ User |

## Key Findings

### 1. Boot Architecture
- **U-Boot:** 32 MB bootloader (likely contains SPL + u-boot.bin)
- **Kernel:** Standard Android bootimg format (zImage + DTB + ramdisk)
- **A/B Updates:** Full redundancy for safe OTA updates
- **Verification:** dm-verity enabled (vbmeta partitions)

### 2. Filesystem Structure
**System (937 MB):**
- Android 11/12 framework
- System apps and services
- APEX modular system components
- Read-only system

**Vendor (110 MB):**
- Hardware-specific drivers
- Allwinner proprietary modules
- Device configuration
- HALs (Hardware Abstraction Layers)

**Product (513 MB):**
- Projector-specific applications
- Custom services and config
- OEM customization

### 3. Device Tree Components
- **Boot DTB:** Embedded in boot_a.bin (kernel device tree)
- **Vendor DTB:** Flattened Device Tree 69.3 KB in vendor_boot_a.bin
- **DTBO:** Device tree overlay (2 MB, allows device hotpatching)

### 4. Security Configuration
- **dm-verity:** Block device verification enabled
- **Secure Boot:** Likely enabled (vbmeta verification)
- **SELinux:** Mandatory Access Control present

## Extraction Methods

### From full_emmc_dump_clean_root.img:

1. **Direct dd extraction** (raw partitions)
   ```bash
   dd if=full_emmc_dump_clean_root.img of=bootloader_a.bin \
      bs=512 skip=73728 count=65536
   ```

2. **lpunpack for logical partitions** (super.bin → system/vendor)
   ```bash
   lpunpack super.bin super_unpacked/
   ```

3. **mount for filesystem access** (ext4 images)
   ```bash
   sudo mount -o ro super_unpacked/system_a.img system_mount
   ```

4. **binwalk for embedded files** (kernel images, DTBs)
   ```bash
   binwalk full_emmc_dump_clean_root.img
   ```

## Storage Status

| Component | Size | Location | Status |
|-----------|------|----------|--------|
| Raw eMMC dump | 7.3 GB | full_emmc_dump_clean_root.img | ✅ |
| Boot components | 130 MB | partitions/*.bin | ✅ |
| Unpacked images | 1.6 GB | partitions/super_unpacked/ | ✅ |
| Mounted filesystems | ~1 GB | partitions/*_mount/ | ✅ |
| **Total allocated** | **~12 GB** | dumps/ | ℹ️ |
| **Free on device** | ~6 GB | Available | ⚠️ |

## Phase I Validation Checklist

- [x] Boot partition extracted and analyzed
- [x] Device tree located and documented
- [x] System/vendor filesystems mounted and accessible
- [x] Partition table completely documented
- [x] Hardware configuration confirmed
- [x] Security features identified
- [x] Backup integrity verified

## Next Steps for Phase II (UART & Bootloader)

1. **Extract bootloader** from bootloader_a.bin
   - Locate U-Boot SPL entry point
   - Extract bootloader source/config

2. **Extract kernel** from boot_a.bin
   - Standard Android bootimg parsing
   - zImage decompression
   - Device tree compilation (dtc)

3. **Kernel module analysis** from vendor_boot_a.bin
   - LZ4 decompression
   - Module dependency analysis
   - H713-specific driver identification

4. **Device tree analysis**
   - Convert DTB to DTS (human-readable)
   - Document all device nodes
   - Cross-reference with hardware capabilities

## Critical Files for Next Phase

| File | Use | Size |
|------|-----|------|
| bootloader_a.bin | U-Boot analysis | 32 MB |
| boot_a.bin | Kernel extraction | 64 MB |
| vendor_boot_a.bin | Module extraction | 32 MB |
| system_mount/* | Driver references | 937 MB |
| vendor_mount/* | Hardware config | 110 MB |

---

**Last Updated:** 2025-11-04  
**Task:** 003 - Complete System Dump  
**Status:** ✅ COMPLETED
