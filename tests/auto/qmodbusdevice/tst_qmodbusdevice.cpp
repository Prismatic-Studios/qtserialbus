/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialBus module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include <QtSerialBus/qmodbusdevice.h>

class DummyDevice : public QModbusDevice
{
    Q_OBJECT
    friend class tst_QModbusDevice;

public:
    DummyDevice()
    {
        qRegisterMetaType<QModbusDevice::State>("QModbusDevice::State");
    }

protected:
    bool open() override { return openState; }
    void close() override {}

    bool openState = false;
};

class tst_QModbusDevice : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void connectDevice();
    void disconnectDevice();
    void state();
    void error();

private:
    DummyDevice *device;
};

void tst_QModbusDevice::initTestCase()
{
    device = new DummyDevice();
}

void tst_QModbusDevice::cleanupTestCase()
{
    delete device;
}

void tst_QModbusDevice::connectDevice()
{
    device->openState = false;
    QVERIFY(!device->connectDevice());
    QCOMPARE(device->state(), QModbusDevice::UnconnectedState);

    device->openState = true;
    QVERIFY(device->connectDevice());
    QCOMPARE(device->state(), QModbusDevice::ConnectingState);

    device->disconnectDevice();
    QCOMPARE(device->state(), QModbusDevice::ClosingState);

    device->setState(QModbusDevice::ClosingState);
    QVERIFY(!device->connectDevice());
}

void tst_QModbusDevice::disconnectDevice()
{
    //see QTBUG-66648
    DummyDevice dev;
    QCOMPARE(dev.state(), QModbusDevice::UnconnectedState);
    dev.disconnectDevice();
    QCOMPARE(dev.state(), QModbusDevice::UnconnectedState);
}

void tst_QModbusDevice::state()
{
    device->setState(QModbusDevice::ConnectedState);
    QCOMPARE(device->state(), QModbusDevice::ConnectedState);
    QSignalSpy spy(device, &DummyDevice::stateChanged);
    device->setState(QModbusDevice::UnconnectedState);
    QCOMPARE(device->state(), QModbusDevice::UnconnectedState);
    device->setState(QModbusDevice::UnconnectedState);
    QCOMPARE(device->state(), QModbusDevice::UnconnectedState);

    QCOMPARE(spy.count(), 1);
}

void tst_QModbusDevice::error()
{
    QCOMPARE(device->error(), QModbusDevice::NoError);
    QVERIFY(device->errorString().isEmpty());

    QString errorString("error string");
    device->setError(errorString, QModbusDevice::ConnectionError);

    QCOMPARE(device->error(), QModbusDevice::ConnectionError);
    QCOMPARE(device->errorString(), errorString);
}

QTEST_MAIN(tst_QModbusDevice)

#include "tst_qmodbusdevice.moc"
