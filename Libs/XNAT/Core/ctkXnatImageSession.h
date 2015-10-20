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

#ifndef ctkXnatImageSession_h
#define ctkXnatImageSession_h

#include "ctkXNATCoreExport.h"

#include "ctkXnatExperiment.h"
#include "ctkXnatDefaultSchemaTypes.h"

class ctkXnatImageSessionPrivate;

/**
 * @ingroup XNAT_Core
 */
class CTK_XNAT_CORE_EXPORT ctkXnatImageSession : public ctkXnatExperiment
{

public:

  ctkXnatImageSession(ctkXnatObject* parent = 0, const QString& schemaType = ctkXnatDefaultSchemaTypes::XSI_IMAGE_SESSION);

  virtual ~ctkXnatImageSession();

  QString dateOfAcquisition() const;
  void setDateOfAcquisition(const QString &dateOfAcquisition);

  QString timeOfAcquisition() const;
  void setTimeOfAcquisition(const QString &timeOfAcquisition);

  QString scannerType() const;
  void setScannerType(const QString &scannerType);

  QString imageModality() const;
  void setImageModality(const QString &imageModality);

  static const QString DATE_OF_ACQUISITION;
  static const QString TIME_OF_ACQUISITION;
  static const QString SCANNER_TYPE;
  static const QString IMAGE_MODALITY;

private:

  virtual void fetchImpl();

  Q_DECLARE_PRIVATE(ctkXnatImageSession)
};

#endif
