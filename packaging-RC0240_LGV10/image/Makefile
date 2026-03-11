all:
	$(Q)./packaging-update ./merge.ini ./linux.bin
	$(Q)./packaging-update ./upgrade.ini ./upgrade.bin
	$(Q)./packaging-raw ./partition.ini ./raw.bin

clean:
	$(Q)rm -f ./oem.squashfs
	$(Q)rm -f ./rootfs.squashfs
	$(Q)rm -f ./linux.bin
	$(Q)rm -f ./upgrade.bin
	$(Q)rm -f ./raw.bin
	$(Q)rm -f ./conf.jffs2