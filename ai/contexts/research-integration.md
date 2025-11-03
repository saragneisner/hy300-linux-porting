# Research Integration Guidelines

**Purpose:** Guide for integrating software analysis research with hardware validation  
**Philosophy:** Hardware Truth > Software Analysis  
**Created:** November 3, 2025

---

## Core Principle: Hardware-First Approach

Live hardware observations **always take precedence** over research findings. Research provides context, patterns, and deeper analysis, but hardware is ground truth.

```
Hardware Reality → Research Validation → Updated Knowledge
```

## Integration Workflow

### 1. Before Starting a Task

**Check if research exists:**

```bash
# Search for relevant research
cd research/docs
grep -ri "keyword" .

# Example: Before HDMI input work
grep -ri "hdmi input" .
grep -ri "v4l2" .
```

**Read relevant research documents:**

```bash
# Identify key documents
research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md  # Component overview
research/docs/ANDROID_KERNEL_DRIVER_ANALYSIS.md     # Driver details
research/docs/FACTORY_DTB_ANALYSIS.md               # Hardware baseline

# Read specific analysis
cat research/docs/TOPIC_ANALYSIS.md
```

**Extract key information:**
- Expected hardware behavior
- Known configuration requirements
- Identified issues or limitations
- Driver dependencies
- Register addresses and IOCTLs

### 2. During Hardware Analysis

**Extract live data from hardware:**

```bash
# From running Android system
adb shell su -c "cat /proc/cpuinfo" > live-cpuinfo.txt
adb shell su -c "lsmod" > live-modules.txt
adb shell su -c "dmesg" > live-dmesg.txt

# Device tree extraction
adb shell su -c "find /proc/device-tree -type f -exec sh -c 'echo {}; cat {}; echo' \;" > live-devicetree.txt

# Hardware registers
adb shell su -c "cat /proc/iomem" > live-iomem.txt
adb shell su -c "cat /proc/interrupts" > live-interrupts.txt
```

**Compare with research findings:**

```bash
# Direct comparison
diff live-data.txt research/docs/expected-data.txt

# Analyze differences
grep -v "^#" live-data.txt | sort > live-sorted.txt
grep -v "^#" research/docs/expected-data.txt | sort > research-sorted.txt
diff live-sorted.txt research-sorted.txt
```

**Document findings:**

Create comparison report in phase documentation:

```bash
cd phases/phase1-hardware-baseline
cat > research-validation-report.md << 'EOF'
# Research Validation Report: [Topic]

## Research Source
- Document: `research/docs/TOPIC_ANALYSIS.md`
- Expected behavior: [description]

## Hardware Findings
- Live observation: [description]
- Command used: `adb shell su -c "command"`

## Comparison
- ✅ Matches: [what matched]
- ⚠️ Differs: [differences found]
- ❌ Contradicts: [contradictions]

## Analysis
[Why differences exist, hardware-specific behavior]

## Action Items
- [ ] Update research docs with hardware findings
- [ ] Adjust configuration based on hardware
- [ ] Document hardware-specific quirks
EOF
```

### 3. After Hardware Validation

**Update research if needed:**

```bash
cd research/docs

# For minor corrections
echo "## Hardware Validation Update ($(date +%Y-%m-%d))" >> TOPIC_ANALYSIS.md
echo "" >> TOPIC_ANALYSIS.md
echo "**Hardware Testing Results:**" >> TOPIC_ANALYSIS.md
echo "- Finding: [description]" >> TOPIC_ANALYSIS.md
echo "- Validated: [what was confirmed]" >> TOPIC_ANALYSIS.md
echo "- Corrected: [what needed adjustment]" >> TOPIC_ANALYSIS.md

# For major updates, create hardware-specific doc
cat > TOPIC_HARDWARE_VALIDATION.md << 'EOF'
# Hardware Validation: [Topic]

**Validation Date:** [date]
**Phase:** [phase number]
**Device:** [device A/B]

## Research Baseline
... (reference to research docs)

## Hardware Results
... (actual findings)

## Validated Findings
✅ [confirmed research findings]

## Hardware-Specific Behavior
⚠️ [hardware variations from research]

## Updated Configuration
... (hardware-validated config)
EOF
```

**Document integration lessons:**

```bash
cd phases/phaseX-name

cat >> research-integration.md << 'EOF'
## Task [number]: [name]

**Research Used:**
- `research/docs/DOCUMENT1.md` - [what it provided]
- `research/drivers/MODULE.c` - [how it helped]
- `research/configs/CONFIG.txt` - [baseline config]

**Validation Results:**
- Research accuracy: [percentage]
- Hardware-specific findings: [count]
- Configuration adjustments: [changes made]

**Key Learnings:**
- [lesson 1]
- [lesson 2]

**Research Updates:**
- [ ] Updated `research/docs/...` with findings
- [ ] Created `research/docs/.../HARDWARE_VALIDATION.md`
EOF
```

---

## Research Document Reference Map

### Phase I: Hardware Baseline

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md` | Component status matrix | Baseline for live component verification |
| `research/docs/FACTORY_DTB_ANALYSIS.md` | Device tree analysis | Compare with `/proc/device-tree/` |
| `research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md` | System layout | Reference for system dump analysis |
| `research/firmware/ROM_ANALYSIS.md` | Firmware structure | Partition layout verification |

**Usage Pattern:**
1. Read research doc to understand expected hardware
2. Extract corresponding live data from device
3. Compare and document differences
4. Update research with hardware truth

### Phase II: UART Access & Boot Analysis

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/docs/SUNXI_TOOLS_H713_SUMMARY.md` | Sunxi-tools H713 support | FEL mode procedures and memory map |
| `research/docs/H713_FEL_PROTOCOL_ANALYSIS.md` | FEL mode issues | Known BROM bugs and workarounds |
| `research/docs/USING_H713_FEL_MODE.md` | FEL usage guide | Step-by-step FEL procedures |
| `research/firmware/DRAM_ANALYSIS.md` | Memory parameters | U-Boot DRAM validation |

**Usage Pattern:**
1. Use research for FEL/UART procedures
2. Capture complete boot logs via UART
3. Validate memory map against research
4. Document any boot-specific hardware behavior

### Phase III: Bootloader Testing

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/ai/contexts/uboot-integration.md` | U-Boot standards | Development procedures |
| `research/firmware/DRAM_ANALYSIS.md` | DRAM parameters | Validate against live hardware |
| `research/configs/hy300_boot_usb_serial.txt` | U-Boot environment | Baseline configuration |
| `research/u-boot-sunxi-with-spl.bin` | Bootloader binary | Test on hardware |

**Usage Pattern:**
1. Use research U-Boot as baseline
2. Test on live hardware via FEL/UART
3. Validate DRAM parameters from boot logs
4. Adjust configuration based on hardware feedback

### Phase IV: Kernel Bringup

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/sun50i-h713-hy300.dts` | Device tree | Starting point for kernel |
| `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md` | Hardware status | Component enablement checklist |
| `research/configs/hy300_kernel_defconfig` | Kernel config | Build configuration baseline |
| `research/docs/FACTORY_DTB_ANALYSIS.md` | DTB analysis | Device tree validation |

**Usage Pattern:**
1. Boot research kernel on hardware
2. Monitor boot via UART console
3. Document what works and what fails
4. Adjust device tree based on hardware behavior
5. Iterate until clean boot

### Phase V: Driver Integration

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/docs/DRIVER_PRIORITY_MATRIX.md` | Integration order | Priority and dependencies |
| `research/drivers/misc/*` | MIPS, keystone, metrics | Load and test drivers |
| `research/drivers/media/platform/sunxi/*` | HDMI input | V4L2 driver testing |
| `research/docs/MIPS_COPROCESSOR_ANALYSIS.md` | MIPS protocol | Communication testing |
| `research/docs/AIC8800_WIFI_DRIVER_REFERENCE.md` | WiFi driver | Driver options and integration |

**Usage Pattern:**
1. Load research driver on live hardware
2. Test functionality with actual hardware
3. Monitor dmesg for errors
4. Adjust driver based on hardware feedback
5. Document hardware-specific quirks

### Phase VI: Armbian Build

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/configs/hy300_kernel_defconfig` | Kernel config | Build configuration |
| `research/docs/ANDROID_CONFIG_CALIBRATION_PHASE3.md` | Calibration | Factory calibration data |
| `research/nixos/*` | Build framework | Reference build system |
| `research/drivers/*` | Validated drivers | Include in Armbian build |

**Usage Pattern:**
1. Use research configs as baseline
2. Integrate validated drivers
3. Include calibration data
4. Build custom ROM
5. Test on hardware with A/B validation

### Phase VII: Privacy Hardening

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md` | Spyware list | Services to remove |
| `research/docs/ANDROID_FIRMWARE_ANALYSIS_COMPLETE_SUMMARY.md` | Telemetry | Telemetry endpoints to block |

**Usage Pattern:**
1. Reference research spyware/telemetry list
2. Remove identified services from Armbian
3. Monitor network traffic on live device
4. Verify no connections to research-identified servers
5. Document privacy improvements

### Phase VIII: System Validation

**Primary Research Documents:**

| Research Doc | Purpose | How to Use |
|--------------|---------|------------|
| `research/nixos/*` | VM testing | Pre-deployment validation |
| `research/ai/contexts/phase8-vm-testing.md` | Test procedures | VM testing workflow |
| `research/ai/contexts/vm-testing-validation.md` | Validation steps | Test framework |

**Usage Pattern:**
1. Use research VM framework for pre-tests
2. Perform final hardware validation
3. Compare VM vs hardware behavior
4. Document any VM-specific differences
5. Prepare production ROM

---

## Common Integration Patterns

### Pattern 1: Direct Validation

**When:** Research finding can be directly tested on hardware

```bash
# Research says: Device has 2GB RAM
# Validate:
adb shell su -c "cat /proc/meminfo | grep MemTotal"

# Research says: MIPS firmware at 0x40000000
# Validate:
adb shell su -c "hexdump -C /dev/mem -s 0x40000000 -n 256"
```

**Document:**
- ✅ Confirmed: Research matches hardware
- Document in phase findings

### Pattern 2: Partial Match

**When:** Research is mostly correct but needs hardware-specific adjustment

```bash
# Research says: UART at ttyS0
# Hardware shows: UART at ttyS1 (different pinmux)

# Action:
# - Note difference in phase docs
# - Update research with hardware-specific variant
# - Adjust configuration accordingly
```

**Document:**
- ⚠️ Partial match: Core concept correct, details differ
- Explain hardware-specific variation
- Update research notes

### Pattern 3: Contradiction

**When:** Research finding contradicts hardware behavior

```bash
# Research says: Mali-G31 GPU
# Hardware shows: Mali-Midgard GPU (different generation)

# Action:
# - Hardware wins (ground truth)
# - Investigate why research was incorrect
# - Update research with correction
# - Document source of original error
```

**Document:**
- ❌ Contradicted: Hardware differs from research
- Explain discrepancy
- Update research with correction note
- Document investigation of error source

### Pattern 4: Hardware Extension

**When:** Hardware has features not covered in research

```bash
# Research: No mention of feature X
# Hardware: Feature X exists and is functional

# Action:
# - Document new finding thoroughly
# - Add to research as new discovery
# - Test feature completely
# - Integrate into project
```

**Document:**
- ➕ New discovery: Not in research
- Complete documentation of new feature
- Add to research as hardware validation discovery

---

## Research Update Procedures

### Minor Updates (In-place)

For small corrections or confirmations:

```bash
cd research/docs

# Add hardware validation section to existing doc
cat >> TOPIC_ANALYSIS.md << 'EOF'

---
## Hardware Validation (Updated: 2025-11-03)

**Validation Phase:** Phase I - Hardware Baseline
**Validation Device:** Device A

### Confirmed Findings
- ✅ [Research finding]: Validated on hardware
- ✅ [Research finding]: Confirmed with [command]

### Hardware-Specific Variations
- ⚠️ [Research finding]: Hardware shows [variation]
  - Reason: [explanation]
  - Adjusted config: [changes]

### Corrections
- ❌ [Research finding]: Hardware shows [different behavior]
  - Investigation: [why research was incorrect]
  - Corrected understanding: [new finding]

**Validation Confidence:** [High/Medium/Low]
**Next Steps:** [further validation needed if any]
EOF
```

### Major Updates (New Document)

For significant new findings or complete hardware validation:

```bash
cd research/docs

cat > TOPIC_HARDWARE_VALIDATION.md << 'EOF'
# Hardware Validation: [Topic]

**Date:** 2025-11-03
**Phase:** Phase X - [Phase Name]
**Device:** Device A
**Status:** ✅ Validated / ⚠️ Partial / ❌ Contradicted

## Research Baseline

**Source Documents:**
- `research/docs/TOPIC_ANALYSIS.md`
- `research/docs/RELATED_DOC.md`

**Research Findings Summary:**
[Summary of what research claimed]

## Hardware Testing Procedure

**Setup:**
- Device: HY300 (Device A)
- Access: Root via ADB
- Tools: [tools used]

**Tests Performed:**
1. [Test 1]: `command`
2. [Test 2]: `command`

## Hardware Results

### Test 1: [Description]
```bash
$ command
output
```

**Analysis:** [interpretation]
**Match with Research:** ✅ / ⚠️ / ❌

### Test 2: [Description]
...

## Validation Summary

### Confirmed (✅)
- [Finding 1]
- [Finding 2]

### Hardware-Specific Variations (⚠️)
- [Variation 1]: [explanation]
- [Variation 2]: [explanation]

### Contradictions (❌)
- [Contradiction 1]: [hardware truth]
  - Investigation: [why research was wrong]

## Hardware-Validated Configuration

```
[Configuration based on actual hardware]
```

## Integration Notes

**For Future Phases:**
- Use hardware-validated config
- Reference this doc for ground truth
- Research docs provide context, this doc provides facts

## Next Steps

- [ ] Update primary research docs with findings
- [ ] Integrate validated config into build
- [ ] Document lessons learned in phase docs
EOF

# Link from original research doc
cat >> TOPIC_ANALYSIS.md << 'EOF'

**Hardware Validation:** See `TOPIC_HARDWARE_VALIDATION.md` for hardware-validated findings.
EOF
```

---

## Best Practices

### DO:

✅ **Always start with research** - Context is valuable  
✅ **Trust hardware over research** - Hardware is ground truth  
✅ **Document all differences** - Even small variations matter  
✅ **Update research with findings** - Keep knowledge base current  
✅ **Cross-reference everything** - Link research to validation docs  
✅ **Test systematically** - Follow research→hardware→validation workflow  
✅ **Explain discrepancies** - Understand why research differed  

### DON'T:

❌ **Don't skip research** - It provides valuable context  
❌ **Don't assume research is wrong** - Validate before dismissing  
❌ **Don't ignore contradictions** - Investigate and document  
❌ **Don't forget to update research** - Future work needs accurate info  
❌ **Don't mix research and hardware docs** - Keep separation clear  
❌ **Don't validate in VM** - Use actual hardware for ground truth  

---

## Troubleshooting

### Research Document Not Found

**Problem:** Can't find relevant research for current task

**Solution:**
```bash
# Broad search
cd research
find . -type f -name "*.md" | xargs grep -l "keyword"

# Content search
grep -ri "topic" docs/

# Check research index
cat research/README.md
cat research/docs/PROJECT_OVERVIEW.md
```

### Research Contradicts Hardware

**Problem:** Research says X, hardware shows Y

**Solution:**
1. **Re-test on hardware** - Verify your test is correct
2. **Check research context** - Maybe it was theoretical
3. **Investigate why** - Understand source of error
4. **Document thoroughly** - Help future work
5. **Update research** - Correct the record

### Research Incomplete

**Problem:** Research doesn't cover specific aspect needed

**Solution:**
1. **Check related docs** - Info might be elsewhere
2. **Test on hardware anyway** - Document new findings
3. **Add to research** - Contribute new knowledge
4. **Reference hardware validation** - Make it discoverable

---

## Examples

### Example 1: Device Tree Validation

**Research Document:** `research/docs/FACTORY_DTB_ANALYSIS.md`

**Task:** Validate device tree nodes against live hardware

```bash
# Step 1: Read research
cat research/docs/FACTORY_DTB_ANALYSIS.md | grep "uart"

# Research says: UART0 at 0x05000000

# Step 2: Extract from hardware
adb shell su -c "find /proc/device-tree/soc/serial* -name reg -exec hexdump -C {} \;"

# Output shows: 0x05000000 (matches!)

# Step 3: Document validation
echo "✅ UART0 address validated: 0x05000000" >> phases/phase1/dtb-validation.md

# Step 4: Update research
echo "Hardware validated: 2025-11-03" >> research/docs/FACTORY_DTB_ANALYSIS.md
```

### Example 2: Driver Testing

**Research Document:** `research/docs/MIPS_COPROCESSOR_ANALYSIS.md`

**Task:** Test MIPS communication driver

```bash
# Step 1: Read research protocol
cat research/docs/MIPS_COPROCESSOR_ANALYSIS.md | grep "register"

# Research documents register layout

# Step 2: Load research driver
adb push research/drivers/misc/hy300-mips.ko /data/local/tmp/
adb shell su -c "insmod /data/local/tmp/hy300-mips.ko"

# Step 3: Test on hardware
adb shell su -c "echo 'test' > /sys/class/hy300/mips/command"
adb shell su -c "dmesg | tail -20"

# Step 4: Compare behavior
# - Research predicted: Response in 10ms
# - Hardware shows: Response in 15ms (acceptable variation)

# Step 5: Document
cat >> phases/phase5/driver-validation.md << 'EOF'
## MIPS Driver Testing

Research: `research/docs/MIPS_COPROCESSOR_ANALYSIS.md`
Driver: `research/drivers/misc/hy300-mips.ko`

Results:
- ✅ Communication working
- ⚠️ Response time 15ms (research predicted 10ms)
- Hardware-specific: Timing variation acceptable
EOF
```

---

## Summary

**Research provides:**
- Context and background
- Expected behavior
- Configuration baselines
- Known issues
- Implementation examples

**Hardware provides:**
- Ground truth
- Actual behavior
- Real-world quirks
- Validation data
- Production reality

**Integration provides:**
- Validated knowledge
- Hardware-specific configs
- Updated understanding
- Production-ready code

**Always remember:** Hardware truth > Software analysis. Research guides, hardware decides.

---

**Last Updated:** November 3, 2025
**Maintainer:** Main project team
**Status:** Active integration guidelines
