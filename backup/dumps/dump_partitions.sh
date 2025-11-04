#!/bin/bash
# Zerlegt einen kompletten eMMC-Dump (GPT) in einzelne Partitionen

IMG="~full_emmc_dump_clean_root.img"
OUTDIR=".""


echo "📖 Lese Partitionstabelle aus $IMG ..."

# Hole Partitionen und deren Start/Sektorgrößen
sgdisk -p "$IMG" | awk '/^[0-9]+ +[0-9]+/ {print $1, $2, $4, $6}' | while read -r num start end name; do
    # GPT nutzt 512-Byte-Sektoren:
    start_sector=$start
    end_sector=$end
    sectors=$((end_sector - start_sector + 1))
    outfile="$OUTDIR/${name}.img"
    echo "➡️  Extrahiere Partition $name (Sektoren $start_sector–$end_sector)..."
    dd if="$IMG" of="$outfile" bs=512 skip="$start_sector" count="$sectors" status=progress
done

echo "✅ Alle Partitionen wurden nach $OUTDIR extrahiert."
