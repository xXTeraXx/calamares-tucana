/* === This file is part of Calamares - <http://github.com/calamares> ===
 *
 *   Copyright 2014, Teo Mrnjavac <teo@kde.org>
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CalamaresApplication.h"

#include "CalamaresWindow.h"
#include "CalamaresVersion.h"
#include "Settings.h"
#include "utils/CalamaresUtils.h"
#include "utils/Logger.h"


CalamaresApplication::CalamaresApplication( int& argc, char *argv[] )
    : QApplication( argc, argv )
    , m_mainwindow( 0 )
{
    setOrganizationName( QLatin1String( CALAMARES_ORGANIZATION_NAME ) );
    setOrganizationDomain( QLatin1String( CALAMARES_ORGANIZATION_DOMAIN ) );
    setApplicationName( QLatin1String( CALAMARES_APPLICATION_NAME ) );
    setApplicationVersion( QLatin1String( CALAMARES_VERSION ) );

    CalamaresUtils::installTranslator( this );
}


void
CalamaresApplication::init()
{
    qDebug() << "CalamaresApplication thread:" << thread();

    //TODO: Icon loader
    Logger::setupLogfile();

    setQuitOnLastWindowClosed( false );

    initSettings();

    initBranding();

    setWindowIcon( QIcon( "from branding" ) );

    initPlugins();

    initJobQueue();

    m_mainwindow = new CalamaresWindow();
    m_mainwindow->show();
}


CalamaresApplication::~CalamaresApplication()
{
    cDebug( LOGVERBOSE ) << "Shutting down Calamares...";

//    if ( JobQueue::instance() )
//        JobQueue::instance()->stop();

//    delete m_mainwindow;

//    delete JobQueue::instance();

    cDebug( LOGVERBOSE ) << "Finished shutdown.";
}


CalamaresApplication*
CalamaresApplication::instance()
{
    return qobject_cast< CalamaresApplication* >( QApplication::instance() );
}


void
CalamaresApplication::initSettings()
{
    new Calamares::Settings( this );
}


void
CalamaresApplication::initBranding()
{

}


void
CalamaresApplication::initPlugins()
{

}


void
CalamaresApplication::initJobQueue()
{

}


