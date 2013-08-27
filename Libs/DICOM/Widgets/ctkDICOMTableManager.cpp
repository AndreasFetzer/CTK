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
#include <QSplitter>

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
  QSplitter* tableSplitter;

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

  //Connect changed tableview selection with query update of "child" table
  QObject::connect(this->patientsTable, SIGNAL(signalSelectionChanged(QStringList)),
                   this->studiesTable, SLOT(onUpdateQuery(QStringList)));
  QObject::connect(this->studiesTable, SIGNAL(signalSelectionChanged(QStringList)),
                   this->seriesTable, SLOT(onUpdateQuery(QStringList)));

  //Connect changed filter with query update of "child" table
  QObject::connect(this->patientsTable, SIGNAL(signalFilterChanged(const QStringList&)),
                   this->studiesTable, SLOT(onUpdateQuery(const QStringList&)));
  QObject::connect(this->studiesTable, SIGNAL(signalFilterChanged(const QStringList&)),
                   this->seriesTable, SLOT(onUpdateQuery(const QStringList&)));
  //This way a patient filter change can be propageted to series table without
  //any selection in the study table
  QObject::connect(this->studiesTable, SIGNAL(signalQueryChanged(QStringList)),
                   this->seriesTable, SLOT(onUpdateQuery(const QStringList&)));

  QObject::connect(this->patientsTable, SIGNAL(signalSelectionChanged(const QItemSelection&, const QItemSelection&)),
                   q, SIGNAL(signalPatientsSelectionChanged(const QItemSelection&, const QItemSelection&)));
  QObject::connect(this->patientsTable, SIGNAL(signalSelectionChanged(const QItemSelection&, const QItemSelection&)),
                   q, SLOT(onPatientSelectionChanged()));
  QObject::connect(this->patientsTable, SIGNAL(signalSelectionChanged(const QStringList&)),
                   q, SLOT(onPatientSelectionChanged(const QStringList&)));

  QObject::connect(this->studiesTable, SIGNAL(signalSelectionChanged(const QItemSelection&, const QItemSelection&)),
                   q, SIGNAL(signalStudiesSelectionsChanged(const QItemSelection&, const QItemSelection&)));
  QObject::connect(this->studiesTable, SIGNAL(signalSelectionChanged(const QItemSelection&, const QItemSelection&)),
                   q, SLOT(signalStudiesSelectionsChanged(const QItemSelection&, const QItemSelection&)));
  QObject::connect(this->studiesTable, SIGNAL(signalSelectionChanged(const QStringList&)),
                   q, SLOT(signalStudiesSelectionsChanged(const QStringList&)));

  QObject::connect(this->seriesTable, SIGNAL(signalSelectionChanged(const QItemSelection&, const QItemSelection&)),
                   q, SIGNAL(signalSeriesSelectionsChanged(const QItemSelection&, const QItemSelection&)));
  QObject::connect(this->seriesTable, SIGNAL(signalSelectionChanged(const QItemSelection&, const QItemSelection&)),
                   q, SLOT(signalSeriesSelectionChanged(const QItemSelection&, const QItemSelection&)));
  QObject::connect(this->seriesTable, SIGNAL(signalSelectionChanged(const QStringList&)),
                   q, SLOT(signalSeriesSelectionChanged(const QStringList&)));

  this->patientsTable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
  this->studiesTable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
  this->seriesTable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

  tableSplitter = new QSplitter();
  tableSplitter->setChildrenCollapsible(false);

  tableSplitter->addWidget(this->patientsTable);
  tableSplitter->addWidget(this->studiesTable);
  tableSplitter->addWidget(this->seriesTable);

  tableSplitter->setStyleSheet("QSplitter::handle {background-color: rgb(224,224,224);}QSplitter::handle:horizontal {width: 2px;}QSplitter::handle:vertical {height: 2px;}");


  QHBoxLayout* buttonLayout = new QHBoxLayout();
  this->changeLayoutButton = new QPushButton();
  this->changeLayoutButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  QPixmap icon(":/Icons/vertical.png");
  this->changeLayoutButton->setIcon(icon);
  QObject::connect(this->changeLayoutButton, SIGNAL(clicked()), q, SLOT(onChangeLayoutPushed()));
  buttonLayout->addWidget(this->changeLayoutButton);

  QSpacerItem* spacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed);

  buttonLayout->addItem(spacer);
  this->layout->addLayout(buttonLayout);
  this->layout->addWidget(this->tableSplitter);

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
  if(d->tableSplitter->orientation() == Qt::Vertical)
  {
    QPixmap icon(":/Icons/vertical.png");
    d->changeLayoutButton->setIcon(icon);
    d->tableSplitter->setOrientation(Qt::Horizontal);
  }
  else
  {
    QPixmap icon(":/Icons/horizontal.png");
    d->changeLayoutButton->setIcon(icon);
    d->tableSplitter->setOrientation(Qt::Vertical);
  }
}

void ctkDICOMTableManager::setCTKDICOMDatabase(QSharedPointer<ctkDICOMDatabase> db)
{
  Q_D(ctkDICOMTableManager);
  d->setCTKDICOMDatabase(db);
}
