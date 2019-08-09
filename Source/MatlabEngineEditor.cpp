/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2014 Open Ephys

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

	startTimer(UI_TIMER_PERIOD);

}

MatlabEngineEditor::~MatlabEngineEditor()
{

}

void MatlabEngineEditor::timerCallback()
{

	//update the UI 

}
void MatlabEngineEditor::buttonEvent(Button* button)
{
	auto startupTime = func_timer<std::chrono::milliseconds>::duration(&MatlabEngine::startMatlab, engine);
	auto dispTime = std::chrono::duration<double, std::milli>(startupTime).count();
	printf("MATLAB startup time: %1.3f\n", dispTime); fflush(stdout);
}


