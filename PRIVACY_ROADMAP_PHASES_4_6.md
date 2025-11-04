# Surveillance Removal - Phases IV-VI

## PHASE IV: Kernel Configuration - Disable Surveillance Drivers

**Key Kernel Config Disables:**
```
CONFIG_VIDEO_V4L2=n              # Disable video v4l2
CONFIG_MEDIA_SUPPORT=n           # Disable all media
CONFIG_PPP=n                     # Disable Point-to-Point
CONFIG_PPP_OVER_ETHERNET=n       # Disable PPPoE
CONFIG_NETFILTER_IPSEC=n         # Disable IPSec hooks
```

**Device Tree Changes:**
- Remove camera device nodes
- Remove audio input nodes
- Disable IR receiver
- Disable HDMI CEC

**Result:** Surveillance hardware completely unavailable to userspace

---

## PHASE V: Driver Porting + Service Removal

**Services to REMOVE from Armbian:**
- camera HAL services (✗)
- pppoe service (✗)
- racoon IPSec daemon (✗)
- qw service - unknown purpose (✗)
- multi_ir service - IR remote (✗)
- incidentd - telemetry (✗)
- dumpstate - system dump (✗)
- gsid - system image installer (✗)
- heapprofd - heap profiler (✗)
- credstore - credential storage (✗)

**Services to KEEP with restrictions:**
- mediaserver: audio output ONLY (no recording)
- audioserver: playback ONLY (no recording)
- networkmanager: WiFi/Ethernet management

**Result:** No services capable of surveillance

---

## PHASE VI: Firewall + Network Isolation

**Firewall Rules (iptables):**
```
DROP:   UDP 500 (IPSec phase 1)
DROP:   UDP 4500 (IPSec NAT-T)
DROP:   Protocol ESP (IPSec)
ALLOW:  UDP 53 (DNS - with monitoring)
ALLOW:  TCP 80/443 (HTTP/HTTPS - user initiated)
DEFAULT POLICY: DROP outbound (whitelist only)
```

**DNS Filtering:**
- Block *.allwinner.com
- Block known telemetry domains
- Use local DNS sinkhole

**AppArmor Profiles:**
- WiFi service: only hardware access
- Audio service: only audio devices
- Display service: only display hardware
- Network services: cannot spawn new processes

**Result:** No outbound connections possible except whitelisted

---

## CRITICAL SUCCESS FACTORS

1. **Every Phase Validates Previous**
   - Phase III verifies bootloader security
   - Phase IV verifies kernel safety
   - Phase V verifies service removal
   - Phase VI verifies firewall effectiveness
   - Phase VIII verifies complete privacy

2. **User Can Verify Themselves**
   - Simple commands: `lsmod`, `systemctl`, `netstat`
   - Packet capture: `tcpdump` shows no suspicious traffic
   - Open source tools: no black boxes

3. **Defense in Depth**
   - Bootloader: cannot update surveillance
   - Kernel: cannot activate surveillance
   - Services: surveillance removed
   - Firewall: blocked even if reactivated
   - Validation: proven through testing

4. **No Compromises**
   - If any surveillance mechanism remains = privacy failed
   - Better to disable functionality than leave backdoor
   - User controls what's enabled, not vendor