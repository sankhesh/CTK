/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#ifndef __ctkSpinBox_h
#define __ctkSpinBox_h

// Qt includes
#include <QWidget>
#include <QDoubleSpinBox>

// CTK includes
#include "ctkWidgetsExport.h"

class ctkSpinBoxPrivate;

/// \brief Smart spinbox item editor
/// Provides a smartspinbox editor for editing double precision value items.
/// \sa ctkSmartSpinBoxDelegate
class CTK_WIDGETS_EXPORT ctkSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
  Q_PROPERTY(int minimumDecimals READ minimumDecimals WRITE setMinimumDecimals)

public:
  typedef QDoubleSpinBox Superclass;

  /// Constructor, creates a QDoubleSpinBox
  ctkSpinBox(QWidget* parent = 0);

  /// Minimum precision to maintain on the spinbox editor.
  /// When the user enters a value, its precision will be compared to
  /// MinimumDecimals and the higher value will be set as the precision
  /// of the model. This is ignored if FixedPrecision is set to true.
  /// \sa ctkSmartSpinBoxDelegate::setMinimumDecimals(), setFixedPrecision()
  int minimumDecimals()const;
  void setMinimumDecimals(int newMinimumDecimals);

  /// Set whether the spinbox has a fixed precision
  /// If set to true, the number of decimals can be set by setDecimals().
  /// If set to false, one can set the number of minimum decimals.
  /// \sa QDoubleSpinBox::setDecimals(), setMinimumDecimals()
  bool fixedPrecision()const;
  void setFixedPrecision(bool enable);

  /// Reimplemented from QDoubleSpinBox. Before changing the value,
  /// this function emits the valueAboutToBeChanged() signal.
  /// \sa valueAboutToBeChanged(), value()
  void setValue(double newValue);

  /// Reimplemented from QDoubleSpinBox. After changing the decimals,
  /// this function emits the decimalsChanged() signal.
  /// \sa decimalsChanged()
  void setDecimals(int newDecimals);

Q_SIGNALS:
  /// This signal is emitted whenever the value of the spinbox is going to
  /// change due to either user events or programmatically.
  /// \sa setValue()
  void valueAboutToBeChanged();

  /// This signal is emitted whenever the precision of the spinbox changes.
  /// The new value of decimals is passed.
  /// \sa setDecimals(), decimals()
  void decimalsChanged(int decimals);

protected:
  /// Overloaded method from QAbstractSpinBox.
  /// Changes the precision of the spinBox to a large value so that the user can
  /// enter value as desired.
  void focusInEvent(QFocusEvent* event);

  /// Overloaded method from QAbstractSpinBox.
  /// Sets the precision of the spinbox based on the number of significant
  /// decimals in the value entered by the user.
  /// NOTE: On leaving focus, if the spinbox decimals are adjusted, it emits
  /// the decimalsChanged() signal first and then the valueChanged() signal.
  void focusOutEvent(QFocusEvent* event);

  /// Converts the double value to the given decimal precision.
  /// Returns whether the operation was successful.
  static bool fixDoublePrecision(double& value, const int decimals);

private Q_SLOTS:
  /// adjustDecimals is called when the user is finished editing the spinBox
  /// value. This method sets the precision of the spinBox based on the number
  /// of significant decimals in the value entered by the user.
  /// \sa ctk::significantDecimals()
  void adjustDecimals();

private:
  ctkSpinBoxPrivate* const d_ptr;
  Q_DECLARE_PRIVATE(ctkSpinBox);
  Q_DISABLE_COPY(ctkSpinBox);
};
#endif //__ctkSpinBox_h
