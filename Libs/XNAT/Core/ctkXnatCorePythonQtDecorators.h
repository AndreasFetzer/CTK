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

#ifndef __ctkXnatCorePythonQtDecorators_h
#define __ctkXnatCorePythonQtDecorators_h

// PythonQt includes
#include <PythonQt.h>

// CTK includes
#include <ctkXnatDataModel.h>
#include <ctkXnatProject.h>
#include <ctkXnatSubject.h>
#include <ctkXnatExperiment.h>
#include <ctkXnatScan.h>
#include <ctkXnatResource.h>
#include <ctkXnatAssessor.h>
#include <ctkXnatFile.h>

#include <ctkXnatLoginProfile.h>
#include <ctkXnatSession.h>

// NOTE:
//
// For decorators it is assumed that the methods will never be called
// with the self argument as NULL.  The self argument is the first argument
// for non-static methods.
//

/// \ingroup ctkCore
class ctkXnatCorePythonQtDecorators : public QObject
{
  Q_OBJECT
public:

  ctkXnatCorePythonQtDecorators()
    {

    PythonQt::self()->registerCPPClass("ctkXnatLoginProfile", 0, "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatSession",      0, "CTKXNATCore");

    PythonQt::self()->registerCPPClass("ctkXnatObject",                   0, "CTKXNATCore");    
    PythonQt::self()->registerCPPClass("ctkXnatDataModel",  "ctkXnatObject", "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatProject",    "ctkXnatObject", "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatSubject",    "ctkXnatObject", "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatExperiment", "ctkXnatObject", "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatScan",       "ctkXnatObject", "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatResource",   "ctkXnatObject", "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatAssessor",   "ctkXnatObject", "CTKXNATCore");
    PythonQt::self()->registerCPPClass("ctkXnatFile",       "ctkXnatObject", "CTKXNATCore");

    }

public Q_SLOTS:

  QString id(ctkXnatObject* object) 
    {
    return object->id();
    }

 
  /********************************************************
   ********************************************************
         Decorators for constructors and destructors
   ********************************************************
   ********************************************************/
 
  ctkXnatDataModel* new_ctkXnatDataModel(ctkXnatSession* connection) { return new ctkXnatDataModel(connection); }
  void delete_ctkXnatDataModel(ctkXnatDataModel * object) { delete object; }
  ctkXnatProject* new_ctkXnatProject() { return new ctkXnatProject(); }
  void delete_ctkXnatProject(ctkXnatProject * object) { delete object; }
  ctkXnatSubject* new_ctkXnatSubject() { return new ctkXnatSubject(); }
  void delete_ctkXnatSubject(ctkXnatSubject * object) { delete object; }
  ctkXnatScan* new_ctkXnatScan() { return new ctkXnatScan(); }
  void delete_ctkXnatScan(ctkXnatScan * object) { delete object; }
  ctkXnatResource* new_ctkXnatResource() { return new ctkXnatResource(); }
  void delete_ctkXnatResource(ctkXnatResource * object) { delete object; }
  ctkXnatFile* new_ctkXnatFile() { return new ctkXnatFile(); }
  void delete_ctkXnatFile(ctkXnatFile * object) { delete object; }

  /********************************************************
   ********************************************************
           Decorators for XnatObject methods
   ********************************************************
   ********************************************************/
  
  QList<ctkXnatResource*> resources(ctkXnatObject* object) const
  {
    return object->resources(); 
  }
  void setId(ctkXnatObject* object, const QString& id)
  {
    object->setId (id); 
  }  

  virtual QString resourceUri(ctkXnatObject* object) const
  {
    return object->resourceUri(); 
  }  

  QString name(ctkXnatObject* object) const
  {
    return object->name(); 
  }  

  void setName(ctkXnatObject* object, const QString& name)
  {
    object->setName (name); 
  }  

  QString description(ctkXnatObject* object ) const
  {
    return object->description(); 
  }  

  void setDescription(ctkXnatObject* object, const QString& description)
  {
    object->setDescription (description); 
  }  

  QString property(ctkXnatObject* object, const QString& name) const
  {
    return object->property(name); 
  }  

  void setProperty(ctkXnatObject* object, const QString& name, const QVariant& value)
  {
    object->setProperty (name, value); 
  }  

  QDateTime lastModifiedTime(ctkXnatObject* object)
  {
    return object->lastModifiedTimeOnServer();
  }  

  void setLastModifiedTime(ctkXnatObject* object, const QDateTime& lastModifiedTime)
  {
    object->setLastModifiedTime (lastModifiedTime); 
  }  

  ctkXnatObject* parent(ctkXnatObject* object) const
  {
    return object->parent();
  }  

  void setParent(ctkXnatObject* object, ctkXnatObject* parent)
  {
    object->setParent(parent); 
  }  

  QList<ctkXnatObject*> children(ctkXnatObject* object) const
  {
    return object->children(); 
  }  

  void add(ctkXnatObject* object, ctkXnatObject* child)
  {
    object->add (child); 
  }  

  void remove(ctkXnatObject* object, ctkXnatObject* child)
  {
    object->remove (child); 
  }  

  bool isFetched(ctkXnatObject* object) const
  {
    return object->isFetched(); 
  }  

  QString schemaType(ctkXnatObject* object) const
  {
    return object->schemaType(); 
  }  

  virtual QString childDataType(ctkXnatObject* object) const
  {
    return object->childDataType(); 
  }  

  virtual void reset(ctkXnatObject* object)
  {
    object->reset(); 
  }  

  void fetch(ctkXnatObject* object)
  {
    object->fetch(); 
  }  

  bool exists(ctkXnatObject* object) const
  {
    return object->exists(); 
  }  

  void save(ctkXnatObject* object)
  {
    object->save(); 
  }  

  void erase(ctkXnatObject* object)
  {
    object->erase(); 
  }  

  virtual void download(ctkXnatObject* object, const QString& str)
  {
    object->download (str);
  }

  ctkXnatResource* addResourceFolder(ctkXnatObject* object, const QString& foldername/*,
                                     const QString& format, const QString& content,
                                     const QString& tags*/)
  {
    return object->addResourceFolder(foldername/*, format, content, tags*/);
  }

  
  /********************************************************
   ********************************************************
         Decorators for XnatObject subclasses methods
   ********************************************************
   ********************************************************/
  
  QList<ctkXnatProject*> projects(ctkXnatDataModel* object) const
  {
    return object->projects(); 
  }
  QList<ctkXnatSubject*> subjects(ctkXnatProject* object) const
  {
    return object->subjects(); 
  }
  QList<ctkXnatExperiment*> experiments(ctkXnatSubject* object) const
  {
    return object->experiments(); 
  }
  QList<ctkXnatScan*> scans(ctkXnatExperiment* object) const
  {
    return object->scans(); 
  }
  QList<ctkXnatAssessor*> assessors(ctkXnatExperiment* object) const
  {
    return object->assessors(); 
  }
  QList<ctkXnatReconstruction*> reconstructions(ctkXnatExperiment* object) const
  {
    return object->reconstructions(); 
  }
  QList<ctkXnatFile*> files(ctkXnatResource* object) const
  {
    return object->files(); 
  }

  ctkXnatSession* session(ctkXnatDataModel* object) const
  {
    return object->session (); 
  }

  void setLocalFilePath(ctkXnatFile* file, const QString& localFilePath)
  {
    file->setLocalFilePath(localFilePath);
  }

  void setFileFormat(ctkXnatFile* file, const QString& fileFormat)
  {
    file->setFileFormat(fileFormat);
  }

  void setFileContent(ctkXnatFile* file, const QString& fileContent)
  {
    file->setFileContent(fileContent);
  }

  void setFileTags(ctkXnatFile* file, const QString& fileTags)
  {
    file->setFileTags(fileTags);
  }

  /********************************************************
   ********************************************************
         Decorators for XnatLoginProfile methods
   ********************************************************
   ********************************************************/

  ctkXnatLoginProfile* new_ctkXnatLoginProfile() { return new ctkXnatLoginProfile(); }
  ctkXnatLoginProfile* new_ctkXnatLoginProfile(const ctkXnatLoginProfile& otherLoginProfile)
    {
    return new ctkXnatLoginProfile(otherLoginProfile);
    }
  void delete_ctkXnatLoginProfile(ctkXnatLoginProfile * object) { delete object; }

  QString name(ctkXnatLoginProfile * object) const
    {
    return object->name();
    }  

  void setName(ctkXnatLoginProfile * object, const QString& profileName)
    {
    object->setName (profileName);
    }

  QUrl serverUrl(ctkXnatLoginProfile * object) const
    {
    return object->serverUrl();
    }

  void setServerUrl(ctkXnatLoginProfile * object, const QUrl& serverUri)
    {
    object->setServerUrl (serverUri);
    }

  QString userName(ctkXnatLoginProfile * object) const
    {
    return object->userName();
    }

  void setUserName(ctkXnatLoginProfile * object, const QString& userName)
    {
    object->setUserName (userName);
    }

  QString password(ctkXnatLoginProfile * object) const
    {
    return object->password();
    }

  void setPassword(ctkXnatLoginProfile * object, const QString& password)
    {
    object->setPassword (password);
    }

  bool isDefault(ctkXnatLoginProfile * object) const
    {
    return object->isDefault();
    }

  void setDefault(ctkXnatLoginProfile * object, bool default_)
    {
    object->setDefault(default_);
    }
    
  /********************************************************
   ********************************************************
         Decorators for XnatSession methods
   ********************************************************
   ********************************************************/

  ctkXnatSession* new_ctkXnatSession(const ctkXnatLoginProfile& loginProfile) { return new ctkXnatSession(loginProfile); }
  void delete_ctkXnatSession(ctkXnatSession * object) { delete object; }

  void open(ctkXnatSession * object)
    {
    object->open();
    }

  void close(ctkXnatSession * object)
    {
  object->close();
    }

  bool isOpen(ctkXnatSession * object) const
    {
  return object->isOpen();
    }

  QString version(ctkXnatSession * object) const
    {
  return object->version();
    }

  QDateTime expirationDate(ctkXnatSession * object) const
    {
  return object->expirationDate();
    }

  QDateTime renew(ctkXnatSession * object)
    {
  return object->renew();
    }

  /* ctkXnatLoginProfile loginProfile(ctkXnatSession * object) const */
  /*   { */
  /* return object->loginProfile(); */
  /*   } */

  QUrl url(ctkXnatSession * object) const
    {
  return object->url();
    }

  QString userName(ctkXnatSession * object) const
    {
  return object->userName();
    }

  QString password(ctkXnatSession * object) const
    {
  return object->password();
    }

  ctkXnatDataModel* dataModel(ctkXnatSession * object) const
    {
  return object->dataModel();
    }

  QUuid httpGet(ctkXnatSession * object,
                const QString& resource,
                const ctkXnatSession::UrlParameters& parameters = ctkXnatSession::UrlParameters(),
                const ctkXnatSession::HttpRawHeaders& rawHeaders = ctkXnatSession::HttpRawHeaders())
    {
  return object->httpGet (resource, parameters, rawHeaders);
    }

  QList<ctkXnatObject*> httpResults(ctkXnatSession * object, const QUuid& uuid, const QString& schemaType)
    {
  return object->httpResults (uuid, schemaType);
    }

  QList<QVariantMap> httpSync(ctkXnatSession * object, const QUuid& uuid)
    {
  return object->httpSync (uuid);
    }

  const QMap<QByteArray, QByteArray> httpHeadSync(ctkXnatSession * object, const QUuid& uuid)
    {
  return object->httpHeadSync (uuid);
    }

  bool exists(ctkXnatSession * object, const ctkXnatObject* arg)
    {
  return object->exists (arg);
    }

  void remove(ctkXnatSession * object, ctkXnatObject* arg)
    {
  object->remove (arg);
    }

  QUuid httpHead(ctkXnatSession * object, const QString& resourceUri)
    {
  return object->httpHead (resourceUri);
    }
  
};

//-----------------------------------------------------------------------------
/// \ingroup Core
void initCTKXNATCorePythonQtDecorators()
{
  PythonQt::self()->addDecorators(new ctkXnatCorePythonQtDecorators);
}

#endif
