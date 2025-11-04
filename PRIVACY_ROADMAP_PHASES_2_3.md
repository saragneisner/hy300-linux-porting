# Surveillance Removal - Phases II & III

## PHASE II: UART Access + Bootloader Security Analysis

**Objectives:**
- Establish UART console (recovery capability)
- Dump complete factory bootloader
- Analyze for hidden update mechanisms
- Document boot process security

**Critical Tasks:**
1. UART Console Validation
   - Connect via UART serial
   - Verify bootloader doesn't log telemetry
   - Check for suspicious network initialization
   - Document boot sequence

2. Bootloader Deep Analysis
   - Dump U-Boot from eMMC via FEL mode
   - Search for hidden partitions
   - Look for pre-boot network code
   - Check for device fingerprinting
   - Compare with mainline U-Boot

**Privacy Blockers:**
- If bootloader has update mechanism → cannot proceed until removed
- UART must work before any bootloader changes

---

## PHASE III: Bootloader Replacement + Security Lock

**Objectives:**
- Replace U-Boot with mainline (no backdoors)
- Disable unsafe boot modes
- Enable secure boot if possible
- Lock bootloader against downgrades

**Critical Tasks:**
1. Select Mainline U-Boot
   - Version 2023.x or later recommended
   - Verify H713 support in mainline
   - Check CVE history for security issues

2. Configure for Privacy
   - Disable network boot
   - Disable bootm from network
   - Disable debug UART commands
   - Remove telemetry code

3. Secure Implementation
   - Build SPL with verification
   - Test via SRAM before eMMC flash
   - Create signing keys for future updates
   - Lock bootloader to prevent downgrades

**Privacy Guarantees After This Phase:**
- ✅ No remote firmware updates possible
- ✅ No bootloader-level telemetry
- ✅ Surveillance cannot be re-enabled at boot time
- ✅ Recovery via UART still available (security vs. recovery tradeoff)

**This phase is CRITICAL security foundation**