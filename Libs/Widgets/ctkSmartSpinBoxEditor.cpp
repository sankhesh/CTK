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

#include <cfloat>
#include <cmath>

// CTK includes
#include "ctkSmartSpinBoxEditor.h"
#include "ctkUtils.h"
#include "ctkPimpl.h"

// Qt includes
#include <QLineEdit>

//-----------------------------------------------------------------------------
class ctkSmartSpinBoxEditorPrivate
{
  Q_DECLARE_PUBLIC(ctkSmartSpinBoxEditor);
protected:
  ctkSmartSpinBoxEditor* const q_ptr;
public:
  ctkSmartSpinBoxEditorPrivate(ctkSmartSpinBoxEditor& object)
    : q_ptr(&object)
    {
    this->MinimumDecimals = 8;
    }

  int MinimumDecimals;
};

//-----------------------------------------------------------------------------
ctkSmartSpinBoxEditor::ctkSmartSpinBoxEditor(QWidget* parent)
  : QDoubleSpinBox(parent)
    ,d_ptr(new ctkSmartSpinBoxEditorPrivate(*this))
{
  connect(this, SIGNAL(editingFinished()), this, SLOT(adjustDecimals()));
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(ctkSmartSpinBoxEditor, int, minimumDecimals, MinimumDecimals);
CTK_SET_CPP(ctkSmartSpinBoxEditor, int, setMinimumDecimals, MinimumDecimals);

//-----------------------------------------------------------------------------
void ctkSmartSpinBoxEditor::focusInEvent(QFocusEvent* event)
{
  Q_UNUSED(event);
  double oldValue = this->value();
  this->blockSignals(true);
  int oldDecimals = this->decimals();
  this->fixDoublePrecision(oldValue, oldDecimals);
  // Set maximum possible precision
  this->setDecimals(DBL_MAX_10_EXP + DBL_DIG);
  QVariant variant(oldValue);
  this->lineEdit()->setText(variant.toString());
  this->blockSignals(false);
}

//-----------------------------------------------------------------------------
void ctkSmartSpinBoxEditor::adjustDecimals()
{
  Q_D(ctkSmartSpinBoxEditor);
  int decimals = qMin(ctk::significantDecimals(this->value()),
                      d->MinimumDecimals);
  this->blockSignals(true);
  this->setDecimals(decimals);
  this->blockSignals(false);
}

//-----------------------------------------------------------------------------
bool ctkSmartSpinBoxEditor::fixDoublePrecision(double& value, const int decimals)
{
  double newValue = value*(pow(10, decimals));
  double longValue = static_cast<long>(newValue);

  // Check if the casting hasn't exceeded the numeric limits of long type
  double error = abs (longValue - newValue);
  if (error > 1e-6)
    {
    return false;
    }
  value = longValue/(pow(10, decimals));
  return true;
}
