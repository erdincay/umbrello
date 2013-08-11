/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2007 Jari-Matti Mäkelä <jmjm@iki.fi>                    *
 *   copyright (C) 2008-2011                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                  *
 ***************************************************************************/

#include "dcodeoperation.h"

#include "dcodegenerator.h"
#include "dcodegenerationpolicy.h"
#include "dclassifiercodedocument.h"
#include "dcodedocumentation.h"
#include "uml.h"

// Constructors/Destructors
//

DCodeOperation::DCodeOperation( DClassifierCodeDocument * doc, UMLOperation *parent,
const QString & body, const QString & comment ) : CodeOperation (doc, parent, body, comment)
{
    // lets not go with the default comment and instead use
    // full-blown d documentation object instead
    setComment(new DCodeDocumentation(doc));

    // these things never change..
    setOverallIndentationLevel(1);
}

DCodeOperation::~DCodeOperation()
{
}

// we basically want to update the doc and start text of this method
void DCodeOperation::updateMethodDeclaration()
{
    CodeDocument * doc = getParentDocument();
    DClassifierCodeDocument * ddoc = dynamic_cast<DClassifierCodeDocument*>(doc);
    UMLOperation * o = getParentOperation();
    bool isInterface = ddoc->getParentClassifier()->isInterface();
    QString endLine = getNewLineEndingChars();

    /*
     * Member function declaration
     *
     * (visibility) (static | abstract | override) retType name (param1, ..., paramN) (; | {)
     *      a          b         c          d         e      f     g                     h
     */

    QString startText;

    // (a) visibility modifier
    //FIXME: startText += o->getVisibility().toString() + " ";

    // (b) static
    if (o->isStatic()) startText += "static ";

    // (c) abstract
    //TODO

    // (d) override
    //TODO

    // (e) return type
    if (!o->isConstructorOperation()) {
    //FIXME:     startText += DCodeGenerator::fixTypeName(o->getTypeName()) + " ";
    }

    // (f) name
    startText += o->name();

    // (g) params
    startText += '(';

    // assemble parameters
    QString paramStr = QString("");
    UMLAttributeList list = getParentOperation()->getParmList();
    int paramNum = list.count();

    foreach (UMLAttribute* parm, list ) {
        QString rType = parm->getTypeName();
        QString paramName = parm->name();
        paramStr += rType + ' ' + paramName;
        paramNum--;

        if (paramNum > 0) paramStr += ", ";
    }

    startText += paramStr;

    startText += ')';

    // (h) function body
    if(isInterface) {
        startText += ';';
        setEndMethodText("");
    } else {
        startText += " {";
        setEndMethodText("}");
    }

    setStartMethodText(startText);

    // Lastly, for text content generation, we fix the comment on the
    // operation, IF the codeop is autogenerated & currently empty
    QString comment = o->doc();
    if(comment.isEmpty() && contentType() == CodeBlock::AutoGenerated)
    {
        UMLAttributeList parameters = o->getParmList();
        foreach (UMLAttribute* currentAtt, parameters ) {
            comment += endLine + "@param " + currentAtt->name() + ' ';
            comment += currentAtt->doc();
        }
        // add a returns statement too
        // TODO proper return type comments
        //if(!returnType.isEmpty())
        //    comment += endLine + "@return " + returnType + ' ';

        getComment()->setText(comment);
    }
}

int DCodeOperation::lastEditableLine()
{
    ClassifierCodeDocument * doc = dynamic_cast<ClassifierCodeDocument*>(getParentDocument());

    // very last line is NOT editable as its a one-line declaration
    // w/ no body in an interface.
    if (doc->parentIsInterface()) return -1;

    return 0;
}

#include "dcodeoperation.moc"
