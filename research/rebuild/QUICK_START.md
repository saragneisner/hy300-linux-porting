# HY300 Rebuild Project - Quick Start Guide

**Project Status:** Phase I Active - Ready to Begin  
**Date:** November 3, 2025  
**Next Action:** Execute Task 001

## What Is This?

Complete rebuild plan for porting mainline Linux to the HY300 Android projector, leveraging **hardware access** (root + UART) for a proper baseline-first approach.

## Key Differences from Previous Approach

| Previous Project | This Rebuild |
|-----------------|--------------|
| Research-first (no hardware) | Hardware-first (root access) |
| Assumptions from firmware | Live validation |
| FEL mode complications | Direct system access |
| Limited validation | Complete baseline |

## Current Status

✅ **Available Now:**
- Root access to running Android device
- Full system dump capability
- Live hardware inspection

🔄 **Coming Soon:**
- UART serial console access
- Bootloader-level access
- Early boot debugging

## Project Structure

```
rebuild/
├── README.md                  ← Project overview
├── PROJECT_ROADMAP.md         ← Complete 6-phase plan
├── agents/
│   └── AGENT_GUIDELINES.md    ← AI agent instructions
├── phases/
│   └── phase1-hardware-baseline/
│       └── README.md          ← Current phase details
├── tasks/
│   ├── pending/
│   │   └── 001-root-access-verification.md  ← Start here
│   ├── in-progress/
│   └── completed/
└── hardware-access/
    └── root-access-guide.md   ← How to access device
```

## Getting Started (3 Steps)

### Step 1: Enter Development Environment

```bash
cd /home/luca/Desktop/sun50iw12p1-research

# Enter Nix development shell (auto with direnv)
nix develop

# Verify tools available
adb --version
aarch64-unknown-linux-gnu-gcc --version
```

### Step 2: Review Current Phase

```bash
# Read Phase I overview
cat rebuild/phases/phase1-hardware-baseline/README.md

# Read first task
cat rebuild/tasks/pending/001-root-access-verification.md

# Check hardware access guide
cat rebuild/hardware-access/root-access-guide.md
```

### Step 3: Start Task 001

```bash
# Mark task as in-progress
ai/tools/task-manager start 001

# Follow instructions in:
# rebuild/tasks/pending/001-root-access-verification.md

# When complete:
ai/tools/task-manager complete 001

# Get next task:
ai/tools/task-manager next
```

## Phase I Overview (Current)

**Goal:** Establish complete hardware baseline  
**Duration:** 2-3 days  
**Output:** Complete system backup and documentation

### Tasks (8 total)
1. ✅ Root Access Verification (30 min) - **START HERE**
2. ⏸️ Complete System Dump (2-3 hours)
3. ⏸️ Kernel Module Documentation (2-3 hours)
4. ⏸️ Hardware Register Mapping (4-6 hours)
5. ⏸️ Calibration Data Extraction (2-3 hours)
6. ⏸️ Boot Process Analysis (2-3 hours)
7. ⏸️ Network Configuration Baseline (1-2 hours)
8. ⏸️ Phase I Summary and Validation (2-3 hours)

## Quick Command Reference

### Task Management
```bash
ai/tools/task-manager find-inprogress  # Check current task
ai/tools/task-manager next             # Get next task
ai/tools/task-manager start 001        # Start task 001
ai/tools/task-manager complete 001     # Complete task 001
```

### Hardware Access
```bash
# Check device connection
adb devices

# Root shell
adb shell
su

# Direct root command
adb shell su -c "command"

# Example: Get device info
adb shell su -c "cat /proc/device-tree/model"
```

### Git Workflow
```bash
git status
git add rebuild/
git commit -m "[Phase I] Description"
git push
```

## Phase Roadmap Overview

```
✅ Phase I:   Hardware Baseline       [2-3 days]  ← YOU ARE HERE
⏸️ Phase II:  UART Access             [1-2 days]  
⏸️ Phase III: U-Boot Replacement      [3-4 days]
⏸️ Phase IV:  Mainline Kernel         [1 week]
⏸️ Phase V:   Driver Porting          [2-3 weeks]
⏸️ Phase VI:  System Integration      [1-2 weeks]

Total: 6-8 weeks
```

## Safety Protocols

### Always Follow
- ✅ Complete backups before modifications
- ✅ Read-only operations in Phase I
- ✅ Document everything
- ✅ Verify before executing
- ✅ Use task-manager workflow

### Never Do
- 🛑 Modify system without backup
- 🛑 Skip task documentation
- 🛑 Work on multiple tasks simultaneously
- 🛑 Ignore validation procedures

## Key Documents

### Must Read
1. `rebuild/README.md` - Project overview
2. `rebuild/PROJECT_ROADMAP.md` - Complete plan
3. `rebuild/phases/phase1-hardware-baseline/README.md` - Current phase
4. `rebuild/tasks/pending/001-root-access-verification.md` - First task

### Reference
- `rebuild/agents/AGENT_GUIDELINES.md` - For AI agents
- `rebuild/hardware-access/root-access-guide.md` - Access procedures
- Previous research in `../docs/` - For reference only

## Common Questions

### Q: How is this different from the previous work?
**A:** Previous project was research-first without hardware. This rebuild uses live hardware access to establish proper baseline before any modifications.

### Q: What about all the previous research?
**A:** It's valuable reference material in `../docs/`, but we validate everything with live hardware now.

### Q: When do we need UART access?
**A:** Phase II (coming soon). Phase I uses only root ADB access.

### Q: Can I use the previous device tree?
**A:** As reference only. We'll create new one based on live hardware baseline.

### Q: How long will this take?
**A:** ~6-8 weeks for complete working system. Phase I alone is 2-3 days.

## Next Actions

1. **Review documents** listed above
2. **Execute Task 001** following the task file
3. **Update task status** using task-manager
4. **Proceed systematically** through Phase I tasks

## Support Resources

### Development Environment
- Nix flakes for reproducible builds
- Cross-compilation toolchain
- ADB and hardware access tools
- Analysis utilities

### Previous Project Artifacts
- Device tree research: `../sun50i-h713-hy300.dts`
- Firmware analysis: `../firmware/`
- Driver experiments: `../drivers/`
- Comprehensive documentation: `../docs/`

### External Resources
- Sunxi community wiki
- Linux kernel documentation
- Allwinner H6/H713 references

## Status Tracking

### Phase I Progress
- [ ] Task 001: Root Access Verification
- [ ] Task 002: Complete System Dump
- [ ] Task 003: Kernel Module Documentation
- [ ] Task 004: Hardware Register Mapping
- [ ] Task 005: Calibration Data Extraction
- [ ] Task 006: Boot Process Analysis
- [ ] Task 007: Network Configuration Baseline
- [ ] Task 008: Phase I Summary and Validation

### Overall Project
- 🎯 **Phase I:** Active (0% complete)
- ⏸️ **Phase II:** Waiting for UART
- ⏸️ **Phases III-VI:** Sequential

---

## Ready to Start?

```bash
# 1. Read the first task
cat rebuild/tasks/pending/001-root-access-verification.md

# 2. Start the task
ai/tools/task-manager start 001

# 3. Connect device and verify access
adb devices
adb shell su -c "id"

# 4. Follow task instructions
# 5. Complete and move to next task
```

**Good luck! 🚀**

---

**Created:** November 3, 2025  
**Last Updated:** November 3, 2025  
**Project Phase:** I - Hardware Baseline Establishment  
**Current Task:** 001 - Root Access Verification
