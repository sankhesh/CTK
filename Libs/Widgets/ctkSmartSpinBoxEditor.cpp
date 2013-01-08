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
#include "ctkSmartSpinBoxEditor.h"
#include <QDebug>
#include <ctkUtils.h>

//-----------------------------------------------------------------------------
ctkSmartSpinBoxEditor::ctkSmartSpinBoxEditor(QWidget* parent) :
  QDoubleSpinBox(parent)
{
  connect(this, SIGNAL(editingFinished()), this, SLOT(adjustDecimals()));
}

//-----------------------------------------------------------------------------
void ctkSmartSpinBoxEditor::focusInEvent(QFocusEvent* event)
{
  double oldValue = this->value();
  this->blockSignals(true);
  int oldDecimals = this->decimals();
  this->setDecimals(oldDecimals + 20);
  this->setValue(oldValue);
  this->blockSignals(false);
}

//-----------------------------------------------------------------------------
void ctkSmartSpinBoxEditor::adjustDecimals()
{
  int decimals = qMin(ctk::significantDecimals(this->value()),8);
  this->blockSignals(true);
  this->setDecimals(decimals);
  this->blockSignals(false);
}
