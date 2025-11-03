# Software Analysis Research Archive

This directory contains the complete `sun50iw12p1-research` repository, which performed extensive software analysis of the HY300 Android system without hardware access.

**Purpose:** Reference material for hardware validation phases  
**Status:** Software analysis complete, hardware validation in progress  
**Usage:** Consult docs during hardware testing to validate findings

## Integration with Main Project

The main project (`../`) follows a **hardware-first approach**:

1. **Extract data from live hardware** - Ground truth from running device
2. **Compare with research findings** in this directory - Context and patterns
3. **Validate or update research** based on hardware truth - Hardware wins
4. **Document integration lessons learned** - Feed findings back

See `../ai/contexts/research-integration.md` for detailed procedures.

## Research Philosophy

This research was conducted through **software analysis only**:
- Factory firmware extraction and reverse engineering
- Device tree analysis from extracted DTB files
- Android system component analysis
- Driver code analysis from factory kernel
- MIPS co-processor firmware disassembly

**Important:** All findings here are **theoretical** until validated against live hardware in the main project phases.

## Directory Structure

### Documentation (`docs/`)
**100+ analysis documents** covering all HY300 components:

- **Hardware Analysis:**
  - `HY300_HARDWARE_ENABLEMENT_STATUS.md` - Component status matrix
  - `HY300_SPECIFIC_HARDWARE.md` - Projector-specific hardware
  - `FACTORY_DTB_ANALYSIS.md` - Device tree analysis
  - `DTB_ANALYSIS_COMPARISON.md` - DTB comparison study

- **Driver Analysis:**
  - `DRIVER_PRIORITY_MATRIX.md` - Driver integration priorities
  - `AIC8800_WIFI_DRIVER_REFERENCE.md` - WiFi driver options
  - `ANDROID_KERNEL_DRIVER_ANALYSIS.md` - Factory kernel drivers
  - `MIPS_COPROCESSOR_ANALYSIS.md` - Display processor protocol

- **System Analysis:**
  - `ANDROID_SYSTEM_COMPREHENSIVE_ANALYSIS.md` - Complete system analysis
  - `ANDROID_FIRMWARE_ANALYSIS_COMPLETE_SUMMARY.md` - Firmware overview
  - `ANDROID_CONFIG_CALIBRATION_PHASE3.md` - Calibration data

- **Hardware Components:**
  - `AV1_HARDWARE_DECODER_ANALYSIS.md` - Hardware video decoder
  - `ACCELEROMETER_GPIO_ANALYSIS.md` - Orientation sensors
  - `ARM_MIPS_COMMUNICATION_PROTOCOL.md` - ARM-MIPS interface

- **Bootloader & FEL:**
  - `SUNXI_TOOLS_H713_SUMMARY.md` - Sunxi-tools H713 support
  - `H713_FEL_PROTOCOL_ANALYSIS.md` - FEL mode issues
  - `USING_H713_FEL_MODE.md` - FEL mode usage guide
  - `FEL_MODE_ANALYSIS.md` - FEL mode technical analysis

### Firmware Components (`firmware/`)
**Extracted firmware from factory ROM:**

- `ROM_ANALYSIS.md` - Complete ROM structure analysis
- `FIRMWARE_COMPONENTS_ANALYSIS.md` - Component breakdown
- `DRAM_ANALYSIS.md` - Memory parameter analysis
- `extracted_components/` - Kernel, initramfs, MIPS firmware
- Various `.bin`, `.dtb` files extracted from factory image

### Drivers (`drivers/`)
**Kernel module implementations (research-based):**

- `misc/` - MIPS communication, keystone motor, metrics
- `media/platform/sunxi/` - HDMI input driver (V4L2)

**Status:** Compiled but not hardware-tested

### Tools (`tools/`)
**Custom analysis utilities:**

- `analyze_boot0.py` - Extract DRAM parameters from boot0.bin
- `compare_dram_params.py` - Compare DRAM configs
- `hex_viewer.py` - Interactive hex viewer

### Build Configs (`configs/`)
**Configuration files for hardware:**

- `hy300_kernel_defconfig` - Kernel configuration
- `hy300_h713_defconfig` - H713 SoC config
- `hy300_boot_usb_serial.txt` - U-Boot environment

### Device Trees
**Mainline device tree implementations:**

- `sun50i-h713-hy300.dts` - Complete mainline device tree (791 lines)
- `sun50i-h713-hy300.dtb` - Compiled device tree blob (10.5KB)
- Various experimental device tree variants

### VM Testing (`nixos/`)
**NixOS VM testing framework:**

- `configuration.nix` - VM configuration
- `services/` - HY300 service implementations
- `packages/` - Custom packages

**Purpose:** Software validation before hardware deployment

### AI Infrastructure (`ai/`)
**AI agent contexts and tools (archived):**

- `contexts/` - Context files for agent delegation
- `tools/` - Task management and automation
- `sessions/` - Session logs

**Note:** Main project uses `../ai/` (migrated and enhanced)

## How to Use This Research

### Before Starting a Hardware Task

1. **Check for relevant analysis:**
   ```bash
   cd research/docs
   grep -r "topic" .
   ```

2. **Read research context:**
   ```bash
   # Example: Before testing HDMI input
   cat docs/ANDROID_KERNEL_DRIVER_ANALYSIS.md
   cat docs/V4L2_IMPLEMENTATION_SUMMARY.md
   ```

3. **Note expected behavior:**
   - What should work according to research
   - Known issues or limitations
   - Configuration requirements

### During Hardware Testing

1. **Extract live data:**
   ```bash
   # From main project root
   adb shell su -c "command" > live-data.txt
   ```

2. **Compare with research:**
   ```bash
   diff live-data.txt research/docs/expected-data.txt
   ```

3. **Document findings:**
   - If matches: Research validated ✅
   - If differs: Document hardware-specific behavior
   - If contradicts: Hardware truth wins, update research

### After Hardware Validation

1. **Update research if needed:**
   ```bash
   cd research/docs
   echo "Hardware validation: [findings]" >> hardware-update.md
   ```

2. **Document integration:**
   ```bash
   cd ../phases/phaseX-name/
   echo "Research used: research/docs/..." >> research-integration.md
   ```

## Research Validation Status

### ✅ Validated Against Hardware
*This section will be populated as hardware testing progresses*

### 🔄 Pending Validation
*Most research findings await hardware testing*

### ❌ Contradicted by Hardware
*Hardware-specific variations will be documented here*

## Key Research Deliverables

### Ready for Hardware Testing

- **U-Boot Bootloader:** `u-boot-sunxi-with-spl.bin` (657.5 KB)
- **Device Tree:** `sun50i-h713-hy300.dts` → `.dtb` (10.5 KB)
- **Kernel Modules:** Complete driver set in `drivers/`
- **Build Configs:** Kernel and U-Boot configurations

### Requires Hardware Validation

- **MIPS Communication:** Protocol documented but untested
- **HDMI Input:** Driver implemented but not validated
- **Keystone Motor:** Driver exists but needs calibration
- **AIC8800 WiFi:** Community drivers identified but not tested
- **Hardware AV1:** IOCTL interface documented but not validated

## Research Contribution Summary

**Total Analysis Documents:** 100+  
**Lines of Driver Code:** ~3,000  
**Firmware Components Extracted:** 15+  
**Device Tree Completeness:** 100% (all hardware components)  
**Build System:** Complete Nix development environment

**Time Invested:** Multiple phases of comprehensive analysis  
**Next Step:** Hardware validation in main project phases

## Contact & Updates

For questions about research findings or to report hardware validation results, update the relevant docs in this directory and reference them in the main project documentation.

**Research Archive Maintained By:** Main project team  
**Last Updated:** November 3, 2025 (Restructuring)
