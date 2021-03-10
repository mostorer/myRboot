#include <SmingCore.h>
#include <Network/RbootHttpUpdater.h>


// download urls, set appropriately
#define ROM_0_URL "http://192.168.0.112:80/rom0.bin"
#define ROM_1_URL "http://192.168.0.112:80/rom1.bin"
#define FWFS_URL "http://192.168.0.112:80/fwfs_rom.bin"

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "WirelessNetworkSSID" // Put you SSID and Password here
#define WIFI_PWD "WirelessNetworkPWD"
#endif


RbootHttpUpdater* otaUpdater;
Storage::Partition fwfsPartition;
IFS::IFileSystem* fwfs;


Storage::Partition findFwfsPartition()
{
	auto fwfsPart = *Storage::findPartition(Storage::Partition::SubType::Data::fwfs);
	if(fwfsPart) {
		debug_i("Found '%s' at 0x%06x, length 0x%06x", fwfsPart.name().c_str(), 
				 fwfsPart.address(), fwfsPart.size());
	}
	return fwfsPart;
}


IFS::IFileSystem* mountFwfsPartition()
{
	debugf("Trying to mount '%s' at 0x%08x, length 0x%08x", fwfsPartition.name().c_str(), fwfsPartition.address(),
			fwfsPartition.size());
	fwfs = IFS::createFirmwareFilesystem(fwfsPartition);

	if(fwfs == nullptr) {
		debug_e("Failed to created filesystem object");
		return (nullptr);
	}
	else {
		debug_i("Firmware filesystem created");;
		int res = fwfs->mount();
		debug_i("mount() returned %d (%s)", res, fwfs->getErrorString(res).c_str());

		if(res < 0) {
			delete fwfs;
			return(nullptr);
		}

		fileSetFileSystem(fwfs);
		debug_i("File system initialised");

		debug_i("=FWFS=====================");
		auto list = fileList();
		for(auto& f : fileList()) {
			debug_i(" - %10i \t\t %s", fileGetSize(f), f.c_str());
		}
		debug_i("%i file(s) found\n", list.count());
		debug_i("==========================");
	}
	return fwfs;
}


void ShowInfo()
{
	debug_i("SDK: v%s", system_get_sdk_version());
	debug_i("Free Heap: %d", system_get_free_heap_size());
	debug_i("CPU Frequency: %d MHz", system_get_cpu_freq());
	debug_i("System Chip ID: %x", system_get_chip_id());
	debug_i("SPI Flash ID: %x", spi_flash_get_id());
	debug_i("SPI Flash Size: 0x%06x", (1 << ((spi_flash_get_id() >> 16) & 0xff)));

	rboot_config conf;
	conf = rboot_get_config();
	uint8 slot = rboot_get_current_rom();

	debug_i("ROM count: %d", conf.count);
	debug_i("ROM 0: 0x%06x", conf.roms[0]);
	debug_i("ROM 1: 0x%06x", conf.roms[1]);
	debug_i("Booting from ROM %d", slot);
}


void otaUpdateCallBack(RbootHttpUpdater& client, bool result)
{
	if(result == true) {
		// success
		uint8 slot;
		slot = rboot_get_current_rom();
		slot ^= 1;
		// set to boot new rom and then reboot
		debug_i("Firmware updated, rebooting to rom %d...\r\n", slot);
		rboot_set_current_rom(slot);
		System.restart();
	} else {
		// fail
		Serial.println("Firmware update failed!");
	}
}

void OtaUpdate()
{
	uint8 slot;
	rboot_config bootconf;

	// need a clean object, otherwise if run before and failed will not run again
	if(otaUpdater) {
		delete otaUpdater;
	}
	otaUpdater = new RbootHttpUpdater();

	// select rom slot to flash
	bootconf = rboot_get_config();
	slot = bootconf.current_rom;
	slot ^= 1;

	otaUpdater->addItem(bootconf.roms[slot], (slot == 0) ? ROM_0_URL : ROM_1_URL);

	fileFreeFileSystem();
	if(fwfsPartition) {
		debug_i("Updating fwfs partition '%s' at address 0x%06x, size 0x%06x",
				fwfsPartition.name().c_str(), fwfsPartition.address(), fwfsPartition.size());
		otaUpdater->addItem(fwfsPartition.address(), FWFS_URL, fwfsPartition.size());
	}

	otaUpdater->setCallback(otaUpdateCallBack);
	otaUpdater->start();
}

void Switch()
{
	uint8 before, after;
	before = rboot_get_current_rom();
	after = before ^ 1;

	Serial.printf("Swapping from rom %d to rom %d.\r\n", before, after);
	rboot_set_current_rom(after);
	Serial.println("Restarting...\r\n");
	System.restart();
}

void serialCallBack(Stream& stream, char arrivedChar, unsigned short availableCharsCount)
{
	int pos = stream.indexOf('\n');
	if(pos > -1) {
		char str[pos + 1];
		for(int i = 0; i < pos + 1; i++) {
			str[i] = stream.read();
			if(str[i] == '\r' || str[i] == '\n') {
				str[i] = '\0';
			}
		}

		if(!strcmp(str, "connect")) {
			// connect to wifi
			WifiAccessPoint.enable(false);
			WifiStation.config(WIFI_SSID, WIFI_PWD);
			WifiStation.enable(true);
			WifiStation.connect();
		} else if(!strcmp(str, "ip")) {
			Serial.print("ip: ");
			Serial.print(WifiStation.getIP());
			Serial.print("mac: ");
			Serial.println(WifiStation.getMacAddress());
		} else if(!strcmp(str, "ota")) {
			OtaUpdate();
		} else if(!strcmp(str, "switch")) {
			Switch();
		} else if(!strcmp(str, "restart")) {
			System.restart();
		} else if(!strcmp(str, "ls")) {
			Vector<String> files = fileList();
			Serial.printf("filecount %d\r\n", files.count());
			for(unsigned int i = 0; i < files.count(); i++) {
				Serial.println(files[i]);
			}
		} else if(!strcmp(str, "cat")) {
			Vector<String> files = fileList();
			if(files.count() > 0) {
				Serial.printf("dumping file %s:\r\n", files[0].c_str());
				Serial.println(fileGetContent(files[0]));
			} else {
				Serial.println("Empty spiffs!");
			}
		} else if(!strcmp(str, "info")) {
			ShowInfo();
		} else if(!strcmp(str, "help")) {
			Serial.println();
			Serial.println("available commands:");
			Serial.println("  help - display this message");
			Serial.println("  ip - show current ip address");
			Serial.println("  connect - connect to wifi");
			Serial.println("  restart - restart the esp8266");
			Serial.println("  switch - switch to the other rom and reboot");
			Serial.println("  ota - perform ota update, switch rom and reboot");
			Serial.println("  info - show esp8266 info");
			if(fwfsPartition) {
				Serial.println("  ls - list files in spiffs");
				Serial.println("  cat - show first file in spiffs");
			}
			Serial.println();
		} else {
			Serial.println("unknown command");
		}
	}
}


void init()
{
	Serial.begin(SERIAL_BAUD_RATE);  // 115200 by default
	Serial.systemDebugOutput(true); // Enable debug output
	debug_i("\n\n********************************************************\n"
			"Hello\n");


	fwfsPartition = findFwfsPartition();
	if(fwfsPartition) {
		fwfs = mountFwfsPartition();
	}

	auto slot = rboot_get_current_rom();
	debug_i("\r\nCurrently running rom %d.\r\n", slot);
	debug_i("Type 'help' and press enter for instructions.\n");

	ShowInfo();

	Serial.onDataReceived(serialCallBack);
}
