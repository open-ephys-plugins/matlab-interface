/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2019 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include "MatlabEngineEditor.h"
#include "MatlabEngine.h"

MatlabEngineEditor::MatlabEngineEditor(MatlabEngine* parentNode, bool useDefaultParameterEditors = true)
	: GenericEditor(parentNode, useDefaultParameterEditors)

{

	engine = parentNode;

	desiredWidth = 150;

	connectButton = new UtilityButton("CONNECT", Font("Small Text", 13, Font::bold));
	connectButton->setRadius(3.0f);
	connectButton->setBounds(10, 25, 65, 25);
	connectButton->addListener(this);
	addAndMakeVisible(connectButton);


	testButton = new UtilityButton("RUN TEST", Font("Small Text", 13, Font::bold));
	testButton->setRadius(3.0f);
	testButton->setBounds(10, 55, 65, 25);
	testButton->addListener(this);
	addAndMakeVisible(testButton);

	socketButton = new UtilityButton("OPEN SOCKET", Font("Small Text", 13, Font::bold));
	socketButton->setRadius(3.0f);
	socketButton->setBounds(10, 85, 100, 25);
	socketButton->addListener(this);
	addAndMakeVisible(socketButton);

	//startTimer(UI_TIMER_PERIOD);


}

MatlabEngineEditor::~MatlabEngineEditor()
{

}

void MatlabEngineEditor::timerCallback()
{

	runTest();
	stopTimer();

}

void MatlabEngineEditor::buttonEvent(Button* button)
{

	if (button == connectButton)
	{
		auto startupTime = func_timer<std::chrono::milliseconds>::duration(&MatlabEngine::start, engine);
		auto dispTime = std::chrono::duration<double, std::milli>(startupTime).count();
		printf("MATLAB startup time: %1.3f [ms]\n", dispTime);
	}
	else if (button == testButton)
	{
		auto testTime = func_timer<std::chrono::milliseconds>::duration(&MatlabEngine::runTest, engine);
		auto dispTime = std::chrono::duration<double, std::micro>(testTime).count();
		printf("Run test time: %.0f [us]\n ", dispTime);
	}
	else if (button == socketButton)
	{
		std::cout << "Opening socket to MATLAB...\n" << std::endl;
		//auto openSocketTime = func_timer<std::chrono::milliseconds>::duration(&MatlabEngine::openSocket, engine);
		//auto dispTime = std::chrono::duration<double, std::milli>(openSocketTime).count();
		//printf("Open socket time: %0.f [ms]\n ", dispTime); 
	}
}

void MatlabEngineEditor::runTest()
{

	/* Simulate pressing socket button */
	std::cout << "[EDITOR] Simulating socket button press..." << std::endl;
	buttonEvent(socketButton);
	std::cout << "[EDITOR] Done simulating socket button press!" << std::endl;

}


