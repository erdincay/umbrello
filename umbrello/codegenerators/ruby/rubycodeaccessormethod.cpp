/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2005                                                    *
 *   Richard Dale  <Richard_Dale@tipitina.demon.co.uk>                     *
 *   copyright (C) 2006-2013                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                  *
 ***************************************************************************/

// own header
#include "rubycodeaccessormethod.h"

// local includes
#include "attribute.h"
#include "codegenerator.h"
#include "classifiercodedocument.h"
#include "codegen_utils.h"
#include "debug_utils.h"
#include "rubyclassifiercodedocument.h"
#include "rubycodegenerationpolicy.h"
#include "rubycodegenerator.h"
#include "rubycodeclassfield.h"
#include "rubycodedocumentation.h"
#include "umlobject.h"
#include "umlrole.h"
#include "uml.h"

// qt includes
#include <QRegExp>

/**
 * Constructor.
 */
RubyCodeAccessorMethod::RubyCodeAccessorMethod(CodeClassField * field, CodeAccessorMethod::AccessorType type)
  : CodeAccessorMethod(field)
{
    setType(type);

    // lets use full-blown comment
    RubyClassifierCodeDocument *rccd = dynamic_cast<RubyClassifierCodeDocument*>(field->getParentDocument());
    setComment(new RubyCodeDocumentation(rccd));
}

/**
 * Empty Destructor.
 */
RubyCodeAccessorMethod::~RubyCodeAccessorMethod()
{
}

/**
 * Set attributes of the node that represents this class
 * in the XMI document.
 */
void RubyCodeAccessorMethod::setAttributesOnNode(QDomDocument& doc, QDomElement& blockElement)
{
    // set super-class attributes
    CodeAccessorMethod::setAttributesOnNode(doc, blockElement);

    // set local attributes now
}

/**
 * Set the class attributes of this object from
 * the passed element node.
 */
void RubyCodeAccessorMethod::setAttributesFromNode(QDomElement& root)
{
    // set attributes from superclass method the XMI
    CodeAccessorMethod::setAttributesFromNode(root);

    // load local stuff
}

void RubyCodeAccessorMethod::updateContent()
{
    CodeClassField * parentField = getParentClassField();
    RubyCodeClassField * rubyfield = dynamic_cast<RubyCodeClassField*>(parentField);
    QString fieldName = rubyfield->getFieldName();
    QString endLine = UMLApp::app()->commonPolicy()->getNewLineEndingChars();

    QString text = "";
    switch(getType()) {
    case CodeAccessorMethod::ADD:
        {
            int maxOccurs = rubyfield->maximumListOccurances();
            QString fieldType = rubyfield->getTypeName();
            QString indent = getIndentation();
            if(maxOccurs > 0)
                text += "if "+fieldName+".size() < "+ QString::number(maxOccurs)+' '+endLine+indent;
            text += fieldName+".push(value)";
            if(maxOccurs > 0)
            {
                text += endLine+"else"+endLine;
                text += indent + "puts(\"ERROR: Cannot add" + fieldType + " to " + fieldName
                     + ", minimum number of items reached.\")" + endLine + "end" + endLine;
            }
            break;
        }
    case CodeAccessorMethod::GET:
//        text = "return "+fieldName;
        break;
    case CodeAccessorMethod::LIST:
        text = "return "+fieldName;
        break;
    case CodeAccessorMethod::REMOVE:
        {
            int minOccurs = rubyfield->minimumListOccurances();
            QString fieldType = rubyfield->getTypeName();
            QString indent = getIndentation();

            if(minOccurs > 0)
                text += "if "+fieldName+".size() >= "+ QString::number(minOccurs)+endLine+indent;
            text += fieldName+".delete(value)";
            if(minOccurs > 0)
            {
                text += endLine+"else"+endLine;
                text += indent + "puts(\"ERROR: Cant remove"+fieldType+" from "+fieldName+", minimum number of items reached.\")"+endLine+"end"+endLine;
            }
            break;
        }
    case CodeAccessorMethod::SET:
//        text = fieldName+" = value";
        break;
    default:
        // do nothing
        break;
    }

    setText(text);
}

void RubyCodeAccessorMethod::updateMethodDeclaration()
{
    RubyCodeClassField * rubyfield = dynamic_cast<RubyCodeClassField*>(getParentClassField());

    // gather defs
    CodeGenerationPolicy *p = UMLApp::app()->commonPolicy();
    Uml::Visibility::Enum scopePolicy = p->getAttributeAccessorScope();
    QString strVis = Uml::Visibility::toString(rubyfield->getVisibility());
    QString fieldName = RubyCodeGenerator::cppToRubyName(rubyfield->getFieldName());
    QString fieldType = RubyCodeGenerator::cppToRubyType(rubyfield->getTypeName());
    QString objectType = rubyfield->getListObjectType();
    if(objectType.isEmpty())
        objectType = fieldName;
    QString endLine = p->getNewLineEndingChars();

    QString description = getParentObject()->doc();
    description.remove(QRegExp("m_[npb](?=[A-Z])"));
    description.remove("m_");
    description.replace(QRegExp("[\\n\\r]+[\\t ]*"), endLine);

    // set scope of this accessor appropriately..if its an attribute,
    // we need to be more sophisticated
    if(rubyfield->parentIsAttribute())
        switch (scopePolicy) {
        case Uml::Visibility::Public:
        case Uml::Visibility::Private:
        case Uml::Visibility::Protected:
            strVis = Uml::Visibility::toString(scopePolicy);
            break;
        default:
        case Uml::Visibility::FromParent:
            // do nothing..already have taken parent value
            break;
        }

    // some variables we will need to populate
    QString headerText = "";
    QString methodReturnType = "";
    QString methodName = "";
    QString methodParams = "";

    switch(getType()) {
    case CodeAccessorMethod::ADD:
        methodName = "add" + Codegen_Utils::capitalizeFirstLetter(fieldType);
        methodReturnType = "";
        methodParams = objectType+" value ";
        headerText = "Add an object of type "+objectType+" to the Array "+fieldName+endLine+description+endLine+"@return nil";
        setStartMethodText("def "+ methodName + '(' + methodParams + ')');
        setEndMethodText("end");
        break;
    case CodeAccessorMethod::GET:
        headerText = "Get the value of " + fieldName + endLine + description;
        setStartMethodText(QString("attr_reader :") + fieldName);
        setEndMethodText("");
        break;
    case CodeAccessorMethod::LIST:
        methodName = "get" + Codegen_Utils::capitalizeFirstLetter(fieldType)+"List";
        methodReturnType = "";
        headerText = "Get the list of "+fieldName+endLine+description+endLine+"_returns_ List of "+fieldName;
        setStartMethodText("def "+ methodName + '(' + methodParams + ')');
        setEndMethodText("end");
        break;
    case CodeAccessorMethod::REMOVE:
        methodName = "remove" + Codegen_Utils::capitalizeFirstLetter(fieldType);
        methodReturnType = "";
        methodParams = objectType+" value ";
        headerText = "Remove an object of type "+objectType+" from the List "+fieldName+endLine+description;
        setStartMethodText("def "+ methodName + '(' + methodParams + ')');
        setEndMethodText("end");
        break;
    case CodeAccessorMethod::SET:
        headerText = "Set the value of " + fieldName + endLine + description;
        setStartMethodText(QString("attr_writer :") + fieldName);
        setEndMethodText("");
        break;
    default:
        // do nothing..no idea what this is
        uWarning() << "Warning: can not generate RubyCodeAccessorMethod for type: " << getType();
        break;
    }

    // set header once.
    if (getComment()->getText().isEmpty())
        getComment()->setText(headerText);
}

/**
 * Must be called before this object is usable.
 */
void RubyCodeAccessorMethod::update()
{
    updateMethodDeclaration();
    updateContent();
}

#include "rubycodeaccessormethod.moc"
