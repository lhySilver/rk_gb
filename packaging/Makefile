ROOT=..
PACKAGING=.
BURNTOOL_DIR= $(ROOT)/burntool
DAEMON_DIR = $(ROOT)/daemon
OBJ_DIR = $(ROOT)/Bin
ROOTFS_DIR = $(PACKAGING)/rootfs_pub
OEM_DIR = $(PACKAGING)/oem_ipc
CONF_DIR = $(PACKAGING)/data
IMAGE_DIR = $(PACKAGING)/image

STRIP = $(CROSS)strip

all:
	$(Q)rm -f $(OEM_DIR)/usr/bin/dgiot
	$(Q)rm -f $(ROOTFS_DIR)/usr/bin/daemon
	$(Q)rm -f $(ROOTFS_DIR)/usr/bin/burntool
	$(Q)cp -R $(OBJ_DIR)/dgiot $(OEM_DIR)/usr/bin
	$(Q)cp -R $(DAEMON_DIR)/daemon $(ROOTFS_DIR)/usr/bin
	$(Q)cp -R $(BURNTOOL_DIR)/burntool $(ROOTFS_DIR)/usr/bin
	
#	$(Q)$(STRIP) $(ROOTFS_DIR)/bin/*
#	$(Q)$(STRIP) $(ROOTFS_DIR)/lib/lib*
#	$(Q)$(STRIP) $(OEM_DIR)/usr/bin/*
#	$(Q)$(STRIP) $(OEM_DIR)/usr/lib/lib*
	
	$(Q)$(STRIP) $(OEM_DIR)/usr/bin/dgiot
#	$(Q)$(STRIP) $(OEM_DIR)/usr/bin/sample_demo_vi_avs_venc
	$(Q)$(STRIP) $(OEM_DIR)/usr/bin/rkaiq_tool_server
	$(Q)$(STRIP) $(ROOTFS_DIR)/usr/bin/daemon
	$(Q)$(STRIP) $(ROOTFS_DIR)/usr/bin/burntool
	
	$(Q)rm -f $(PACKAGING)/conf.jffs2
	$(Q)$(PACKAGING)/mkfs.jffs2 -r $(CONF_DIR) -o $(PACKAGING)/conf.jffs2 --pad=0x90000
	
	$(Q)rm -f $(PACKAGING)/rootfs.squashfs
	$(Q)$(PACKAGING)/mksquashfs $(ROOTFS_DIR) $(PACKAGING)/rootfs.squashfs -comp xz
	
	$(Q)rm -f $(PACKAGING)/oem.squashfs
	$(Q)$(PACKAGING)/mksquashfs $(OEM_DIR) $(PACKAGING)/oem.squashfs -comp xz
	
	$(Q)mv $(PACKAGING)/conf.jffs2 $(IMAGE_DIR)/ -f
	$(Q)mv $(PACKAGING)/rootfs.squashfs $(IMAGE_DIR)/ -f
	$(Q)mv $(PACKAGING)/oem.squashfs $(IMAGE_DIR)/ -f
	
	make -C $(IMAGE_DIR);
	$(Q)rm -f $(ROOT)/Release/*.bin
	$(Q)rm -f $(ROOT)/Release/*.txt
	$(Q)cp $(IMAGE_DIR)/raw.bin $(ROOT)/Release
	$(Q)cp $(IMAGE_DIR)/linux.bin $(ROOT)/Release
	$(Q)cp $(IMAGE_DIR)/upgrade.bin $(ROOT)/Release
	
#	$(Q)cd $(IMAGE_DIR)
#	$(Q)$(IMAGE_DIR)/packaging $(IMAGE_DIR)/merge.ini $(PACKAGING)/linux.bin
#	$(Q)cd -
#	$(Q)$(IMAGE_DIR)/packaging.sh $(IMAGE_DIR)

clean:
	$(Q)rm -f $(ROOT)/Release/*.bin
	$(Q)rm -f $(ROOT)/Release/*.txt
	make clean -C $(IMAGE_DIR);