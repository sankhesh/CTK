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

#ifndef __ctkSmartSpinBoxEditor_h
#define __ctkSmartSpinBoxEditor_h

// Qt includes
#include <QWidget>
#include <QDoubleSpinBox>

// CTK includes
#include "ctkWidgetsExport.h"

class ctkSmartSpinBoxEditorPrivate;

/// \brief Smart spinbox item editor
/// Provides a smartspinbox editor for editing double precision value items.
/// \sa ctkSmartSpinBoxDelegate
class CTK_WIDGETS_EXPORT ctkSmartSpinBoxEditor : public QDoubleSpinBox
{
  Q_OBJECT
  Q_PROPERTY(int minimumDecimals READ minimumDecimals WRITE setMinimumDecimals)

public:
  /// Constructor, creates a QDoubleSpinBox
  ctkSmartSpinBoxEditor(QWidget* parent = 0);

  /// Minimum precision to maintain on the spinbox editor.
  /// When the user enters a value, its precision will be compared to
  /// MinimumDecimals and the higher value will be set as the precision
  /// of the model.
  /// \sa ctkSmartSpinBoxDelegate::setMinimumDecimals
  int minimumDecimals()const;
  void setMinimumDecimals(int newMinimumDecimals);

protected:
  /// Overloaded method from QAbstractSpinBox.
  /// Changes the precision of the spinBox to a large value so that the user can
  /// enter value as desired.
  void focusInEvent(QFocusEvent* event);

  /// Converts the double value to the given decimal precision
  /// Returns whether the operation was successful
  static bool fixDoublePrecision(double& value, const int decimals);

private slots:
  /// adjustDecimals is called when the user is finished editing the spinBox
  /// value. This method sets the precision of the spinBox based on the number
  /// of significant decimals in the value entered by the user.
  /// \sa ctk::significantDecimals()
  void adjustDecimals();

private:
  ctkSmartSpinBoxEditorPrivate* const d_ptr;
  Q_DECLARE_PRIVATE(ctkSmartSpinBoxEditor);
  Q_DISABLE_COPY(ctkSmartSpinBoxEditor);
};
#endif //__ctkSmartSpinBoxEditor_h
