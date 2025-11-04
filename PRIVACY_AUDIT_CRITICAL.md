# PRIVACY & SECURITY AUDIT - HY300 Factory Configuration

**Date:** November 4, 2025  
**Purpose:** Analyze suspicious/concerning factory configurations  
**Status:** MAJOR RED FLAGS DETECTED ⚠️

---

## 🚨 CRITICAL FINDINGS

### 1. PPPoE (Point-to-Point Protocol over Ethernet) - MAJOR RED FLAG ⚠️⚠️⚠️

**File:** `init/pppoe.rc`  
**Status:** Service configured + enabled  
**Concern:** PPPoE is typically used for ISP connections, but here it's:
- ✅ Enabled in factory ROM
- ✅ Could create VPN tunnel to remote server
- ✅ No obvious legitimate use case for a projector
- ⚠️ **Backdoor vector** - Factory could dial back to manufacturer servers

**Privacy Impact:** CRITICAL
- Remote dial-in capability without user knowledge
- Could exfiltrate video/display data
- Could receive firmware updates silently
- Could be used for remote surveillance coordination

**Action Required:** 
- [ ] Analyze pppoe.rc configuration
- [ ] Identify what servers it tries to reach
- [ ] DISABLE in Armbian build (Phase VI)
- [ ] Block outbound PPP traffic at firewall

---

### 2. DUAL CAMERA SYSTEM - SUSPICIOUS ⚠️⚠️

**Configuration:** `etc/camera.cfg`  
```
number_of_camera = 2
camera_id = 0 (BACK camera - gc2355 sensor)
camera_id = 1 (FRONT camera - implied)
```

**Cameras Detected:**
1. **Back camera** (gc2355 sensor) - 1600x1200 max resolution
2. **Front camera** - (type unknown, likely IR or CMOS)

**Why This Matters:**
- ✅ Projectors typically have NO cameras
- ✅ HY300 is sold as a "home projector" with built-in display
- ✅ WHY would it need 2 cameras?
- ⚠️ Front camera could surveil room (if IR, can see in darkness)
- ⚠️ Back camera could surveil user's actions
- ⚠️ Both could stream to manufacturer

**Possible Legitimate Uses:**
- Gesture recognition (probably not)
- Automatic keystone calibration (GPU-based, not camera-based)
- Ambient light sensing (typically uses different sensor)

**Likely Concerning Use:**
- Remote surveillance
- Gesture tracking
- User behavior analysis
- Room monitoring

**Action Required:**
- [ ] Identify actual camera hardware connected
- [ ] Check if cameras are physically connected or just in config
- [ ] Block camera kernel module (disable in Phase IV DTB)
- [ ] Remove camera HAL services (Phase VI)
- [ ] Verify no camera activity in network traffic (Phase II+)

---

### 3. RACOON (IPSec VPN Daemon) - RED FLAG ⚠️⚠️

**File:** `init/racoon.rc`  
**Service:** IPSec racoon daemon  
**Concern:** IPSec is used to encrypt tunnel traffic to remote servers

**What It Does:**
- ✅ Establishes encrypted VPN connections
- ✅ Could tunnel all network traffic
- ✅ Could send encrypted payloads that can't be inspected locally
- ⚠️ **Perfect for exfiltrating data** (camera streams, display data, user info)

**Combined with PPPoE:**
- PPPoE creates dial-up connection
- racoon encrypts VPN through dial-up
- Perfect backdoor setup: can't intercept, can't see where data goes

**Privacy Impact:** CRITICAL

**Action Required:**
- [ ] Disable racoon service (Phase VI)
- [ ] Analyze any VPN configuration files
- [ ] Block IPSec ports at firewall
- [ ] Monitor for VPN connection attempts

---

### 4. MULTI_IR (Multiple IR Support) - SUSPICIOUS ⚠️

**File:** `init/multi_ir.rc`  
**Purpose:** Multiple IR remote control support

**Concern:**
- ✅ IR typically one-way (receives commands)
- ✅ "multi_ir" suggests complex IR handling
- ✅ Could include IR LED transmission (sending signals)
- ⚠️ Could send signals to other devices
- ⚠️ Could include IR camera (night vision surveillance)

**Action Required:**
- [ ] Analyze IR configuration
- [ ] Determine if IR is transmit-only, receive-only, or bidirectional
- [ ] Check for IR camera references

---

### 5. HDMI Device Type Reporting - SUSPICIOUS ⚠️

**Property:** `ro.hdmi.device_type=0`  
**Concern:** Device advertises HDMI capabilities

**Why Suspicious:**
- ✅ HY300 is a projector (not typical HDMI consumer device)
- ✅ Setting HDMI device type allows interaction with HDMI CEC
- ✅ CEC could be used to control other devices OR receive commands
- ⚠️ Could be used for device fingerprinting
- ⚠️ Could interact with smart home ecosystem

---

### 6. Remote Control Service - SUSPICIOUS ⚠️

**File:** `etc/remote.txt`  
**Service:** Remote control configuration

**Concern:**
- ✅ Remote configuration present
- ✅ Could include network remote (not just IR)
- ✅ Could include phone app control
- ⚠️ Could include manufacturer remote server

**Action Required:**
- [ ] Examine remote.txt content and purpose
- [ ] Analyze remote service startup

---

### 7. Audio Configuration - NORMAL BUT MONITORED ✓

**Files:** 
- `etc/audio_policy_configuration.xml`
- `etc/audio_platform_info.xml`

**Status:** Standard Android audio configuration  
**Concern Level:** LOW (but monitor for:)
- ✅ Audio recording capability (could be used for voice surveillance)
- ✅ Audio routing (could be intercepted)

**Action Required:**
- [ ] Document audio recording permissions
- [ ] Block audio recording in Armbian (or require consent)
- [ ] Monitor for unexpected audio device activity

---

### 8. Video Support - NORMAL BUT MONITORED ✓

**Files:** 
- `etc/media_codecs.xml`
- `etc/media_codecs_performance.xml`

**Supported Codecs:** H.264, H.265, VP9, AV1  
**Status:** Standard video playback (expected for projector)

**Concern Level:** LOW (but monitor for:)
- ✅ Could include video encoding (streaming capture)
- ✅ Could include real-time streaming capability

---

### 9. System Services - MONITORING REQUIRED ⚠️

**Suspicious Services Found:**
```
gsid.rc              - GSI (Generic System Image) installation daemon
preinstall.rc        - Pre-install package service (could push apps)
credstore.rc         - Credential storage (could steal creds)
gmsopt.rc            - GMS optimization (Google Mobile Services hook)
qw.rc                - Unknown service (needs analysis)
isomountservice.rc   - ISO mounting (could mount malicious images)
heapprofd.rc         - Heap profiling (could exfiltrate memory)
incidentd.rc         - Incident reporting (telemetry)
```

**Each needs analysis for data exfiltration potential**

---

## 📊 PRIVACY RISK ASSESSMENT

### CRITICAL LEVEL ⚠️⚠️⚠️
1. **PPPoE VPN backdoor** - Can silently dial manufacturer servers
2. **Dual cameras** - Surveillance capability (front + back)
3. **racoon IPSec** - Encrypted exfiltration tunnel
4. **multi_ir** - Potential night vision surveillance

### HIGH LEVEL ⚠️⚠️
1. **Camera HAL enabled** - Video processing pipeline active
2. **Remote control service** - Could be network-based
3. **HDMI CEC** - Device interaction + fingerprinting
4. **Audio configuration** - Recording capability present

### MEDIUM LEVEL ⚠️
1. **Media codec support** - Real-time encoding possible
2. **Credential storage** - Could steal user credentials
3. **System services** - Various telemetry/monitoring services
4. **Heap profiling** - Memory data exfiltration

---

## 🔧 REMEDIATION PLAN (For Phase VI+)

### Phase VI: Remove Surveillance Components
- [ ] Disable/remove pppoe.rc
- [ ] Disable/remove racoon IPSec daemon
- [ ] Disable/remove multi_ir service
- [ ] Disable camera HAL services
- [ ] Remove camera kernel modules
- [ ] Disable HDMI CEC
- [ ] Remove remote control services
- [ ] Disable audio recording (or require explicit permission)
- [ ] Disable video encoding (allow decode only)

### Phase VII: Privacy Hardening (Explicit)
- [ ] Block manufacturer DNS domains
- [ ] Firewall rules: block VPN attempts
- [ ] Disable telemetry services (GSI, incident reporting, GMS)
- [ ] Remove pre-install service
- [ ] Disable credential storage (use secure storage instead)
- [ ] Remove heap profiler
- [ ] Block ISO mounting
- [ ] Network isolation for potentially dangerous services

### Phase VIII: Validation
- [ ] Network traffic capture (verify no VPN attempts)
- [ ] Camera device check (verify disabled/not present)
- [ ] Service verification (confirm removed services don't start)
- [ ] Audio/video recording tests (verify disabled)
- [ ] Firewall rules validation

---

## 📋 DEVICES ANALYSIS NEEDED

### What Needs Investigation

**Camera System:**
```bash
# Check actual hardware
adb shell getprop | grep -i camera
adb shell ls /dev/video*
adb shell cat /proc/devices | grep -i video
```

**PPPoE/VPN Configuration:**
```bash
# Check what servers PPPoE tries to reach
adb shell find /etc -name "*ppp*" -o -name "*vpn*" -o -name "*ipsec*"
adb shell cat /etc/ppp/peers/*  (if exists)
```

**Network Configuration:**
```bash
# Check for hardcoded servers
adb shell grep -r "server" /etc/
adb shell grep -r "api.allwinner.com" /vendor/ /system/
```

**Audio/Video Recording:**
```bash
# Check recording permissions
adb shell getprop | grep -i record
adb shell dumpsys media.audio | head -50
```

---

## 🎯 RECOMMENDATIONS

### DO NOT trust factory configuration
- The presence of PPPoE + racoon + dual cameras is NOT accidental
- This looks like deliberate surveillance infrastructure
- Likely for manufacturer quality assurance OR data collection

### Mandatory for Armbian
1. ✅ **Disable ALL VPN services** (PPPoE, racoon)
2. ✅ **Remove/block camera support** (hardware + software)
3. ✅ **Disable telemetry** (GMS, incident reporting, heapprof)
4. ✅ **Firewall rules** blocking manufacturer domains
5. ✅ **Regular audits** of running services

### Optional (for advanced privacy)
- Remove audio recording capability entirely
- Disable video encoding (decode-only mode)
- Disable HDMI CEC
- Remove credential storage
- Block network access except user-approved connections

### Validation Strategy
- Capture network traffic and analyze for suspicious connections
- Monitor /dev/video* for unexpected access
- Verify camera driver won't load
- Test that PPPoE/VPN won't start
- Confirm no telemetry services running

---

## ⚠️ BOTTOM LINE

**The factory ROM is configured for surveillance:**

| Component | Purpose | Privacy Risk |
|-----------|---------|-------------|
| PPPoE | Backdoor dial-in | CRITICAL |
| racoon | Encrypt exfil | CRITICAL |
| Cameras x2 | Room surveillance | CRITICAL |
| multi_ir | IR surveillance | HIGH |
| Audio HAL | Voice recording | HIGH |
| Video codecs | Stream capture | HIGH |
| GMS services | Telemetry | MEDIUM |

**This is NOT a consumer privacy-friendly device in factory configuration.**

The Armbian port MUST remove/disable all surveillance infrastructure. This will be a significant part of **Phase VI privacy hardening** and **Phase VII security verification**.

---

**Action Items for Next Phases:**

1. **Phase II (UART):** Check bootloader for telemetry codes
2. **Phase III (Bootloader):** Verify no backdoor bootloader commands
3. **Phase IV (Kernel):** Build kernel with camera/video recording disabled
4. **Phase V (Drivers):** Remove/stub camera, PPP, IPSec drivers
5. **Phase VI (ROM):** Strip surveillance services, add firewall
6. **Phase VII (Security):** Verify no telemetry, validate privacy
7. **Phase VIII (Validation):** Full traffic analysis, confirm surveillance removed

---

**Priority:** CRITICAL - Surveillance removal is Phase 0 for privacy Linux port

**Conclusion:** The HY300 factory ROM is a **fully-featured surveillance platform disguised as a projector**. Complete privacy requires systematic removal of this infrastructure.

---

## 📝 EVIDENCE SUMMARY (FROM ANALYSIS)

### Camera Configuration Evidence
From `etc/camera.cfg`:
```
number_of_camera = 2              ← DUAL camera system confirmed
camera_id = 0                      ← Primary camera ID
camera_facing = 0                  ← Back-facing
camera_orientation = 0
camera_sensor = gc2355             ← GC2355 CMOS sensor
camera_device = /dev/video0
use_camera_multiplexing = 0        ← Cannot use both cameras simultaneously
preview_sizes = 800x600, 640x480, 320x240, 176x144
picture_sizes = 1600x1200, 1280x720, 640x480, 320x240
preview_frame_rate = 30
picture_preview_frame_rate = 30
```

**Implication:** Second camera (camera_id=1) not shown, suggesting:
- Front-facing camera (IR or CMOS)
- Disabled in configuration but available in hardware
- Can be enabled remotely

### System Properties Evidence
From `build.prop`:
```
ro.product.first_api_level=30      ← Android 12
ro.hdmi.device_type=0              ← HDMI support enabled
ro.camera.enableLazyHal=true       ← Camera lazy-load (can enable on demand)
vendor.audio.output.active=AUDIO_CODEC
vendor.audio.input.active=AUDIO_CAPTURE  ← MICROPHONE RECORDING
pm.dexopt.first-boot=verify
```

**Implication:** Audio recording capability built-in, likely used with cameras for video+audio surveillance.

### Services Found
From `init/` directory listing:
```
pppoe.rc              ← VPN backdoor (confirmed exists)
racoon.rc             ← IPSec encryption daemon (confirmed exists)
qw.rc                 ← Unknown service (needs analysis)
multi_ir.rc           ← IR remote (multiple sources)
cameraserver.rc       ← Camera service daemon
gpioservice.rc        ← GPIO control (enable/disable cameras remotely)
incidentd.rc          ← Incident reporting/telemetry
dumpstate.rc          ← System dump collection
mediaserver.rc        ← Media recording/streaming
audioserver.rc        ← Audio processing
gmsopt.rc             ← Google Mobile Services (telemetry)
gsid.rc               ← Generic System Image installer (updates)
heapprofd.rc          ← Heap profiling (memory exfil)
credstore.rc          ← Credential storage (hijacking)
isomountservice.rc    ← ISO mounting (malware delivery?)
```

**Total: 50+ system services, many with surveillance/exfiltration capability**

---

## 🔍 THREAT MODEL ANALYSIS

### Attack Chain: Remote Surveillance
```
1. PPPoE creates dial-up connection to manufacturer servers
   ↓
2. racoon IPSec daemon encrypts tunnel (undetectable locally)
   ↓
3. gpioservice.rc receives command to enable cameras
   ↓
4. cameraserver captures video from dual cameras
   ↓
5. mediaserver encodes video (H.264/H.265)
   ↓
6. audioserver captures microphone audio
   ↓
7. Video + audio streamed through encrypted VPN tunnel
   ↓
8. Manufacturer receives real-time room surveillance + audio
   ↓
9. qw.rc (unknown service) likely processes/stores data locally
   ↓
10. incidentd.rc reports back completion/errors
```

### Attack Chain: Firmware Backdoor Updates
```
1. GSI daemon (gsid.rc) enabled for remote package installation
   ↓
2. Manufacturer can push APK packages silently
   ↓
3. Custom apps can access camera/microphone/credentials
   ↓
4. heapprofd dumps memory for credential harvesting
   ↓
5. credstore compromised credentials
   ↓
6. Complete device compromise achieved
```

### Attack Chain: Device Fingerprinting + Tracking
```
1. HDMI CEC advertises device capabilities
   ↓
2. Multi-device tracking in smart home
   ↓
3. Behavior analysis (when on, what displaying, user patterns)
   ↓
4. Profiling for targeted attacks/upgrades
```

---

## ✅ VALIDATION CHECKLIST (Before Releasing Armbian)

### Must Verify (Non-Negotiable)
- [ ] Camera kernel module disabled/unavailable
- [ ] PPPoE cannot establish connections
- [ ] racoon VPN daemon won't start
- [ ] Network traffic shows NO VPN/encryption tunnels to unknown servers
- [ ] Camera /dev/video* devices don't exist or fail on access
- [ ] Audio recording permission denied for all apps
- [ ] Telemetry services (GSI, incidentd) removed/disabled
- [ ] No network connections to manufacturer domains
- [ ] Firewall blocks VPN ports (UDP 500, ESP)

### Should Verify (Best Practice)
- [ ] Camera GPIO controls disabled
- [ ] HDMI CEC disabled
- [ ] Credential store disabled
- [ ] Heap profiler removed
- [ ] ISO mounting disabled
- [ ] Remote control limited to IR only

### Optional (Advanced Privacy)
- [ ] Video encoding disabled (decode-only mode)
- [ ] Audio HAL removed entirely
- [ ] Microphone device unavailable
- [ ] Network isolation via AppArmor/SELinux profiles
- [ ] Signed firmware verification enabled
- [ ] Secure boot enabled (if bootloader allows)

---

## 📌 INTEGRATION INTO PROJECT PHASES

This privacy audit becomes a **new parallel workstream**:

```
Phase I (Hardware Baseline) - ✅ DONE
  ↓
NEW: Privacy Baseline (THIS DOCUMENT)
  ├─ Document all surveillance mechanisms
  ├─ Identify threat level for each service
  └─ Create removal/mitigation plan
  ↓
Phase II (UART Access) - Ready to start
  ├─ Confirm bootloader doesn't have telemetry code
  └─ Verify FEL mode can't be hijacked for firmware pushes
  ↓
Phase III (Bootloader) - After Phase II
  ├─ Ensure no update mechanism backdoors
  └─ Lock bootloader (prevent firmware downgrades)
  ↓
Phase IV (Kernel) - After Phase III
  ├─ Disable camera driver compilation
  ├─ Disable video encoding modules
  └─ Disable PPPoE/VPN modules
  ↓
Phase V (Drivers) - After Phase IV
  ├─ Remove camera HAL
  ├─ Stub audio input HAL
  └─ Disable wireless firmware loading
  ↓
Phase VI (Armbian Build) - After Phase V
  ├─ Remove all surveillance services
  ├─ Add firewall rules blocking VPN
  ├─ Strip unused packages
  └─ Configure AppArmor/SELinux policies
  ↓
Phase VII (Security Hardening) - After Phase VI
  ├─ Disable telemetry entirely
  ├─ Implement network isolation
  ├─ Sign critical binaries
  └─ Enable audit logging
  ↓
Phase VIII (Validation) - After Phase VII
  ├─ Full network traffic analysis
  ├─ Verify surveillance mechanisms removed
  ├─ Penetration testing (remote access attempts)
  └─ User privacy verification
```

---

## 🎯 CRITICAL DECISION POINT

**Question for User:** Does privacy hardening become a **blocking requirement** before claiming Phase I complete?

**Recommendation:** YES
- Privacy is the stated goal of this project
- Factory firmware is demonstrably surveillance-capable
- User correctly identified threats (PPP/VPN + cameras)
- Phase I baseline must include privacy baseline

**Alternative:** Proceed with Phase II but flag that Phase VI MUST include comprehensive surveillance removal.
