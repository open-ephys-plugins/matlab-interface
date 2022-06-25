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

#ifndef __MATLAB_INTERFACE_EDITOR_H__
#define __MATLAB_INTERFACE_EDITOR_H__

#include <EditorHeaders.h>
#include "MatlabInterface.h"

#define UI_TIMER_PERIOD 2000 //Update the UI every UI_TIMER_PERIOD ms

class MatlabInterface;

class ConnectionViewer : 
	public Thread,
	public Component, 
	public Button::Listener
{
public:

	/** Constructor */
	ConnectionViewer(MatlabInterface* parentNode);

	/** Destructor */
	~ConnectionViewer();

	/** Respond to button clicks*/
	void buttonClicked(Button*);

	int buttonSize;
	int width;
	int height;

	bool isConnected;

private:
	MatlabInterface* interface;

	/** Run connection thread*/
	void run() override;
};

class MatlabInterfaceEditor : public GenericEditor,
	public Button::Listener
{
public:

	/** Constructor */
	MatlabInterfaceEditor(MatlabInterface* parentNode);

	/** Destructor */
	virtual ~MatlabInterfaceEditor();

	/** Responds to "connect" button */
	void buttonClicked(Button* button);

private:

	int count;

	MatlabInterface* interface;

	ScopedPointer<ConnectionViewer> viewer;

	std::unique_ptr<UtilityButton> connectButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatlabInterfaceEditor);

};

#endif  // __MATLAB_ENGINE_EDITOR_H__
