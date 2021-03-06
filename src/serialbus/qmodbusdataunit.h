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

#ifndef QMODBUSDATAUNIT_H
#define QMODBUSDATAUNIT_H

#include <QtCore/qlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qmetatype.h>

QT_BEGIN_NAMESPACE

class QModbusDataUnit
{
public:
    enum RegisterType {
        Invalid,
        DiscreteInputs,
        Coils,
        InputRegisters,
        HoldingRegisters
    };

    QModbusDataUnit() = default;

    constexpr explicit QModbusDataUnit(RegisterType type) noexcept
        : m_type(type)
        , m_startAddress(0)
    {}

    QModbusDataUnit(RegisterType type, int newStartAddress, quint16 newValueCount)
        : QModbusDataUnit(type, newStartAddress, QList<quint16>(newValueCount))
    {}

    QModbusDataUnit(RegisterType type, int newStartAddress, const QList<quint16> &newValues)
        : m_type(type)
        , m_startAddress(newStartAddress)
        , m_values(newValues)
        , m_valueCount(newValues.size())
    {}

    RegisterType registerType() const { return m_type; }
    void setRegisterType(RegisterType type) { m_type = type; }

    inline int startAddress() const { return m_startAddress; }
    inline void setStartAddress(int newAddress) { m_startAddress = newAddress; }

    inline QList<quint16> values() const { return m_values; }
    inline void setValues(const QList<quint16> &newValues)
    {
        m_values = newValues;
        m_valueCount = newValues.size();
    }

    inline qsizetype valueCount() const { return m_valueCount; }
    inline void setValueCount(qsizetype newCount) { m_valueCount = newCount; }

    inline void setValue(qsizetype index, quint16 newValue)
    {
        if (m_values.isEmpty() || index >= m_values.size())
            return;
        m_values[index] = newValue;
    }
    inline quint16 value(qsizetype index) const { return m_values.value(index); }

    bool isValid() const { return m_type != Invalid && m_startAddress != -1; }

private:
    RegisterType m_type = Invalid;
    int m_startAddress = -1;
    QList<quint16> m_values;
    qsizetype m_valueCount = 0;
};
typedef QMap<QModbusDataUnit::RegisterType, QModbusDataUnit> QModbusDataUnitMap;

Q_DECLARE_TYPEINFO(QModbusDataUnit, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(QModbusDataUnit::RegisterType, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QModbusDataUnit::RegisterType)

#endif // QMODBUSDATAUNIT_H
