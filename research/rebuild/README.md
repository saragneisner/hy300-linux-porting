# HY300 Linux Porting - Complete Rebuild Plan

**Status:** Phase I - Hardware Access & Baseline Establishment  
**Start Date:** November 3, 2025  
**Hardware Access:** ✅ Root access available, 🔄 UART access upcoming  
**End Goal:** 🛡️ Privacy-Focused Armbian Custom ROM

## 🎯 Project Mission

**Replace factory Android firmware (containing spyware, telemetry, and malware) with a privacy-focused custom ROM based on Armbian.**

## Project Overview

Complete rebuild of the HY300 Android projector Linux porting project, leveraging full hardware access to establish a proper baseline and systematic development approach. The end goal is creating a secure, privacy-respecting system based on Armbian (Debian for ARM).

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

```
rebuild/
├── README.md                    # This file
├── PROJECT_ROADMAP.md          # Complete phase breakdown
├── HARDWARE_BASELINE.md        # Current system state documentation
├── phases/                     # Phase-specific documentation
│   ├── phase1-hardware-baseline/
│   ├── phase2-uart-access/
│   ├── phase3-uboot-replacement/
│   ├── phase4-mainline-kernel/
│   ├── phase5-driver-porting/
│   └── phase6-system-integration/
├── tasks/                      # Active task tracking
│   ├── pending/
│   ├── in-progress/
│   └── completed/
├── agents/                     # Agent guidelines and delegation
│   ├── AGENT_GUIDELINES.md
│   ├── task-delegation-protocol.md
│   └── specialized-agents/
└── hardware-access/            # Hardware interaction documentation
    ├── root-access-guide.md
    ├── uart-setup.md
    ├── hardware-dump-procedures.md
    └── safe-testing-protocols.md
```

## Phase Overview (8 Phases, 7-9 Weeks)

### Phase I: Hardware Baseline Establishment 🎯 **CURRENT**
**Goal:** Document complete current state of working Android system  
**Duration:** 2-3 days  
**Prerequisites:** Root access (✅ Available)  
**Safety Level:** 🟢 LOW RISK (read-only)

**Key Tasks:**
1. Complete system dump and backup
2. Document all running drivers and kernel modules
3. Extract calibration data and hardware configuration
4. Map hardware component addresses and GPIO pins
5. Document boot process via kernel logs
6. **Privacy audit:** Identify all spyware/telemetry components

### Phase II: UART Access & Boot Analysis
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

### Phase I Completion Criteria
- [ ] Complete filesystem backup created
- [ ] All kernel modules documented with parameters
- [ ] Hardware register map established
- [ ] Calibration data extracted and preserved
- [ ] Boot process fully documented
- [ ] Hardware component inventory complete
- [ ] **Spyware/telemetry audit** complete

### Project Completion Criteria
- [ ] **Privacy-focused Armbian ROM** boots reliably
- [ ] All essential hardware functional
- [ ] **Zero spyware/telemetry** verified
- [ ] Projector-specific features working
- [ ] System performance acceptable
- [ ] **A/B testing** on both devices successful
- [ ] Complete documentation maintained
- [ ] Recovery procedures validated

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

## Next Steps

1. Review `PROJECT_ROADMAP.md` for complete phase breakdown
2. Read `hardware-access/root-access-guide.md` for access procedures
3. Execute `ai/tools/task-manager next` to start Phase I tasks
4. Begin with `phases/phase1-hardware-baseline/README.md`

---

**Last Updated:** November 3, 2025  
**Current Phase:** I - Hardware Baseline Establishment  
**Next Milestone:** Complete system documentation and backup
