# AI Context Files - Navigation & Reference

**Last Updated:** November 4, 2025  
**Purpose:** Central reference for AI agents working on HY300 project  
**Read This First:** Yes - critical for any new agent session

---

## 🎯 QUICK START FOR AI AGENTS (Phase I Complete)

### Essential Reading (In Order)
1. **`01-PROJECT-STATUS.md`** ⭐ START HERE (10 min) - Current phase + blockers
2. **`PROJECT_ROADMAP.md`** (20 min) - Full timeline context
3. **Phase-specific README** (phase-dependent) - Current work details
4. **`RECOVERY_TEMPLATE.md`** (20 min) - Safety procedures MANDATORY

**Total Time to Full Context:** ~50 minutes

---

## 📁 CONTEXT FILES IN THIS DIRECTORY

### 01-PROJECT-STATUS.md ⭐ CRITICAL
**What It Contains:**
- Phase I completion status (9/9 tasks ✅)
- Phase II readiness (CP2102 blocked)
- Hardware specifications summary
- Immediate next steps
- Success metrics by phase
- Handoff protocol

**When to Read:** Every session start (5-10 min refresh)  
**Key Status:** Phase I 100% complete, Phase II blocked on hardware

### 02-HARDWARE-ARCHITECTURE.md (TODO)
Hardware architecture details (SoC, boot chain, memory map)

### 03-PHASE-DEPENDENCIES.md (TODO)
Phase gating requirements + prerequisites

### 04-RECOVERY-PROCEDURES.md (TODO)
Safety procedures and recovery references

### 05-RESEARCH-INTEGRATION.md (TODO)
How to use `research/` findings + RESEARCH_MAPPING

### 06-TASK-MANAGEMENT.md (TODO)
Task workflow + conventions

### 07-SAFETY-PROTOCOLS.md (TODO)
Hardware safety rules + ABSOLUTE constraints

---

## 📚 LEGACY CONTEXT FILES (Still Valid)

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