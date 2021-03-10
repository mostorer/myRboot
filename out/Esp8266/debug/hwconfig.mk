# Generated from hardware configuration 'Two ROM with Hybrid FS in 1M standard'
SMING_ARCH_HW = Esp8266
PARTITION_TABLE_OFFSET = 0x00002000
PARTITION_TABLE_LENGTH = 0x0c00
SPIFLASH_PARTITION_NAMES = phy_init sys_param rom0 spiffsOvl rom1 fwfs0v1
HWCONFIG_DEPENDS = /Users/a230977/ESPdev/myRboot/map/custom.hw
STORAGE_DEVICE_NAMES = spiFlash
STORAGE_DEVICE_spiFlash_TYPE = flash
STORAGE_DEVICE_spiFlash_SIZE = 1M
STORAGE_DEVICE_spiFlash_MODE = dio
STORAGE_DEVICE_spiFlash_SPEED = 40
STORAGE_DEVICE_spiFlash_SIZE_BYTES = 0x100000
PARTITION_NAMES = phy_init sys_param rom0 spiffsOvl rom1 fwfs0v1
PARTITION_phy_init_DEVICE = spiFlash
PARTITION_phy_init_ADDRESS = 0x00003000
PARTITION_phy_init_SIZE = 4K
PARTITION_phy_init_TYPE = data
PARTITION_phy_init_SUBTYPE = phy
PARTITION_phy_init_READONLY = 0
PARTITION_phy_init_ENCRYPTED = 0
PARTITION_phy_init_FILENAME = $(FLASH_INIT_DATA)
PARTITION_phy_init_SIZE_BYTES = 0x1000
PARTITION_sys_param_DEVICE = spiFlash
PARTITION_sys_param_ADDRESS = 0x00004000
PARTITION_sys_param_SIZE = 16K
PARTITION_sys_param_TYPE = data
PARTITION_sys_param_SUBTYPE = sysparam
PARTITION_sys_param_READONLY = 0
PARTITION_sys_param_ENCRYPTED = 0
PARTITION_sys_param_FILENAME = 
PARTITION_sys_param_SIZE_BYTES = 0x4000
PARTITION_rom0_DEVICE = spiFlash
PARTITION_rom0_ADDRESS = 0x00008000
PARTITION_rom0_SIZE = 440K
PARTITION_rom0_TYPE = app
PARTITION_rom0_SUBTYPE = factory
PARTITION_rom0_READONLY = 0
PARTITION_rom0_ENCRYPTED = 0
PARTITION_rom0_FILENAME = $(RBOOT_ROM_0_BIN)
PARTITION_rom0_SIZE_BYTES = 0x6e000
PARTITION_spiffsOvl_DEVICE = spiFlash
PARTITION_spiffsOvl_ADDRESS = 0x00076000
PARTITION_spiffsOvl_SIZE = 40K
PARTITION_spiffsOvl_TYPE = data
PARTITION_spiffsOvl_SUBTYPE = spiffs
PARTITION_spiffsOvl_READONLY = 0
PARTITION_spiffsOvl_ENCRYPTED = 0
PARTITION_spiffsOvl_FILENAME = $(SPIFF_BIN_OUT)
PARTITION_spiffsOvl_SIZE_BYTES = 0xa000
PARTITION_rom1_DEVICE = spiFlash
PARTITION_rom1_ADDRESS = 0x00080000
PARTITION_rom1_SIZE = 440K
PARTITION_rom1_TYPE = app
PARTITION_rom1_SUBTYPE = ota_0
PARTITION_rom1_READONLY = 0
PARTITION_rom1_ENCRYPTED = 0
PARTITION_rom1_FILENAME = $(RBOOT_ROM_1_BIN)
PARTITION_rom1_SIZE_BYTES = 0x6e000
PARTITION_fwfs0v1_DEVICE = spiFlash
PARTITION_fwfs0v1_ADDRESS = 0x000ee000
PARTITION_fwfs0v1_SIZE = 72K
PARTITION_fwfs0v1_TYPE = data
PARTITION_fwfs0v1_SUBTYPE = fwfs
PARTITION_fwfs0v1_READONLY = 0
PARTITION_fwfs0v1_ENCRYPTED = 0
PARTITION_fwfs0v1_FILENAME = $(PROJECT_DIR)/out/fwfs_rom.bin
PARTITION_fwfs0v1_SIZE_BYTES = 0x12000