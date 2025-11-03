# Live System Analysis Procedures

**Purpose:** Safe data extraction procedures for hardware-first analysis  
**Device:** HY300 Android Projector with root access  
**Safety Level:** 🟢 Read-only operations  
**Created:** November 3, 2025

---

## Prerequisites

### Required Access
- ✅ Root access via ADB
- ✅ Complete device backup
- ✅ USB debugging enabled
- ✅ Development environment ready

### Safety Verification

```bash
# Verify ADB connection
adb devices
# Should show: [serial]  device

# Verify root access
adb shell su -c "id"
# Should show: uid=0(root) gid=0(root)

# Verify backup exists
ls -lh backup/device-a/full-dump.img
# Should show backup file
```

---

## Safe Data Extraction Commands

### System Information

#### CPU and Hardware

```bash
# CPU information
adb shell su -c "cat /proc/cpuinfo" > logs/live-cpuinfo.txt

# Memory information
adb shell su -c "cat /proc/meminfo" > logs/live-meminfo.txt

# Hardware platform
adb shell su -c "cat /proc/device-tree/model" > logs/live-model.txt
adb shell su -c "cat /proc/device-tree/compatible" > logs/live-compatible.txt

# SoC information
adb shell su -c "getprop | grep ro.hardware" > logs/live-hardware-props.txt
```

#### Kernel and Modules

```bash
# Kernel version
adb shell su -c "uname -a" > logs/live-kernel-version.txt

# Loaded kernel modules
adb shell su -c "lsmod" > logs/live-modules.txt

# Module details
adb shell su -c "cat /proc/modules" > logs/live-modules-detailed.txt

# Kernel command line
adb shell su -c "cat /proc/cmdline" > logs/live-cmdline.txt

# Kernel messages
adb shell su -c "dmesg" > logs/live-dmesg.txt
```

#### Device Tree Extraction

```bash
# Complete device tree dump
adb shell su -c "find /proc/device-tree -type f -exec sh -c 'echo {}; cat {}; echo ---' \;" > logs/live-devicetree-complete.txt

# Device tree structure
adb shell su -c "find /proc/device-tree -print" > logs/live-devicetree-structure.txt

# Specific subsystems
adb shell su -c "find /proc/device-tree/soc -type f -name compatible -exec sh -c 'echo {}; cat {}; echo' \;" > logs/live-soc-devices.txt
```

### Hardware Resources

#### Memory and I/O

```bash
# Physical memory map
adb shell su -c "cat /proc/iomem" > logs/live-iomem.txt

# I/O ports
adb shell su -c "cat /proc/ioports" > logs/live-ioports.txt

# DMA information
adb shell su -c "cat /proc/dma" > logs/live-dma.txt

# Interrupts
adb shell su -c "cat /proc/interrupts" > logs/live-interrupts.txt
```

#### Storage and Partitions

```bash
# Block devices
adb shell su -c "cat /proc/partitions" > logs/live-partitions.txt

# Mount points
adb shell su -c "mount" > logs/live-mounts.txt

# Filesystem information
adb shell su -c "df -h" > logs/live-df.txt

# eMMC information
adb shell su -c "cat /sys/class/mmc_host/mmc0/mmc0:0001/name" > logs/live-emmc-name.txt
adb shell su -c "cat /sys/class/mmc_host/mmc0/mmc0:0001/cid" > logs/live-emmc-cid.txt
```

### Drivers and Devices

#### Character and Block Devices

```bash
# Character devices
adb shell su -c "ls -l /dev/char" > logs/live-char-devices.txt

# Block devices
adb shell su -c "ls -l /dev/block" > logs/live-block-devices.txt

# Input devices
adb shell su -c "ls -l /dev/input" > logs/live-input-devices.txt

# Video devices
adb shell su -c "ls -l /dev/video*" > logs/live-video-devices.txt
```

#### Sysfs Device Hierarchy

```bash
# Complete sysfs dump (large)
adb shell su -c "find /sys/devices -type f -name name -exec sh -c 'echo {}; cat {} 2>/dev/null; echo' \;" > logs/live-sysfs-devices.txt

# Driver list
adb shell su -c "ls -l /sys/bus/platform/drivers" > logs/live-platform-drivers.txt

# Device tree devices
adb shell su -c "ls -l /sys/firmware/devicetree" > logs/live-dt-devices.txt
```

### Network and Connectivity

#### Network Interfaces

```bash
# Network interfaces
adb shell su -c "ip addr show" > logs/live-ip-addr.txt
adb shell su -c "ip link show" > logs/live-ip-link.txt

# Network routes
adb shell su -c "ip route show" > logs/live-routes.txt

# Network statistics
adb shell su -c "cat /proc/net/dev" > logs/live-net-dev.txt

# WiFi information (if available)
adb shell su -c "iw dev" > logs/live-wifi-dev.txt
adb shell su -c "iw list" > logs/live-wifi-capabilities.txt
```

#### Bluetooth

```bash
# Bluetooth devices
adb shell su -c "ls -l /sys/class/bluetooth" > logs/live-bluetooth-devices.txt

# Bluetooth adapter info
adb shell su -c "hciconfig -a" > logs/live-hci-config.txt
```

### Hardware-Specific Components

#### Display and Graphics

```bash
# Framebuffer devices
adb shell su -c "ls -l /dev/fb*" > logs/live-framebuffer.txt
adb shell su -c "cat /sys/class/graphics/fb0/name" > logs/live-fb-name.txt

# Display information
adb shell su -c "dumpsys display" > logs/live-display-info.txt

# GPU information
adb shell su -c "cat /sys/kernel/debug/mali/version" > logs/live-mali-version.txt 2>/dev/null || echo "Mali debug not available"
```

#### Input Devices

```bash
# Input device details
adb shell su -c "getevent -p" > logs/live-input-events.txt

# IR remote (if available)
adb shell su -c "ls -l /dev/input/by-path/*ir*" > logs/live-ir-devices.txt
```

#### Sensors

```bash
# Sensor list
adb shell su -c "dumpsys sensorservice" > logs/live-sensors.txt

# Accelerometer (keystone)
adb shell su -c "ls -l /sys/class/input/input*/name" > logs/live-input-names.txt
adb shell su -c "cat /sys/class/input/input*/name" > logs/live-input-details.txt
```

### Android System

#### Properties

```bash
# All system properties
adb shell su -c "getprop" > logs/live-getprop-all.txt

# Hardware properties
adb shell su -c "getprop | grep hardware" > logs/live-hardware-props.txt
adb shell su -c "getprop | grep ro.board" > logs/live-board-props.txt

# Build information
adb shell su -c "getprop | grep ro.build" > logs/live-build-props.txt
```

#### Services

```bash
# Running services
adb shell su -c "dumpsys -l" > logs/live-services-list.txt

# Service details (select important ones)
adb shell su -c "dumpsys activity services" > logs/live-activity-services.txt
adb shell su -c "dumpsys media.player" > logs/live-media-services.txt
```

#### Processes

```bash
# Process list
adb shell su -c "ps -A" > logs/live-processes.txt

# Process tree
adb shell su -c "ps -A -T" > logs/live-process-tree.txt
```

---

## Comparison with Research Findings

### Step-by-Step Comparison Procedure

#### 1. Identify Research Baseline

```bash
# Find relevant research document
cd research/docs
grep -l "TOPIC" *.md

# Example: For device tree comparison
research_doc="research/docs/FACTORY_DTB_ANALYSIS.md"
```

#### 2. Extract Comparable Data

```bash
# Extract specific sections from research
grep "memory@" $research_doc > research-memory.txt
grep "uart@" $research_doc > research-uart.txt

# Extract corresponding live data
adb shell su -c "find /proc/device-tree -name memory* -type d" > live-memory-nodes.txt
adb shell su -c "find /proc/device-tree -name serial* -type d" > live-uart-nodes.txt
```

#### 3. Create Comparison Report

```bash
cat > comparison-report.md << 'EOF'
# Hardware vs Research Comparison: [Topic]

**Date:** $(date +%Y-%m-%d)
**Research Doc:** research/docs/DOCUMENT.md
**Live Data Source:** [command/file]

## Comparison Results

### Category: [Component]

| Aspect | Research | Hardware | Match? |
|--------|----------|----------|--------|
| [Item 1] | [value] | [value] | ✅/⚠️/❌ |
| [Item 2] | [value] | [value] | ✅/⚠️/❌ |

### Analysis

**Matches (✅):**
- [Finding 1]
- [Finding 2]

**Differences (⚠️):**
- [Difference 1]: Reason/explanation
- [Difference 2]: Reason/explanation

**Contradictions (❌):**
- [Contradiction 1]: Hardware truth vs research claim
  - Investigation: [why research was incorrect]

## Conclusions

[Overall assessment of research accuracy]

## Action Items

- [ ] Update research docs with hardware findings
- [ ] Document hardware-specific behavior
- [ ] Adjust configuration based on hardware truth
EOF
```

#### 4. Automated Comparison Script

```bash
#!/bin/bash
# compare-research-hardware.sh

topic="$1"
research_file="$2"
live_file="$3"

echo "# Automated Comparison: $topic"
echo "**Date:** $(date)"
echo ""
echo "## Files Compared"
echo "- Research: $research_file"
echo "- Hardware: $live_file"
echo ""

# Extract key-value pairs and compare
echo "## Differences Found"
diff <(sort $research_file) <(sort $live_file) || echo "Files are identical"

echo ""
echo "## Match Percentage"
total_lines=$(wc -l < $research_file)
matching_lines=$(comm -12 <(sort $research_file) <(sort $live_file) | wc -l)
percentage=$((matching_lines * 100 / total_lines))
echo "Match: $matching_lines / $total_lines lines ($percentage%)"
```

Usage:
```bash
./compare-research-hardware.sh "CPU Info" research-cpu.txt logs/live-cpuinfo.txt
```

---

## Documentation Standards

### File Naming Convention

```
logs/live-[category]-[description].txt

Examples:
logs/live-cpuinfo.txt
logs/live-devicetree-complete.txt
logs/live-modules.txt
logs/live-dmesg.txt
```

### Metadata in Files

Include extraction metadata:

```bash
# Create log with metadata
{
    echo "# Live System Data: [Topic]"
    echo "# Date: $(date)"
    echo "# Device: HY300 Device A"
    echo "# Command: adb shell su -c 'command'"
    echo "---"
    adb shell su -c "command"
} > logs/live-data.txt
```

### Comparison Report Format

```markdown
# Hardware Validation Report: [Topic]

**Date:** [date]
**Phase:** Phase X - [name]
**Device:** Device A/B
**Tester:** [name]

## Objective
[What was being validated]

## Research Baseline
**Document:** research/docs/DOCUMENT.md
**Expected:** [summary of research findings]

## Hardware Extraction
**Method:** [commands used]
**Files:** logs/live-*.txt

## Comparison Results

### Matches ✅
- [Item 1]: Validated
- [Item 2]: Confirmed

### Differences ⚠️
- [Item 1]: [hardware value] vs research [value]
  - **Reason:** [explanation]
  - **Impact:** [low/medium/high]

### Contradictions ❌
- [Item 1]: Research claimed [X], hardware shows [Y]
  - **Investigation:** [why research was wrong]
  - **Action:** Update research with correction

## Overall Assessment
**Research Accuracy:** [percentage]
**Hardware-Specific Findings:** [count]
**Confidence Level:** [high/medium/low]

## Next Steps
- [ ] Update research docs
- [ ] Integrate findings into configuration
- [ ] Document hardware-specific quirks
```

---

## Safety Protocols

### Read-Only Operations (Safe)

All extraction commands above are **read-only** and safe to execute:

✅ `cat` - Reading files  
✅ `ls` - Listing directories  
✅ `getprop` - Reading properties  
✅ `dumpsys` - Dumping system state  
✅ `dmesg` - Reading kernel log  
✅ `find` - Searching filesystem  

### Operations to AVOID

❌ **NEVER** use these during extraction:

```bash
# DANGEROUS - DO NOT USE
adb shell su -c "echo X > /sys/..."         # Writing to sysfs
adb shell su -c "insmod module.ko"          # Loading modules
adb shell su -c "reboot"                    # Rebooting device
adb shell su -c "setprop ..."               # Modifying properties
adb shell su -c "rm ..."                    # Deleting files
```

### Emergency Stop

If device becomes unresponsive:

```bash
# Try soft reboot first
adb reboot

# If ADB unresponsive
# Power cycle device manually
```

---

## Complete Extraction Script

```bash
#!/bin/bash
# full-system-extraction.sh - Complete live system data extraction
# Safety Level: 🟢 READ ONLY
# Usage: ./full-system-extraction.sh [device-name]

DEVICE="${1:-device-a}"
TIMESTAMP=$(date +%Y%m%d-%H%M%S)
LOGDIR="logs/extraction-${DEVICE}-${TIMESTAMP}"

mkdir -p "$LOGDIR"

echo "Starting full system extraction..."
echo "Device: $DEVICE"
echo "Timestamp: $TIMESTAMP"
echo "Output: $LOGDIR/"
echo ""

# System Information
echo "[1/10] Extracting system information..."
adb shell su -c "cat /proc/cpuinfo" > "$LOGDIR/cpuinfo.txt"
adb shell su -c "cat /proc/meminfo" > "$LOGDIR/meminfo.txt"
adb shell su -c "uname -a" > "$LOGDIR/kernel-version.txt"
adb shell su -c "getprop" > "$LOGDIR/properties.txt"

# Kernel and Modules
echo "[2/10] Extracting kernel information..."
adb shell su -c "lsmod" > "$LOGDIR/modules.txt"
adb shell su -c "cat /proc/cmdline" > "$LOGDIR/cmdline.txt"
adb shell su -c "dmesg" > "$LOGDIR/dmesg.txt"

# Device Tree
echo "[3/10] Extracting device tree..."
adb shell su -c "find /proc/device-tree -type f -exec sh -c 'echo {}; cat {}; echo ---' \;" > "$LOGDIR/devicetree-complete.txt"
adb shell su -c "find /proc/device-tree -print" > "$LOGDIR/devicetree-structure.txt"

# Hardware Resources
echo "[4/10] Extracting hardware resources..."
adb shell su -c "cat /proc/iomem" > "$LOGDIR/iomem.txt"
adb shell su -c "cat /proc/interrupts" > "$LOGDIR/interrupts.txt"
adb shell su -c "cat /proc/partitions" > "$LOGDIR/partitions.txt"

# Storage
echo "[5/10] Extracting storage information..."
adb shell su -c "mount" > "$LOGDIR/mounts.txt"
adb shell su -c "df -h" > "$LOGDIR/df.txt"

# Devices
echo "[6/10] Extracting device information..."
adb shell su -c "ls -lR /dev" > "$LOGDIR/dev-devices.txt"
adb shell su -c "ls -l /sys/bus/platform/drivers" > "$LOGDIR/platform-drivers.txt"

# Network
echo "[7/10] Extracting network information..."
adb shell su -c "ip addr show" > "$LOGDIR/ip-addr.txt"
adb shell su -c "ip link show" > "$LOGDIR/ip-link.txt"
adb shell su -c "ip route show" > "$LOGDIR/ip-route.txt"

# Display and Graphics
echo "[8/10] Extracting display information..."
adb shell su -c "dumpsys display" > "$LOGDIR/display.txt"
adb shell su -c "ls -l /dev/fb*" > "$LOGDIR/framebuffer.txt"

# Input Devices
echo "[9/10] Extracting input devices..."
adb shell su -c "getevent -p" > "$LOGDIR/input-events.txt"
adb shell su -c "dumpsys sensorservice" > "$LOGDIR/sensors.txt"

# Processes
echo "[10/10] Extracting process information..."
adb shell su -c "ps -A" > "$LOGDIR/processes.txt"
adb shell su -c "ps -A -T" > "$LOGDIR/process-tree.txt"

echo ""
echo "Extraction complete!"
echo "Files saved to: $LOGDIR/"
echo ""
echo "File count: $(ls -1 $LOGDIR | wc -l)"
echo "Total size: $(du -sh $LOGDIR | cut -f1)"
echo ""
echo "Next steps:"
echo "1. Review extracted data"
echo "2. Compare with research findings in research/docs/"
echo "3. Document validation results"
```

---

## Quick Reference

### Essential Extractions (Minimum)

```bash
# Quick baseline extraction (5 min)
adb shell su -c "cat /proc/cpuinfo" > live-cpuinfo.txt
adb shell su -c "lsmod" > live-modules.txt
adb shell su -c "dmesg" > live-dmesg.txt
adb shell su -c "find /proc/device-tree -print" > live-dt-structure.txt
adb shell su -c "getprop" > live-properties.txt
```

### Research Comparison (Quick)

```bash
# Compare with research quickly
diff <(sort live-modules.txt) <(sort research/docs/expected-modules.txt)
```

### Documentation (Quick)

```bash
# Quick validation note
echo "✅ Validated: [item]" >> phases/phase1/validation-notes.md
```

---

**Safety Reminder:** All commands in this guide are read-only and safe. Never execute write operations during extraction phase.

**Last Updated:** November 3, 2025
