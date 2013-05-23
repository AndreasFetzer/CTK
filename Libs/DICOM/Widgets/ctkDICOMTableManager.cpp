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

// ctk includes
#include "ctkDICOMTableManager.h"
#include "ctkDICOMTableView.h"

// Qt includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>

class ctkDICOMTableManagerPrivate
{
  Q_DECLARE_PUBLIC(ctkDICOMTableManager)

protected:
  ctkDICOMTableManager* const q_ptr;

public:
  ctkDICOMTableManagerPrivate(ctkDICOMTableManager& obj);
  ~ctkDICOMTableManagerPrivate();

  QVBoxLayout* layout;
  QBoxLayout* layoutTables;
  QPushButton* changeLayoutButton;

  ctkDICOMTableView* patientsTable;
  ctkDICOMTableView* studiesTable;
  ctkDICOMTableView* seriesTable;

  void init();
  void setCTKDICOMDatabase(QSharedPointer<ctkDICOMDatabase> db);
};

ctkDICOMTableManagerPrivate::ctkDICOMTableManagerPrivate(ctkDICOMTableManager &obj)
  : q_ptr(&obj)
{

}

ctkDICOMTableManagerPrivate::~ctkDICOMTableManagerPrivate()
{

}

void ctkDICOMTableManagerPrivate::init()
{
  //setup UI
  Q_Q(ctkDICOMTableManager);

  this->layout = new QVBoxLayout();
  this->layoutTables = new QBoxLayout(QBoxLayout::LeftToRight);
  this->patientsTable = new ctkDICOMTableView(q, "Patients");
  this->studiesTable = new ctkDICOMTableView(q, "Studies");
  this->studiesTable->setQueryForeignKey("PatientsUID");
  this->seriesTable = new ctkDICOMTableView(q, "Series");
  this->seriesTable->setQueryForeignKey("StudyInstanceUID");

  QObject::connect(this->patientsTable, SIGNAL(signalSelectionChanged(QStringList)),
                   this->studiesTable, SLOT(onUpdateQuery(QStringList)));
  QObject::connect(this->studiesTable, SIGNAL(signalSelectionChanged(QStringList)),
                   this->seriesTable, SLOT(onUpdateQuery(QStringList)));
  QObject::connect(this->patientsTable, SIGNAL(signalFilterChanged(const QStringList&)),
                   this->studiesTable, SLOT(onUpdateQuery(const QStringList&)));
  QObject::connect(this->studiesTable, SIGNAL(signalFilterChanged(const QStringList&)),
                   this->seriesTable, SLOT(onUpdateQuery(const QStringList&)));
  QObject::connect(this->studiesTable, SIGNAL(signalQueryChanged(QStringList)),
                   this->seriesTable, SLOT(onUpdateQuery(const QStringList&)));
  this->layoutTables->addWidget(patientsTable);
  this->layoutTables->addWidget(studiesTable);
  this->layoutTables->addWidget(seriesTable);

  this->changeLayoutButton = new QPushButton();
  QPixmap icon(":/Icons/vertical.png");
  this->changeLayoutButton->setIcon(icon);
  QObject::connect(this->changeLayoutButton, SIGNAL(clicked()), q, SLOT(onChangeLayoutPushed()));


  this->layout->addWidget(this->changeLayoutButton);
  this->layout->insertStretch(0);
  this->layout->addLayout(this->layoutTables);

  q->setLayout(layout);
}

void ctkDICOMTableManagerPrivate::setCTKDICOMDatabase(QSharedPointer<ctkDICOMDatabase> db)
{
  this->patientsTable->setCTKDicomDataBase(db);
  this->studiesTable->setCTKDicomDataBase(db);
  this->seriesTable->setCTKDicomDataBase(db);
}

//----------------------------------------------------------------------------
// ctkDICOMTableManager methods

//----------------------------------------------------------------------------

ctkDICOMTableManager::ctkDICOMTableManager(QWidget *parent)
  :Superclass(parent)
  , d_ptr(new ctkDICOMTableManagerPrivate(*this))
{
  Q_D(ctkDICOMTableManager);
  d->init();
}

ctkDICOMTableManager::ctkDICOMTableManager(QSharedPointer<ctkDICOMDatabase> db, QWidget *parent)
  : Superclass(parent)
  , d_ptr(new ctkDICOMTableManagerPrivate(*this))
{
  Q_D(ctkDICOMTableManager);
  d->init();
  d->setCTKDICOMDatabase(db);
}

ctkDICOMTableManager::~ctkDICOMTableManager()
{

}

void ctkDICOMTableManager::onChangeLayoutPushed()
{
  Q_D(ctkDICOMTableManager);
  if (d->layoutTables->direction() == QBoxLayout::TopToBottom)
  {
    this->changeTableLayout(QBoxLayout::LeftToRight);
    QPixmap icon(":/Icons/vertical.png");
    d->changeLayoutButton->setIcon(icon);
  }
  else
  {
    this->changeTableLayout(QBoxLayout::TopToBottom);
    QPixmap icon(":/Icons/horizontal.png");
    d->changeLayoutButton->setIcon(icon);
  }
}

void ctkDICOMTableManager::changeTableLayout(QBoxLayout::Direction direction)
{
  Q_D(ctkDICOMTableManager);
  d->layoutTables->removeWidget(d->patientsTable);
  d->layoutTables->removeWidget(d->studiesTable);
  d->layoutTables->removeWidget(d->seriesTable);
  delete d->layoutTables;

  d->layoutTables = new QBoxLayout(direction);
  d->layoutTables->addWidget(d->patientsTable);
  d->layoutTables->addWidget(d->studiesTable);
  d->layoutTables->addWidget(d->seriesTable);
  d->layout->addLayout(d->layoutTables);
}

void ctkDICOMTableManager::setCTKDICOMDatabase(QSharedPointer<ctkDICOMDatabase> db)
{
  Q_D(ctkDICOMTableManager);
  d->setCTKDICOMDatabase(db);
}
