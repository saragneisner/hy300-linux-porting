# Repository Restructuring - COMPLETE ✅

**Date Started:** November 3, 2025  
**Date Completed:** November 4, 2025, 23:55 UTC  
**Status:** RESTRUCTURING COMPLETE ✅  
**Priority:** CRITICAL - Affects entire project structure

---

## Executive Summary

This document outlines the fundamental restructuring of the HY300 Linux porting project to align with a **hardware-first research approach**. The existing `sun50iw12p1-research` repository (software analysis focused) will become a subdirectory of the new `rebuild/` root structure.

### Key Changes

1. **New Root Directory**: Current `rebuild/` becomes project root
2. **Research Integration**: `sun50iw12p1-research` becomes `research/` subdirectory
3. **Hardware-First Philosophy**: Live system analysis drives software research integration
4. **AI Context Migration**: `/ai/contexts` become central to new structure
5. **Phase Integration**: Software analysis findings feed into hardware validation phases

---

## Current vs. Target Structure

### Current Structure (Problem)

```
sun50iw12p1-research/                   # Root repo
├── ai/contexts/                        # AI agent contexts
├── docs/                               # 100+ analysis documents
├── firmware/                           # Extracted firmware
├── drivers/                            # Kernel drivers
├── rebuild/                            # NEW rebuild plan (isolated)
│   ├── phases/
│   ├── tasks/
│   └── ...
├── tools/
└── ...
```

**Problems:**
- `rebuild/` is isolated from main research
- Research context not accessible to rebuild workflow
- Software analysis disconnected from hardware approach
- AI contexts only available to old structure

### Target Structure (Solution)

```
hy300-linux-porting/                    # NEW ROOT (was rebuild/)
├── .git/                               # Git repo root
├── README.md                           # Main project README
├── PROJECT_ROADMAP.md                  # 8-phase roadmap
├── QUICK_START.md                      # Getting started
├── CONFIRMED_HARDWARE_FINDINGS.md      # Hardware validation
│
├── ai/                                 # AI AGENT INFRASTRUCTURE
│   ├── contexts/                       # Context files (migrated + enhanced)
│   │   ├── README.md
│   │   ├── current-status.md          # Updated for hardware-first
│   │   ├── hardware.md
│   │   ├── firmware-analysis.md
│   │   ├── cross-compilation.md
│   │   ├── development-environment.md
│   │   ├── hardware-safety.md
│   │   ├── hardware-testing-protocol.md
│   │   ├── delegation-standards.md
│   │   ├── documentation-standards.md
│   │   ├── git-standards.md
│   │   └── research-integration.md    # NEW: How to use research/ findings
│   │
│   ├── tools/                          # AI management tools
│   │   ├── task-manager               # Task management CLI
│   │   ├── context-manager            # Context management
│   │   └── git-manager                # Git automation
│   │
│   └── sessions/                       # AI session logs
│
├── phases/                             # EXECUTION PHASES
│   ├── phase1-hardware-baseline/       # Live system analysis
│   ├── phase2-uart-access/             # Serial console + recovery
│   ├── phase3-bootloader-testing/      # U-Boot FEL validation
│   ├── phase4-kernel-bringup/          # Mainline kernel boot
│   ├── phase5-driver-integration/      # Hardware driver enablement
│   ├── phase6-armbian-build/           # Custom ROM creation
│   ├── phase7-privacy-hardening/       # Privacy & security
│   └── phase8-validation/              # System testing
│
├── tasks/                              # TASK TRACKING
│   ├── pending/                        # Tasks to do
│   ├── in-progress/                    # Currently working
│   └── completed/                      # Finished tasks
│
├── hardware-access/                    # HARDWARE PROCEDURES
│   ├── root-access-guide.md
│   ├── uart-access-guide.md
│   ├── fel-recovery-guide.md
│   └── backup-procedures.md
│
├── agents/                             # AGENT GUIDELINES
│   └── AGENT_GUIDELINES.md
│
├── research/                           # SOFTWARE ANALYSIS ARCHIVE
│   │                                   # (Previously sun50iw12p1-research)
│   ├── README.md                       # Research archive overview
│   ├── docs/                           # 100+ analysis documents
│   │   ├── HY300_HARDWARE_ENABLEMENT_STATUS.md
│   │   ├── FACTORY_DTB_ANALYSIS.md
│   │   ├── ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md
│   │   ├── MIPS_COPROCESSOR_ANALYSIS.md
│   │   ├── AIC8800_WIFI_DRIVER_REFERENCE.md
│   │   └── ... (100+ docs)
│   │
│   ├── firmware/                       # Extracted components
│   │   ├── ROM_ANALYSIS.md
│   │   ├── FIRMWARE_COMPONENTS_ANALYSIS.md
│   │   ├── extracted_components/
│   │   └── boot0.bin, *.dtb, etc.
│   │
│   ├── drivers/                        # Kernel driver implementations
│   │   ├── misc/                       # MIPS, keystone, metrics
│   │   └── media/platform/sunxi/       # HDMI input
│   │
│   ├── tools/                          # Analysis utilities
│   │   ├── analyze_boot0.py
│   │   ├── compare_dram_params.py
│   │   └── hex_viewer.py
│   │
│   ├── configs/                        # Build configurations
│   │   ├── hy300_kernel_defconfig
│   │   └── hy300_h713_defconfig
│   │
│   ├── nixos/                          # VM testing framework
│   │   ├── configuration.nix
│   │   ├── services/
│   │   └── packages/
│   │
│   └── flake.nix                       # Development environment
│
├── build/                              # BUILD ARTIFACTS
│   ├── u-boot/                         # Bootloader builds
│   ├── kernel/                         # Kernel builds
│   ├── dtb/                            # Device tree blobs
│   └── images/                         # Final ROM images
│
├── backup/                             # DEVICE BACKUPS
│   ├── device-a/                       # Primary device
│   │   ├── full-dump.img
│   │   ├── partitions/
│   │   └── calibration/
│   │
│   └── device-b/                       # Test device
│       └── full-dump.img
│
├── logs/                               # EXECUTION LOGS
│   ├── phase1/
│   ├── phase2/
│   └── ...
│
└── flake.nix                           # Root development environment
```

---

## Integration Philosophy: Hardware-First with Software Research

### The Paradigm Shift

**Old Approach (sun50iw12p1-research):**
```
Software Analysis → Theoretical Understanding → (Hardware Testing Pending)
```

**New Approach (hy300-linux-porting):**
```
Live Hardware Analysis → Software Research Validation → Iterative Integration
```

### How It Works

#### Phase I: Hardware Baseline (NEW)
**Primary Source:** Live running device
```bash
# Extract everything from running Android system
adb shell su -c "cat /proc/cpuinfo"
adb shell su -c "lsmod"
adb shell su -c "cat /proc/device-tree/..."
```

**Research Reference:** Check against `research/docs/` findings
```bash
# Compare live findings with previous analysis
diff live-cpuinfo.txt research/docs/hardware-analysis/cpu-specs.txt
```

**Validation:**
- ✅ Live data is ground truth
- ✅ Research docs provide context and deeper analysis
- ✅ Discrepancies trigger investigation

#### Phase II: UART Access (NEW)
**Primary Source:** Serial console boot logs
```bash
# Capture complete boot sequence
minicom -D /dev/ttyUSB0 -C boot-log.txt
```

**Research Reference:** Compare with research findings
```bash
# Check U-Boot environment against research
diff live-uboot-env.txt research/firmware/uboot-analysis.txt
```

#### Phase III-VIII: Iterative Hardware Validation
**Pattern for Each Phase:**
1. **Extract live data** from hardware
2. **Consult research docs** for known patterns
3. **Validate against hardware** - live system wins
4. **Update research docs** with new findings
5. **Document integration** lessons learned

---

## AI Context Integration Strategy

### Context File Migration and Enhancement

#### Existing Contexts (Migrate from sun50iw12p1-research)

**Core Development:**
- ✅ `hardware.md` - Migrate as-is
- ✅ `firmware-analysis.md` - Enhance with live system procedures
- ✅ `cross-compilation.md` - Migrate as-is
- ✅ `development-environment.md` - Merge with root flake.nix
- ✅ `sunxi-tools.md` - Migrate as-is

**Safety and Testing:**
- ✅ `hardware-safety.md` - Enhance with A/B testing protocols
- ✅ `hardware-testing-protocol.md` - Expand with UART recovery
- ✅ `uboot-integration.md` - Migrate as-is
- ✅ `driver-integration-strategy.md` - Migrate as-is

**VM Testing (Phase VIII):**
- ✅ `phase8-vm-testing.md` - Migrate to phases/phase8-validation/
- ✅ `vm-testing-validation.md` - Migrate with research references
- ✅ `kodi-integration.md` - Migrate as-is
- ✅ `prometheus-metrics.md` - Migrate as-is

**Project Management:**
- ✅ `current-status.md` - **REWRITE** for hardware-first approach
- ✅ `delegation-standards.md` - Enhance with research integration
- ✅ `documentation-standards.md` - Migrate as-is
- ✅ `git-standards.md` - Migrate as-is

#### New Contexts (Create)

**`research-integration.md`** - NEW CRITICAL CONTEXT
```markdown
# Research Integration Guidelines

## Philosophy
**Hardware Truth > Software Analysis**

Live hardware observations always take precedence over research findings.
Research provides context, patterns, and deeper analysis.

## How to Use research/ Directory

### 1. Before Starting a Task
Check if research exists:
```bash
grep -r "topic" research/docs/
```

### 2. During Hardware Analysis
Cross-reference findings:
```bash
# Live data
adb shell su -c "command" > live-data.txt

# Compare with research
diff live-data.txt research/docs/relevant-analysis.txt
```

### 3. After Hardware Validation
Update research if needed:
```bash
# Document new findings
echo "New observation: ..." >> research/docs/hardware-update.md
git commit -m "[Phase X] Update research with hardware findings"
```

## Research Document Reference Map
... (detailed mapping of docs to phases)
```

**`live-system-analysis.md`** - NEW
```markdown
# Live System Analysis Procedures

## Safe Data Extraction Commands
... (ADB commands, safety protocols)

## Comparison with Research Findings
... (how to validate research)

## Documentation Standards
... (where to record live findings)
```

---

## Phase Integration with Research

### Phase I: Hardware Baseline
**Research Integration:**
- Use `research/docs/FACTORY_DTB_ANALYSIS.md` as comparison baseline
- Validate device tree nodes against live `/proc/device-tree/`
- Document discrepancies in `phases/phase1-hardware-baseline/findings.md`

**New Deliverables:**
- Live device tree dump
- Live kernel module list
- Live hardware register map
- Comparison report: live vs research

### Phase II: UART Access
**Research Integration:**
- Use `research/docs/SUNXI_TOOLS_H713_SUMMARY.md` for FEL procedures
- Reference `research/docs/H713_FEL_PROTOCOL_ANALYSIS.md` for known issues
- Validate U-Boot environment against `research/firmware/uboot-env.txt`

**New Deliverables:**
- Complete boot log from UART
- U-Boot environment comparison
- Memory map validation
- UART recovery procedures tested on live hardware

### Phase III: Bootloader Testing
**Research Integration:**
- Use `research/u-boot-sunxi-with-spl.bin` as baseline
- Reference `research/docs/uboot-integration.md` for FEL testing
- Cross-check DRAM parameters with `research/firmware/DRAM_ANALYSIS.md`

**Validation:**
- Boot research U-Boot on live hardware
- Document any differences in behavior
- Update research with hardware-validated findings

### Phase IV: Kernel Bringup
**Research Integration:**
- Use `research/sun50i-h713-hy300.dts` as starting device tree
- Reference `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md`
- Test research kernel configs on live hardware

**Iterative Process:**
1. Boot research kernel on hardware
2. Document what works, what doesn't
3. Adjust device tree based on hardware feedback
4. Update research with validated configuration

### Phase V: Driver Integration
**Research Integration:**
- Use `research/drivers/` kernel modules
- Reference `research/docs/DRIVER_PRIORITY_MATRIX.md`
- Test research drivers: MIPS, keystone, HDMI input

**Hardware Validation:**
- Load each research driver on live system
- Test functionality with actual hardware
- Document hardware-specific quirks
- Update drivers based on real hardware behavior

### Phase VI: Armbian Build
**Research Integration:**
- Use `research/configs/hy300_kernel_defconfig`
- Reference `research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md` for services
- Integrate validated drivers from Phase V

**Custom ROM:**
- Build Armbian with research-validated components
- Test on hardware with UART monitoring
- A/B testing with two devices

### Phase VII: Privacy Hardening
**Research Integration:**
- Reference `research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md` for spyware list
- Remove identified telemetry services
- Validate network traffic on live system

**Validation:**
- Monitor network with `tcpdump` on live device
- Verify no connections to Chinese servers
- Document privacy improvements

### Phase VIII: System Validation
**Research Integration:**
- Use `research/nixos/` VM testing framework for pre-deployment validation
- Reference `research/docs/phase8-vm-testing.md`
- Final validation on live hardware

**Deliverables:**
- Complete hardware test report
- Performance benchmarks
- Stability testing results
- Production ROM image ready for daily use

---

## Migration Steps (Detailed Procedure)

### Step 0: Preparation (CRITICAL)
**Backup everything before restructuring**

```bash
# Full project backup
cd /home/luca/Desktop
tar -czf sun50iw12p1-research-backup-$(date +%Y%m%d).tar.gz sun50iw12p1-research/

# Verify backup
tar -tzf sun50iw12p1-research-backup-*.tar.gz | wc -l
```

### Step 1: Create New Root Structure
**Create clean slate for new root**

```bash
cd /home/luca/Desktop
mkdir -p hy300-linux-porting
cd hy300-linux-porting

# Initialize git in new root
git init
git config user.name "Your Name"
git config user.email "your.email@example.com"
```

### Step 2: Migrate rebuild/ to Root
**Move rebuild content to new root**

```bash
# Copy rebuild structure to new root
cp -r ../sun50iw12p1-research/rebuild/* .
cp -r ../sun50iw12p1-research/rebuild/.git .  # Preserve history if desired

# Verify structure
tree -L 2
```

### Step 3: Clone sun50iw12p1-research as research/
**Integrate research as subdirectory**

```bash
# Option A: Full clone (preserves git history)
git clone /home/luca/Desktop/sun50iw12p1-research research/

# Option B: Copy without git history (cleaner)
mkdir research/
cp -r ../sun50iw12p1-research/* research/
cp ../sun50iw12p1-research/.gitignore research/

# Add research README explaining purpose
cat > research/README.md << 'EOF'
# Software Analysis Research Archive

This directory contains the complete `sun50iw12p1-research` repository, which performed extensive software analysis of the HY300 Android system.

**Purpose:** Reference material for hardware validation phases
**Status:** Software analysis complete, hardware validation in progress
**Usage:** Consult docs during hardware testing to validate findings

## Integration with Main Project

The main project (`../`) follows a hardware-first approach:
1. Extract data from live hardware
2. Compare with research findings in this directory
3. Validate or update research based on hardware truth
4. Document integration lessons learned

See `../ai/contexts/research-integration.md` for detailed procedures.
EOF
```

### Step 4: Migrate AI Contexts
**Move and enhance AI infrastructure**

```bash
# Create ai/ directory in new root
mkdir -p ai/contexts ai/tools ai/sessions

# Copy context files
cp research/ai/contexts/*.md ai/contexts/

# Copy AI tools
cp -r research/ai/tools/* ai/tools/

# Preserve session history if desired
cp -r research/ai/sessions/* ai/sessions/
```

### Step 5: Create New Context Files
**Add hardware-first context files**

```bash
cd ai/contexts

# Create research-integration.md
cat > research-integration.md << 'EOF'
# Research Integration Guidelines
... (content from above)
EOF

# Create live-system-analysis.md
cat > live-system-analysis.md << 'EOF'
# Live System Analysis Procedures
... (content from above)
EOF

# Update current-status.md for hardware-first approach
# (Edit file with hardware-first phase status)
```

### Step 6: Update Context README
**Update ai/contexts/README.md**

```bash
cat >> ai/contexts/README.md << 'EOF'

### Research Integration
- `research-integration.md` - How to use research/ findings
- `live-system-analysis.md` - Live system data extraction

### Research Reference
- Hardware findings: `../../research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md`
- Firmware analysis: `../../research/firmware/ROM_ANALYSIS.md`
- Driver implementations: `../../research/drivers/`
EOF
```

### Step 7: Update Phase Documentation
**Link phases to research findings**

```bash
# Update each phase README with research references
cd phases/

for phase in phase*/; do
  cat >> "$phase/README.md" << 'EOF'

## Research Integration

**Relevant Research Documents:**
- Check `../../research/docs/` for related analysis
- Cross-reference findings with live hardware
- Document validation results

See `../../ai/contexts/research-integration.md` for procedures.
EOF
done
```

### Step 8: Create Build Directory Structure
**Organize build artifacts**

```bash
cd ../../  # Back to root
mkdir -p build/{u-boot,kernel,dtb,images}
mkdir -p backup/{device-a,device-b}
mkdir -p logs

# Create .gitignore for build artifacts
cat > build/.gitignore << 'EOF'
# Build artifacts (exclude from git)
*.bin
*.img
*.dtb
*.elf
*.o
*.a
*.so
*.ko
EOF
```

### Step 9: Migrate Flake Configuration
**Create root flake.nix**

```bash
# Copy research flake as starting point
cp research/flake.nix .

# Enhance with additional tools for hardware access
# (Edit flake.nix to add adb, minicom, screen, etc.)
```

### Step 10: Update Root Documentation
**Update README.md, PROJECT_ROADMAP.md**

```bash
# Update README.md with new structure
sed -i 's|sun50iw12p1-research|hy300-linux-porting|g' README.md

# Add research directory explanation
cat >> README.md << 'EOF'

## Directory Structure

- `phases/` - Execution phases (hardware-first approach)
- `tasks/` - Task tracking system
- `research/` - Software analysis archive (sun50iw12p1-research)
- `ai/` - AI agent infrastructure and contexts
- `build/` - Build artifacts
- `backup/` - Device backups
- `logs/` - Execution logs

See `research/README.md` for software analysis documentation.
EOF
```

### Step 11: Initial Commit
**Commit new structure**

```bash
git add .
git commit -m "[Restructuring] Create hy300-linux-porting root with research integration

- Migrated rebuild/ to root directory
- Integrated sun50iw12p1-research as research/ subdirectory
- Migrated and enhanced AI contexts
- Created research integration guidelines
- Updated phase documentation with research references
- Hardware-first approach with software research validation"
```

### Step 12: Verification
**Verify complete structure**

```bash
# Check structure
tree -L 2 -I 'research'

# Verify AI contexts
ls -la ai/contexts/

# Verify research integration
ls -la research/docs/ | head -20

# Test development environment
nix develop

# Verify task management
ai/tools/task-manager list
```

---

## Updated Workflow for Agents

### Starting a Task (New Procedure)

```bash
# 1. Check for in-progress tasks
ai/tools/task-manager find-inprogress

# 2. Get next priority task
ai/tools/task-manager next

# 3. Check relevant research
task_topic=$(ai/tools/task-manager status 001 | grep "Topic")
grep -r "$task_topic" research/docs/

# 4. Review research context
# Read relevant docs from research/

# 5. Start task with research context
ai/tools/task-manager start 001
```

### During Task Execution

```bash
# Extract live hardware data
adb shell su -c "command" > live-data.txt

# Compare with research
diff live-data.txt research/docs/expected-data.txt

# Document findings
echo "Hardware validation: ..." >> phases/phase1-hardware-baseline/findings.md

# Update research if needed
echo "Update: ..." >> research/docs/hardware-update.md
```

### Completing a Task

```bash
# Validate success criteria
# ... (task-specific validation)

# Document research integration
cat >> phases/phase1-hardware-baseline/research-integration.md << EOF
## Task 001: Research Validation
- Used: research/docs/FACTORY_DTB_ANALYSIS.md
- Validated: Device tree nodes against live system
- Findings: 98% match, documented 3 discrepancies
- Updated: research/docs/dtb-update.md
EOF

# Complete task
ai/tools/task-manager complete 001

# Commit with research references
git commit -m "[Phase I Task 001] Hardware validation with research integration

- Validated live device tree against research findings
- Cross-referenced with research/docs/FACTORY_DTB_ANALYSIS.md
- Documented 3 hardware-specific variations
- Updated research with live system data"
```

---

## Research Document Reference Map

### Phase I: Hardware Baseline
**Key Research Docs:**
- `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md` - Component status
- `research/docs/FACTORY_DTB_ANALYSIS.md` - Device tree baseline
- `research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md` - System layout
- `research/firmware/ROM_ANALYSIS.md` - Firmware structure

### Phase II: UART Access
**Key Research Docs:**
- `research/docs/SUNXI_TOOLS_H713_SUMMARY.md` - FEL procedures
- `research/docs/H713_FEL_PROTOCOL_ANALYSIS.md` - Known issues
- `research/docs/USING_H713_FEL_MODE.md` - FEL mode usage

### Phase III: Bootloader Testing
**Key Research Docs:**
- `research/docs/uboot-integration.md` - U-Boot context
- `research/firmware/DRAM_ANALYSIS.md` - Memory parameters
- `research/configs/hy300_boot_usb_serial.txt` - U-Boot environment

### Phase IV: Kernel Bringup
**Key Research Docs:**
- `research/sun50i-h713-hy300.dts` - Device tree
- `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md` - Hardware status
- `research/configs/hy300_kernel_defconfig` - Kernel config

### Phase V: Driver Integration
**Key Research Docs:**
- `research/docs/DRIVER_PRIORITY_MATRIX.md` - Driver priorities
- `research/drivers/misc/` - MIPS, keystone drivers
- `research/drivers/media/platform/sunxi/` - HDMI input
- `research/docs/MIPS_COPROCESSOR_ANALYSIS.md` - MIPS protocol
- `research/docs/AIC8800_WIFI_DRIVER_REFERENCE.md` - WiFi driver

### Phase VI: Armbian Build
**Key Research Docs:**
- `research/configs/hy300_kernel_defconfig` - Kernel config
- `research/docs/ANDROID_CONFIG_CALIBRATION_PHASE3.md` - Calibration
- `research/nixos/` - Build framework reference

### Phase VII: Privacy Hardening
**Key Research Docs:**
- `research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md` - Spyware list
- `research/docs/ANDROID_FIRMWARE_ANALYSIS_COMPLETE_SUMMARY.md` - Telemetry

### Phase VIII: System Validation
**Key Research Docs:**
- `research/nixos/` - VM testing framework
- `research/ai/contexts/phase8-vm-testing.md` - VM validation
- `research/ai/contexts/vm-testing-validation.md` - Test procedures

---

## Risk Assessment

### Risks and Mitigation

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| Data loss during restructuring | Critical | Low | Complete backup before migration |
| Git history loss | Medium | Low | Option A: Clone with history |
| Context confusion | Medium | Medium | Clear documentation in research/README.md |
| Research outdated | Low | Medium | Hardware validation finds truth |
| Agent workflow disruption | Medium | Low | Updated context files guide new workflow |

### Rollback Plan

If restructuring causes issues:

```bash
# Restore from backup
cd /home/luca/Desktop
rm -rf hy300-linux-porting
tar -xzf sun50iw12p1-research-backup-*.tar.gz
cd sun50iw12p1-research

# Continue with old structure
# (No data lost, can retry restructuring)
```

---

## Timeline

### Estimated Duration: 2-3 hours

**Step 0: Preparation** ⏱️ 15 min
- Create backup
- Verify backup integrity

**Steps 1-6: Core Migration** ⏱️ 45 min
- Create new root
- Migrate rebuild/
- Clone research/
- Migrate AI contexts
- Create new context files

**Steps 7-10: Integration** ⏱️ 45 min
- Update phase documentation
- Create build structure
- Migrate flake configuration
- Update root documentation

**Steps 11-12: Finalization** ⏱️ 30 min
- Initial commit
- Comprehensive verification
- Test agent workflows

---

## Success Criteria

### Structure Verification
- [ ] New root `hy300-linux-porting/` exists
- [ ] `research/` subdirectory contains complete sun50iw12p1-research
- [ ] All phase documentation migrated
- [ ] AI contexts migrated and enhanced
- [ ] Build structure created
- [ ] Root flake.nix configured

### Integration Verification
- [ ] Research integration context created
- [ ] Phase docs link to research findings
- [ ] AI context README updated
- [ ] Workflow documentation updated

### Functional Verification
- [ ] `nix develop` works in new root
- [ ] `ai/tools/task-manager` works
- [ ] Can access research docs from root
- [ ] Agent guidelines updated with research integration

### Documentation Verification
- [ ] README.md reflects new structure
- [ ] PROJECT_ROADMAP.md updated
- [ ] All cross-references working
- [ ] research/README.md explains purpose

---

## Next Steps After Restructuring

### Immediate (Same Session)
1. **Verify structure** with comprehensive tests
2. **Update AGENTS.md** with new workflow
3. **Test task management** in new structure
4. **Create first task** using research integration

### Phase I Preparation
1. **Review research findings** from `research/docs/`
2. **Prepare data extraction scripts** for live system
3. **Set up comparison framework** (live vs research)
4. **Start Task 001** with hardware-first approach

---

**Status:** Planning complete, ready for execution
**Next Action:** User approval, then execute migration steps
**Rollback:** Backup available, safe to proceed
