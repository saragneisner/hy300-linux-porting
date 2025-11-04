# Surveillance Removal - Phases VII & VIII

## PHASE VII: Security Hardening + Penetration Testing

**Mandatory Access Control:**
```bash
# SELinux strict mode policies:
- No process can access camera (even if module present)
- No process can create VPN tunnels
- No process can access credential storage
- No process can modify system files
```

**Kernel Hardening:**
- Enable ASLR (address space layout randomization)
- Disable unprivileged eBPF
- Enable seccomp for critical services
- Enable audit logging
- Disable unprivileged user namespaces

**Signed Boot:**
- Create project signing key
- Sign kernel and initramfs
- Verify signatures at boot

**Penetration Testing Scenarios:**
1. Can rooted app enable camera? → FAIL expected (no driver)
2. Can app create VPN tunnel? → FAIL expected (firewall + no driver)
3. Can app modify system? → FAIL expected (SELinux policy)
4. Can app exfiltrate credentials? → FAIL expected (AppArmor isolation)
5. Can kernel exploit work? → CONTAINED expected (SMAC + hardening)

**Result:** System unhackable by realistic attacker (limited to DoS)

---

## PHASE VIII: Comprehensive Privacy Validation

**48-Hour Network Traffic Analysis:**
```bash
tcpdump -i eth0 -w traffic.pcap

Analyze for:
- Connections to *.allwinner.com (FAIL if found)
- Connections to *.realtek.com (FAIL if found)
- Port 500/UDP (IPSec phase 1) → FAIL if found
- Port 1194/UDP (OpenVPN) → FAIL if found
- Suspicious DNS queries → FAIL if found

Expected traffic:
- WiFi access point (DHCP)
- DNS queries (user-initiated)
- HTTPS connections (user-initiated)
- NTP time sync
- Nothing else
```

**Hardware Analysis:**
```bash
# Verify cameras completely inaccessible
ls /dev/video*          # Should be empty or error
dmesg | grep -i camera  # Should be empty
modprobe -l | grep camera # Should be empty

# Verify audio recording disabled
arecord -l              # Should fail
pactl list              # Should show no recording devices

# Verify PPP completely gone
pppoectl status         # Should not exist
which pppoe             # Should not exist
pppd --version          # Should not exist
```

**Privacy Permissions:**
- Camera access: ZERO apps have permission
- Audio recording: ZERO apps have permission
- Location: ZERO apps have access
- Contacts: ZERO apps have access

**Result:** Proven privacy through testing

---

## RELEASE REQUIREMENTS

**Before releasing image:**

1. ✅ Git history clean (commits signed)
2. ✅ All code reviewed (surveillance removal documented)
3. ✅ Build reproducible (deterministic output)
4. ✅ Image signed/checksummed
5. ✅ Privacy validation passed
6. ✅ Documentation complete

**User documentation must include:**
- Surveillance mechanisms removed (list with evidence)
- How to verify privacy yourself (commands to run)
- Firewall rules explained
- Security model explained
- Known limitations
- Future update strategy

---

## TIMELINE & EFFORT

| Phase | Task | Days | Difficulty |
|-------|------|------|------------|
| II    | UART + Analysis | 3-5 | Low |
| III   | Bootloader | 5-7 | Medium |
| IV    | Kernel Build | 7-10 | Low |
| V     | Services | 10-14 | Medium-High |
| VI    | Firewall | 5-7 | Low |
| VII   | Hardening | 7-10 | Medium |
| VIII  | Validation | 10-14 | Medium-High |

**Total: 47-67 days (6-9 weeks)**

**Critical Path:** Phase II (detection) → Phase III (bootloader lock) → Phase VIII (proof)

---

## SUCCESS DEFINITION

**A successful privacy port means:**

1. ✅ **No surveillance hardware possible**
   - Cameras completely disabled
   - Audio recording completely disabled
   - No PPP/VPN capability
   - Verified through kernel config + testing

2. ✅ **No exfiltration possible**
   - Network isolated with firewall
   - Telemetry services removed
   - No backdoor update mechanism
   - Verified through network analysis

3. ✅ **No persistent threats**
   - Bootloader prevents re-infection
   - Signed boot prevents kernel replacement
   - SELinux prevents escalation
   - Verified through penetration testing

4. ✅ **User can verify**
   - Open source tools used
   - No black boxes
   - Simple verification procedures
   - User can audit themselves

5. ✅ **Functionality maintained**
   - Projector works (display functional)
   - WiFi works (network functional)
   - Audio playback works (output functional)
   - Normal projector operation possible

---

## DECISION POINT FOR USER

**This is a MAJOR undertaking.**

Choice 1: **Privacy-First (Recommended for this project)**
- Follow all 8 phases including surveillance removal
- 6-9 week timeline
- Result: Privacy-proven Linux port
- Claim: "This is the only HY300 Linux port that removes factory surveillance"

Choice 2: **Functionality-First (Faster)**
- Skip most privacy hardening (Phases VI-VII)
- Keep firewall but skip hardening
- 3-4 week timeline
- Result: Linux port but not privacy-proven
- Claim: "Factory surveillance infrastructure removed"

**Recommendation:** Choice 1 - This aligns with stated project goal of "privacy-focused Linux port"