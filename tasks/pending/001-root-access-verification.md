# Task 001: Root Access Verification and Setup

**Task ID:** 001  
**Phase:** I - Hardware Baseline Establishment  
**Status:** pending  
**Priority:** CRITICAL  
**Estimated Duration:** 30 minutes  
**Dependencies:** None

## Objective

Verify root access to the HY300 device and establish secure backup procedures before any system analysis or modification.

## Context

This is the first task of the rebuild project. We need to confirm we have proper root access via ADB, assess available storage for backups, and document reliable access methods. This task blocks all other Phase I work.

## Prerequisites

- HY300 device powered on and accessible
- USB cable connected to development machine
- ADB installed on development machine (`adb` command available)
- Development machine has sufficient storage (~20GB free)

## Steps

### 1. Verify ADB Connection (5 min)

```bash
# Check if device is connected
adb devices

# Expected output:
# List of devices attached
# <device_id>    device
```

**If device not found:**
- Check USB cable connection
- Enable USB debugging on Android device
- Try different USB port
- Restart ADB server: `adb kill-server && adb start-server`

### 2. Verify Root Access (5 min)

```bash
# Enter root shell
adb shell
su

# Verify root
id

# Expected output:
# uid=0(root) gid=0(root) groups=0(root),...
```

**Alternative method (direct command):**
```bash
adb shell su -c "id"
```

### 3. Assess Device Storage (5 min)

```bash
# Check all mounted filesystems
adb shell su -c "df -h" > device_storage_info.txt

# Check for external storage (SD card, USB)
adb shell su -c "mount" | grep -E "(sdcard|usb|storage)"

# Find largest available space
adb shell su -c "df -h | sort -rh -k4"
```

**Document:**
- Total system storage
- Available free space
- External storage options
- Best location for temporary dumps

### 4. Assess Development Machine Storage (5 min)

```bash
# Check available space on development machine
df -h $(pwd)

# Create backup directory structure
mkdir -p backup/{filesystem,partitions,configs,modules,logs}

# Verify write permissions
touch backup/test.txt && rm backup/test.txt
```

**Minimum requirements:**
- 20GB free space for complete system dump
- Fast storage preferred (SSD)

### 5. Test File Transfer (5 min)

```bash
# Test push to device
echo "test" > test_push.txt
adb push test_push.txt /sdcard/
adb shell su -c "cat /sdcard/test_push.txt"

# Test pull from device
adb shell su -c "echo 'test_pull' > /sdcard/test_pull.txt"
adb pull /sdcard/test_pull.txt
cat test_pull.txt

# Cleanup
rm test_push.txt test_pull.txt
adb shell su -c "rm /sdcard/test_*.txt"
```

### 6. Document Access Methods (5 min)

Create `hardware-access/root-access-verification.md` with:
- ADB device ID
- Root access method
- Device storage assessment
- Backup storage plan
- Any access issues encountered

### 7. Check Critical System Info (5 min)

```bash
# Device model
adb shell su -c "cat /proc/device-tree/model" > device_info/model.txt

# Kernel version
adb shell su -c "uname -a" > device_info/kernel.txt

# Android version
adb shell su -c "getprop ro.build.version.release" > device_info/android_version.txt

# SoC information
adb shell su -c "cat /proc/cpuinfo" > device_info/cpuinfo.txt
```

## Success Criteria

- [ ] ADB connection established and stable
- [ ] Root access confirmed (uid=0)
- [ ] Device storage documented (>2GB free minimum)
- [ ] Development machine has >20GB free space
- [ ] File transfer (push/pull) working
- [ ] Access methods documented in `hardware-access/root-access-verification.md`
- [ ] Basic device info collected
- [ ] Backup directory structure created

## Deliverables

### Files Created

```
rebuild/hardware-access/root-access-verification.md
backup/device_storage_info.txt
backup/device_info/model.txt
backup/device_info/kernel.txt
backup/device_info/android_version.txt
backup/device_info/cpuinfo.txt
```

### Documentation Template

**hardware-access/root-access-verification.md:**
```markdown
# Root Access Verification Report

**Date:** [Date]
**Device:** HY300 Android Projector
**Operator:** [Name]

## ADB Connection
- Device ID: [device_id]
- Connection: USB
- Status: ✅ Connected

## Root Access
- Method: adb shell → su
- Status: ✅ Verified (uid=0)
- Issues: [None / Describe any]

## Storage Assessment

### Device Storage
- Total: [size]
- Used: [size]
- Available: [size]
- Temp location: /sdcard/ [or other]

### Development Machine
- Backup location: /home/luca/Desktop/sun50iw12p1-research/backup/
- Available space: [size]
- Filesystem type: [ext4/etc]

## Device Information
- Model: [from model.txt]
- Kernel: [from kernel.txt]
- Android Version: [version]
- SoC: Allwinner H713

## File Transfer Test
- Push: ✅ Working
- Pull: ✅ Working
- Speed: [approximate]

## Issues Encountered
[None / List any issues and resolutions]

## Next Steps
Ready to proceed with Task 002: Complete System Dump
```

## Validation Procedure

```bash
# Run this validation script
cat > validate_task001.sh << 'EOF'
#!/bin/bash
set -e

echo "Validating Task 001 Completion..."

# Check ADB connection
echo -n "ADB connection: "
adb devices | grep -q "device$" && echo "✅" || echo "❌"

# Check root access
echo -n "Root access: "
adb shell su -c "id" | grep -q "uid=0" && echo "✅" || echo "❌"

# Check backup directory
echo -n "Backup directory: "
[ -d backup ] && echo "✅" || echo "❌"

# Check documentation
echo -n "Documentation: "
[ -f rebuild/hardware-access/root-access-verification.md ] && echo "✅" || echo "❌"

# Check device info
echo -n "Device info collected: "
[ -f backup/device_info/model.txt ] && echo "✅" || echo "❌"

echo ""
echo "Validation complete!"
EOF

chmod +x validate_task001.sh
./validate_task001.sh
```

## Common Issues

### Issue: Device Not Found
**Symptoms:** `adb devices` shows empty or "unauthorized"
**Solution:**
```bash
# Restart ADB server
adb kill-server
adb start-server

# Check USB connection
lsusb | grep -i allwinner

# Try different USB port
# Check device USB debugging settings
```

### Issue: Root Access Denied
**Symptoms:** `su` command fails or permission denied
**Solution:**
- Verify device is rooted
- Check SuperSU/Magisk app settings
- Try `su 0` instead of just `su`
- Reboot device and try again

### Issue: Insufficient Storage
**Symptoms:** Not enough space for backups
**Solution:**
- Use external SD card or USB storage
- Compress data on device before pulling
- Stream directly without device storage:
  ```bash
  adb shell su -c "tar cz /system" | cat > system.tar.gz
  ```

## Safety Notes

- ⚠️ **DO NOT** modify anything in this task
- ⚠️ **READ-ONLY** operations only
- ✅ **Safe to run** multiple times
- ✅ **No permanent changes** made to device

## Time Tracking

- Estimated: 30 minutes
- Actual: _____ minutes
- Issues encountered: _____

## Task Completion

When all success criteria are met:

```bash
# Mark task as complete
ai/tools/task-manager complete 001

# Move to next task
ai/tools/task-manager next
```

---

**Created:** November 3, 2025  
**Last Updated:** November 3, 2025  
**Status:** pending → Ready to start
