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

	hostLabel = new Label("HostLabel", "Host:");
	hostLabel->setBounds(10, 25, 50, 20);
	addAndMakeVisible(hostLabel);

	hostEntry = new Label("HostEntry", "127.0.0.1");
	hostEntry->setEditable(true);
	hostEntry->setBounds(60, 25, 75, 20);
	addAndMakeVisible(hostEntry);

	portLabel = new Label("PortLabel", "Port:");
	portLabel->setBounds(10, 50, 50, 20);
	addAndMakeVisible(portLabel);

	portEntry = new Label("PortEntry", "1234");
	portEntry->setEditable(true);
	portEntry->setBounds(60, 50, 75, 20);
	addAndMakeVisible(portEntry);

	connectButton = new UtilityButton("CONNECT", Font("Small Text", 13, Font::bold));
	connectButton->setRadius(3.0f);
	connectButton->setBounds(20, 75, 100, 20);
	connectButton->addListener(this);
	addAndMakeVisible(connectButton);

	channelLabel = new Label("ChannelLabel", "Channel:");
	channelLabel->setBounds(10, 100, 70, 20);
	addAndMakeVisible(channelLabel);

	channelSelect = new ComboBox("ChannelSelect");
	channelSelect->setBounds(80, 100, 50, 20);
	addAndMakeVisible(channelSelect);
}

MatlabEngineEditor::~MatlabEngineEditor()
{

}

void MatlabEngineEditor::timerCallback()
{

	stopTimer();

}

void MatlabEngineEditor::buttonEvent(Button* button)
{
}


void MatlabEngineEditor::comboBoxChanged(ComboBox *combo)
{
	CoreServices::updateSignalChain(this);
}



