# GitHub Copilot Instructions for HY300 Linux Porting

**Project:** Privacy-focused Linux port for HY300 Android projector  
**Hardware:** Allwinner H713 (sun50iw12p1) SoC  
**Approach:** Hardware-first, evidence-based development with zero bricking tolerance  
**Last Updated:** November 4, 2025

---

## 🎯 Project Architecture at a Glance

### The "Big Picture"
This is a **hardware porting project**, not a typical software build. We're replacing Android factory firmware with a privacy-focused Armbian custom ROM. The unique challenge: we can't brick the device, so every modification must be validated on hardware first.

**Key Architectural Decision: Hardware-First + Research Integration**
- **Hardware-First:** Live system analysis from two HY300 devices is ground truth
- **Research Integration:** 100+ analysis documents (`research/`) provide patterns and context
- **Atomic Validation:** Each phase locks down one layer before moving up the stack
- **A/B Testing:** Device A (development), Device B (control) prevents catastrophic failures
- **Recovery Always Enabled:** UART console must work before any risky bootloader changes

### Directory Structure & Purpose
```
hy300-linux-porting/
├── phases/                          # EXECUTION - Phase-based development (I-VIII)
│   ├── phase1-hardware-baseline/   # Phase I: Document current state (ACTIVE)
│   ├── phase2-uart-access/         # Phase II: UART serial console setup
│   ├── phase3-uboot-replacement/   # Phase III: Bootloader swap (HIGH RISK)
│   └── [phase4-8]                  # Kernel, drivers, ROM, security, validation
│
├── tasks/                           # TASK TRACKING - Active work items
│   ├── pending/                    # Not yet started
│   ├── in-progress/                # Currently being worked (ONE ONLY)
│   └── completed/                  # Finished and validated
│
├── ai/                              # AI INFRASTRUCTURE
│   ├── contexts/                   # Context docs (phase integration, research workflow)
│   └── tools/                       # task-manager, context-manager, git-manager
│
├── research/                        # SOFTWARE ANALYSIS ARCHIVE (100+ docs)
│   ├── docs/                       # Firmware analysis, driver reverse engineering
│   ├── firmware/                   # Extracted Android ROM components
│   ├── drivers/                    # Kernel module references from research
│   └── tools/                      # Analysis utilities (firmware parsers, etc.)
│
├── hardware-access/                 # HARDWARE PROCEDURES
│   ├── root-access-guide.md        # ADB/SSH procedures for Device A+B
│   ├── uart-setup.md               # Serial console connection
│   └── hardware-dump-procedures.md # Complete backup procedures
│
└── agents/                          # AGENT GUIDELINES
    └── AGENT_GUIDELINES.md         # Detailed agent protocols and delegation
```

### Critical Files for Agent Productivity
- **`README.md`** (487 lines) - Project status, hardware access, phase overview
- **`PROJECT_ROADMAP.md`** (1096 lines) - All 8 phases, timelines, success criteria
- **`phases/README.md`** (309 lines) - Phase navigation and critical docs index
- **`phases/RECOVERY_TEMPLATE.md`** (719 lines) - Recovery procedures for every scenario
- **`phases/phase2-uart-access/UART_BOOTLOADER_SAFETY_PROTOCOL.md`** (557 lines) - Safe SRAM testing
- **`phases/research-validation/RESEARCH_MAPPING.md`** (576 lines) - Hardware findings validated

---

## 🛡️ Hardware Architecture & Safety Model

### Hardware Stack (Bottom-Up)
```
BOOTLOADER (U-Boot)
  ↑ verified via UART before replacing
  
KERNEL (Mainline Linux)  
  ↑ flashed to eMMC after bootloader proven
  
DRIVERS (H713-specific modules)
  ↑ reverse-engineered from Android or written from datasheet
  
ARMBIAN ROM (Userspace)
  ↑ built with custom privacy patches
```

### A/B Device Testing Strategy
- **Device A:** Primary development, all dangerous changes tested here first
- **Device B:** Factory firmware control device, baseline for comparison
- **Safety Rule:** Never modify Device B until proven safe on Device A
- **Recovery Path:** Any Device A failure → restore from complete backup via UART FEL mode

### The "Zero Bricking" Architecture
1. **Before Phase III (U-Boot swap):** UART must be fully tested and working
2. **SRAM Testing First:** All bootloader changes tested via SRAM before flashing eMMC
3. **Factory Kernel Preserved:** Android kernel remains on device until Phase IV ready
4. **Complete Backups Mandatory:** Every phase has verified backup procedures
5. **Decision Gates:** Major phases require documented validation before proceeding

---

## 🚀 Critical Workflows & Developer Patterns

### Workflow #1: Starting Daily Work
```bash
# ALWAYS FIRST - Check for incomplete work
ai/tools/task-manager find-inprogress

# If task found → continue it
# If nothing found → get next priority
ai/tools/task-manager next

# Start your task (moves to in-progress/)
ai/tools/task-manager start <task_id>
```

**Why This Matters:** The project spans weeks. Tasks might be incomplete from previous sessions. Always resume before starting new work.

### Workflow #2: Adding New Hardware Feature
1. **Check research:** `phases/research-validation/RESEARCH_MAPPING.md`
   - Find validation status (✅ confirmed, ⚠️ partial, ❌ unknown)
   - Read cross-references to firmware evidence
2. **Read phase docs:** `phases/phase[N]-*/README.md`
   - Understand phase objectives and constraints
3. **Create implementation task:** `ai/tools/task-manager create "feature-name"`
   - Clear atomic objective (not "implement driver" but "extract H713 GPIO timeouts from factory kernel")
4. **Document validation plan:** Before coding, document how you'll verify on real hardware

**Example:** Implementing new WiFi driver
- Research: Check `research/docs/AIC8800_WIFI_DRIVER_REFERENCE.md` for firmware evidence
- Phase: Confirm driver porting is Phase V, current phase is Phase I
- Task: Create atomic task "Extract AIC8800 firmware parameters from running kernel"
- Validate: Plan to test driver load on Device A via SSH before flashing to Device B

### Workflow #3: Recovery from Hardware Issues
1. **Consult template:** `phases/RECOVERY_TEMPLATE.md`
   - Find matching phase and failure scenario
   - Follow recovery steps (ADB restore, UART FEL, bootloader reload)
2. **Mark task blocked:** `ai/tools/task-manager block <task_id> "Hardware issue: <description>"`
3. **Document findings:** Update phase README with new recovery procedures if needed
4. **Test recovery:** Verify Device B can still be recovered before trying again on Device A

### Workflow #4: Validating Research Against Hardware
When implementing a feature found in `research/` docs:
1. **Read research doc** in `research/docs/` (identifies firmware location)
2. **Extract from running Device:** Live system analysis confirms feature exists
3. **Cross-check with Device Tree:** `research/sun50i-h713-hy300.dts` shows driver requirements
4. **Update RESEARCH_MAPPING.md:** Mark finding as ✅ confirmed with UART evidence
5. **Proceed with implementation:** Now feature is proven to exist on real hardware

**Example:** AV1 Decoder implementation
- Research: `/research/docs/AV1_HARDWARE_DECODER_ANALYSIS.md` (extracted from firmware)
- Hardware: Run `adb shell dmesg | grep -i av1` on Device A
- DTB: Check `sun50i-h713-hy300.dts` for `allwinner,sunxi-google-ve` device node
- Map: Update `/phases/research-validation/RESEARCH_MAPPING.md` with UART evidence
- Implement: Now you know device exists and have documentation

---

## 🎯 Project-Specific Conventions

### Task Naming & IDs
- **Format:** `###-hyphenated-description.md` (e.g., `001-root-access-verification.md`)
- **ID Sequence:** Continuous across all phases (001-150+, not per-phase)
- **Priority:** Indicated in task file frontmatter (CRITICAL, HIGH, MEDIUM, LOW)
- **Status:** pending → in-progress → completed (only ONE in-progress at a time)

### Documentation Cross-References
Every phase README includes sections:
- **Objectives** - What gets locked down this phase
- **Hardware Requirements** - What access is needed (ADB, UART, FEL)
- **Success Criteria** - How we know phase is complete
- **Abort & Recover** - References to RECOVERY_TEMPLATE.md for failure scenarios
- **Research Integration** - Links to relevant `research/docs/` for feature details

### Hardware Validation Evidence Format
When documenting findings, use this structure (from RESEARCH_MAPPING.md):
```
## Finding #N: [Feature Name]
**Status:** ✅ Confirmed / ⚠️ Partial / ❌ Unknown
**Validation Date:** YYYY-MM-DD
**Research Source:** `research/docs/[filename].md` (line #)
**Hardware Evidence:**
- Device Tree: `sun50i-h713-hy300.dts` (lines #-#)
- UART Log: Extract from `adb shell dmesg`, search for [keyword]
- Live Inspection: `adb shell cat /proc/[path]` shows [evidence]
**Implementation Impact:** [What this means for driver porting]
```

### File Organization Rules
- **One responsibility per file:** Each documentation file covers one phase or one major component
- **Backlinks required:** Documents must link to related files (e.g., recovery procedures link back to specific phase)
- **Archive everything:** Completed task files move to `tasks/completed/` but are never deleted
- **Version in comments:** Major documentation files include "Last Updated" and "Version" metadata

---

## 🔧 Essential Tools & Commands

### Task Management (ai/tools/task-manager)
```bash
find-inprogress          # Check for incomplete work (RUN FIRST)
next                     # Get highest priority task
list                     # Show all tasks with status
active                   # Show only pending + in-progress + blocked
start <id>               # Mark task in-progress, move to in-progress/
complete <id>            # Mark task completed, move to completed/
block <id> <reason>      # Mark blocked (with documented reason)
status <id>              # Show detailed task info
validate                 # Check all task files for format issues
```

### Phase Progression Commands
```bash
# At phase transition points:
git log --oneline phases/   # Review what changed in phase docs
ai/tools/task-manager complete <last_phase_task>
ai/tools/task-manager next  # Should suggest Phase N+1 first task

# Before risky changes (Phase III+):
phases/RECOVERY_TEMPLATE.md  # Review recovery procedures
ai/tools/task-manager block <task_id> "Awaiting UART validation"
```

### Hardware Access (documented in hardware-access/)
```bash
# Common Device A operations (via SSH or ADB):
adb shell su -c "id"                    # Verify root access
adb shell dmesg | tail -50              # Check kernel logs for hardware
adb shell cat /proc/device-tree/[path]  # Read device tree values
adb shell df -h                         # Check available space
adb pull /system /local/backup/         # Full filesystem backup

# UART operations (Phase II+):
minicom -D /dev/ttyUSB0 -b 115200       # Connect to serial console
# Use sunxi-fel for recovery (FEL mode)
```

---

## ⚠️ Critical "Don't Do This" Patterns

### ❌ Never Violate Hardware Safety
- Don't skip UART validation before Phase III (bootloader replacement)
- Don't modify Device B until proven safe on Device A
- Don't flash bootloader changes directly to eMMC (test via SRAM first per UART_BOOTLOADER_SAFETY_PROTOCOL.md)
- Don't proceed past Phase III without working recovery (UART FEL must work)

### ❌ Never Ignore Task Management
- Don't combine multiple unrelated work items into one task
- Don't update task status manually via file edit (use task-manager tool)
- Don't have multiple tasks in in-progress state
- Don't skip running find-inprogress before starting new work

### ❌ Never Skip Research Integration
- Don't implement features without checking RESEARCH_MAPPING.md first
- Don't make assumptions about hardware without live validation
- Don't ignore contradictions between research docs and actual hardware behavior
- Don't forget to cross-reference new findings back into RESEARCH_MAPPING.md

### ❌ Never Modify Code Directly for Large Files
- All `.c` files in `research/` exceed edit tool limits
- Use bash patch application for modifications: `patch < file.patch`
- Never attempt to edit `.c` files directly (will crash session)
- Always create patch files, apply, test, commit (skip committing patch file)

### ❌ CRITICAL: Never Execute Write Operations on Hardware Without Explicit User Confirmation
**THIS IS AN ABSOLUTE SAFETY RULE - NO EXCEPTIONS**

- **NEVER** write to device via `adb` without user confirmation
- **NEVER** send commands via `uart`/serial console without user confirmation
- **NEVER** use `sunxi-fel` to write/flash without user confirmation
- **NEVER** use `fastboot flash` without user confirmation
- **NEVER** perform any firmware/bootloader modifications without explicit user approval

**What "Write Operations" Means:**
- ✅ SAFE: Reading (`adb shell cat`, `dmesg`, `dmesg | grep`)
- ✅ SAFE: Listing (`adb shell ls`, `adb shell ps`)
- ✅ SAFE: Querying (`adb shell getprop`, device tree reads)
- ❌ DANGEROUS: `adb push`, `adb reboot`, `adb shell su -c "write"`, `fastboot flash`
- ❌ DANGEROUS: Serial console writes, FEL mode writes, bootloader flashing
- ❌ DANGEROUS: Any modification to `/system`, `/data`, bootloader, or kernel

**Protocol When Dangerous Operation is Needed:**
1. **STOP** - do not execute the command
2. **DOCUMENT** - explain exactly what you want to do and why
3. **WAIT** - for explicit user confirmation with specific command text
4. **EXECUTE** - only after user says "yes, execute: [exact command]"
5. **REPORT** - document the result with timestamps

**Example - CORRECT Workflow:**
```
Agent: "I need to flash the new U-Boot to Device A via sunxi-fel.
The command would be: sunxi-fel uboot u-boot-sunxi-with-spl.bin

Please confirm if I should proceed."

User: "Yes, execute: sunxi-fel uboot u-boot-sunxi-with-spl.bin"

Agent: [executes command]
```

**Example - WRONG Workflow:**
```
Agent: [executes] sunxi-fel uboot u-boot-sunxi-with-spl.bin
[ERROR: Device is now in unknown state]
```

---

## 📚 Essential Reading for Different Roles

### For Phase I (Hardware Baseline) Work
1. Read: `README.md` (project overview)
2. Read: `PROJECT_ROADMAP.md` Phase I section
3. Read: `phases/phase1-hardware-baseline/README.md` (complete)
4. Consult: `hardware-access/` directory for access procedures
5. Use: `ai/tools/task-manager` to find and start Phase I tasks

### For Phase II+ (UART & Bootloader) Work
1. Read: `phases/phase2-uart-access/UART_BOOTLOADER_SAFETY_PROTOCOL.md` (ENTIRE FILE)
2. Read: `phases/RECOVERY_TEMPLATE.md` Phase II section
3. Understand: A/B device testing strategy in PROJECT_ROADMAP.md
4. Before flashing: Review success criteria in relevant phase README
5. On failure: Jump to RECOVERY_TEMPLATE.md, follow procedures exactly

### For Driver Implementation (Phase V+)
1. Research: `phases/research-validation/RESEARCH_MAPPING.md` for your feature
2. Analyze: Relevant files in `research/docs/` and `research/firmware/`
3. Compare: Factory device tree vs mainline requirements
4. Validate: Live system testing before integration
5. Integrate: Follow patterns from previous driver implementations in `research/drivers/`

### For Recovery & Troubleshooting
1. First: `phases/RECOVERY_TEMPLATE.md` (find matching phase + failure type)
2. Second: Phase-specific README (Abort & Recover section)
3. Third: Hardware procedures in `hardware-access/` (connection setup, FEL mode)
4. Fourth: Task documentation for context on what was being attempted

---

## 🔄 Working with AI Tools

### For Delegated Tasks (Specialized Agents)
When an AI agent is delegated work, the delegating agent must provide:
1. **Complete task context** (cannot access project files)
2. **Safety protocols** (hardware-first, recovery requirements)
3. **Specific success criteria** (how validation happens)
4. **File paths** (exact locations of resources needed)
5. **Integration points** (how results feed back into other work)

### Context Files to Reference
- `ai/contexts/` contains specialized contexts for:
  - Research integration workflow
  - Hardware access procedures  
  - Phase transition gates
  - Recovery procedures
  - Driver porting patterns

### Tool Integration Pattern
```
General Agent (you)
  ↓ delegates atomic task with complete context
Specialized Agent (delegated)
  ↓ completes task, returns results
General Agent (you)
  ↓ integrates results, updates documentation/tasks
  ↓ proceeds to next atomic task or phase gate
```

---

## ✅ Success Indicators & Validation

### Phase Completion Checklist
- [ ] All phase tasks completed (moved to `tasks/completed/`)
- [ ] Phase README updated with findings and next steps  
- [ ] Recovery procedures tested on Device B (if applicable)
- [ ] New hardware findings documented in RESEARCH_MAPPING.md
- [ ] Cross-references to other phases updated
- [ ] git history shows clean commits with task IDs

### Task Completion Criteria
- [ ] Deliverables documented (specific files or measurements)
- [ ] Hardware validation performed (not theoretical)
- [ ] Device B tested before Device A proceeds (if high-risk)
- [ ] Related documentation updated
- [ ] Recovery procedures validated (for risky changes)
- [ ] Status marked completed via task-manager (not manual edit)

### When to Escalate
- Device won't boot after changes → RECOVERY_TEMPLATE.md Phase III section
- Unexpected hardware behavior → Create research task, don't assume
- UART not responding → Review hardware-access/uart-setup.md
- FEL mode failing → sunxi-tools documentation, but Device B fallback available

