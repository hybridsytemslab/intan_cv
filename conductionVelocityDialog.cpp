//  ------------------------------------------------------------------------
//
//  This file is part of the Intan Technologies RHD2000 Interface
//  Version 1.3
//  Copyright (C) 2013 Intan Technologies
//
//  ------------------------------------------------------------------------
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui>
#include <iostream>

#include "qtincludes.h"

#include "globalconstants.h"
#include "conductionVelocityDialog.h"
#include "signalchannel.h"
#include "signalgroup.h"
#include "spikeplot_cv.h"

// Spike scope dialog.
// This dialog allows users to view 3-msec snippets of neural spikes triggered
// either from a selectable voltage threshold or a digital input threshold.  Multiple
// spikes are superimposed on the display so that users can compare spike shapes.

ConductionVelocityDialog::ConductionVelocityDialog(SignalProcessor *inSignalProcessor, SignalSources *inSignalSources,
                                   SignalChannel *initialChannel, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Conduction Velocity"));

    signalProcessor = inSignalProcessor;
    signalSources = inSignalSources;

    spikePlot_CV = new SpikePlot_CV(signalProcessor, initialChannel, this, this);
    spikePlot_CV2 = new SpikePlot_CV(signalProcessor, initialChannel, this, this);
    currentChannel = initialChannel;

    resetToZeroButton = new QPushButton(tr("Zero"));
    resetToZeroButton2 = new QPushButton(tr("Zero"));
    clearScopeButton = new QPushButton(tr("Clear Scope"));
    applyToAllButton = new QPushButton(tr("Apply to Entire Port"));

    connect(resetToZeroButton, SIGNAL(clicked()),
            this, SLOT(resetThresholdToZero()));
    connect(resetToZeroButton2, SIGNAL(clicked()),
            this, SLOT(resetThresholdToZero2()));
    connect(clearScopeButton, SIGNAL(clicked()),
            this, SLOT(clearScope()));
    connect(applyToAllButton, SIGNAL(clicked()),
            this, SLOT(applyToAll()));

    triggerTypeComboBox = new QComboBox();
    triggerTypeComboBox->addItem(tr("Voltage Threshold"));
    triggerTypeComboBox->addItem(tr("Digital Input"));
    triggerTypeComboBox->setCurrentIndex(0);

    connect(triggerTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setTriggerType(int)));

    thresholdSpinBox = new QSpinBox();
    thresholdSpinBox->setRange(-5000, 5000);
    thresholdSpinBox->setSingleStep(5);
    thresholdSpinBox->setValue(0);


    thresholdSpinBox2 = new QSpinBox();
    thresholdSpinBox2->setRange(-5000, 5000);
    thresholdSpinBox2->setSingleStep(5);
    thresholdSpinBox2->setValue(0);

    connect(thresholdSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(setVoltageThreshold(int)));
    connect(thresholdSpinBox2, SIGNAL(valueChanged(int)),
            this, SLOT(setVoltageThreshold(int)));


    channelSelectSpinBox = new QSpinBox();
    channelSelectSpinBox->setRange(1,32);
    channelSelectSpinBox->setSingleStep(1);
    channelSelectSpinBox->setValue(1);

    channelSelectSpinBox2 = new QSpinBox();
    channelSelectSpinBox2->setRange(1,32);
    channelSelectSpinBox2->setSingleStep(1);
    channelSelectSpinBox2->setValue(1);

    QHBoxLayout *thresholdSpinBoxLayout = new QHBoxLayout;
    thresholdSpinBoxLayout->addWidget(resetToZeroButton);
    thresholdSpinBoxLayout->addWidget(thresholdSpinBox);
    thresholdSpinBoxLayout->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    // thresholdSpinBoxLayout->addStretch(1);

    QHBoxLayout *thresholdSpinBoxLayout2 = new QHBoxLayout;
    thresholdSpinBoxLayout2->addWidget(resetToZeroButton2);
    thresholdSpinBoxLayout2->addWidget(thresholdSpinBox2);
    thresholdSpinBoxLayout2->addWidget(new QLabel(QSTRING_MU_SYMBOL + "V"));
    // thresholdSpinBoxLayout->addStretch(1);

    QHBoxLayout *channelSelectLayout = new QHBoxLayout;
    channelSelectLayout->addWidget(channelSelectSpinBox);

    QHBoxLayout *channelSelectLayout2 = new QHBoxLayout;
    channelSelectLayout2->addWidget(channelSelectSpinBox2);


    digitalInputComboBox = new QComboBox();
    digitalInputComboBox->addItem(tr("Digital Input 0"));
    digitalInputComboBox->addItem(tr("Digital Input 1"));
    digitalInputComboBox->addItem(tr("Digital Input 2"));
    digitalInputComboBox->addItem(tr("Digital Input 3"));
    digitalInputComboBox->addItem(tr("Digital Input 4"));
    digitalInputComboBox->addItem(tr("Digital Input 5"));
    digitalInputComboBox->addItem(tr("Digital Input 6"));
    digitalInputComboBox->addItem(tr("Digital Input 7"));
    digitalInputComboBox->addItem(tr("Digital Input 8"));
    digitalInputComboBox->addItem(tr("Digital Input 9"));
    digitalInputComboBox->addItem(tr("Digital Input 10"));
    digitalInputComboBox->addItem(tr("Digital Input 11"));
    digitalInputComboBox->addItem(tr("Digital Input 12"));
    digitalInputComboBox->addItem(tr("Digital Input 13"));
    digitalInputComboBox->addItem(tr("Digital Input 14"));
    digitalInputComboBox->addItem(tr("Digital Input 15"));
    digitalInputComboBox->setCurrentIndex(0);

    connect(digitalInputComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setDigitalInput(int)));

    edgePolarityComboBox = new QComboBox();
    edgePolarityComboBox->addItem(tr("Rising Edge"));
    edgePolarityComboBox->addItem(tr("Falling Edge"));
    edgePolarityComboBox->setCurrentIndex(0);

    connect(edgePolarityComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setEdgePolarity(int)));

    numSpikesComboBox = new QComboBox();
    numSpikesComboBox->addItem(tr("Show 10 Spikes"));
    numSpikesComboBox->addItem(tr("Show 20 Spikes"));
    numSpikesComboBox->addItem(tr("Show 30 Spikes"));
    numSpikesComboBox->setCurrentIndex(1);

    connect(numSpikesComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setNumSpikes(int)));

    yScaleList.append(50);
    yScaleList.append(100);
    yScaleList.append(200);
    yScaleList.append(500);
    yScaleList.append(1000);
    yScaleList.append(2000);
    yScaleList.append(5000);

    yScaleComboBox = new QComboBox();
    for (int i = 0; i < yScaleList.size(); ++i) {
        yScaleComboBox->addItem("+/-" + QString::number(yScaleList[i]) +
                                " " + QSTRING_MU_SYMBOL + "V");
    }
    yScaleComboBox->setCurrentIndex(4);

    connect(yScaleComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeYScale(int)));

    QVBoxLayout *triggerLayout = new QVBoxLayout;
    triggerLayout->addWidget(new QLabel(tr("Channel Number:")));
    triggerLayout->addLayout(channelSelectLayout);
    triggerLayout->addWidget(new QLabel(tr("Voltage Threshold:")));
    triggerLayout->addLayout(thresholdSpinBoxLayout);

    QVBoxLayout *triggerLayout2 = new QVBoxLayout;
    triggerLayout->addWidget(new QLabel(tr("Channel Number:")));
    triggerLayout->addLayout(channelSelectLayout2);
    triggerLayout2->addWidget(new QLabel(tr("Voltage Threshold:")));
    triggerLayout2->addLayout(thresholdSpinBoxLayout2);

    QVBoxLayout *displayLayout = new QVBoxLayout;
    displayLayout->addWidget(new QLabel(tr("Voltage Scale:")));
    displayLayout->addWidget(yScaleComboBox);
    displayLayout->addWidget(numSpikesComboBox);
    displayLayout->addWidget(clearScopeButton);

    QGroupBox *triggerGroupBox = new QGroupBox(tr("Start Channel"));
    triggerGroupBox->setLayout(triggerLayout);


    QGroupBox *triggerGroupBox2 = new QGroupBox(tr("End Channel"));
    triggerGroupBox2->setLayout(triggerLayout2);

    QGroupBox *displayGroupBox = new QGroupBox(tr("Display Settings"));
    displayGroupBox->setLayout(displayLayout);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(triggerGroupBox);
    leftLayout->addWidget(triggerGroupBox2);
    //leftLayout->addWidget(applyToAllButton);
    leftLayout->addWidget(displayGroupBox);
    leftLayout->addStretch(1);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(spikePlot_CV);
    rightLayout->addStretch(1);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    mainLayout->addWidget(spikePlot_CV2);

    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 2);

    setLayout(mainLayout);

    setTriggerType(triggerTypeComboBox->currentIndex());
    setNumSpikes(numSpikesComboBox->currentIndex());
    setVoltageThreshold(thresholdSpinBox->value());
    setDigitalInput(digitalInputComboBox->currentIndex());
    setEdgePolarity(edgePolarityComboBox->currentIndex());
}

void ConductionVelocityDialog::changeYScale(int index)
{
    spikePlot_CV->setYScale(yScaleList[index]);
}

void ConductionVelocityDialog::setYScale(int index)
{
    yScaleComboBox->setCurrentIndex(index);
    spikePlot_CV->setYScale(yScaleList[index]);
}

void ConductionVelocityDialog::setSampleRate(double newSampleRate)
{
    spikePlot_CV->setSampleRate(newSampleRate);
}

// Select a voltage trigger if index == 0.
// Select a digital input trigger if index == 1.
void ConductionVelocityDialog::setTriggerType(int index)
{
    thresholdSpinBox->setEnabled(index == 0);
    resetToZeroButton->setEnabled(index == 0);
    digitalInputComboBox->setEnabled(index == 1);
    edgePolarityComboBox->setEnabled(index == 1);
    spikePlot_CV->setVoltageTriggerMode(index == 0);
}

void ConductionVelocityDialog::resetThresholdToZero()
{
    thresholdSpinBox->setValue(0);
}
void ConductionVelocityDialog::resetThresholdToZero2()
{
    thresholdSpinBox2->setValue(0);
}

void ConductionVelocityDialog::updateWaveform(int numBlocks)
{
    spikePlot_CV->updateWaveform(numBlocks);
}

// Set number of spikes plotted superimposed.
void ConductionVelocityDialog::setNumSpikes(int index)
{
    int num;

    switch (index) {
    case 0: num = 10; break;
    case 1: num = 20; break;
    case 2: num = 30; break;
    }

    spikePlot_CV->setMaxNumSpikeWaveforms(num);
}

void ConductionVelocityDialog::clearScope()
{
    spikePlot_CV->clearScope();
}

void ConductionVelocityDialog::setDigitalInput(int index)
{
    spikePlot_CV->setDigitalTriggerChannel(index);
}

void ConductionVelocityDialog::setVoltageThreshold(int value)
{
    spikePlot_CV->setVoltageThreshold(value);
}

void ConductionVelocityDialog::setVoltageThresholdDisplay(int value)
{
    thresholdSpinBox->setValue(value);
}

void ConductionVelocityDialog::setEdgePolarity(int index)
{
    spikePlot_CV->setDigitalEdgePolarity(index == 0);
}

// Set Spike Scope to a new signal channel source.
void ConductionVelocityDialog::setNewChannel(SignalChannel* newChannel)
{
    spikePlot_CV->setNewChannel(newChannel);
    currentChannel = newChannel;
    if (newChannel->voltageTriggerMode) {
        triggerTypeComboBox->setCurrentIndex(0);
    } else {
        triggerTypeComboBox->setCurrentIndex(1);
    }
    thresholdSpinBox->setValue(newChannel->voltageThreshold);
    digitalInputComboBox->setCurrentIndex(newChannel->digitalTriggerChannel);
    if (newChannel->digitalEdgePolarity) {
        edgePolarityComboBox->setCurrentIndex(0);
    } else {
        edgePolarityComboBox->setCurrentIndex(1);
    }
}

void ConductionVelocityDialog::expandYScale()
{
    if (yScaleComboBox->currentIndex() > 0) {
        yScaleComboBox->setCurrentIndex(yScaleComboBox->currentIndex() - 1);
        changeYScale(yScaleComboBox->currentIndex());
    }
}

void ConductionVelocityDialog::contractYScale()
{
    if (yScaleComboBox->currentIndex() < yScaleList.size() - 1) {
        yScaleComboBox->setCurrentIndex(yScaleComboBox->currentIndex() + 1);
        changeYScale(yScaleComboBox->currentIndex());
    }
}

// Apply trigger settings to all channels on selected port.
void ConductionVelocityDialog::applyToAll()
{
    QMessageBox::StandardButton r;
    r = QMessageBox::question(this, tr("Trigger Settings"),
                                 tr("Do you really want to copy the current channel's trigger "
                                    "settings to <b>all</b> amplifier channels on this port?"),
                                 QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::Yes) {
        for (int i = 0; i < currentChannel->signalGroup->numChannels(); ++i) {
            currentChannel->signalGroup->channel[i].voltageTriggerMode = currentChannel->voltageTriggerMode;
            currentChannel->signalGroup->channel[i].voltageThreshold = currentChannel->voltageThreshold;
            currentChannel->signalGroup->channel[i].digitalTriggerChannel = currentChannel->digitalTriggerChannel;
            currentChannel->signalGroup->channel[i].digitalEdgePolarity = currentChannel->digitalEdgePolarity;
        }
    }
}
