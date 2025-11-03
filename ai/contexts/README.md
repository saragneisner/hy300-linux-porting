# AI Context Files

This directory contains context information to help AI agents complete tasks effectively.

## Project Philosophy: Hardware-First Approach

This project follows a **hardware-first development methodology**:
- Live hardware analysis provides ground truth
- Software research (in `../../research/`) provides context and patterns
- Hardware validation drives all decisions

**Key Principle:** Hardware Truth > Software Analysis

## Available Contexts

### Hardware-First Development (CRITICAL)
- `research-integration.md` - **START HERE** - How to use research/ findings with live hardware
- `live-system-analysis.md` - Safe data extraction procedures from running device
- `hardware-safety.md` - Hardware safety protocols and A/B testing
- `hardware.md` - HY300 hardware specifications and components
- `hardware-testing-protocol.md` - FEL mode testing and recovery procedures

### Development Environment
- `development-environment.md` - Development environment setup
- `cross-compilation.md` - Cross-compilation setup and procedures
- `firmware-analysis.md` - Stock firmware analysis procedures

### Bootloader and Recovery
- `uboot-integration.md` - U-Boot development and FEL testing standards
- `sunxi-tools.md` - Sunxi tools usage and FEL mode
- `h713-fel-reverse-engineering.md` - H713 FEL mode technical details

### Driver Integration
- `driver-integration-strategy.md` - Systematic driver integration methodology
- `audio-display-integration.md` - Projector audio/display system integration
- `android-calibration-integration.md` - Factory calibration data integration

### VM Testing (Phase VIII Reference)
- `phase8-vm-testing.md` - Phase VIII overview and objectives
- `vm-testing-validation.md` - VM testing framework and procedures
- `kodi-integration.md` - Kodi integration standards and procedures
- `prometheus-metrics.md` - Metrics collection implementation

### Project Management
- `current-status.md` - Current project status for delegated agents
- `delegation-standards.md` - Agent delegation protocols
- `documentation-standards.md` - Documentation requirements
- `git-standards.md` - Git workflow and commit standards

## Research Archive Integration

The `../../research/` directory contains **100+ software analysis documents** from previous work. These provide valuable context but must be validated against live hardware.

**How to use research:**
1. Read `research-integration.md` first (mandatory)
2. Consult relevant research docs for context
3. Extract live data from hardware (see `live-system-analysis.md`)
4. Compare and validate research findings
5. Document results and update research if needed

**Research Document Map:** See `research-integration.md` for phase-specific research document references.

## Context File Selection Guide

### For Phase I: Hardware Baseline
**Required:**
- `research-integration.md` - How to use research findings
- `live-system-analysis.md` - Data extraction procedures
- `hardware-safety.md` - Safety protocols

**Reference:**
- `../../research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md`
- `../../research/docs/FACTORY_DTB_ANALYSIS.md`
- `../../research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md`

### For Phase II: UART Access
**Required:**
- `hardware-safety.md` - Recovery procedures
- `sunxi-tools.md` - FEL mode usage
- `uboot-integration.md` - U-Boot standards

**Reference:**
- `../../research/docs/SUNXI_TOOLS_H713_SUMMARY.md`
- `../../research/docs/H713_FEL_PROTOCOL_ANALYSIS.md`
- `../../research/docs/USING_H713_FEL_MODE.md`

### For Phase III: Bootloader Testing
**Required:**
- `uboot-integration.md` - U-Boot development
- `hardware-testing-protocol.md` - Testing procedures
- `hardware-safety.md` - A/B testing protocols

**Reference:**
- `../../research/firmware/DRAM_ANALYSIS.md`
- `../../research/configs/hy300_boot_usb_serial.txt`

### For Phase IV: Kernel Bringup
**Required:**
- `hardware-testing-protocol.md` - Kernel testing
- `research-integration.md` - Device tree validation

**Reference:**
- `../../research/sun50i-h713-hy300.dts`
- `../../research/docs/FACTORY_DTB_ANALYSIS.md`
- `../../research/configs/hy300_kernel_defconfig`

### For Phase V: Driver Integration
**Required:**
- `driver-integration-strategy.md` - Integration methodology
- `hardware-testing-protocol.md` - Driver testing
- `research-integration.md` - Driver validation

**Reference:**
- `../../research/docs/DRIVER_PRIORITY_MATRIX.md`
- `../../research/drivers/` - Driver implementations
- `../../research/docs/MIPS_COPROCESSOR_ANALYSIS.md`
- `../../research/docs/AIC8800_WIFI_DRIVER_REFERENCE.md`

### For Phase VI: Armbian Build
**Required:**
- `cross-compilation.md` - Build procedures
- `android-calibration-integration.md` - Calibration data

**Reference:**
- `../../research/configs/hy300_kernel_defconfig`
- `../../research/docs/ANDROID_CONFIG_CALIBRATION_PHASE3.md`

### For Phase VII: Privacy Hardening
**Reference:**
- `../../research/docs/ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md`
- `../../research/docs/ANDROID_FIRMWARE_ANALYSIS_COMPLETE_SUMMARY.md`

### For Phase VIII: System Validation
**Required:**
- `vm-testing-validation.md` - VM testing procedures

**Reference:**
- `../../research/nixos/` - VM framework
- All phase-specific contexts for final validation

### For General Tasks
**For Hardware Work**: `hardware.md`, `hardware-safety.md`, `live-system-analysis.md`
**For Research Integration**: `research-integration.md` (always start here)
**For Agent Delegation**: `delegation-standards.md`, `current-status.md`
**For Documentation**: `documentation-standards.md`, `git-standards.md`

## Quick Start for New Agents

1. **Read `current-status.md`** - Understand project state
2. **Read `research-integration.md`** - Understand hardware-first philosophy
3. **Check task requirements** - What phase/task are you working on?
4. **Select relevant contexts** - Use guide above
5. **Consult research archive** - Use `../../research/docs/` for background
6. **Follow safety protocols** - Always use A/B testing for risky operations

## Critical Reminders

⚠️ **Hardware truth always wins** - If hardware contradicts research, hardware is correct  
⚠️ **Safety first** - Never skip A/B testing for destructive operations  
⚠️ **Research is context** - Not gospel, must be validated  
⚠️ **Document everything** - Especially research vs hardware differences  

---

**Last Updated:** November 3, 2025 (Restructuring)  
**Project Root:** `../../` (hy300-linux-porting)  
**Research Archive:** `../../research/` (sun50iw12p1-research)