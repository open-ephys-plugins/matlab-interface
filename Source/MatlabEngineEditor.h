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

#ifndef __MATLAB_ENGINE_EDITOR_H__
#define __MATLAB_ENGINE_EDITOR_H__

#include <EditorHeaders.h>
#include "MatlabEngine.h"

#define UI_TIMER_PERIOD 2000 //Update the UI every UI_TIMER_PERIOD ms

class MatlabEngine;

class MatlabEngineEditor : public GenericEditor, public ComboBoxListener
{
public:

	MatlabEngineEditor(MatlabEngine* parentNode, bool useDefaultParameterEditors);
	virtual ~MatlabEngineEditor();

	void updateSettings();
	void timerCallback();

	void buttonEvent(Button* button);
	void comboBoxChanged(ComboBox* combo);

private:

	MatlabEngine* engine;

	ScopedPointer<Label> hostLabel;
	ScopedPointer<Label> hostEntry;
	ScopedPointer<Label> portLabel;
	ScopedPointer<Label> portEntry;
	ScopedPointer<UtilityButton> connectButton;
	ScopedPointer<Label> channelLabel;
	ScopedPointer<ComboBox> channelSelect;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatlabEngineEditor);

};

#endif  // __MATLAB_ENGINE_EDITOR_H__
