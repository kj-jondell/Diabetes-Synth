/********************************************************************************
** Form generated from reading UI file 'synth-controller.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNTH_2D_CONTROLLER_H
#define UI_SYNTH_2D_CONTROLLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGroupBox *envelope;
    QDial *attack;
    QLabel *label;
    QDial *decay;
    QLabel *label_2;
    QDial *sustain;
    QLabel *label_3;
    QDial *release;
    QLabel *label_4;
    QLabel *label_11;
    QDial *velocity_sensitivity;
    QLabel *label_12;
    QDial *envelope_random;
    QGroupBox *buffer;
    QDial *buffer_no;
    QLabel *label_5;
    QDial *buffer_random;
    QLabel *label_9;
    QGroupBox *tuning_group;
    QDial *detune_factor;
    QLabel *label_6;
    QComboBox *tuning;
    QLabel *label_7;
    QLabel *label_14;
    QDial *flutter;
    QLabel *label_15;
    QSpinBox *equal_temperament;
    QLabel *label_18;
    QSpinBox *root_freq;
    QSpinBox *octave;
    QLabel *label_19;
    QGroupBox *spatiality;
    QDial *stereo_spread;
    QLabel *label_8;
    QGroupBox *output_settings;
    QComboBox *port;
    QLabel *label_10;
    QLabel *label_13;
    QComboBox *midichannel;
    QGroupBox *buffer_2;
    QDial *buffer_no_2;
    QLabel *label_16;
    QDial *buffer_random_2;
    QLabel *label_17;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(500, 400);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Form->sizePolicy().hasHeightForWidth());
        Form->setSizePolicy(sizePolicy);
        Form->setMinimumSize(QSize(500, 400));
        Form->setMaximumSize(QSize(500, 400));
        envelope = new QGroupBox(Form);
        envelope->setObjectName(QString::fromUtf8("envelope"));
        envelope->setGeometry(QRect(10, 10, 111, 211));
        attack = new QDial(envelope);
        attack->setObjectName(QString::fromUtf8("attack"));
        attack->setGeometry(QRect(10, 30, 41, 61));
        attack->setMaximum(127);
        label = new QLabel(envelope);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 59, 16));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        decay = new QDial(envelope);
        decay->setObjectName(QString::fromUtf8("decay"));
        decay->setGeometry(QRect(60, 30, 41, 61));
        decay->setMaximum(127);
        label_2 = new QLabel(envelope);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 20, 59, 16));
        label_2->setFont(font);
        sustain = new QDial(envelope);
        sustain->setObjectName(QString::fromUtf8("sustain"));
        sustain->setGeometry(QRect(10, 90, 41, 61));
        sustain->setMaximum(127);
        label_3 = new QLabel(envelope);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 80, 59, 16));
        label_3->setFont(font);
        release = new QDial(envelope);
        release->setObjectName(QString::fromUtf8("release"));
        release->setGeometry(QRect(60, 90, 41, 61));
        release->setMaximum(127);
        label_4 = new QLabel(envelope);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 80, 59, 16));
        label_4->setFont(font);
        label_11 = new QLabel(envelope);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 140, 59, 16));
        label_11->setFont(font);
        velocity_sensitivity = new QDial(envelope);
        velocity_sensitivity->setObjectName(QString::fromUtf8("velocity_sensitivity"));
        velocity_sensitivity->setEnabled(false);
        velocity_sensitivity->setGeometry(QRect(10, 150, 41, 61));
        velocity_sensitivity->setMaximum(127);
        label_12 = new QLabel(envelope);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(60, 140, 59, 16));
        label_12->setFont(font);
        envelope_random = new QDial(envelope);
        envelope_random->setObjectName(QString::fromUtf8("envelope_random"));
        envelope_random->setEnabled(false);
        envelope_random->setGeometry(QRect(60, 150, 41, 61));
        envelope_random->setMaximum(127);
        buffer = new QGroupBox(Form);
        buffer->setObjectName(QString::fromUtf8("buffer"));
        buffer->setGeometry(QRect(130, 10, 131, 211));
        buffer_no = new QDial(buffer);
        buffer_no->setObjectName(QString::fromUtf8("buffer_no"));
        buffer_no->setGeometry(QRect(10, 30, 41, 61));
        buffer_no->setMaximum(255);
        buffer_no->setWrapping(false);
        label_5 = new QLabel(buffer);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 20, 59, 16));
        label_5->setFont(font);
        buffer_random = new QDial(buffer);
        buffer_random->setObjectName(QString::fromUtf8("buffer_random"));
        buffer_random->setEnabled(false);
        buffer_random->setGeometry(QRect(70, 30, 41, 61));
        buffer_random->setMaximum(255);
        buffer_random->setWrapping(false);
        label_9 = new QLabel(buffer);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(70, 20, 59, 16));
        label_9->setFont(font);
        tuning_group = new QGroupBox(Form);
        tuning_group->setObjectName(QString::fromUtf8("tuning_group"));
        tuning_group->setGeometry(QRect(270, 10, 181, 211));
        detune_factor = new QDial(tuning_group);
        detune_factor->setObjectName(QString::fromUtf8("detune_factor"));
        detune_factor->setGeometry(QRect(10, 30, 41, 61));
        detune_factor->setMaximum(127);
        label_6 = new QLabel(tuning_group);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 20, 81, 16));
        label_6->setFont(font);
        tuning = new QComboBox(tuning_group);
        tuning->addItem(QString());
        tuning->addItem(QString());
        tuning->addItem(QString());
        tuning->addItem(QString());
        tuning->addItem(QString());
        tuning->setObjectName(QString::fromUtf8("tuning"));
        tuning->setGeometry(QRect(60, 40, 121, 32));
        tuning->setFont(font);
        label_7 = new QLabel(tuning_group);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(100, 20, 81, 16));
        label_7->setFont(font);
        label_14 = new QLabel(tuning_group);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 80, 81, 16));
        label_14->setFont(font);
        flutter = new QDial(tuning_group);
        flutter->setObjectName(QString::fromUtf8("flutter"));
        flutter->setGeometry(QRect(10, 90, 41, 61));
        flutter->setMaximum(127);
        label_15 = new QLabel(tuning_group);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(70, 80, 121, 20));
        label_15->setFont(font);
        equal_temperament = new QSpinBox(tuning_group);
        equal_temperament->setObjectName(QString::fromUtf8("equal_temperament"));
        equal_temperament->setEnabled(false);
        equal_temperament->setGeometry(QRect(90, 100, 42, 22));
        equal_temperament->setMinimum(5);
        equal_temperament->setMaximum(29);
        equal_temperament->setValue(12);
        label_18 = new QLabel(tuning_group);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(70, 130, 81, 16));
        label_18->setFont(font);
        root_freq = new QSpinBox(tuning_group);
        root_freq->setObjectName(QString::fromUtf8("root_freq"));
        root_freq->setEnabled(false);
        root_freq->setGeometry(QRect(70, 150, 42, 22));
        root_freq->setMinimum(220);
        root_freq->setMaximum(660);
        root_freq->setValue(220);
        octave = new QSpinBox(tuning_group);
        octave->setObjectName(QString::fromUtf8("octave"));
        octave->setEnabled(false);
        octave->setGeometry(QRect(130, 150, 42, 22));
        octave->setMinimum(-3);
        octave->setMaximum(3);
        octave->setValue(0);
        label_19 = new QLabel(tuning_group);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(130, 130, 81, 16));
        label_19->setFont(font);
        spatiality = new QGroupBox(Form);
        spatiality->setObjectName(QString::fromUtf8("spatiality"));
        spatiality->setGeometry(QRect(10, 220, 111, 161));
        stereo_spread = new QDial(spatiality);
        stereo_spread->setObjectName(QString::fromUtf8("stereo_spread"));
        stereo_spread->setEnabled(false);
        stereo_spread->setGeometry(QRect(10, 30, 41, 61));
        stereo_spread->setMaximum(255);
        stereo_spread->setWrapping(false);
        label_8 = new QLabel(spatiality);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 20, 59, 16));
        label_8->setFont(font);
        output_settings = new QGroupBox(Form);
        output_settings->setObjectName(QString::fromUtf8("output_settings"));
        output_settings->setGeometry(QRect(270, 220, 131, 161));
        port = new QComboBox(output_settings);
        port->setObjectName(QString::fromUtf8("port"));
        port->setEnabled(true);
        port->setGeometry(QRect(40, 20, 81, 32));
        port->setFont(font);
        label_10 = new QLabel(output_settings);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 20, 31, 31));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_13 = new QLabel(output_settings);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 50, 51, 31));
        label_13->setFont(font);
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        midichannel = new QComboBox(output_settings);
        midichannel->setObjectName(QString::fromUtf8("midichannel"));
        midichannel->setEnabled(true);
        midichannel->setGeometry(QRect(60, 50, 61, 32));
        midichannel->setFont(font);
        buffer_2 = new QGroupBox(Form);
        buffer_2->setObjectName(QString::fromUtf8("buffer_2"));
        buffer_2->setGeometry(QRect(130, 220, 131, 161));
        buffer_no_2 = new QDial(buffer_2);
        buffer_no_2->setObjectName(QString::fromUtf8("buffer_no_2"));
        buffer_no_2->setEnabled(false);
        buffer_no_2->setGeometry(QRect(10, 30, 41, 61));
        buffer_no_2->setMaximum(255);
        buffer_no_2->setWrapping(false);
        label_16 = new QLabel(buffer_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 20, 59, 16));
        label_16->setFont(font);
        buffer_random_2 = new QDial(buffer_2);
        buffer_random_2->setObjectName(QString::fromUtf8("buffer_random_2"));
        buffer_random_2->setEnabled(false);
        buffer_random_2->setGeometry(QRect(70, 30, 41, 61));
        buffer_random_2->setMaximum(255);
        buffer_random_2->setWrapping(false);
        label_17 = new QLabel(buffer_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(70, 20, 59, 16));
        label_17->setFont(font);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        envelope->setTitle(QCoreApplication::translate("Form", "Envelope", nullptr));
        label->setText(QCoreApplication::translate("Form", "Attack", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "Decay", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "Sustain", nullptr));
        label_4->setText(QCoreApplication::translate("Form", "Release", nullptr));
        label_11->setText(QCoreApplication::translate("Form", "Velocity", nullptr));
        label_12->setText(QCoreApplication::translate("Form", "Random", nullptr));
        buffer->setTitle(QCoreApplication::translate("Form", "Buffer", nullptr));
        label_5->setText(QCoreApplication::translate("Form", "Buffer no.", nullptr));
        label_9->setText(QCoreApplication::translate("Form", "Random", nullptr));
        tuning_group->setTitle(QCoreApplication::translate("Form", "Tuning", nullptr));
        label_6->setText(QCoreApplication::translate("Form", "Detune factor", nullptr));
        tuning->setItemText(0, QCoreApplication::translate("Form", "Just Intonation", nullptr));
        tuning->setItemText(1, QCoreApplication::translate("Form", "Equal Temperament", nullptr));
        tuning->setItemText(2, QCoreApplication::translate("Form", "Harmonic Series 24", nullptr));
        tuning->setItemText(3, QCoreApplication::translate("Form", "Bohlen-Pierce", nullptr));
        tuning->setItemText(4, QCoreApplication::translate("Form", "Meantone, 1/5 Pythagorean Comma", nullptr));

        label_7->setText(QCoreApplication::translate("Form", "Tuning", nullptr));
        label_14->setText(QCoreApplication::translate("Form", "Flutter", nullptr));
        label_15->setText(QCoreApplication::translate("Form", "Equal temperament", nullptr));
        label_18->setText(QCoreApplication::translate("Form", "Root freq", nullptr));
        label_19->setText(QCoreApplication::translate("Form", "Octave", nullptr));
        spatiality->setTitle(QCoreApplication::translate("Form", "Spatiality", nullptr));
        label_8->setText(QCoreApplication::translate("Form", "Spread", nullptr));
        output_settings->setTitle(QCoreApplication::translate("Form", "Output settings", nullptr));
        label_10->setText(QCoreApplication::translate("Form", "Port:", nullptr));
        label_13->setText(QCoreApplication::translate("Form", "MIDI ch:", nullptr));
        buffer_2->setTitle(QCoreApplication::translate("Form", "Filter", nullptr));
        label_16->setText(QCoreApplication::translate("Form", "Envelope", nullptr));
        label_17->setText(QCoreApplication::translate("Form", "Random", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNTH_2D_CONTROLLER_H
