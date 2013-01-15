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
#include "ctkSmartSpinBoxDelegate.h"
#include "ctkSmartSpinBoxEditor.h"
#include "ctkUtils.h"
#include "ctkPimpl.h"

//-----------------------------------------------------------------------------
class ctkSmartSpinBoxDelegatePrivate
{
  Q_DECLARE_PUBLIC(ctkSmartSpinBoxDelegate);
protected:
  ctkSmartSpinBoxDelegate* const q_ptr;
public:
  ctkSmartSpinBoxDelegatePrivate(ctkSmartSpinBoxDelegate& object);

  void init();
  // Parameters for the spinbox
  int Decimals;
  int MinimumDecimals;
  double Maximum;
  double Minimum;
  double SingleStep;
};

//-----------------------------------------------------------------------------
ctkSmartSpinBoxDelegatePrivate::ctkSmartSpinBoxDelegatePrivate(
                                  ctkSmartSpinBoxDelegate& object)
  :q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
void ctkSmartSpinBoxDelegatePrivate::init()
{
  this->Decimals = 8;
  this->MinimumDecimals = 8;
  this->Maximum = 100;
  this->Minimum = 0;
  this->SingleStep = 1;
}

//-----------------------------------------------------------------------------
CTK_SET_CPP(ctkSmartSpinBoxDelegate, int, setDecimals, Decimals);
CTK_GET_CPP(ctkSmartSpinBoxDelegate, int, decimals, Decimals);
CTK_SET_CPP(ctkSmartSpinBoxDelegate, int, setMinimumDecimals, MinimumDecimals);
CTK_GET_CPP(ctkSmartSpinBoxDelegate, int, minimumDecimals, MinimumDecimals);
CTK_SET_CPP(ctkSmartSpinBoxDelegate, double, setMaximum, Maximum);
CTK_GET_CPP(ctkSmartSpinBoxDelegate, double, maximum, Maximum);
CTK_SET_CPP(ctkSmartSpinBoxDelegate, double, setMinimum, Minimum);
CTK_GET_CPP(ctkSmartSpinBoxDelegate, double, minimum, Minimum);
CTK_SET_CPP(ctkSmartSpinBoxDelegate, double, setSingleStep, SingleStep);
CTK_GET_CPP(ctkSmartSpinBoxDelegate, double, singleStep, SingleStep);

//-----------------------------------------------------------------------------
ctkSmartSpinBoxDelegate::ctkSmartSpinBoxDelegate(QWidget* parent)
  : Superclass(parent)
    ,d_ptr(new ctkSmartSpinBoxDelegatePrivate(*this))
{
  Q_D(ctkSmartSpinBoxDelegate);
  d->init();
}

//-----------------------------------------------------------------------------
QWidget* ctkSmartSpinBoxDelegate::createEditor(QWidget* parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index)const
{
  Q_UNUSED(option);
  Q_UNUSED(index);
  ctkSmartSpinBoxEditor* editor = new ctkSmartSpinBoxEditor(parent);
  editor->setDecimals(this->decimals());
  editor->setMinimumDecimals(this->minimumDecimals());
  editor->setMaximum(this->maximum());
  editor->setMinimum(this->minimum());
  editor->setSingleStep(this->singleStep());
  connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
  return editor;
}

//-----------------------------------------------------------------------------
void ctkSmartSpinBoxDelegate::commitAndCloseEditor()
{
  ctkSmartSpinBoxEditor* editor = qobject_cast<ctkSmartSpinBoxEditor *>(sender());
  // Get the new precision from the editor
  this->setDecimals(editor->decimals());
  emit commitData(editor);
  emit closeEditor(editor);
}

//-----------------------------------------------------------------------------
void ctkSmartSpinBoxDelegate::setEditorData(QWidget* editor,
                                            const QModelIndex &index)const
{
  if (qVariantCanConvert<double>(index.data()))
    {
    ctkSmartSpinBoxEditor* spinboxEditor = qobject_cast<ctkSmartSpinBoxEditor *>(editor);
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
void ctkSmartSpinBoxDelegate::setModelData(QWidget *editor,
                                           QAbstractItemModel *model,
                                           const QModelIndex &index) const
{
  if (qVariantCanConvert<double>(index.data()))
    {
    ctkSmartSpinBoxEditor* spinboxEditor = qobject_cast<ctkSmartSpinBoxEditor *>(editor);
    Q_ASSERT(spinboxEditor);
    model->setData(index, qVariantFromValue(spinboxEditor->value()));
    }
  else
    {
    QStyledItemDelegate::setModelData(editor, model, index);
    }
}
