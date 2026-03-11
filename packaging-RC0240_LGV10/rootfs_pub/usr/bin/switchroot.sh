#!/bin/sh
#
# This script at first changes root filesystem to ramfs-based path,
# then unmounts old squashfs-based rootfs and kills all unrelated
# background processes, and finally updates the data stored on flash
#
# switchroot.sh
IMAGE_BASE=$(basename "$1")
IMAGE_DIR=$(dirname "$1")
echo "switchroot.sh start..."

echo "drop_caches"
echo 3 > /proc/sys/vm/drop_caches

mkdir /tmp/new-root -p
mount -t tmpfs mytmpfs  /tmp/new-root
mv "$1" "/tmp/new-root/${IMAGE_BASE}"
mkdir /tmp/new-root/old-root -p
cp /var /sbin /opt /lib /run /bin /tmp/new-root/ -d -r

mkdir /tmp/new-root/sys /tmp/new-root/proc /tmp/new-root/dev /tmp/new-root/usr

cp /usr/bin /tmp/new-root/usr/ -r
cp /usr/sbin /tmp/new-root/usr/ -r

/bin/mount -o noatimve,move /proc /tmp/new-root/proc
/sbin/pivot_root /tmp/new-root /tmp/new-root/old-root
/bin/mount -o noatimve,move /old-root/sys/ /sys
/bin/mount -o noatimve,move /old-root/dev/ /dev

cd /
mkdir -p "${IMAGE_DIR}"
mv "/${IMAGE_BASE}" "${IMAGE_DIR}/${IMAGE_BASE}"

echo "switchroot.sh end..."
