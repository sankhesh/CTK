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
#include <QVariant>
#include <QMouseEvent>

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
    this->MinimumDecimals = 2;
    this->FixedPrecision = false;
    this->Decimals = 2;
    }

  int MinimumDecimals;
  bool FixedPrecision;
  int Decimals;

  void setMaximumDecimals(int currentDecimals,
                          bool blockSignals = true);
};

//-----------------------------------------------------------------------------
void ctkSpinBoxPrivate::setMaximumDecimals(int currentDecimals,
                                           bool blockSignals)
{
  Q_Q(ctkSpinBox);
  if (this->FixedPrecision)
    {
    return;
    }
  double oldValue = q->value();
  bool blocked;
  if (blockSignals)
    {
    blocked = q->blockSignals(true);
    }
  q->fixDoublePrecision(oldValue, currentDecimals);
  q->Superclass::setDecimals(DBL_MAX_10_EXP + DBL_DIG);
  QVariant variant(oldValue);
  q->lineEdit()->setText(variant.toString());
  if (blockSignals)
    {
    q->blockSignals(blocked);
    }
}

//-----------------------------------------------------------------------------
ctkSpinBox::ctkSpinBox(QWidget* parent)
  : QDoubleSpinBox(parent)
    ,d_ptr(new ctkSpinBoxPrivate(*this))
{
  this->lineEdit()->installEventFilter(this);
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(ctkSpinBox, int, minimumDecimals, MinimumDecimals);
CTK_GET_CPP(ctkSpinBox, bool, fixedPrecision, FixedPrecision);
CTK_SET_CPP(ctkSpinBox, bool, setFixedPrecision, FixedPrecision);

//-----------------------------------------------------------------------------
void ctkSpinBox::focusInEvent(QFocusEvent* event)
{
  Q_D(ctkSpinBox);
  emit this->valueAboutToBeChanged();
  d->setMaximumDecimals(this->decimals());
  Superclass::focusInEvent(event);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::focusOutEvent(QFocusEvent* event)
{
  Q_D(ctkSpinBox);
  if (!d->FixedPrecision)
    {
    int decimals = qMax(ctk::significantDecimals(this->value()),
                        d->MinimumDecimals);
    this->setDecimals(decimals);
    }
  Superclass::focusOutEvent(event);
}

//-----------------------------------------------------------------------------
bool ctkSpinBox::eventFilter( QObject* object, QEvent* event)
{
  Q_UNUSED(object);
  Q_D(ctkSpinBox);
  if ( event->type() == QEvent::MouseButtonPress ||
       event->type() == QEvent::KeyPress)
    {
    int decimals = qMax(ctk::significantDecimals(this->value()),
                        d->MinimumDecimals);
    d->setMaximumDecimals(decimals);
    return false;
    }
  else
    {
    return false;
    }
}

//-----------------------------------------------------------------------------
void ctkSpinBox::keyPressEvent(QKeyEvent* event)
{
  Q_D(ctkSpinBox);
  if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
      event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown ||
      event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
    int decimals = qMax(ctk::significantDecimals(this->value()),
                        d->MinimumDecimals);
    this->setDecimals(decimals);
    }
  Superclass::keyPressEvent(event);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::keyReleaseEvent(QKeyEvent* event)
{
  Q_D(ctkSpinBox);
  if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
      event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown)
    {
    int decimals = qMax(ctk::significantDecimals(this->value()),
                        d->MinimumDecimals);
    d->setMaximumDecimals(decimals);
    }
  Superclass::keyReleaseEvent(event);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::mousePressEvent(QMouseEvent* event)
{
  Q_D(ctkSpinBox);
  emit valueAboutToBeChanged();
  int decimals = qMax(ctk::significantDecimals(this->value()),
                      d->MinimumDecimals);
  this->setDecimals(decimals);
  Superclass::mousePressEvent(event);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::mouseReleaseEvent(QMouseEvent* event)
{
  Q_D(ctkSpinBox);
  int decimals = qMax(ctk::significantDecimals(this->value()),
                      d->MinimumDecimals);
  d->setMaximumDecimals(decimals);
  Superclass::mouseReleaseEvent(event);
}

//-----------------------------------------------------------------------------
bool ctkSpinBox::fixDoublePrecision(double& value, const int decimals)
{
  double newValue = value*(pow(10, decimals));
  QVariant var(newValue);
  bool castOK = false;
  qlonglong longValue = var.toLongLong(&castOK);

  // Check if the value could be converted to an integer
  if (!castOK)
    {
    return false;
    }

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
    this->setDecimals(decimals);
    }
  Superclass::setValue(newValue);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::setDecimals(int newDecimals)
{
  Q_D(ctkSpinBox);
  if (!(d->FixedPrecision) && (newDecimals < d->MinimumDecimals))
    {
    // Make sure that minimum precision is preserved.
    newDecimals = d->MinimumDecimals;
    }
  if (d->Decimals != newDecimals)
    {
    d->Decimals = newDecimals;
    emit this->decimalsChanged(newDecimals);
    }
  Superclass::setDecimals(newDecimals);
}

//-----------------------------------------------------------------------------
void ctkSpinBox::setMinimumDecimals(int newMinimumDecimals)
{
  Q_D(ctkSpinBox);
  if (d->FixedPrecision)
    {
    return;
    }
  d->MinimumDecimals = newMinimumDecimals;
  if (d->Decimals < newMinimumDecimals)
    {
    this->setDecimals(newMinimumDecimals);
    }
}
