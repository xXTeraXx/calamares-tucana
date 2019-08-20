/* === This file is part of Calamares - <https://github.com/calamares> ===
 *
 *   Copyright 2019, Adriaan de Groot <groot@kde.org>
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

/** @brief Loading items from AppData XML files.
 *
 * Only used if QtXML is found, implements PackageItem::fromAppData().
 */
#include "PackageModel.h"

#include "locale/LabelModel.h"
#include "utils/Logger.h"
#include "utils/Variant.h"

#include <AppStreamQt/image.h>
#include <AppStreamQt/pool.h>
#include <AppStreamQt/screenshot.h>

/// @brief Return number of pixels in a size, for < ordering purposes
static inline quint64
sizeOrder( const QSize& size )
{
    return size.width() * size.height();
}

/// @brief Sets a screenshot in @p map from @p screenshot, if a usable one is found
static void
setScreenshot( QVariantMap& map, const AppStream::Screenshot& screenshot )
{
    if ( screenshot.images().count() < 1 )
    {
        return;
    }

    // Pick the smallest
    QUrl url;
    quint64 size = sizeOrder( screenshot.images().first().size() );
    for ( const auto& img : screenshot.images() )
    {
        if ( sizeOrder( img.size() ) <= size )
        {
            url = img.url();
        }
    }

    if ( url.isValid() )
    {
        map.insert( "screenshot", url.toString() );
    }
}

/// @brief Interpret an AppStream Component
static PackageItem
fromComponent( AppStream::Component& component )
{
    QVariantMap map;
    map.insert( "id", component.id() );
    map.insert( "name", component.name() );
    map.insert( "description", component.description() );
    map.insert( "package", component.packageNames().join( "," ) );

    auto screenshots = component.screenshots();
    if ( screenshots.count() > 0 )
    {
        bool done = false;
        for ( const auto& s : screenshots )
        {
            if ( s.isDefault() )
            {
                setScreenshot( map, s );
                done = true;
                break;
            }
        }
        if ( !done )
        {
            setScreenshot( map, screenshots.first() );
        }
    }

    return PackageItem( map );
}

PackageItem
fromAppStream( const QVariantMap& item_map )
{
    QString appstreamId = CalamaresUtils::getString( item_map, "appstream" );
    if ( appstreamId.isEmpty() )
    {
        cWarning() << "Can't load AppStream without a suitable appstreamId.";
        return PackageItem();
    }
    cDebug() << "Loading AppStream data for" << appstreamId;

    AppStream::Pool pool;
    if ( !pool.load() )
    {
        cWarning() << "AppStream load failed" << pool.lastError();
        return PackageItem();
    }

    auto itemList = pool.componentsById( appstreamId );
    if ( itemList.count() < 1 )
    {
        cWarning() << "No AppStream data for" << appstreamId;
        return PackageItem();
    }
    if ( itemList.count() > 1 )
    {
        cDebug() << "Multiple AppStream data for" << appstreamId << "using first.";
    }

    auto r = fromComponent( itemList.first() );
    if ( r.isValid() )
    {
        QString id = CalamaresUtils::getString( item_map, "id" );
        QString screenshotPath = CalamaresUtils::getString( item_map, "screenshot" );
        if ( !id.isEmpty() )
        {
            r.id = id;
        }
        if ( !screenshotPath.isEmpty() )
        {
            r.screenshot = screenshotPath;
        }
    }
    return r;
}
