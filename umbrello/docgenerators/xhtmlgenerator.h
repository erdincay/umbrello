/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006      Gael de Chalendar (aka Kleag) kleag@free.fr   *
 *   copyright (C) 2006-2009                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                  *
 ***************************************************************************/

#ifndef XHTMLGENERATOR_H
#define XHTMLGENERATOR_H

#include <kurl.h>
#include <ktempdir.h>

#include <QObject>

class UMLDoc;

class Docbook2XhtmlGeneratorJob;

/**
 * class XhtmlGenerator is a documentation generator for UML documents.
 * It uses first @ref DocbookGenerator to convert the XMI generated by
 * UMLDoc::saveToXMI to docbook and next libxslt through
 * the XSLT file stored in resources to convert the docbook file to XHTML.
 * The latter uses the XSLT available on the Web at
 * http://docbook.sourceforge.net/release/xsl/snapshot/html/docbook.xsl
 *
 * @todo allow to specify the destination and ensure that it works with distant
 * ones
 */
class XhtmlGenerator : public QObject
{
    Q_OBJECT
public:

    XhtmlGenerator();
    virtual ~XhtmlGenerator();

    bool generateXhtmlForProject();
    bool generateXhtmlForProjectInto(const KUrl& destDir);

signals:

    void finished(bool status);

protected slots:

    void slotDocbookToXhtml(bool status);
    void slotHtmlGenerated(const QString& tmpFileName);

    void threadFinished();

private:

    Docbook2XhtmlGeneratorJob* m_d2xg;

    bool m_pStatus;
    bool m_pThreadFinished;

    KUrl m_destDir;  ///< Destination directory where the final documentation will be written.
    UMLDoc* m_umlDoc;
};

#endif // XHTMLGENERATOR_H
