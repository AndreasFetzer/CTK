/*=============================================================================

  Library: XNAT/Core

  Copyright (c) University College London,
    Centre for Medical Image Computing

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#include "ctkXnatSubject.h"

#include "ctkXnatDefaultSchemaTypes.h"
#include "ctkXnatExperiment.h"
#include "ctkXnatObjectPrivate.h"
#include "ctkXnatProject.h"
#include "ctkXnatSession.h"

#include <QDebug>

const QString ctkXnatSubject::INSERT_DATE = "insert_date";
const QString ctkXnatSubject::INSERT_USER = "insert_user";

//----------------------------------------------------------------------------
class ctkXnatSubjectPrivate : public ctkXnatObjectPrivate
{
public:

  ctkXnatSubjectPrivate()
  : ctkXnatObjectPrivate()
  {
  }

  void reset()
  {
    insertDate.clear();
    insertUser.clear();
//    uri.clear();
    projects.clear();
  }

  QString insertDate;
  QString insertUser;
//  QString uri;

  QList<ctkXnatProject*> projects;
};


//----------------------------------------------------------------------------
ctkXnatSubject::ctkXnatSubject(ctkXnatObject* parent, const QString& schemaType)
: ctkXnatObject(*new ctkXnatSubjectPrivate(), parent, schemaType)
{
}

//----------------------------------------------------------------------------
ctkXnatSubject::~ctkXnatSubject()
{
}


// --------------------------------------------------------------------------
QList<ctkXnatExperiment*> ctkXnatSubject::experiments() const
{
  QList<ctkXnatExperiment*> result;
  foreach(ctkXnatObject* obj, this->children())
  {
    if (ctkXnatExperiment* o = dynamic_cast<ctkXnatExperiment*>(obj))
    {
      result.push_back(o);
    }
  }
  return result;
}

//----------------------------------------------------------------------------
QString ctkXnatSubject::name() const
{
  return this->label();
}

//----------------------------------------------------------------------------
void ctkXnatSubject::setName(const QString &name)
{
  this->setLabel(name);
}

//----------------------------------------------------------------------------
QString ctkXnatSubject::label() const
{
  return this->property(LABEL);
}

//----------------------------------------------------------------------------
void ctkXnatSubject::setLabel(const QString &label)
{
  this->setProperty(LABEL, label);
}

//----------------------------------------------------------------------------
const QString& ctkXnatSubject::insertDate() const
{
  Q_D(const ctkXnatSubject);
  return d->insertDate;
}

//----------------------------------------------------------------------------
void ctkXnatSubject::setInsertDate(const QString& insertDate)
{
  Q_D(ctkXnatSubject);
  d->insertDate = insertDate;
}

//----------------------------------------------------------------------------
const QString& ctkXnatSubject::insertUser() const
{
  Q_D(const ctkXnatSubject);
  return d->insertUser;
}

//----------------------------------------------------------------------------
void ctkXnatSubject::setInsertUser(const QString& insertUser)
{
  Q_D(ctkXnatSubject);
  d->insertUser = insertUser;
}

//----------------------------------------------------------------------------
QString ctkXnatSubject::resourceUri() const
{
  return QString("%1/subjects/%2").arg(parent()->resourceUri(), this->id());
}

//----------------------------------------------------------------------------
void ctkXnatSubject::reset()
{
  Q_D(ctkXnatSubject);
  d->reset();
}

//----------------------------------------------------------------------------
void ctkXnatSubject::fetchImpl()
{
  QString experimentsUri = this->resourceUri() + "/experiments";
  ctkXnatSession* const session = this->session();
  QUuid queryId = session->httpGet(experimentsUri);
  QList<ctkXnatObject*> experiments = session->httpResults(queryId,
                                                           ctkXnatDefaultSchemaTypes::XSI_EXPERIMENT);

  foreach (ctkXnatObject* experiment, experiments)
  {
    QString label = experiment->name();
    if (!label.isEmpty())
    {
      experiment->setId(label);
    }

    this->add(experiment);
  }
  this->fetchResources();
}

//----------------------------------------------------------------------------
void ctkXnatSubject::downloadImpl(const QString& filename)
{
  qDebug() << "ctkXnatSubject::downloadImpl(const QString& filename) not yet implemented or not available by REST API";
}
