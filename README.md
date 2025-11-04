# HY300 Linux Porting - Complete Rebuild Plan

**Status:** ✅ Phase I COMPLETE (9/9 tasks) → Phase 1.5 (sunxi-tools) starting  
**Start Date:** November 3, 2025  
**Current Phase:** Phase 1.5 - sunxi-tools Implementation (🔄 Awaiting CP2102)  
**Hardware Access:** ✅ Root access available, 🔄 UART/FEL access upcoming  
**End Goal:** 🛡️ Privacy-Focused Armbian Custom ROM

## 🎯 Project Mission

**Replace factory Android firmware (containing spyware, telemetry, and malware) with a privacy-focused custom ROM based on Armbian.**

## Project Overview

This project ports mainline Linux to the HY300 Android projector using a **hardware-first development approach**. Live system analysis from the running device drives all decisions, validated against extensive software research.

### Key Differentiators

**Hardware-First Philosophy:**
- Live hardware analysis = ground truth
- Software research (`research/`) = context and patterns  
- All decisions validated against actual hardware behavior
- Two devices available for safe A/B testing

**Research Integration:**
- 100+ analysis documents from software-only research phase
- Complete firmware extraction and driver implementation
- All research findings require hardware validation
- See `ai/contexts/research-integration.md` for procedures

## Key Differences from Previous Approach

### Previous Project (Research-First)
- Started with firmware analysis without hardware
- Made assumptions based on extracted files
- Limited validation capabilities
- FEL mode complications (H713 BROM issues)

### This Rebuild (Hardware-First)
- ✅ **Root access to running device**
- ✅ **Full system dump capability**
- 🔄 **UART serial console access** (coming soon)
- ✅ **Live hardware validation** at every step
- ✅ **Proper baseline establishment** before modifications

## 🛡️ Hardware Safety Protocol

**CRITICAL: Two devices available for A/B testing**
- ✅ **Device A:** Primary development and testing
- ✅ **Device B:** Control device with factory firmware
- ✅ **Complete factory backup image** exists
- ✅ **NO BRICKING TOLERANCE** - all changes must be reversible
- ⚠️ **UART recovery required** before any risky operations

## Hardware Access Status

### Available Now
- ✅ Root shell access via ADB/SSH
- ✅ Full filesystem access (including /system)
- ✅ Live kernel module inspection
- ✅ Direct hardware register access
- ✅ Real-time system monitoring
- ✅ Factory firmware preservation
- ✅ **Two devices for A/B testing**
- ✅ **Complete backup image**

### Coming Soon (Next Few Days)
- 🔄 **UART serial console** (bootloader access)
- 🔄 U-Boot environment inspection
- 🔄 Boot process monitoring
- 🔄 Early kernel debug output
- 🔄 **Recovery procedures validated**

## Project Structure

## Directory Structure

```
hy300-linux-porting/         # NEW ROOT
├── README.md                # This file
├── PROJECT_ROADMAP.md       # 8-phase detailed roadmap
├── QUICK_START.md           # Getting started guide
│
├── phases/                  # Execution phases (hardware-first)
├── tasks/                   # Task tracking system
├── hardware-access/         # Hardware procedures and guides
├── agents/                  # AI agent guidelines
│
├── ai/                      # AI INFRASTRUCTURE
│   ├── contexts/            # Context files (migrated + enhanced)
│   ├── tools/               # Task management tools
│   └── sessions/            # Session logs
│
├── research/                # SOFTWARE ANALYSIS ARCHIVE
│   │                        # (Complete sun50iw12p1-research repo)
│   ├── README.md            # Research archive overview
│   ├── docs/                # 100+ analysis documents
│   ├── firmware/            # Extracted components
│   ├── drivers/             # Kernel modules (research)
│   ├── tools/               # Analysis utilities
│   └── ai/                  # Original AI contexts (archived)
│
├── build/                   # Build artifacts
│   ├── u-boot/              # Bootloader builds
│   ├── kernel/              # Kernel builds
│   ├── dtb/                 # Device tree blobs
│   └── images/              # Final ROM images
│
├── backup/                  # Device backups
│   ├── device-a/            # Primary device
│   └── device-b/            # Test device
│
├── logs/                    # Execution logs
└── flake.nix                # Development environment
```

**Key Integration Points:**
- `ai/contexts/research-integration.md` - How to use research findings
- `ai/contexts/live-system-analysis.md` - Hardware data extraction
- `research/docs/` - Software analysis for context
- `research/drivers/` - Driver implementations (need hardware testing)
- **`phases/research-validation/RESEARCH_MAPPING.md` - Hardware findings validated against UART logs** ⭐ NEW
- **`phases/RECOVERY_TEMPLATE.md` - Standard recovery procedures for all phases** ⭐ NEW
- **`phases/phase2-uart-access/UART_BOOTLOADER_SAFETY_PROTOCOL.md` - Safe SRAM-based bootloader testing** ⭐ NEW

## Phase Overview (8 Phases + Intermediate, 7-9 Weeks Total)

### Phase I: Hardware Baseline Establishment ✅ **COMPLETE (9/9 tasks)**
**Goal:** Document complete current state of working Android system  
**Duration:** 2-3 days (COMPLETED)  
**Prerequisites:** Root access (✅ Available)  
**Safety Level:** 🟢 LOW RISK (read-only)

**Completed Tasks:**
1. ✅ Complete system dump and backup
2. ✅ Document all running drivers and kernel modules
3. ✅ Extract calibration data and hardware configuration
4. ✅ Map hardware component addresses and GPIO pins
5. ✅ Document boot process via kernel logs
6. ✅ **Privacy audit:** Identify all spyware/telemetry components
7. ✅ Device Tree analysis (sun50i-h713-hy300.dtb)
8. ✅ Recovery capability documentation (UART FEL mode)
9. ✅ Phase I comprehensive summary

**Result:** ✅ GATE PASSED → Ready for Phase 1.5

---

### Phase 1.5: sunxi-tools Implementation ⏳ **NEXT**
**Goal:** Build and configure sunxi-tools for FEL mode recovery  
**Duration:** 2-3 days (starting soon)  
**Prerequisites:** Phase I complete (✅), CP2102 available (🔄)  
**Safety Level:** 🟢 LOW RISK (testing only, no hardware writes yet)

**Key Tasks:**
1. Build sunxi-tools from source for H713 support
2. Test FEL mode detection with connected device
3. Verify device enters FEL mode via USB
4. Create FEL boot scripts and recovery procedures
5. Test safe SRAM loading without eMMC modification
6. Document recovery workflow for all phases ahead

**Dependencies:** Awaiting CP2102 UART adapter (in transit)  
**Outputs:** FEL tools ready for Phase II bootloader testing

---

### Phase II: UART Access & Boot Analysis ⏳ **BLOCKED**
**Goal:** Establish serial console access and bootloader inspection  
**Duration:** 1-2 days  
**Prerequisites:** UART hardware connection (🔄 Upcoming)  
**Safety Level:** 🟢 LOW RISK (read-only)

**Key Tasks:**
1. UART pinout identification and connection
2. U-Boot environment extraction
3. Boot sequence documentation
4. Bootloader command testing
5. **Recovery procedure validation**

### Phase III: U-Boot Replacement
**Goal:** Replace factory bootloader with mainline U-Boot  
**Duration:** 3-4 days  
**Prerequisites:** UART access, complete hardware baseline  
**Safety Level:** 🔴 HIGH RISK (bootloader modification)

**Key Tasks:**
1. Mainline U-Boot configuration
2. **A/B testing on Device B first**
3. Safe bootloader replacement via UART
4. Boot validation with factory kernel
5. Emergency recovery procedures

### Phase IV: Mainline Kernel Bootstrap
**Goal:** Boot mainline Linux with minimal hardware support  
**Duration:** 1 week  
**Prerequisites:** Working U-Boot, complete device tree  
**Safety Level:** 🟡 MEDIUM RISK (system boot changes)

**Key Tasks:**
1. Minimal device tree creation from baseline
2. Essential driver identification
3. First mainline kernel boot
4. Serial console validation

### Phase V: Driver Porting
**Goal:** Port essential hardware drivers to mainline kernel  
**Duration:** 2-3 weeks  
**Prerequisites:** Booting mainline kernel, factory driver analysis  
**Safety Level:** 🟢 LOW RISK (driver development)

**Key Tasks:**
1. **MIPS co-processor driver** (display.bin integration)
2. **AV1 hardware decoder driver** (premium feature)
3. Display pipeline and GPU drivers
4. Input device drivers (IR, motor, accelerometers)
5. **AIC8800 WiFi/Bluetooth** (community drivers)

### Phase VI: Armbian Custom ROM Integration
**Goal:** Build privacy-focused Armbian system  
**Duration:** 1-2 weeks  
**Prerequisites:** All essential drivers working  
**Safety Level:** 🟡 MEDIUM RISK (system replacement)

**Key Tasks:**
1. **Armbian build system setup** (H713/H6 base)
2. Driver packaging as DKMS modules
3. Board support package creation
4. System services implementation
5. **A/B deployment testing**

### Phase VII: Privacy Hardening & Spyware Removal
**Goal:** Remove all spyware, telemetry, and malware  
**Duration:** 3-4 days  
**Prerequisites:** Armbian booting successfully  
**Safety Level:** 🟢 LOW RISK (software configuration)

**Key Tasks:**
1. **Factory firmware privacy audit** (from Phase I)
2. **Spyware component removal verification**
3. Network privacy configuration (firewall, DNS blocking)
4. System hardening (minimal attack surface)
5. **Offline operation validation**

### Phase VIII: System Validation & Final Testing
**Goal:** Comprehensive testing and A/B validation  
**Duration:** 3-4 days  
**Prerequisites:** Privacy hardening complete  
**Safety Level:** 🟢 LOW RISK (testing only)

**Key Tasks:**
1. **A/B deployment** (both devices)
2. Hardware feature validation
3. Performance benchmarking
4. 24-hour stability testing
5. Recovery procedure verification

## ✅ Confirmed Hardware Components

**Source:** `/docs/` directory - validated against factory DTB files  
**Details:** See `CONFIRMED_HARDWARE_FINDINGS.md`

### Critical Discoveries from Previous Research
- 🎬 **Hardware AV1 Decoder** - Google-Allwinner collaboration, complete IOCTL interface
- 🖥️ **MIPS Co-processor** - Display subsystem with display.bin firmware (4KB extracted)
- 🎮 **Mali-Midgard GPU** - Confirmed (previous "Mali-G31" claims corrected)
- 📡 **AIC8800 WiFi/BT** - Community drivers identified and documented
- ⚙️ **Keystone Motor** - 4-phase stepper with limit switch, custom driver created
- 📊 **Dual Accelerometers** - STK8BA58 / KXTJ3-1057 for auto-keystone
- 💾 **eMMC Storage** - 8-bit bus, HS200 support

**Previous Research:** 100+ analysis documents in `/docs/` directory

## Success Criteria

### Phase I Completion Criteria ✅ **COMPLETE**
- [x] Complete filesystem backup created
- [x] All kernel modules documented with parameters
- [x] Hardware register map established
- [x] Calibration data extracted and preserved
- [x] Boot process fully documented
- [x] Hardware component inventory complete
- [x] **Spyware/telemetry audit** complete (10+ threats identified)
- [x] All 9 Phase I tasks completed and validated

### Phase 1.5 Completion Criteria (sunxi-tools) ⏳ **IN PROGRESS**
- [ ] sunxi-tools built for H713 (FEL mode support)
- [ ] FEL mode device detection working
- [ ] Safe SRAM loading procedures documented
- [ ] FEL recovery workflow tested
- [ ] Recovery procedures validated with Device B
- [ ] Ready for Phase II UART bootloader testing

### Project Completion Criteria
- [ ] **Privacy-focused Armbian ROM** boots reliably
- [ ] All essential hardware functional
- [ ] **Zero spyware/telemetry** verified
- [ ] Projector-specific features working
- [ ] System performance acceptable
- [ ] **A/B testing** on both devices successful
- [ ] Complete documentation maintained
- [ ] Recovery procedures validated

## 🆕 Critical Engineering Enhancements (Phase II Ready)

Three new comprehensive documents added to support safe development:

### 1. UART Bootloader Safety Protocol
📄 **File:** `phases/phase2-uart-access/UART_BOOTLOADER_SAFETY_PROTOCOL.md`

Defines **safe bootloader testing via SRAM** without risking device modification. Includes:
- Why SRAM loading is safer than direct flashing (zero bricking risk)
- Complete U-Boot compilation guide for SRAM execution
- sunxi-fel command examples with detailed explanations
- Bootloader validation checkpoints (safety gates)
- Issue diagnosis and recovery procedures
- UART loading alternative methods
- Device Tree analysis for safety

**When to use:** Phase II UART validation, Phase III bootloader replacement planning

---

### 2. Recovery Template - Standard Procedures
📄 **File:** `phases/RECOVERY_TEMPLATE.md`

Provides **reusable recovery procedures** for all phases. Includes:
- Failure scenarios per phase with H713-specific recovery
- ADB connection restoration (lost connections)
- Backup corruption detection and retry
- UART communication troubleshooting
- FEL mode recovery (BROM issues)
- Device brick recovery procedures
- A/B testing fallback strategy
- Backup structure and decision trees

**When to use:** Every phase's "Abort & Recover" section references this

**Phase READMEs updated:** Phase I and Phase II now include recovery procedures

---

### 3. Research Hardware-Validation Mapping
📄 **File:** `phases/research-validation/RESEARCH_MAPPING.md`

Maps **100+ research findings to actual UART evidence**. Includes:
- 10 major hardware discoveries validated against real device
- Theory vs. reality comparison for each finding
- Cross-references to research documents and UART logs
- Validation status (✅ Confirmed / ⚠️ Partial / ❌ Contradicted / 🔄 Pending)
- Implementation impact analysis for each finding
- Integration guidance for all phases

**Validated findings:**
- GPIO voltage rails (⚠️ partial - needs measurement)
- MIPS co-processor (✅ confirmed - safe to implement)
- AV1 decoder hardware (✅ confirmed - Phase V ready)
- AIC8800 WiFi/Bluetooth (✅ confirmed - community drivers)
- Display TCON0 (✅ confirmed - driver needed)
- IR remote control (✅ confirmed - simple GPIO driver)
- Motor/PWM control (✅ confirmed - daemon needed)
- Storage eMMC + SPI-NOR (✅ confirmed - safe layout)
- 2GB DDR3 memory (✅ confirmed - no changes needed)
- Thermal sensor (✅ confirmed - mainline support)

**When to use:** Before implementing any hardware feature, consult this mapping

---

## Risk Mitigation

### Hardware Risks
- **Bricking:** Multiple backup strategies, UART recovery
- **Data Loss:** Complete dumps before modifications
- **Hardware Damage:** Careful voltage/current verification

### Software Risks
- **Driver Incompatibility:** Maintain factory kernel as fallback
- **Configuration Loss:** Preserve all calibration data
- **Boot Failure:** UART-based recovery procedures

## Development Environment

Using Nix flakes for reproducible development:

```bash
# Enter development environment
cd /home/luca/Desktop/sun50iw12p1-research
nix develop

# All cross-compilation tools available
aarch64-unknown-linux-gnu-gcc --version
```

## Task Management

### Using task-manager Tool

```bash
# Check current status
ai/tools/task-manager list

# Start a task
ai/tools/task-manager start <task_id>

# Complete a task
ai/tools/task-manager complete <task_id>

# Get next recommended task
ai/tools/task-manager next
```

### Task Lifecycle
1. **pending** - Defined, ready to start
2. **in_progress** - Currently being worked (one at a time)
3. **completed** - Finished with validation
4. **blocked** - Cannot proceed (document blocker)

## Agent Guidelines

### General Agent Responsibilities
1. **Check for in-progress work:** Always run `ai/tools/task-manager find-inprogress` first
2. **Follow task priorities:** Use `ai/tools/task-manager next` for recommendations
3. **Update task status:** Mark tasks in-progress and completed properly
4. **Document findings:** Update relevant documentation as work progresses
5. **Validate at every step:** Use hardware access to verify assumptions

### Specialized Agents
- **hardware-access-agent:** Root access operations, system dumps
- **uart-agent:** Serial console operations, bootloader interaction
- **driver-analysis-agent:** Kernel module reverse engineering
- **device-tree-agent:** DTB creation and validation
- **integration-agent:** System assembly and testing

### Delegation Protocol
- **Atomic tasks only:** One clear objective per delegation
- **Complete context:** Include all necessary information in prompt
- **Hardware safety:** Always include safety protocols
- **Validation requirements:** Define success criteria clearly

## Quick Start - Phase I

### Step 1: Hardware Access Verification
```bash
# Verify root access
adb shell su -c "id"

# Check kernel version
adb shell uname -a

# Verify storage space for dumps
adb shell df -h
```

### Step 2: Begin System Documentation
```bash
# Start first task
ai/tools/task-manager start 001
```

### Step 3: Create Complete Backup
See `hardware-access/hardware-dump-procedures.md` for detailed procedures.

## Resources

### Previous Project Artifacts (Reference Only)
- Device tree research: `../sun50i-h713-hy300.dts`
- Firmware analysis: `../firmware/`
- Driver experiments: `../drivers/`
- Documentation: `../docs/`

### Hardware Documentation
- H713 SoC datasheet (when available)
- Allwinner H6 reference (similar architecture)
- Factory kernel sources (if extracted)

### External Resources
- Sunxi community wiki
- Allwinner mainline kernel support
- ARM Trusted Firmware documentation

## Notes

- **Leverage previous research:** Use existing analysis as reference, but validate everything with hardware
- **Safety first:** Always maintain backups and recovery paths
- **Document everything:** Hardware access is temporary, documentation is permanent
- **Iterate quickly:** Hardware validation enables rapid iteration

## Research Archive Integration

This project builds upon extensive software analysis completed in the `research/` directory:

**What's in Research:**
- 100+ analysis documents (`research/docs/`)
- Complete firmware extraction (`research/firmware/`)
- Kernel driver implementations (`research/drivers/`)
- U-Boot bootloader binary (`research/u-boot-sunxi-with-spl.bin`)
- Device tree (`research/sun50i-h713-hy300.dts`)
- VM testing framework (`research/nixos/`)

**How to Use Research:**
1. **Read context first:** `ai/contexts/research-integration.md`
2. **Consult research docs** for component background
3. **Extract live hardware data** using procedures in `ai/contexts/live-system-analysis.md`
4. **Compare and validate** research against hardware
5. **Document findings** and update research with hardware truth

**Critical Principle:** Hardware truth always wins. Research provides context, hardware provides facts.

## Next Steps

1. **Read**: `ai/contexts/research-integration.md` - Understand hardware-first approach
2. **Review**: `PROJECT_ROADMAP.md` for complete 8-phase breakdown
3. **Start**: `ai/tools/task-manager next` to begin Phase I
4. **Execute**: `phases/phase1-hardware-baseline/README.md` for first tasks

---

**Last Updated:** November 3, 2025 (Restructuring)  
**Current Phase:** I - Hardware Baseline (Ready to start)  
**Research Archive:** `research/` (sun50iw12p1-research integrated)  
**Next Milestone:** Complete live system extraction and research validation
