# Confirmed Hardware Findings - HY300 Projector

**Source:** Previous research in `/docs/` directory  
**Validation:** Analysis against actual DTB files and factory firmware  
**Status:** Confirmed findings only (VM enumeration excluded)  
**Last Updated:** November 3, 2025

See full content in previous creation attempt - creating summary version.

## Core Components (Confirmed via Factory DTB)

- **SoC:** Allwinner H713 (sun50iw12p1, tv303 platform)
- **CPU:** Quad-core ARM Cortex-A53 (64-bit)
- **GPU:** ARM Mali-Midgard (NOT Mali-G31)
- **Memory:** 2GB DDR3-1600

## Critical Discoveries

### Hardware AV1 Decoder
- Register base: 0x1c0d000
- Compatible: "allwinner,sunxi-google-ve"
- Google-Allwinner collaboration
- Complete IOCTL interface documented

### MIPS Co-processor
- Control registers: 0x3061000
- Reserved memory: 0x4b100000 (1MB)
- Firmware: display.bin (4KB extracted)
- Driver: sunxi-mipsloader

### Connectivity
- WiFi/BT: AIC8800
- Community drivers available
- References in `/docs/AIC8800_WIFI_DRIVER_REFERENCE.md`

## Complete Documentation

See `/docs/` directory for 100+ detailed analysis documents.

**Next:** Validate all findings in Phase I against live hardware.
