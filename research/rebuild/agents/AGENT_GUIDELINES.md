# Agent Guidelines for HY300 Rebuild Project

**Version:** 2.0 (Hardware-First Approach)  
**Last Updated:** November 3, 2025

## Project Context

This is a **hardware-first** rebuild of the HY300 Linux porting project. Unlike the previous research-first approach, we have:

- ✅ **Root access** to the running Android device
- ✅ **Full system dump** capability
- 🔄 **UART access** coming soon
- ✅ **Live hardware validation** at every step

## Core Principles

### 1. Hardware-First Development
**Always validate with hardware before making assumptions.**

- Use root access to verify hardware state
- Extract data from running system when possible
- Test incrementally with live hardware
- Document actual hardware behavior

### 2. No Shortcuts Policy (MANDATORY)
**Never mock, stub, or simulate functionality.**

- Implement complete, working solutions
- Run all available tests
- Fix issues rather than disabling tests
- Break complex tasks into smaller atomic pieces

### 3. Safety First
**Never risk bricking the device.**

- Complete backups before any modifications
- Test via UART when possible
- Maintain factory kernel as fallback
- Document recovery procedures

### 4. Evidence-Based Development
**All decisions must be supported by evidence.**

- Reference specific files and line numbers
- Provide concrete examples from hardware
- Document validation procedures
- Cross-reference multiple sources

## Task Management

### Using the task-manager Tool

```bash
# ALWAYS run this first when starting work
ai/tools/task-manager find-inprogress

# Get next priority task
ai/tools/task-manager next

# Start working on a task
ai/tools/task-manager start <task_id>

# Complete a task
ai/tools/task-manager complete <task_id>

# Block a task with reason
ai/tools/task-manager block <task_id> "reason"
```

### Task Workflow

1. **Check for in-progress tasks first** - CRITICAL
2. Continue any in-progress work before starting new tasks
3. Use `next` command to get recommended priorities
4. Mark tasks `in_progress` when starting (ONE AT A TIME)
5. Complete tasks immediately when finished
6. Update task documentation as you work

### Task Status Lifecycle

- **pending** - Ready to start, prerequisites met
- **in_progress** - Currently being worked on (ONLY ONE)
- **completed** - Finished with validation
- **blocked** - Cannot proceed (document blocker clearly)

## Agent Responsibilities

### Before Starting Work

1. **Run** `ai/tools/task-manager find-inprogress`
2. **Read** current project status from `README.md`
3. **Check** phase documentation in `phases/`
4. **Verify** development environment (`echo $IN_NIX_SHELL`)
5. **Review** hardware access status

### During Task Execution

1. **Update task status** using task-manager
2. **Document findings** in real-time
3. **Validate with hardware** whenever possible
4. **Follow safety protocols** strictly
5. **Commit frequently** with clear messages

### When Completing Tasks

1. **Validate success criteria** from task definition
2. **Update documentation** with results
3. **Mark task complete** using task-manager
4. **Prepare next task** prerequisites
5. **Commit all changes** with task reference

## Specialized Agent Roles

### hardware-access-agent
**Responsibilities:**
- Root access operations
- System dumps and backups
- Live hardware inspection
- Data extraction from running system

**Context Required:**
- Current hardware access methods
- Safety protocols
- Backup procedures
- Validation requirements

### uart-agent
**Responsibilities:**
- Serial console operations
- Bootloader interaction
- Early boot monitoring
- U-Boot command execution

**Context Required:**
- UART pinout and configuration
- U-Boot command reference
- Boot sequence documentation
- Recovery procedures

### driver-analysis-agent
**Responsibilities:**
- Factory driver reverse engineering
- Module parameter documentation
- Register mapping analysis
- Mainline driver identification

**Context Required:**
- Factory module sources
- Hardware register maps
- Mainline kernel documentation
- Similar SoC references

### device-tree-agent
**Responsibilities:**
- Device tree creation/modification
- DTB compilation and validation
- Hardware description accuracy
- Cross-reference with baseline

**Context Required:**
- Hardware baseline documentation
- Factory DTB analysis
- Mainline kernel DT bindings
- H6/H713 platform specifics

### integration-agent
**Responsibilities:**
- System assembly
- Component integration testing
- End-to-end validation
- Performance optimization

**Context Required:**
- All phase completion reports
- Hardware capabilities
- Performance requirements
- Integration test procedures

## Delegation Protocol

### When to Delegate

Delegate atomic tasks to specialized agents when:
- Task requires specific domain expertise
- Task is self-contained and clearly defined
- Complete context can be provided
- Task has clear validation criteria

### Atomic Task Requirements

Each delegated task must be:
- **Single objective** - One clear goal
- **Self-contained** - All context provided in prompt
- **Testable** - Clear success criteria
- **Time-bounded** - Reasonable completion estimate

### Required Delegation Context

Always include in delegation prompt:

1. **Project overview** - HY300 rebuild context
2. **Current phase** - Where we are in roadmap
3. **Specific task** - Clear objective with success criteria
4. **Technical standards** - Coding, documentation, safety
5. **File context** - Relevant files and paths
6. **Safety protocols** - Hardware safety, backup requirements
7. **Validation** - How to verify completion
8. **Cross-references** - Related documentation
9. **Tools/environment** - Required tools and commands
10. **Integration** - How task fits into larger work

### Example Delegation Template

```
You are a specialized {role} agent for the HY300 Linux porting rebuild project.

PROJECT CONTEXT:
This is a hardware-first porting project for the HY300 Android projector 
with Allwinner H713 SoC. Current phase: {phase}. We have root access to 
the running device and are establishing a complete hardware baseline.

ATOMIC TASK:
{clear, single objective task description}

SUCCESS CRITERIA:
- {specific, measurable criterion 1}
- {specific, measurable criterion 2}
- {specific, measurable criterion 3}

SAFETY PROTOCOLS:
- Always maintain complete backups
- Document all access methods
- Never modify without backup
- Validate before committing

REQUIRED DELIVERABLES:
- {file 1 with full path}
- {file 2 with full path}
- {documentation with specifics}

VALIDATION PROCEDURE:
{step-by-step validation process}

CONTEXT FILES:
{extract relevant sections - don't just reference files}

TOOLS REQUIRED:
{specific commands and tools needed}

Complete this task following all standards above. 
Maximum time: {time estimate}.
```

### Post-Delegation Verification

After each delegated task:
1. **Verify completion** - Check all deliverables present
2. **Validate results** - Run validation procedures
3. **Check commits** - Ensure proper git commits made
4. **Update tasks** - Mark tasks complete using task-manager
5. **Document** - Ensure findings documented
6. **Prepare next** - Only proceed after clean completion

## Hardware Access Protocols

### Root Access Operations

```bash
# Verify access
adb shell su -c "id"

# Safe system inspection
adb shell su -c "cat /proc/device-tree/model"
adb shell su -c "lsmod"

# Data extraction
adb pull /path/to/file ./backup/

# Never modify without backup
adb pull /system/partition ./backup/
# ... then modify if needed
```

### UART Access Operations

```bash
# Connect to serial console
screen /dev/ttyUSB0 115200

# U-Boot commands (read-only first)
printenv
help
mmc list

# Only after verification
setenv bootcmd "custom command"
saveenv
```

### Safety Checklist

Before any hardware modification:
- [ ] Complete backup exists
- [ ] Validation procedure defined
- [ ] Recovery method documented
- [ ] Test on non-critical component first
- [ ] UART access available for recovery

## File and Code Standards

### Documentation Standards

- **Markdown format** for all documentation
- **Clear structure** with proper headings
- **Evidence-based** with specific references
- **Cross-referenced** with related docs
- **Timestamped** with last update date

### Code Standards

- **Follow kernel coding style** for drivers
- **Complete implementations** - no stubs
- **Comprehensive error handling**
- **Inline documentation** for complex logic
- **Validation tests** where applicable

### Git Standards

```bash
# Commit format
git commit -m "[Task XXX] Brief description

Detailed explanation of changes.
References to related documentation.

Files changed:
- file1.c: Added feature X
- file2.md: Updated with findings"
```

### C File Editing Rules

**CRITICAL: .c files and Edit tool incompatibility**

- **NEVER use Edit tool on .c files** - They exceed tool limits
- **ALWAYS create patches** for C file changes
- **Use bash commands** to generate patches
- **Apply and verify** before committing

Example workflow:
```bash
# Create patch
diff -u original.c modified.c > changes.patch

# Apply patch
patch -p0 < changes.patch

# Verify compilation
make driver.o

# Commit modified file (not patch)
git add modified.c
git commit -m "[Task XXX] Applied changes"
```

## Development Environment

### Nix Environment

```bash
# Enter development shell
cd /home/luca/Desktop/sun50iw12p1-research
nix develop

# Verify environment
echo $IN_NIX_SHELL  # Should output "impure"

# Available tools
aarch64-unknown-linux-gnu-gcc --version
dtc --version
sunxi-fel --version
```

### Cross-Compilation

```bash
# Compile for ARM64
export ARCH=arm64
export CROSS_COMPILE=aarch64-unknown-linux-gnu-

# Build kernel
make defconfig
make Image dtbs modules

# Build U-Boot
make hy300_h713_defconfig
make
```

## Documentation Cross-Reference Map

### For Phase I Work:
- Start: `rebuild/PROJECT_ROADMAP.md`
- Tasks: `rebuild/tasks/`
- Hardware: `rebuild/hardware-access/`
- Previous research: `../docs/` (reference only)

### For Hardware Access:
- Root guide: `rebuild/hardware-access/root-access-guide.md`
- UART setup: `rebuild/hardware-access/uart-setup.md`
- Dump procedures: `rebuild/hardware-access/hardware-dump-procedures.md`
- Safety: `rebuild/hardware-access/safe-testing-protocols.md`

### For Driver Work:
- Analysis: `rebuild/phases/phase5-driver-porting/`
- Previous research: `../docs/` (validation reference)
- Factory modules: Extracted from hardware in Phase I

### For Integration:
- Kodi: `rebuild/phases/phase6-system-integration/`
- Calibration: Extracted in Phase I
- Testing: Phase-specific test procedures

## Emergency Procedures

### If Device Doesn't Boot

1. **Try UART access** - Can boot via U-Boot
2. **FEL recovery mode** - Re-flash bootloader
3. **Factory restore** - From Phase I backups
4. **Document incident** - Update recovery procedures

### If Root Access Lost

1. **Document loss cause**
2. **Attempt ADB recovery**
3. **UART bootloader access**
4. **Factory restore if needed**

### If Data Corruption

1. **Stop immediately**
2. **Verify backup integrity**
3. **Restore from backup**
4. **Document corruption cause**

## Success Metrics

### Task Completion
- All success criteria met
- Validation procedures passed
- Documentation complete
- Changes committed

### Phase Completion
- All phase tasks completed
- Phase summary documentation
- Next phase prerequisites ready
- Hardware state validated

### Project Progress
- Steady forward momentum
- No repeated failures
- Complete audit trail
- Reproducible results

## Quick Reference

### Common Commands

```bash
# Task management
ai/tools/task-manager find-inprogress
ai/tools/task-manager next
ai/tools/task-manager start 001
ai/tools/task-manager complete 001

# Hardware access
adb shell su -c "command"
adb pull /path/to/file ./backup/

# Git workflow
git status
git add file
git commit -m "[Task XXX] Description"
git push

# Build commands
make ARCH=arm64 CROSS_COMPILE=aarch64-unknown-linux-gnu-
```

### Key Files

- `rebuild/README.md` - Project overview
- `rebuild/PROJECT_ROADMAP.md` - Complete roadmap
- `rebuild/AGENT_GUIDELINES.md` - This file
- `rebuild/tasks/` - Task tracking
- `rebuild/phases/` - Phase documentation

### Support Resources

- Previous project analysis: `../docs/`
- Firmware analysis: `../firmware/`
- Device trees: `../sun50i-h713-hy300.dts`
- Tools: `../tools/`

---

**Remember:**
1. Hardware validation first
2. No shortcuts ever
3. Safety protocols mandatory
4. Evidence-based decisions only
5. Complete documentation always

**Last Updated:** November 3, 2025  
**Current Phase:** Phase I - Hardware Baseline Establishment
