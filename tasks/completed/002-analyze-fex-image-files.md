---
id: 002
title: Analyze FEX Image Files - Hardware Parameters Extraction
status: completed ✅
priority: CRITICAL
created: 2025-11-04
completed: 2025-11-04
---

# Task 002: Analyze FEX Image Files - Hardware Parameters Extraction

**Objective:** Parse and extract hardware configuration data from unpacked FEX files in `stock_image/` directory

**Duration:** ~1-2 hours (parallel with Task 001)

**Depends On:** Task 001 (root access for validation comparison)

---

## Context for Delegated Agent

### Project Overview
This is a hardware porting project to replace Android factory firmware with privacy-focused Linux on HY300 Android projector (Allwinner H713 SoC). We're using a hardware-first approach where live system analysis is ground truth.

### Task Definition
**ATOMIC TASK:** Parse FEX files from factory image to extract and document hardware parameters, then cross-reference with existing research and UART evidence.

### Critical Files
- **FEX Files:** `/home/luca/Desktop/hy300-linux-porting/stock_image/*.fex`
- **Main Config:** `stock_image/sys_config.fex` (primary source)
- **Variant Config:** `stock_image/sys_partition.fex` (partition layout)
- **Device Tree:** `stock_image/sunxi.dts` (device tree source)
- **Output Destination:** `stock_image/ANALYSIS/` directory
- **Research Reference:** `research/docs/DRAM_ANALYSIS.md`, `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md`
- **Validation Reference:** `phases/research-validation/RESEARCH_MAPPING.md`

### Hardware Context
**Key Components to Extract:**
1. **DRAM Parameters**
   - Data rate, CAS latency, timings
   - Voltage settings
   - Memory size detection

2. **GPIO Configuration**
   - GPIO voltage rails (critical for motor, IR, sensors)
   - Pin mappings for peripheral devices
   - Drive strength and pull-up/down settings

3. **Display Settings**
   - TCON (timing controller) parameters
   - Resolution and refresh rate
   - Pixel clock and timings
   - Color space settings

4. **Thermal Management**
   - Fan PWM configuration
   - Temperature thresholds
   - Thermal sensor I2C address

5. **Peripheral Addresses**
   - I2C device addresses
   - SPI device parameters
   - Motor control pins
   - IR receiver configuration

6. **Boot Configuration**
   - U-Boot environment
   - Bootloader parameters
   - UART console settings

### Success Criteria

**Extraction Complete:**
- [ ] DRAM parameters extracted to `stock_image/ANALYSIS/dram-parameters.txt`
- [ ] GPIO configuration documented in `stock_image/ANALYSIS/gpio-configuration.txt`
- [ ] Display settings extracted to `stock_image/ANALYSIS/display-settings.txt`
- [ ] Peripheral addresses in `stock_image/ANALYSIS/peripheral-addresses.txt`
- [ ] Thermal settings in `stock_image/ANALYSIS/thermal-config.txt`
- [ ] Boot parameters in `stock_image/ANALYSIS/boot-config.txt`

**Analysis Complete:**
- [ ] Create summary document: `stock_image/ANALYSIS/fex-extraction-summary.md`
- [ ] Document any discrepancies from existing research
- [ ] Identify new/unexpected parameters
- [ ] Note variant-specific differences if present

**Validation:**
- [ ] Cross-reference DRAM values with `research/docs/DRAM_ANALYSIS.md`
- [ ] Compare GPIO mappings with `research/sun50i-h713-hy300.dts`
- [ ] Check device addresses against known implementations
- [ ] Document validation results in summary

**Integration:**
- [ ] Update `stock_image/README.md` with extraction status
- [ ] Update `stock_image/ANALYSIS/` table in `stock_image/README.md`
- [ ] Add findings to `phases/research-validation/RESEARCH_MAPPING.md` if new discoveries

### Tools & Commands

```bash
# Parse FEX file structure
grep "^\[" stock_image/sys_config.fex | sort | uniq

# Extract DRAM parameters
grep -A 30 "^\[dram_para\]" stock_image/sys_config.fex

# Extract GPIO configuration
grep -A 50 "^\[gpio_para\]" stock_image/sys_config.fex

# Extract display settings
grep -A 40 "^\[lcd_para\]" stock_image/sys_config.fex
grep -A 40 "^\[lcd\d_para\]" stock_image/sys_config.fex

# Extract I2C/SPI devices
grep -A 20 "^\[twi\|^\[spi" stock_image/sys_config.fex

# Extract thermal settings
grep -A 10 "^\[ths_para\]" stock_image/sys_config.fex

# Extract boot configuration
grep -A 20 "^\[boot_init_para\]\|^\[uart_para\]" stock_image/sys_config.fex

# Create formatted output
cat stock_image/sys_config.fex | awk '/^\[/{section=$0} /[a-z_]*=/{print section ": " $0}' > temp.txt
```

### Safety & Process

**Important Notes:**
- This is read-only analysis - no modifications to device
- FEX files are factory data - extract and preserve as-is
- All discoveries should be documented with specific line references
- Any discrepancies with research should be flagged for review
- No assumptions about hardware - stick to documented values

### Deliverables

**Files to Create:**
1. `stock_image/ANALYSIS/dram-parameters.txt` - Extracted DRAM config
2. `stock_image/ANALYSIS/gpio-configuration.txt` - GPIO pinouts and settings
3. `stock_image/ANALYSIS/display-settings.txt` - Display/TCON parameters
4. `stock_image/ANALYSIS/peripheral-addresses.txt` - Device addresses
5. `stock_image/ANALYSIS/thermal-config.txt` - Thermal management
6. `stock_image/ANALYSIS/boot-config.txt` - Boot parameters
7. `stock_image/ANALYSIS/fex-extraction-summary.md` - Overall summary with analysis
8. `stock_image/METADATA/fex-sections.txt` - List of all sections found in FEX files

**Summary Document Should Include:**
- Overview of extracted parameters
- Comparison with existing research (what matches, what's new)
- Any discrepancies noted
- Key findings that inform Phase II/III planning
- Next steps for validation via UART/ADB

### Integration Points

**After Extraction:**
- Task 001 (Root Access) will compare FEX data with running system via ADB
- Task 004 (Register Map) will use extracted addresses for hardware mapping
- Task 008 (Phase Summary) will synthesize all findings
- Phase II planning will reference DRAM/boot parameters
- RESEARCH_MAPPING.md will be updated with validated findings

---

## Execution Notes

**When Starting:**
1. Create output files in `stock_image/ANALYSIS/`
2. Extract sections systematically (use grep commands above)
3. Format output for readability
4. Add source line numbers when relevant

**During Extraction:**
1. Preserve exact formatting from FEX files
2. Note any unusual or unexpected values
3. Check for multiple variants of settings (variant A vs B)
4. Document default vs custom parameters

**When Finished:**
1. Review summary for completeness
2. Cross-check critical parameters (DRAM, boot, GPIO)
3. Update `stock_image/README.md` status table
4. Commit all analysis files
5. Update task status to completed

---

## References

### FEX File Format
- **Structure:** INI-like format with `[section]` headers and `key=value` pairs
- **Encoding:** ASCII text
- **Common Sections:** `[dram_para]`, `[gpio_para]`, `[lcd_para]`, `[twi]`, `[uart_para]`, `[ths_para]`

### Related Documentation
- **DRAM Analysis:** `research/docs/DRAM_ANALYSIS.md`
- **Hardware Status:** `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md`
- **Device Tree:** `research/sun50i-h713-hy300.dts` (mainline equivalent)
- **Research Mapping:** `phases/research-validation/RESEARCH_MAPPING.md`
- **Phase I Overview:** `phases/phase1-hardware-baseline/README.md`

---

## Status

**Current:** ⏳ Pending delegation to FEX analysis agent

**Expected Timeline:**
- Extraction: 45 minutes
- Analysis: 30 minutes
- Cross-reference: 30 minutes
- Documentation: 15 minutes

