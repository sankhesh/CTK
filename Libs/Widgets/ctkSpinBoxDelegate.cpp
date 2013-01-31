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

// CTK includes
#include "ctkPimpl.h"
#include "ctkSpinBoxDelegate.h"
#include "ctkSpinBox.h"
#include "ctkUtils.h"

//-----------------------------------------------------------------------------
class ctkSpinBoxDelegatePrivate
{
  Q_DECLARE_PUBLIC(ctkSpinBoxDelegate);
protected:
  ctkSpinBoxDelegate* const q_ptr;
public:
  ctkSpinBoxDelegatePrivate(ctkSpinBoxDelegate& object);

  void init();
  // Parameters for the spinbox
  int Decimals;
  bool FixedPrecision;
  int MinimumDecimals;
  double Maximum;
  double Minimum;
  double SingleStep;
};

//-----------------------------------------------------------------------------
ctkSpinBoxDelegatePrivate::ctkSpinBoxDelegatePrivate(
                                  ctkSpinBoxDelegate& object)
  :q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
void ctkSpinBoxDelegatePrivate::init()
{
  this->Decimals = 2;
  this->FixedPrecision = false;
  this->MinimumDecimals = 2;
  this->Maximum = 100;
  this->Minimum = 0;
  this->SingleStep = 1;
}

//-----------------------------------------------------------------------------
CTK_SET_CPP(ctkSpinBoxDelegate, int, setDecimals, Decimals);
CTK_GET_CPP(ctkSpinBoxDelegate, int, decimals, Decimals);
CTK_SET_CPP(ctkSpinBoxDelegate, bool, setFixedPrecision, FixedPrecision);
CTK_GET_CPP(ctkSpinBoxDelegate, bool, fixedPrecision, FixedPrecision);
CTK_SET_CPP(ctkSpinBoxDelegate, int, setMinimumDecimals, MinimumDecimals);
CTK_GET_CPP(ctkSpinBoxDelegate, int, minimumDecimals, MinimumDecimals);
CTK_SET_CPP(ctkSpinBoxDelegate, double, setMaximum, Maximum);
CTK_GET_CPP(ctkSpinBoxDelegate, double, maximum, Maximum);
CTK_SET_CPP(ctkSpinBoxDelegate, double, setMinimum, Minimum);
CTK_GET_CPP(ctkSpinBoxDelegate, double, minimum, Minimum);
CTK_SET_CPP(ctkSpinBoxDelegate, double, setSingleStep, SingleStep);
CTK_GET_CPP(ctkSpinBoxDelegate, double, singleStep, SingleStep);

//-----------------------------------------------------------------------------
ctkSpinBoxDelegate::ctkSpinBoxDelegate(QWidget* parent)
  : Superclass(parent)
    ,d_ptr(new ctkSpinBoxDelegatePrivate(*this))
{
  Q_D(ctkSpinBoxDelegate);
  d->init();
}

//-----------------------------------------------------------------------------
QWidget* ctkSpinBoxDelegate::createEditor(QWidget* parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index)const
{
  Q_UNUSED(option);
  Q_UNUSED(index);
  ctkSpinBox* editor = new ctkSpinBox(parent);
  editor->setDecimals(this->decimals());
  editor->setFixedPrecision(this->fixedPrecision());
  editor->setMinimumDecimals(this->minimumDecimals());
  editor->setMaximum(this->maximum());
  editor->setMinimum(this->minimum());
  editor->setSingleStep(this->singleStep());
  connect(editor, SIGNAL(decimalsChanged(int)),
          this, SIGNAL(decimalsChanged(int)));
  connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
  return editor;
}

//-----------------------------------------------------------------------------
void ctkSpinBoxDelegate::commitAndCloseEditor()
{
  ctkSpinBox* editor = qobject_cast<ctkSpinBox *>(sender());
  // Get the new precision from the editor
  this->setDecimals(editor->decimals());
  emit commitData(editor);
  emit closeEditor(editor);
}

//-----------------------------------------------------------------------------
void ctkSpinBoxDelegate::setEditorData(QWidget* editor,
                                            const QModelIndex &index)const
{
  if (qVariantCanConvert<double>(index.data()))
    {
    ctkSpinBox* spinboxEditor = qobject_cast<ctkSpinBox *>(editor);
    Q_ASSERT(spinboxEditor);
    double value = index.data().toDouble();
    spinboxEditor->blockSignals(true);
    spinboxEditor->setValue(value);
    spinboxEditor->blockSignals(false);
    }
  else
    {
    QStyledItemDelegate::setEditorData(editor, index);
    }
}

//-----------------------------------------------------------------------------
void ctkSpinBoxDelegate::setModelData(QWidget *editor,
                                           QAbstractItemModel *model,
                                           const QModelIndex &index) const
{
  if (qVariantCanConvert<double>(index.data()))
    {
    ctkSpinBox* spinboxEditor = qobject_cast<ctkSpinBox *>(editor);
    Q_ASSERT(spinboxEditor);
    model->setData(index, qVariantFromValue(spinboxEditor->value()));
    }
  else
    {
    QStyledItemDelegate::setModelData(editor, model, index);
    }
}
