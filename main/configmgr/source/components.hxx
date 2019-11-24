/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef INCLUDED_CONFIGMGR_SOURCE_COMPONENTS_HXX
#define INCLUDED_CONFIGMGR_SOURCE_COMPONENTS_HXX

#include "sal/config.h"

#include <map>
#include <set>

#include "boost/noncopyable.hpp"
#include "com/sun/star/beans/Optional.hpp"
#include "com/sun/star/uno/Reference.hxx"
#include "rtl/ref.hxx"

#include "additions.hxx"
#include "data.hxx"
#include "modifications.hxx"
#include "nodemap.hxx"
#include "path.hxx"

namespace com { namespace sun { namespace star {
    namespace beans { class XPropertySet; }
    namespace uno {
        class Any;
        class XComponentContext;
    }
} } }
namespace rtl {
    class Bootstrap;
    class OUString;
}

namespace configmgr {

class Broadcaster;
class Node;
class Partial;
class RootAccess;

class Components: private boost::noncopyable {
public:
    static Components & getSingleton(
        com::sun::star::uno::Reference< com::sun::star::uno::XComponentContext >
            const & context);

    static bool allLocales(rtl::OUString const & locale);

    rtl::Reference< Node > resolvePathRepresentation(
        rtl::OUString const & pathRepresentation,
        rtl::OUString * canonicRepresenation, Path * path, int * finalizedLayer)
        const;

    rtl::Reference< Node > getTemplate(
        int layer, rtl::OUString const & fullName) const;

    void addRootAccess(rtl::Reference< RootAccess > const & access);

    void removeRootAccess(RootAccess * access);

    void initGlobalBroadcaster(
        Modifications const & modifications,
        rtl::Reference< RootAccess > const & exclude,
        Broadcaster * broadcaster);

    void addModification(Path const & path);

    void writeModifications();

    void flushModifications();
        // must be called with configmgr::lock unaquired; must be called before
        // shutdown if writeModifications has ever been called (probably
        // indirectly, via removeExtensionXcuFile)

    void insertExtensionXcsFile(bool shared, rtl::OUString const & fileUri);

    void insertExtensionXcuFile(
        bool shared, rtl::OUString const & fileUri,
        Modifications * modifications);

    void removeExtensionXcuFile(
        rtl::OUString const & fileUri, Modifications * modifications);

    void insertModificationXcuFile(
        rtl::OUString const & fileUri,
        std::set< rtl::OUString > const & includedPaths,
        std::set< rtl::OUString > const & excludedPaths,
        Modifications * modifications);

    com::sun::star::beans::Optional< com::sun::star::uno::Any >
    getExternalValue(rtl::OUString const & descriptor);

private:
    typedef void FileParser(
        rtl::OUString const &, int, Data &, Partial const *, Modifications *,
        Additions *);

    Components(
        com::sun::star::uno::Reference< com::sun::star::uno::XComponentContext >
            const & context);

    ~Components();

    void parseFileLeniently(
        FileParser * parseFile, rtl::OUString const & url, int layer,
        Data & data, Partial const * partial, Modifications * modifications,
        Additions * additions);

    void parseFiles(
        int layer, rtl::OUString const & extension, FileParser * parseFile,
        rtl::OUString const & url, bool recursive);

    void parseFileList(
        int layer, FileParser * parseFile, rtl::OUString const & urls,
        rtl::Bootstrap const & ini, bool recordAdditions);

    void parseXcdFiles(int layer, rtl::OUString const & url);

    void parseXcsXcuLayer(int layer, rtl::OUString const & url);

    void parseXcsXcuIniLayer(
        int layer, rtl::OUString const & url, bool recordAdditions);

    void parseModuleLayer(int layer, rtl::OUString const & url);

    void parseResLayer(int layer, rtl::OUString const & url);

    rtl::OUString getModificationFileUrl() const;

    void parseModificationLayer();

    typedef std::set< RootAccess * > WeakRootSet;

    typedef
        std::map<
            rtl::OUString,
            com::sun::star::uno::Reference<
                com::sun::star::beans::XPropertySet > >
        ExternalServices;

    class WriteThread;

    com::sun::star::uno::Reference< com::sun::star::uno::XComponentContext >
        context_;
    Data data_;
    WeakRootSet roots_;
    ExternalServices externalServices_;
    rtl::Reference< WriteThread > writeThread_;
};

}

#endif
