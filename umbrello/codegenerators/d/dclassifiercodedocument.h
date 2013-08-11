/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2007 Jari-Matti Mäkelä <jmjm@iki.fi>                    *
 *   copyright (C) 2008-2013                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                  *
 ***************************************************************************/

#ifndef DCLASSIFIERCODEDOCUMENT_H
#define DCLASSIFIERCODEDOCUMENT_H

#include "codeclassfieldlist.h"
#include "classifiercodedocument.h"
#include "classifier.h"
#include "hierarchicalcodeblock.h"
#include "dcodeclassfield.h"
#include "dcodeoperation.h"

#include <QString>

class DClassDeclarationBlock;
class DCodeGenerationPolicy;

/**
 * A D UMLClassifier Code Document.
 * We carve the D document up into sections as follows:
 * - header
 * - package declaration
 * - import statements
 * - class declaration
 * -   guts of the class (e.g. field decl, accessor methods, operations, dependant classes)
 */
class DClassifierCodeDocument : public ClassifierCodeDocument
{
    Q_OBJECT
public:

    explicit DClassifierCodeDocument(UMLClassifier * classifier);
    virtual ~DClassifierCodeDocument();

//    CodeDocumentDialog getDialog();

    DCodeGenerationPolicy * getDPolicy();

    QString getDClassName(const QString &name);

    QString getPath();

    bool addCodeOperation(CodeOperation * op);

    void updateContent();

protected:

    void resetTextBlocks();

    virtual void loadChildTextBlocksFromNode(QDomElement & root);

    void addOrUpdateCodeClassFieldMethodsInCodeBlock(CodeClassFieldList &list, DClassDeclarationBlock * codeBlock);

    bool forceDoc();

private:

    DClassDeclarationBlock * classDeclCodeBlock;
    HierarchicalCodeBlock * constructorBlock;
    HierarchicalCodeBlock * operationsBlock;

/*
    QString fileName; // Just for our convience in creating code
    QString endLine; // characters for ending line. Just for our convience in creating code
    QString DClassName;
*/
    void init();
    DClassDeclarationBlock * getClassDecl();

};

#endif // DCLASSIFIERCODEDOCUMENT_H
