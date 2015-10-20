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

#include "ctkXnatSubjectVariables.h"

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


//----------------------------------------------------------------------------
class ctkXnatSubjectVariablesPrivate : public ctkXnatObjectPrivate
{
public:

  ctkXnatSubjectVariablesPrivate()
  : ctkXnatObjectPrivate()
  {
  }

  void reset()
  {
  }

};


//----------------------------------------------------------------------------
ctkXnatSubjectVariables::ctkXnatSubjectVariables(ctkXnatObject* parent, const QString& schemaType)
: ctkXnatExperiment(*new ctkXnatSubjectVariablesPrivate(), parent, schemaType)
{
}

//----------------------------------------------------------------------------
ctkXnatSubjectVariables::~ctkXnatSubjectVariables()
{
}

//----------------------------------------------------------------------------
void ctkXnatSubjectVariables::fetchImpl()
{
  this->fetchResources();
}
