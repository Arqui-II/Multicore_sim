/*
 * Display.cpp
 *
 *  Created on: 31 may. 2020
 *      Author: Juan Pablo Brenes C.
 */

#include <display/Display.h>

Display::Display() :
		_running(false) {

	this->init();
}

Display::~Display() {
	delwin(_P0_C0L1);
	delwin(_P0_C1L1);
	delwin(_P1_C0L1);
	delwin(_P1_C1L1);
	delwin(_P0L2);
	delwin(_P1L2);
	delwin(_RAM);
	endwin();
}

void Display::init() {
	initscr();
	curs_set(0);
	noecho();
	getmaxyx(stdscr, _mainY, _mainX);
	printw("Multicore system simulation.   2 chips, 4 cores");

	_P0_C0L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, 0);
	_P0_C1L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, L1_WIN_SIZEX);
	_P1_C0L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEX * 2) + 5);
	_P1_C1L1 = newwin(L1_WIN_SIZEY, L1_WIN_SIZEX, 1, (L1_WIN_SIZEX * 3) + 5);

	_P0L2 = newwin(L2_WIN_SIZEY, L2_WIN_SIZEX, L1_WIN_SIZEY + 2, L1_WIN_SIZEX / 2);
	_P1L2 = newwin(L2_WIN_SIZEY, L2_WIN_SIZEX, L1_WIN_SIZEY + 2, ((L1_WIN_SIZEX / 2) * 5));
	_RAM = newwin(RAM_WIN_SIZEY, RAM_WIN_SIZEX, (L2_WIN_SIZEY + L1_WIN_SIZEY) + 2,
			(_mainX / 2) - RAM_WIN_SIZEX);
	refresh();

	box(_P0_C0L1, 0, 0);
	box(_P0_C1L1, 0, 0);
	box(_P1_C0L1, 0, 0);
	box(_P1_C1L1, 0, 0);
	box(_P0L2, 0, 0);
	box(_P1L2, 0, 0);
	box(_RAM, 0, 0);

	scrollok(_RAM, NULL);
	this->printHeaders();
	this->refreshWindows();
}

void Display::start() {
	_running = true;
	_thread = std::thread(&Display::update, this);
}

void Display::stop() {
	_running = false;
	_thread.join();
	this->exitRoutine();
}

void Display::exitRoutine() {
	mvwprintw(stdscr, _mainY - 1, 0, "Simulation finished... Press enter to exit");
	refresh();
	getch();
	//endwin();
}

void Display::setChipCache(CacheL1 **pL1Array, CacheL2 *pL2, int pChipID) {
	_processors[pChipID].cachesL1[0] = pL1Array[0]->getCacheL1();
	_processors[pChipID].cachesL1[1] = pL1Array[1]->getCacheL1();
	_processors[pChipID].cacheL2 = pL2->getL2();
}

void Display::setSystemRAM(RAM::Block *pRam) {
	_ram = pRam;
}

void Display::setCoreInfo(Instruction_gen::Instruction *pInst, volatile bool *pRunning, const int pChipID,
		const int pCoreID) {
	if (pChipID == 0) {
		_coreInstructions[pCoreID] = pInst;
		_runningCores[pCoreID] = pRunning;
	} else {
		_coreInstructions[pCoreID + 2] = pInst;
		_runningCores[pCoreID + 2] = pRunning;
	}
}

void Display::printHeaders() {
	//HEADERS FOR L1 WINDOWS
	std::string headerL1 = "Block|State|Address|Data";
	mvwprintw(_P0_C0L1, 0, 1, "Chip 0 -- Core 0");
	mvwprintw(_P0_C0L1, 3, 1, headerL1.c_str());

	mvwprintw(_P0_C1L1, 0, 1, "Chip 0 -- Core 1");
	mvwprintw(_P0_C1L1, 3, 1, headerL1.c_str());

	mvwprintw(_P1_C0L1, 0, 1, "Chip 1 -- Core 0");
	mvwprintw(_P1_C0L1, 3, 1, headerL1.c_str());

	mvwprintw(_P1_C1L1, 0, 1, "Chip 1 -- Core 1");
	mvwprintw(_P1_C1L1, 3, 1, headerL1.c_str());

	//HEADERS FOR L2 WINDOWS
	std::stringstream headerL2;
	headerL2 << std::setw(5) << std::setfill(' ') << std::left << "Block" << "|" << std::setw(4) << "State"
			<< "|" << std::setw(10) << "Owner" << "|" << std::setw(7) << "Address" << "|" << std::setw(4)
			<< "Data";
	mvwprintw(_P0L2, 0, 1, "Chip 0 -- Cache L2");
	mvwprintw(_P0L2, 2, 1, headerL2.str().c_str());

	mvwprintw(_P1L2, 0, 1, "Chip 1 -- Cache L2");
	mvwprintw(_P1L2, 2, 1, headerL2.str().c_str());

	//HEADER FOR RAM WINDOW
	std::stringstream headerRAM;
	headerRAM << std::setw(7) << std::setfill(' ') << std::left << "Address" << "|" << std::setw(5) << "Owner"
			<< "|" << std::setw(4) << "Data";

	mvwprintw(_RAM, 0, 1, "RAM");
	mvwprintw(_RAM, 2, 1, headerRAM.str().c_str());
}

std::string Display::getInstuctionString(Instruction_gen::Instruction *pInst) {
	std::stringstream inst;
	switch (pInst->op) {
	case cons::inst::WRITE:
		inst << "P" << pInst->chip << "," << pInst->core << ": " << std::setw(5) << std::setfill(' ')
				<< cons::logger::TYPES[pInst->op] << " " << std::setw(2) << std::setfill(' ') << pInst->dest
				<< " " << std::setw(4) << std::hex << std::uppercase << pInst->data;
		break;
	case cons::inst::READ:
		inst << "P" << pInst->chip << "," << pInst->core << ": " << std::setw(5) << std::setfill(' ')
				<< cons::logger::TYPES[pInst->op] << " " << std::setw(2) << std::setfill(' ') << pInst->dest
				<< " " << std::setw(4) << std::hex << std::uppercase << " ";
		break;
	case cons::inst::CALC:
		inst << "P" << pInst->chip << "," << pInst->core << ": " << std::setw(5) << std::setfill(' ')
				<< cons::logger::TYPES[pInst->op] << " " << std::setw(2) << std::setfill(' ') << " "
				<< std::setw(5) << std::hex << std::uppercase << " ";
		break;
	}

	return inst.str();

}

void Display::printL1() {

	int pId = 0;
	int l1Id = 0;
	for (int i = 0; i < 4; i++) {
		if (i == 2) {
			pId++;
			l1Id = 0;
		};

		Instruction_gen::Instruction *current = _coreInstructions[i];
		std::string inst = this->getInstuctionString(current);

		for (int j = 0; j < cons::memory::L1_SIZE; j++) {
			CacheL1::Block *currentL1 = _processors[pId].cachesL1[l1Id];
			std::stringstream tmp;
			tmp << std::setw(5) << std::setfill(' ') << std::left << j << "|" << std::setw(5)
					<< cons::logger::STATES_L1[currentL1[j].state] << "|" << std::setw(7)
					<< currentL1[j].ramAddress << std::dec << "|" << std::setw(4) << std::hex
					<< std::uppercase << currentL1[j].data;

			switch (i) {
			case 0:
				if (*_runningCores[i] == false)
					mvwprintw(_P0_C0L1, 2, 1, "Core stopped");
				else
					mvwprintw(_P0_C0L1, 2, 1, "            ");
				mvwprintw(_P0_C0L1, 1, 1, inst.c_str());
				mvwprintw(_P0_C0L1, 4 + j, 1, tmp.str().c_str());
				break;
			case 1:
				if (*_runningCores[i] == false)
					mvwprintw(_P0_C1L1, 2, 1, "Core stopped");
				else
					mvwprintw(_P0_C1L1, 2, 1, "            ");
				mvwprintw(_P0_C1L1, 1, 1, inst.c_str());
				mvwprintw(_P0_C1L1, 4 + j, 1, tmp.str().c_str());
				break;
			case 2:
				if (*_runningCores[i] == false)
					mvwprintw(_P1_C0L1, 2, 1, "Core stopped");
				else
					mvwprintw(_P1_C0L1, 2, 1, "            ");
				mvwprintw(_P1_C0L1, 1, 1, inst.c_str());
				mvwprintw(_P1_C0L1, 4 + j, 1, tmp.str().c_str());
				break;
			case 3:
				if (*_runningCores[i] == false)
					mvwprintw(_P1_C1L1, 2, 1, "Core stopped");
				else
					mvwprintw(_P1_C1L1, 2, 1, "            ");
				mvwprintw(_P1_C1L1, 1, 1, inst.c_str());
				mvwprintw(_P1_C1L1, 4 + j, 1, tmp.str().c_str());
				break;
			}
		}

		l1Id++;
	}
}

void Display::printL2() {

	for (int i = 0; i < cons::NUMBER_OF_CHIPS; i++) {
		for (int j = 0; j < cons::memory::L2_SIZE; j++) {
			CacheL2::Block currentL2 = _processors[i].cacheL2[j];
			std::stringstream owners;
			int ownersSize = *(&currentL2.owners + 1) - currentL2.owners;
			for (int k = 0; k < ownersSize; k++) {
				CacheL2::Owner owner = currentL2.owners[k];
				if (owner.chip == -1) continue;
				owners << "P" << owner.chip << "," << owner.core;
				if ((k + 1 < ownersSize) & (currentL2.owners[k + 1].chip != -1)) owners << ";";
			}

			if (_processors[i].cacheL2[j].external) owners << ";E";

			std::stringstream tmp;
			tmp << std::setw(5) << std::setfill(' ') << std::left << j << "|" << std::setw(5)
					<< cons::logger::STATES_L2[currentL2.state] << "|" << std::setw(10) << owners.str() << "|"
					<< std::setw(7) << currentL2.ramAddress << "|" << std::setw(4) << std::hex
					<< std::uppercase << currentL2.data;

			if (i == 0)
				mvwprintw(_P0L2, 3 + j, 1, tmp.str().c_str());
			else
				mvwprintw(_P1L2, 3 + j, 1, tmp.str().c_str());
		}
	}
}

void Display::printRAM() {
	for (int i = 0; i < cons::memory::RAM_SIZE; i++) {
		RAM::Block currentBlock = _ram[i];

		std::stringstream owners;
		int ownersSize = *(&currentBlock.owners + 1) - currentBlock.owners;
		for (int j = 0; j < ownersSize; j++) {
			if (currentBlock.owners[j] == -1) continue;
			owners << "P" << currentBlock.owners[j];
			if ((j + 1 < ownersSize) & (currentBlock.owners[j + 1] != -1)) {
				owners << ";";
			}
		}

		std::stringstream tmp;
		tmp << std::setw(7) << std::setfill(' ') << std::left << i << "|" << std::setw(5) << owners.str()
				<< "|" << std::setw(4) << std::hex << std::uppercase << currentBlock.data;

		mvwprintw(_RAM, 3 + i, 1, tmp.str().c_str());
	}
}

void Display::refreshWindows() {
	wrefresh(_P0_C0L1);
	wrefresh(_P0_C1L1);
	wrefresh(_P1_C0L1);
	wrefresh(_P1_C1L1);

	wrefresh(_P0L2);
	wrefresh(_P1L2);

	wrefresh(_RAM);
}

void Display::update() {
	while (_running) {
		this->printL1();
		this->printL2();
		this->printRAM();

		this->refreshWindows();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
