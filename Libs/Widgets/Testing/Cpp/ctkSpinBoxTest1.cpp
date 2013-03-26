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

// Qt includes
#include <QApplication>
#include <QTimer>

// CTK includes
#include "ctkSpinBox.h"

// STD includes
#include <cstdlib>
#include <iostream>

//-----------------------------------------------------------------------------
int ctkSpinBoxTest1(int argc, char * argv [] )
{
  QApplication app(argc, argv);

  ctkSpinBox spinBox;

  // the first part of the tests are for fixedPrecision of 3 decimals
  spinBox.setFixedPrecision(true);
  spinBox.setDecimals(3);
  if (spinBox.decimals() != 3)
    {
    std::cerr << "ctkSpinBox::decimals value failed."
              << spinBox.decimals() << std::endl;
    return EXIT_FAILURE;
    }

  if (spinBox.minimumDecimals() != 2)
    {
    std::cerr << "ctkSpinBox::minimumDecimals default value failed."
              << spinBox.minimumDecimals() << std::endl;
    return EXIT_FAILURE;
    }

  spinBox.setValue(3.123);
  if (!qFuzzyCompare(spinBox.value(), 3.123))
    {
    std::cerr << "ctkSpinBox::setValue failed."
              << spinBox.value() << std::endl;
    return EXIT_FAILURE;
    }

  spinBox.setMinimum(10.01234);
  if (!qFuzzyCompare(spinBox.minimum(), 10.012) ||
      !qFuzzyCompare(spinBox.value(), 10.012))
    {
    std::cerr << "ctkSpinBox::setMinimum failed. Minimum = "
              << spinBox.minimum() << " & Value = " << spinBox.value();
    return EXIT_FAILURE;
    }

  // the second part of the tests are for interactive precision
  spinBox.setFixedPrecision(false);

  spinBox.setDecimals(1);
  if (spinBox.decimals() != 2)
    {
    std::cerr << "ctkSpinBox::setDecimals failed with Decimals = "
              << spinBox.decimals() << " & MinimumDecimals = "
              << spinBox.minimumDecimals() << std::endl;
    return EXIT_FAILURE;
    }

  if (!qFuzzyCompare(spinBox.value(), 10.01))
    {
    std::cerr << "ctkSpinBox value not adjusted with change in decimals."
              << " Value = " << spinBox.value() << " & Decimals = "
              << spinBox.decimals() << std::endl;
    return EXIT_FAILURE;
    }

  spinBox.setValue(13.12345);
  if (!qFuzzyCompare(spinBox.value(), 13.12345) ||
      (spinBox.decimals() != 5))
    {
    std::cerr << "ctkSpinBox decimals not adjusted to value."
              << " Value = " << spinBox.value() << " & Decimals = "
              << spinBox.decimals() << std::endl;
    return EXIT_FAILURE;
    }

//-----------------------------------------------------------------------------
  spinBox.show();
  if (argc < 2 || QString(argv[1]) != "-I" )
    {
    QTimer::singleShot(100, &app, SLOT(quit()));
    }

  return app.exec();
}
