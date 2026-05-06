#!/bin/bash

### ====== CẤU HÌNH ======
WIN_IP="192.xx.xx.201"
WIN_SHARE="data"     # tên share trên Windows: Chuột phải → Properties → Sharing → Advanced Sharing
WIN_USER="your_user"
WIN_PASS="your_pass"
MOUNT_POINT="/mnt/winshare"
CRED_FILE="/root/.smbcred"
### =======================

echo "== Install cifs-utils =="
sudo apt update
sudo apt install -y cifs-utils

echo "== Create mount point =="
sudo mkdir -p "$MOUNT_POINT"

echo "== Create credentials file =="
sudo bash -c "cat > $CRED_FILE" <<EOF
username=$WIN_USER
password=$WIN_PASS
EOF
sudo chmod 600 "$CRED_FILE"

echo "== Add to /etc/fstab for auto-mount =="
FSTAB_LINE="//$WIN_IP/$WIN_SHARE $MOUNT_POINT cifs credentials=$CRED_FILE,vers=3.0,uid=1000,gid=1000,_netdev 0 0"
grep -qxF "$FSTAB_LINE" /etc/fstab || echo "$FSTAB_LINE" | sudo tee -a /etc/fstab

echo "== Mount now =="
sudo mount -a

echo ""
echo "DONE. Mở File Manager và vào: $MOUNT_POINT"