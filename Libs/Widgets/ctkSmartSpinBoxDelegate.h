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

#ifndef __ctkSmartSpinBoxDelegate_h
#define __ctkSmartSpinBoxDelegate_h

// Qt includes
#include <QStyledItemDelegate>

#include "ctkWidgetsExport.h"

class ctkSmartSpinBoxDelegatePrivate;

/// \brief Item delegate for smart spinBox editing.
/// Uses ctkSmartSpinBoxEditor to automatically adjust the precision of the
/// spinBox according the new value entered by the user. Used in ctkMatrixWidget
/// \sa ctkSmartSpinBoxEditor, ctkMatrixWidget
class CTK_WIDGETS_EXPORT ctkSmartSpinBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  Q_PROPERTY(int decimals READ decimals WRITE setDecimals)
  Q_PROPERTY(int minimumDecimals READ minimumDecimals WRITE setMinimumDecimals)
  Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
  Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
  Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep)

public:
  /// Superclass typedef
  typedef QStyledItemDelegate Superclass;

  /// Constructor
  ctkSmartSpinBoxDelegate(QWidget* parent = 0);

  /// Creates and returns the editor to be used for editing the data item with
  /// the given index.
  /// \sa ctkSmartSpinBoxEditor, QAbstractItemDelegate::createEditor()
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;

  /// Set the contents of the given editor to the data for the item at the
  /// given index.
  /// \sa setModelData(), QAbstractItemDelegate::setEditorData()
  void setEditorData(QWidget *editor, const QModelIndex &index) const;

  /// Sets the data for the item at the given index in the model to the contents
  /// of the given editor.
  /// \sa setEditorData(), QAbstractItemDelegate::setModelData()
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const;

  /// This property holds the precision that will be set on the spinbox editor,
  /// in decimals. It also holds the precision of the new value in the
  /// editor entered by the user.
  int decimals()const;
  void setDecimals(int newDecimals);

  /// This property holds the minimum precision of the double value that the
  /// spinbox editor should resolve to. When the user enters a value, its
  /// precision will be compared to MinimumDecimals and the higher value will
  /// be set as the precision of the model.
  /// \sa decimals(), ctkSmartSpinBoxEditor::setMinimumDecimals
  int minimumDecimals()const;
  void setMinimumDecimals(int newMinimumDecimals);

  /// This property holds the maximum value that will be set on the spinbox
  /// editor.
  double maximum()const;
  void setMaximum(double newMaximum);

  /// This property holds the minimum value that will be set on the spinbox
  /// editor.
  double minimum()const;
  void setMinimum(double newMinimum);

  /// This property holds the step value that will be set on the spinbox editor.
  double singleStep()const;
  void setSingleStep(double step);

private slots:
  /// commitAndCloseEditor() is invoked when the editingFinished() signal is
  /// fired by the ctkSmartSpinBoxEditor. This slot emits the commitData and
  /// closeEditor signals to update the model data and close the editor.
  /// \sa editingFinished(), commitData(), closeEditor()
  void commitAndCloseEditor();

private:
  ctkSmartSpinBoxDelegatePrivate* const d_ptr;
  Q_DECLARE_PRIVATE(ctkSmartSpinBoxDelegate);
  Q_DISABLE_COPY(ctkSmartSpinBoxDelegate);
};
#endif //__ctkSmartSpinBoxDelegate_h
