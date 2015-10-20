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

#include "ctkXnatImageSession.h"

#include "ctkXnatSession.h"
#include "ctkXnatObjectPrivate.h"
#include "ctkXnatSubject.h"
#include "ctkXnatScan.h"
#include "ctkXnatReconstruction.h"
#include "ctkXnatAssessor.h"
#include "ctkXnatScanFolder.h"
#include "ctkXnatReconstructionFolder.h"
#include "ctkXnatAssessorFolder.h"
#include "ctkXnatDefaultSchemaTypes.h"

#include <QDebug>

const QString ctkXnatImageSession::DATE_OF_ACQUISITION = "date";
const QString ctkXnatImageSession::TIME_OF_ACQUISITION = "time";
const QString ctkXnatImageSession::SCANNER_TYPE = "scanner";
const QString ctkXnatImageSession::IMAGE_MODALITY = "modality";

//----------------------------------------------------------------------------
class ctkXnatImageSessionPrivate : public ctkXnatObjectPrivate
{
public:

  ctkXnatImageSessionPrivate()
  : ctkXnatObjectPrivate()
  {
  }

  void reset()
  {
  }
};


//----------------------------------------------------------------------------
ctkXnatImageSession::ctkXnatImageSession(ctkXnatObject* parent, const QString& schemaType)
: ctkXnatExperiment(*new ctkXnatImageSessionPrivate(), parent, schemaType)
{
}

//----------------------------------------------------------------------------
ctkXnatImageSession::~ctkXnatImageSession()
{
}

//----------------------------------------------------------------------------
QString ctkXnatImageSession::dateOfAcquisition() const
{
  return this->property(DATE_OF_ACQUISITION);
}

//----------------------------------------------------------------------------
void ctkXnatImageSession::setDateOfAcquisition(const QString &dateOfAcquisition)
{
  this->setProperty(DATE_OF_ACQUISITION, dateOfAcquisition);
}

//----------------------------------------------------------------------------
QString ctkXnatImageSession::timeOfAcquisition() const
{
  return this->property(TIME_OF_ACQUISITION);
}

//----------------------------------------------------------------------------
void ctkXnatImageSession::setTimeOfAcquisition(const QString &timeOfAcquisition)
{
  this->setProperty(TIME_OF_ACQUISITION, timeOfAcquisition);
}

//----------------------------------------------------------------------------
QString ctkXnatImageSession::scannerType() const
{
  return this->property(SCANNER_TYPE);
}

//----------------------------------------------------------------------------
void ctkXnatImageSession::setScannerType(const QString &scannerType)
{
  this->setProperty(SCANNER_TYPE, scannerType);
}

//----------------------------------------------------------------------------
QString ctkXnatImageSession::imageModality() const
{
  return this->property(IMAGE_MODALITY);
}

//----------------------------------------------------------------------------
void ctkXnatImageSession::setImageModality(const QString &imageModality)
{
  this->setProperty(IMAGE_MODALITY, imageModality);
}

//----------------------------------------------------------------------------
void ctkXnatImageSession::fetchImpl()
{
  QString scansUri = this->resourceUri() + "/scans";
  ctkXnatSession* const session = this->session();
  QUuid scansQueryId = session->httpGet(scansUri);

  QList<ctkXnatObject*> scans;
  
  try
  {
    scans = session->httpResults(scansQueryId,
				 ctkXnatDefaultSchemaTypes::XSI_SCAN);
  }
  catch (const ctkException& e)
  {
    qWarning() << QString(e.what());
  }

  if (!scans.isEmpty())
  {
    ctkXnatScanFolder* scanFolder = new ctkXnatScanFolder();
    this->add(scanFolder);
  }

  QString reconstructionsUri = this->resourceUri() + "/reconstructions";
  QUuid reconstructionsQueryId = session->httpGet(reconstructionsUri);

  QList<ctkXnatObject*> reconstructions;
  try
  {
    reconstructions = session->httpResults(reconstructionsQueryId,
					   ctkXnatDefaultSchemaTypes::XSI_RECONSTRUCTION);
  }
  catch (const ctkException& e)
  {
    qWarning() << QString(e.what());
  }
  
  if (!reconstructions.isEmpty())
  {
    ctkXnatReconstructionFolder* reconstructionFolder = new ctkXnatReconstructionFolder();
    this->add(reconstructionFolder);
  }

  QString assessorsUri = this->resourceUri() + "/assessors";
  QUuid assessorsQueryId = session->httpGet(assessorsUri);
  
  QList<ctkXnatObject*> assessors;
  
  try
  {
    assessors = session->httpResults(assessorsQueryId,
				     ctkXnatDefaultSchemaTypes::XSI_ASSESSOR);
  }
  catch (const ctkException& e)
  {
    qWarning() << QString(e.what());
  }

  if (!assessors.isEmpty())
  {
    ctkXnatAssessorFolder* assessorFolder = new ctkXnatAssessorFolder(this);
    this->add(assessorFolder);
  }

  this->fetchResources();
}
