#!/bin/bash

WIN_IP="192.xx.xx.201"
WIN_SHARE="log"
WIN_USER="your_user"
WIN_PASS="your_pass"

LOCAL_LOG_DIR="/var/log/mytest"
MOUNT_POINT="/mnt/winlog"

echo "== Install packages =="
apt update
apt install -y cifs-utils rsync inotify-tools

echo "== Create mount point =="
mkdir -p $MOUNT_POINT

echo "== Create credentials file =="
cat <<EOF > /root/.smbcred
username=$WIN_USER
password=$WIN_PASS
EOF
chmod 600 /root/.smbcred

echo "== Add auto mount to fstab =="
grep -qxF "//$WIN_IP/$WIN_SHARE $MOUNT_POINT cifs credentials=/root/.smbcred,vers=3.0,_netdev 0 0" /etc/fstab || \
echo "//$WIN_IP/$WIN_SHARE $MOUNT_POINT cifs credentials=/root/.smbcred,vers=3.0,_netdev 0 0" >> /etc/fstab

mount -a

echo "== Create log sync script =="
cat <<'EOF' > /usr/local/bin/log_sync.sh
#!/bin/bash
LOCAL_DIR="/var/log/mytest"
REMOTE_DIR="/mnt/winlog"

inotifywait -m -r -e close_write,create,move "$LOCAL_DIR" --format '%w%f' |
while read FILE
do
    sleep 1
    rsync -az --inplace "$FILE" "$REMOTE_DIR"
done
EOF

chmod +x /usr/local/bin/log_sync.sh

echo "== Create systemd service =="
cat <<EOF > /etc/systemd/system/logsync.service
[Unit]
Description=Log Sync Service to Windows Server
After=network.target

[Service]
ExecStart=/usr/local/bin/log_sync.sh
Restart=always
User=root

[Install]
WantedBy=multi-user.target
EOF

echo "== Enable service =="
systemctl daemon-reload
systemctl enable logsync
systemctl start logsync

echo "== DONE =="
echo "Log sync is running."