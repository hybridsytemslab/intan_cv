#ifndef SPIKEPLOT_CV_H
#define SPIKEPLOT_CV_H
#define SPIKEPLOT_X_SIZE 320
#define SPIKEPLOT_Y_SIZE 346

#include <QWidget>
#include "qtincludes.h"

using namespace std;

class SignalProcessor;
class SpikeScopeDialog;
class ConductionVelocityDialog;
class SignalChannel;

class SpikePlot_CV : public QWidget
{
    Q_OBJECT
public:
    explicit SpikePlot_CV(SignalProcessor *inSignalProcessor, SignalChannel *initialChannel,
                       ConductionVelocityDialog *inConductionVelocityDialog, QWidget *parent = 0);
    void setYScale(int newYScale);
    void setSampleRate(double newSampleRate);
    void updateWaveform(int numBlocks);
    void setMaxNumSpikeWaveforms(int num);
    void clearScope();
    void setVoltageTriggerMode(bool voltageMode);
    void setVoltageThreshold(int threshold);
    void setDigitalTriggerChannel(int channel);
    void setDigitalEdgePolarity(bool risingEdge);
    void setNewChannel(SignalChannel* newChannel);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent* event);

private:
    void drawAxisLines();
    void drawAxisText();
    void updateSpikePlot(double rms);
    void initializeDisplay();

    SignalProcessor *signalProcessor;
    SpikeScopeDialog *spikeScopeDialog;
    ConductionVelocityDialog *conductionVelocityDialog;


    QVector<QVector<double> > spikeWaveform;
    QVector<double> spikeWaveformBuffer;
    QVector<int> digitalInputBuffer;
    int spikeWaveformIndex;
    int numSpikeWaveforms;
    int maxNumSpikeWaveforms;
    bool voltageTriggerMode;
    int voltageThreshold;
    int digitalTriggerChannel;
    bool digitalEdgePolarity;

    int preTriggerTSteps;
    int totalTSteps;
    bool startingNewChannel;
    int rmsDisplayPeriod;

    SignalChannel *selectedChannel;

    QRect frame;

    double tStepMsec;
    int yScale;
    double savedRms;

    QPixmap pixmap;

    QVector<QVector<QColor> > scopeColors;

};
#endif // SPIKEPLOT_CV_H
