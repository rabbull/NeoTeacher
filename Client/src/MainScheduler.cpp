//
// Created by karl on 18-7-31.
//

#include <fstream>
#include <json/json.h>
#include <zconf.h>
#include "MainScheduler.h"
#include "UserOperations.h"

MainScheduler::MainScheduler(std::string path) {
    init(std::move(path));
}

void MainScheduler::init(std::string path) {
    setting = new Setting(std::move(path));
//    systemController = new SystemController(setting);
    TCPSerial = new TransmissionControlProtocolSerial(setting);
}

void MainScheduler::end() {
    delete this;
}

MainScheduler::~MainScheduler() {
    delete setting;
//    delete systemController;
    delete TCPSerial;
}

void MainScheduler::start() {
    // debug
//    UserOperations::Login(TCPSerial);
//    fprintf (stderr, "[STAT] succeed logged in..\n");
//    sleep(10);
//    UserOperations::Quit(TCPSerial);
    UserOperations::HandIn(TCPSerial);
    // do something
}
