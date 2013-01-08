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

#include <QWidget>
#include <QDoubleSpinBox>

class ctkSmartSpinBoxEditor : public QDoubleSpinBox
{
  Q_OBJECT
public:
  /// Constructor, creates a QDoubleSpinBox
  ctkSmartSpinBoxEditor(QWidget* parent = 0);

protected:
  /// Overloaded method from QAbstractSpinBox.
  /// Changes the precision of the spinBox to a large value so that the user can
  /// enter value as desired.
  void focusInEvent(QFocusEvent* event);

private slots:
  /// adjustDecimals is called when the user is finished editing the spinBox
  /// value. This method sets the precision of the spinBox based on the number
  /// of significant decimals in the value entered by the user.
  /// \sa ctk::significantDecimals()
  void adjustDecimals();
};

#endif //__ctkSmartSpinBoxEditor_h
