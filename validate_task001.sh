#!/bin/bash
set -e

echo "Validating Task 001 Completion..."
echo "=================================="
echo ""

# Check ADB connection
echo -n "ADB connection: "
if adb devices | grep -q "device$"; then
  echo "✅"
else
  echo "❌"
fi

# Check root access
echo -n "Root access: "
if adb shell su -c "id" | grep -q "uid=0"; then
  echo "✅"
else
  echo "❌"
fi

# Check backup directory
echo -n "Backup directory structure: "
if [ -d backup ] && [ -d backup/device_info ]; then
  echo "✅"
else
  echo "❌"
fi

# Check documentation
echo -n "Documentation (root-access-verification.md): "
if [ -f hardware-access/root-access-verification.md ]; then
  echo "✅"
else
  echo "❌"
fi

# Check device info files
echo -n "Device info collection: "
if [ -f backup/device_info/model.txt ] && [ -f backup/device_info/kernel.txt ] && [ -f backup/device_info/cpuinfo.txt ]; then
  echo "✅"
else
  echo "❌"
fi

# Check storage info
echo -n "Storage info collected: "
if [ -f backup/device_storage_info.txt ]; then
  echo "✅"
else
  echo "❌"
fi

echo ""
echo "=================================="
echo "Validation complete!"
echo ""
echo "Summary:"
grep -E "processor|model name" /home/luca/Desktop/hy300-linux-porting/backup/device_info/cpuinfo.txt | head -2
echo "Device ID: $(adb devices | grep device | awk '{print $1}')"
echo "Root: $(adb shell su -c 'id' | grep -o 'uid=[0-9]*')"
