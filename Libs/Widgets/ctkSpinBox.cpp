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
#include "ctkSpinBox.h"
#include "ctkUtils.h"
#include "ctkPimpl.h"

// Qt includes
#include <QLineEdit>

//-----------------------------------------------------------------------------
class ctkSpinBoxPrivate
{
  Q_DECLARE_PUBLIC(ctkSpinBox);
protected:
  ctkSpinBox* const q_ptr;
public:
  ctkSpinBoxPrivate(ctkSpinBox& object)
    : q_ptr(&object)
    {
    this->MinimumDecimals = 8;
    this->FixedPrecision = false;
    this->Decimals = 8;
    }

  int MinimumDecimals;
  bool FixedPrecision;
  int Decimals;
};

//-----------------------------------------------------------------------------
ctkSpinBox::ctkSpinBox(QWidget* parent)
  : QDoubleSpinBox(parent)
    ,d_ptr(new ctkSpinBoxPrivate(*this))
{
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(ctkSpinBox, int, minimumDecimals, MinimumDecimals);
CTK_SET_CPP(ctkSpinBox, int, setMinimumDecimals, MinimumDecimals);
CTK_GET_CPP(ctkSpinBox, bool, fixedPrecision, FixedPrecision);
CTK_SET_CPP(ctkSpinBox, bool, setFixedPrecision, FixedPrecision);

//-----------------------------------------------------------------------------
void ctkSpinBox::focusInEvent(QFocusEvent* event)
{
  Q_D(ctkSpinBox);
  emit this->valueAboutToBeChanged();
  if (!d->FixedPrecision)
    {
    double oldValue = this->value();
    this->blockSignals(true);
    int decimals = this->decimals();
    this->fixDoublePrecision(oldValue, decimals);
    // Set maximum possible precision
    this->setDecimals(DBL_MAX_10_EXP + DBL_DIG);
    QVariant variant(oldValue);
    this->lineEdit()->setText(variant.toString());
    this->blockSignals(false);
    }
  Superclass::focusInEvent(event);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::focusOutEvent(QFocusEvent* event)
{
  Q_D(ctkSpinBox);
  if (!d->FixedPrecision)
    {
    this->adjustDecimals();
    }
  Superclass::focusOutEvent(event);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::adjustDecimals()
{
  Q_D(ctkSpinBox);
  int decimals = qMax(ctk::significantDecimals(this->value()),
                      d->MinimumDecimals);
  this->blockSignals(true);
  this->setDecimals(decimals);
  this->blockSignals(false);
  if (decimals != d->Decimals)
    {
    d->Decimals = decimals;
    emit this->decimalsChanged(d->Decimals);
    }
}

//-----------------------------------------------------------------------------
bool ctkSpinBox::fixDoublePrecision(double& value, const int decimals)
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

//-----------------------------------------------------------------------------
void ctkSpinBox::setValue(double newValue)
{
  Q_D(ctkSpinBox);
  emit valueAboutToBeChanged();
  if(!d->FixedPrecision)
    {
    // Change precision to accomodate the new value
    int decimals = qMax(ctk::significantDecimals(newValue),
                        d->MinimumDecimals);
    if(d->Decimals != decimals)
      {
      d->Decimals = decimals;
      this->setDecimals(d->Decimals);
      emit this->decimalsChanged(d->Decimals);
      }
    }
  Superclass::setValue(newValue);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::setDecimals(int newDecimals)
{
  Q_D(ctkSpinBox);
  Superclass::setDecimals(newDecimals);
  if (d->Decimals != newDecimals)
    {
    d->Decimals = newDecimals;
    emit this->decimalsChanged(newDecimals);
    }
}
