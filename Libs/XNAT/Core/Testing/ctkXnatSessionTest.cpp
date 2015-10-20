/*=========================================================================

  Library:   CTK

  Copyright (c) 2013 University College London, Centre for Medical Image Computing

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

#include "ctkXnatSessionTest.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSignalSpy>
#include <QStringList>
#include <QTest>
#include <QTime>
#include <QTimer>
#include <QUrl>
#include <QUuid>

#include <ctkXnatDataModel.h>
#include <ctkXnatImageSession.h>
#include <ctkXnatLoginProfile.h>
#include <ctkXnatSession.h>
#include <ctkXnatProject.h>
#include <ctkXnatSubject.h>
#include <ctkXnatSubjectVariables.h>
#include <ctkXnatException.h>

class ctkXnatSessionTestCasePrivate
{
public:
  ctkXnatSession* Session;

  ctkXnatLoginProfile LoginProfile;

  QString Project;
  QString Subject;
  QString Experiment;

  QDateTime DateTime;
};

// --------------------------------------------------------------------------
ctkXnatSessionTestCase::ctkXnatSessionTestCase()
: d_ptr(new ctkXnatSessionTestCasePrivate())
{
}

// --------------------------------------------------------------------------
ctkXnatSessionTestCase::~ctkXnatSessionTestCase()
{
}

// --------------------------------------------------------------------------
void ctkXnatSessionTestCase::initTestCase()
{
  Q_D(ctkXnatSessionTestCase);

  d->LoginProfile.setName("ctk");
  d->LoginProfile.setServerUrl(QString("http://192.168.56.101"));
  d->LoginProfile.setUserName("admin");
  d->LoginProfile.setPassword("admin");
//  d->LoginProfile.setServerUrl(QString("https://central.xnat.org"));
//  d->LoginProfile.setUserName("ctk");
//  d->LoginProfile.setPassword("ctk-xnat2015");
}

void ctkXnatSessionTestCase::init()
{
  Q_D(ctkXnatSessionTestCase);

  d->DateTime = QDateTime::currentDateTime();
  d->Session = new ctkXnatSession(d->LoginProfile);
  d->Session->open();
}

void ctkXnatSessionTestCase::cleanupTestCase()
{
}

void ctkXnatSessionTestCase::cleanup()
{
  Q_D(ctkXnatSessionTestCase);

  delete d->Session;
  d->Session = NULL;
}

void ctkXnatSessionTestCase::testProjectList()
{
  Q_D(ctkXnatSessionTestCase);

  ctkXnatObject* dataModel = d->Session->dataModel();
  dataModel->fetch();

  QList<ctkXnatObject*> projects = dataModel->children();

  QVERIFY(projects.size() > 0);
}

void ctkXnatSessionTestCase::testResourceUri()
{
  Q_D(ctkXnatSessionTestCase);

  ctkXnatObject* dataModel = d->Session->dataModel();
  QVERIFY(!dataModel->resourceUri().isNull());
  QVERIFY(dataModel->resourceUri().isEmpty());
}

void ctkXnatSessionTestCase::testParentChild()
{
  Q_D(ctkXnatSessionTestCase);

  ctkXnatDataModel* dataModel = d->Session->dataModel();

  ctkXnatProject* project = new ctkXnatProject(dataModel);

  QVERIFY(project->parent() == dataModel);

  QVERIFY(dataModel->children().contains(project));

  dataModel->add(project);

  int numberOfOccurrences = 0;
  foreach (ctkXnatObject* serverProject, dataModel->children())
  {
    if (serverProject == project || serverProject->id() == project->id())
    {
      ++numberOfOccurrences;
    }
  }
  QVERIFY(numberOfOccurrences == 1);

  dataModel->remove(project);
  numberOfOccurrences = 0;
  foreach (ctkXnatObject* serverProject, dataModel->children())
  {
    if (serverProject == project || serverProject->id() == project->id())
    {
      ++numberOfOccurrences;
    }
  }
  QVERIFY(numberOfOccurrences == 0);
  delete project;
}

void ctkXnatSessionTestCase::testSession()
{
  Q_D(ctkXnatSessionTestCase);

  QVERIFY(d->Session->isOpen());
  QDateTime expirationDate = d->Session->expirationDate();

  QVERIFY(d->DateTime < expirationDate);

  QTest::qSleep(2000);

  QUuid uuid = d->Session->httpGet("/data/version");
  QVERIFY(!uuid.isNull());
  d->Session->httpSync(uuid);

  QVERIFY(expirationDate < d->Session->expirationDate());

  try
  {
    d->Session->httpSync(uuid);
    QFAIL("Exception for unknown uuid expected");
  }
  catch(const ctkInvalidArgumentException&)
  {}

  d->Session->close();
  try
  {
    d->Session->dataModel();
    QFAIL("Exception for closed session expected");
  }
  catch(const ctkXnatInvalidSessionException&)
  {}
}

void ctkXnatSessionTestCase::testAuthenticationError()
{
  ctkXnatLoginProfile loginProfile;
  loginProfile.setName("error");
  loginProfile.setServerUrl(QString("https://central.xnat.org"));
  loginProfile.setUserName("x");
  loginProfile.setPassword("y");

  ctkXnatSession session(loginProfile);
  try
  {
    session.open();
    QFAIL("Authenication error exception expected");
  }
  catch (const ctkXnatAuthenticationException&)
  {}
}

void ctkXnatSessionTestCase::testCreateProject()
{
  Q_D(ctkXnatSessionTestCase);

  ctkXnatDataModel* dataModel = d->Session->dataModel();

  QString projectId = QString("CTK_") + QUuid::createUuid().toString().mid(1, 8);
  d->Project = projectId;

  ctkXnatProject* project = new ctkXnatProject(dataModel);
  project->setId(projectId);
  project->setName(projectId);
  project->setDescription("CTK_test_project");

  bool exists = d->Session->exists(project);
  QVERIFY(!exists);

//  d->Session->save(project);
  project->save();

  exists = d->Session->exists(project);
  QVERIFY(exists);

//  d->Session->remove(project);
  project->erase();

  exists = d->Session->exists(project);
  QVERIFY(!exists);
}

void ctkXnatSessionTestCase::testCreateSubject()
{
  Q_D(ctkXnatSessionTestCase);

  ctkXnatDataModel* dataModel = d->Session->dataModel();

  QString projectId = QString("CTK_") + QUuid::createUuid().toString().mid(1, 8);
  d->Project = projectId;

  ctkXnatProject* project = new ctkXnatProject(dataModel);
  project->setId(projectId);
  project->setName(projectId);
  project->setDescription("CTK_test_project");

  QVERIFY(!project->exists());

  project->save();

  QVERIFY(project->exists());

  ctkXnatSubject* subject = new ctkXnatSubject(project);

  QString subjectName = QString("CTK_S") + QUuid::createUuid().toString().mid(1, 8);
  subject->setId(subjectName);
  subject->setName(subjectName);

  subject->save();

  QVERIFY(!subject->id().isNull());

  subject->erase();

  QVERIFY(!subject->exists());

  project->erase();

  QVERIFY(!project->exists());
}

void ctkXnatSessionTestCase::testCreateExperiment()
{
  Q_D(ctkXnatSessionTestCase);

  ctkXnatDataModel* dataModel = d->Session->dataModel();

  QString projectId = QString("CTK_") + QUuid::createUuid().toString().mid(1, 8);
  d->Project = projectId;

  ctkXnatProject* project = new ctkXnatProject(dataModel);
  project->setId(projectId);
  project->setName(projectId);
  project->setDescription("CTK_test_project");

  QVERIFY(!project->exists());

  project->save();

  QVERIFY(project->exists());

  ctkXnatSubject* subject = new ctkXnatSubject(project);

  QString subjectName = QString("CTK_S") + QUuid::createUuid().toString().mid(1, 8);
  subject->setId(subjectName);
  subject->setName(subjectName);

  subject->save();

  QVERIFY(!subject->id().isNull());

  // Create an experiment of type xnat:SubjectVariablesData
  ctkXnatSubjectVariables* subVarData = new ctkXnatSubjectVariables(subject);
  QString subjectVDName = QString("CTK_SVD_") + QUuid::createUuid().toString().mid(1, 8);
  subVarData->setName(subjectVDName);
  subVarData->save();

  QVERIFY(subVarData->exists());

  // Create an experiment of type xnat:XnatImageSessionData,
  ctkXnatImageSession* subImageSession = new ctkXnatImageSession(subject, ctkXnatDefaultSchemaTypes::XSI_CT_SESSION);
  QString subjectISName = QString("CTK_SIS_") + QUuid::createUuid().toString().mid(1, 8);
  subImageSession->setName(subjectISName);
  subImageSession->save();

  QVERIFY(subImageSession->exists());

  // Clean up
  subVarData->erase();

  QVERIFY(!subVarData->exists());

  subImageSession->erase();

  QVERIFY(!subImageSession->exists());

  subject->erase();

  QVERIFY(!subject->exists());

  project->erase();

  QVERIFY(!project->exists());
}

// --------------------------------------------------------------------------
int ctkXnatSessionTest(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);
  ctkXnatSessionTestCase test;
  return QTest::qExec(&test, argc, argv);
}
