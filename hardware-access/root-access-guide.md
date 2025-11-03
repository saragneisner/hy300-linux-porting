# Root Access Guide - HY300 Rebuild Project

**Last Updated:** November 3, 2025  
**Project Phase:** I - Hardware Baseline

## Overview

This guide documents how to access the HY300 Android projector with root privileges for system analysis, backup, and development. All procedures are **read-only** and **non-destructive** unless explicitly noted.

## Access Methods

### Method 1: ADB Root Shell (Primary)

**Prerequisites:**
- HY300 connected via USB
- ADB installed on development machine
- USB debugging enabled on device
- Device already rooted (SuperSU/Magisk)

**Connection Steps:**
```bash
# 1. Check device connection
adb devices

# 2. Enter shell
adb shell

# 3. Become root
su

# 4. Verify root access
id
# Should show: uid=0(root) gid=0(root)
```

**Direct Command Execution:**
```bash
# Execute commands without interactive shell
adb shell su -c "command here"

# Examples:
adb shell su -c "ls -la /system"
adb shell su -c "cat /proc/cpuinfo"
adb shell su -c "dmesg"
```

### Method 2: SSH Access (If Available)

**Check if SSH is running:**
```bash
adb shell su -c "netstat -tlnp | grep :22"
```

**If SSH server is available:**
```bash
# Find device IP
adb shell su -c "ip addr show wlan0"

# Connect via SSH
ssh root@<device_ip>
```

## Safety Protocols

### Read-Only Operations (Safe)

These commands are safe to run anytime:

```bash
# System information
adb shell su -c "cat /proc/cpuinfo"
adb shell su -c "cat /proc/meminfo"
adb shell su -c "uname -a"
adb shell su -c "getprop"

# Hardware information
adb shell su -c "cat /proc/device-tree/model"
adb shell su -c "cat /proc/iomem"
adb shell su -c "cat /proc/interrupts"

# Storage information
adb shell su -c "df -h"
adb shell su -c "cat /proc/partitions"
adb shell su -c "mount"

# Module information
adb shell su -c "lsmod"
adb shell su -c "cat /proc/modules"

# Process information
adb shell su -c "ps -A"
adb shell su -c "top -n 1"

# Log reading
adb shell su -c "dmesg"
adb shell su -c "logcat -d"
```

### Potentially Disruptive Operations (Caution)

⚠️ **Use with care - may affect running system:**

```bash
# Loading/unloading modules
adb shell su -c "modprobe module_name"
adb shell su -c "rmmod module_name"

# Changing permissions
adb shell su -c "chmod ..."

# Creating/modifying files
adb shell su -c "echo ... > /path/file"
```

### Dangerous Operations (NEVER Without Backup)

🛑 **NEVER run these without complete backups:**

```bash
# Modifying system partitions
adb shell su -c "mount -o remount,rw /system"

# Writing to block devices
adb shell su -c "dd if=... of=/dev/block/..."

# Flashing partitions
adb shell su -c "flash_image ..."

# Factory reset
adb shell su -c "recovery --wipe_data"
```

## File Operations

### Reading Files

```bash
# View file contents
adb shell su -c "cat /path/to/file"

# View with pagination
adb shell su -c "cat /path/to/file" | less

# Search file contents
adb shell su -c "grep pattern /path/to/file"
```

### Extracting Files

```bash
# Single file
adb pull /path/on/device ./local/path

# For root-only files
adb shell su -c "cp /root/only/file /sdcard/"
adb pull /sdcard/file ./local/path
adb shell su -c "rm /sdcard/file"

# Multiple files
adb shell su -c "tar czf /sdcard/archive.tar.gz /path/to/dir"
adb pull /sdcard/archive.tar.gz
```

### Creating Temporary Files

```bash
# Safe temporary location
adb shell su -c "mkdir -p /sdcard/backup_temp"

# Copy files for extraction
adb shell su -c "cp /system/file /sdcard/backup_temp/"

# Cleanup after use
adb shell su -c "rm -rf /sdcard/backup_temp"
```

## System Analysis Commands

### Hardware Information

```bash
# SoC and CPU
adb shell su -c "cat /proc/cpuinfo" > hardware_info/cpuinfo.txt
adb shell su -c "cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_cur_freq"

# Memory
adb shell su -c "cat /proc/meminfo" > hardware_info/meminfo.txt
adb shell su -c "free -h"

# Storage
adb shell su -c "df -h" > hardware_info/storage.txt
adb shell su -c "cat /proc/partitions" > hardware_info/partitions.txt

# Device tree
adb shell su -c "ls -lR /proc/device-tree" > hardware_info/device_tree_structure.txt
```

### Kernel and Modules

```bash
# Kernel version and config
adb shell su -c "uname -a" > kernel_info/version.txt
adb shell su -c "cat /proc/version" > kernel_info/full_version.txt
adb shell su -c "zcat /proc/config.gz" > kernel_info/config.txt 2>/dev/null || echo "Config not available"

# Loaded modules
adb shell su -c "lsmod" > kernel_info/loaded_modules.txt
adb shell su -c "cat /proc/modules" > kernel_info/modules_detailed.txt

# Module details (for each module)
for module in $(adb shell su -c "lsmod" | tail -n +2 | awk '{print $1}'); do
    adb shell su -c "modinfo $module" > "kernel_info/modinfo_${module}.txt" 2>/dev/null
done
```

### Network Information

```bash
# Interfaces
adb shell su -c "ip addr" > network_info/interfaces.txt
adb shell su -c "ifconfig" > network_info/ifconfig.txt

# Routing
adb shell su -c "ip route" > network_info/routing.txt

# WiFi status
adb shell su -c "cat /proc/net/wireless" > network_info/wifi_status.txt

# Network stats
adb shell su -c "cat /proc/net/dev" > network_info/net_stats.txt
```

### Process and Service Information

```bash
# Running processes
adb shell su -c "ps -A" > process_info/all_processes.txt
adb shell su -c "ps -ef" > process_info/processes_full.txt

# Service status
adb shell su -c "service list" > process_info/services.txt

# Resource usage
adb shell su -c "top -n 1 -b" > process_info/top_snapshot.txt
```

## Backup Procedures

### Quick Backup Script

```bash
#!/bin/bash
# Quick hardware snapshot

DATE=$(date +%Y%m%d_%H%M%S)
BACKUP_DIR="backup/snapshot_${DATE}"

mkdir -p "$BACKUP_DIR"

echo "Creating hardware snapshot..."

# System info
adb shell su -c "uname -a" > "$BACKUP_DIR/uname.txt"
adb shell su -c "cat /proc/cpuinfo" > "$BACKUP_DIR/cpuinfo.txt"
adb shell su -c "cat /proc/meminfo" > "$BACKUP_DIR/meminfo.txt"
adb shell su -c "df -h" > "$BACKUP_DIR/storage.txt"

# Kernel modules
adb shell su -c "lsmod" > "$BACKUP_DIR/lsmod.txt"

# Device tree model
adb shell su -c "cat /proc/device-tree/model" > "$BACKUP_DIR/model.txt"

# Logs
adb shell su -c "dmesg" > "$BACKUP_DIR/dmesg.txt"

echo "Snapshot created in $BACKUP_DIR"
```

### Full System Dump (Covered in Task 002)

See Task 002 for complete backup procedures.

## Troubleshooting

### Issue: "su: not found"

**Cause:** Device not rooted or su binary not in PATH

**Solution:**
```bash
# Find su binary
adb shell find / -name su -type f 2>/dev/null

# Common locations:
# /system/xbin/su
# /system/bin/su
# /sbin/su

# Use full path
adb shell /system/xbin/su -c "id"
```

### Issue: "Permission denied" with su

**Cause:** SuperSU/Magisk blocking ADB root access

**Solution:**
1. Open SuperSU/Magisk app on device
2. Settings → Enable "ADB root access"
3. Grant permission when prompted
4. Try again

### Issue: ADB connection unstable

**Cause:** USB power management or cable issues

**Solution:**
```bash
# Disable USB power management
echo 'on' | sudo tee /sys/bus/usb/devices/*/power/control

# Use better USB cable
# Connect to USB 2.0 port (more stable than 3.0)
# Use powered USB hub
```

### Issue: Slow file transfers

**Cause:** USB mode or large file size

**Solution:**
```bash
# Compress before transfer
adb shell su -c "tar czf /sdcard/data.tar.gz /large/directory"
adb pull /sdcard/data.tar.gz

# Stream without device storage
adb shell su -c "tar czf - /large/directory" > data.tar.gz
```

## Security Considerations

### Device Access Control

- ⚠️ Root access bypasses Android security
- 🔒 Keep development machine secure
- 📝 Document all access times
- 🔐 Use encrypted storage for backups

### Data Privacy

- 🔒 Backups may contain sensitive data
- 📝 Document what data is extracted
- 🗑️ Securely delete backups when done
- 🚫 Don't share backups publicly

### Network Security

- 🚫 Don't enable ADB over network unless necessary
- 🔐 Use strong passwords if SSH enabled
- 🔥 Firewall development machine
- 📝 Log all remote access attempts

## Best Practices

### Before Each Session

1. ✅ Verify device connection stable
2. ✅ Check available storage space
3. ✅ Document session purpose
4. ✅ Review safety protocols

### During Operations

1. 📝 Log all commands executed
2. ⚠️ Verify read-only intent
3. 💾 Save important output immediately
4. 🔍 Monitor for errors

### After Operations

1. ✅ Verify all data extracted
2. 🗑️ Clean up temporary files on device
3. 📝 Document findings
4. 💾 Backup important discoveries

## Quick Reference

### Most Used Commands

```bash
# Device info
adb shell su -c "cat /proc/device-tree/model"

# Storage check
adb shell su -c "df -h"

# Module list
adb shell su -c "lsmod"

# Kernel log
adb shell su -c "dmesg"

# Process list
adb shell su -c "ps -A"

# Extract file
adb shell su -c "cp /root/file /sdcard/ && exit" && adb pull /sdcard/file
```

### Emergency Commands

```bash
# Reboot device
adb reboot

# Reboot to recovery
adb reboot recovery

# Kill ADB server
adb kill-server

# Restart ADB with root
adb root  # (if supported)
```

## Related Documentation

- Task 001: Root Access Verification
- Task 002: Complete System Dump
- `rebuild/phases/phase1-hardware-baseline/README.md`
- `rebuild/agents/AGENT_GUIDELINES.md`

---

**Created:** November 3, 2025  
**Maintained By:** HY300 Rebuild Project  
**Version:** 1.0
