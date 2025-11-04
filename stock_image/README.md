# Stock Image - Factory Android Baseline

**Purpose:** Archive of unpacked FEX files from factory HY300 Android image  
**Status:** Phase I Hardware Baseline  
**Created:** November 4, 2025

---

## Directory Structure

```
stock_image/
├── README.md                 # This file
├── sys_config.fex           # System configuration
├── sys_partition.fex        # Partition layout
├── [other fex files]        # Boot binaries and resources
│
├── ANALYSIS/                # ✅ Task 002 Extraction Results
│   ├── fex-extraction-summary.md      # Complete analysis summary
│   ├── dram-parameters.txt            # DRAM clock & timing config
│   ├── boot-config.txt                # eMMC/SD controller config
│   ├── uart-config.txt                # UART0 debug pins
│   ├── i2c-config.txt                 # I2C2 configuration
│   ├── partition-layout.txt           # Full partition table
│   ├── sys-config-full.txt            # Original sys_config.fex copy
│   └── gpio-configuration.txt         # GPIO pin mappings
│
└── METADATA/                # Analysis Metadata
    ├── fex-sections.txt     # All sections found in sys_config.fex
    └── boot-fex-sections.txt # Boot-level FEX analysis
```

---

## Phase I Analysis Status

### Task 002: FEX Image Analysis ✅ **COMPLETED**

**Extraction Date:** November 4, 2025

**Findings:**
- ✅ 7 configuration sections identified in sys_config.fex
- ✅ DRAM: DDR3 @ 624 MHz (1248 MT/s equivalent)
- ✅ Boot: Dual SD/eMMC with A/B redundancy
- ✅ UART: Debug console on UART0 (PH00/PH01)
- ✅ Storage: eMMC 4.6GB primary, SD secondary
- ✅ I2C2: Enabled for peripheral devices

**Key Files:**
- See `ANALYSIS/fex-extraction-summary.md` for complete analysis
- See `ANALYSIS/partition-layout.txt` for firmware layout
- Regional or hardware variant settings
- Custom calibration per device batch
- Feature flags

---

## Extraction Process

### Steps:
1. Extract factory Android image (`.img` file)
2. Locate `script.bin` in boot partition or root
3. Unpack `script.bin` using FEX tools to generate text `.fex` files
4. Place unpacked FEX files here
5. Run analysis tools (see below)

### Tools:
```bash
# Extract script.bin to FEX format
sunxi-tools-fex-unpack script.bin sys_config.fex

# View FEX structure
cat sys_config.fex | head -50

# Parse specific sections
grep -A 20 "\[dram_para\]" sys_config.fex
```

---

## Analysis Integration

### Automated Extraction (from FEX files):
```bash
# Extract DRAM parameters
grep -A 50 "dram_para" sys_config.fex > ANALYSIS/dram-parameters.txt

# Extract GPIO definitions
grep -A 100 "gpio_para" sys_config.fex > ANALYSIS/gpio-definitions.txt

# Extract display settings
grep -A 50 "lcd_para" sys_config.fex > ANALYSIS/display-settings.txt
```

### Cross-Reference with Existing Research:
- Compare extracted DRAM parameters with `research/docs/DRAM_ANALYSIS.md`
- Validate GPIO mappings against `research/sun50i-h713-hy300.dts`
- Check display settings against factory kernel parameters

---

## Hardware Baseline Data

### DRAM Parameters (extracted)
```
[Status: PENDING - awaiting FEX extraction]
```

### GPIO Configuration (extracted)
```
[Status: PENDING - awaiting FEX extraction]
```

### Display Settings (extracted)
```
[Status: PENDING - awaiting FEX extraction]
```

### Calibration Data (extracted)
```
[Status: PENDING - awaiting FEX extraction]
```

---

## Phase I Integration

### Task 001: Root Access Verification
- [ ] Document available storage for backups
- [ ] Identify location of factory firmware on Device A
- [ ] Plan extraction path for FEX files

### Task 004: Register Map & Hardware Addresses
- [ ] Extract hardware addresses from FEX `[hardware_para]` section
- [ ] Map GPIO pins to physical connectors
- [ ] Document I2C/SPI device addresses
- [ ] Cross-reference with DTB findings

### Task 008: Phase I Summary
- [ ] All FEX data extracted and analyzed
- [ ] Findings documented in ANALYSIS/
- [ ] Cross-references validated with RESEARCH_MAPPING.md

---

## Key Findings Reference

### From FEX Analysis:
| Component | Parameter | Value | Status |
|-----------|-----------|-------|--------|
| DRAM | Data Rate | [extracting] | 🔄 PENDING |
| DRAM | Timing | [extracting] | 🔄 PENDING |
| GPIO | Pin Mapping | [extracting] | 🔄 PENDING |
| Display | TCON Config | [extracting] | 🔄 PENDING |
| Thermal | Fan PWM | [extracting] | 🔄 PENDING |

---

## Related Documentation

- **Phase I Baseline:** `phases/phase1-hardware-baseline/README.md`
- **Hardware Research:** `research/docs/HY300_HARDWARE_ENABLEMENT_STATUS.md`
- **DRAM Analysis:** `research/docs/DRAM_ANALYSIS.md`
- **Device Tree:** `research/sun50i-h713-hy300.dts`
- **Research Mapping:** `phases/research-validation/RESEARCH_MAPPING.md`

---

## Status Tracking

**Extraction Progress:**
- [ ] FEX files copied to `stock_image/`
- [ ] Initial analysis run
- [ ] DRAM parameters extracted and analyzed
- [ ] GPIO configuration documented
- [ ] Display settings validated
- [ ] Calibration data extracted
- [ ] All findings cross-referenced with research
- [ ] Phase I Task 008 completed

**Current:** ⏳ Waiting for FEX files to be placed in this directory

