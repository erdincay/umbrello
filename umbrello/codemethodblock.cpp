
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by: 
 *      Author : thomas
 *      Date   : Fri Jun 20 2003
 */

#include "codemethodblock.h"
#include "classifiercodedocument.h"

// Constructors/Destructors
//  

CodeMethodBlock::CodeMethodBlock ( ClassifierCodeDocument * doc, QString body, QString comment)
        : CodeBlockWithComments ((CodeDocument*)doc, body, comment), OwnedCodeBlock ((UMLObject*) doc->getParentClassifier()) 
{
        initFields();
}

CodeMethodBlock::CodeMethodBlock ( ClassifierCodeDocument * doc) 
    : CodeBlockWithComments ((CodeDocument*)doc), OwnedCodeBlock ((UMLObject*) doc->getParentClassifier()) 
{
	initFields();
}

CodeMethodBlock::~CodeMethodBlock ( ) { }

//  
// Methods
//  


// Accessor methods
//  

// we can just call the superclass
CodeDocument * CodeMethodBlock::getParentDocument() {
	return TextBlock::getParentDocument();
}

/**
 * Get the starting text that begins this method before the body is printed.
 */
QString CodeMethodBlock::getStartMethodText() const {
        return m_startMethod;
}

/**
 * Get the ending text that finishes this method after the body is printed.
 */
QString CodeMethodBlock::getEndMethodText() const {
        return m_endMethod;
}

/**
 * Set the starting text that begins this method before the body is printed.
 */
void CodeMethodBlock::setStartMethodText (QString value) {
        m_startMethod = value;
}

/**
 * Set the ending text that finishes this method after the body is printed.
 */
void CodeMethodBlock::setEndMethodText (QString value) {
        m_endMethod = value;
}

// Other methods
//  

 /** set attributes of the node that represents this class
  * in the XMI document.
  */
void CodeMethodBlock::setAttributesOnNode ( QDomDocument & doc, QDomElement & elem) {

        // set super-class attributes
        CodeBlockWithComments::setAttributesOnNode(doc, elem);
        OwnedCodeBlock::setAttributesOnNode(doc, elem);

        // set local class attributes
        QString endLine = getParentDocument()->getNewLineEndingChars();
        elem.setAttribute("startMethodText",encodeText(getStartMethodText(),endLine));
        elem.setAttribute("endMethodText",encodeText(getEndMethodText(),endLine));


}

 /** set the class attributes of this object from
  * the passed element node.
  */
void CodeMethodBlock::setAttributesFromNode ( QDomElement & elem) {

	// set attributes from the XMI
        CodeBlockWithComments::setAttributesFromNode(elem); // superclass load
        OwnedCodeBlock::setAttributesFromNode(elem); // superclass load

	// now load local attributes
        QString endLine = getParentDocument()->getNewLineEndingChars();
        setStartMethodText(decodeText(elem.attribute("startMethodText",""),endLine));
        setEndMethodText(decodeText(elem.attribute("endMethodText",""),endLine));

}

/**
 * @return	QString
 */
QString CodeMethodBlock::toString ( ) {

        QString string = QString("");

        if(getWriteOutText()) {

                QString indent = getIndentationString(getIndentationLevel());
                QString bodyIndent = getIndentationString(getIndentationLevel()+1);
                QString endLine = getNewLineEndingChars();

                QString startMethod = formatMultiLineText ( getStartMethodText(), indent, endLine);
                QString body = formatMultiLineText (getText(), bodyIndent, endLine);
                QString endMethod = formatMultiLineText( getEndMethodText(), indent, endLine);

                QString comment = getComment()->toString();
                if(!comment.isEmpty())
                        string.append(comment);

                if(!startMethod.isEmpty())
                	string.append(startMethod);

                if(!body.isEmpty())
                	string.append(body);

                if(!endMethod.isEmpty())
                	string.append(endMethod);

        }

        return string;

}

void CodeMethodBlock::initFields ( ) {

	m_startMethod = QString("");
        m_endMethod = QString("");
}

void CodeMethodBlock::syncToParent ( )
{

	getComment()->setText(getParentObject()->getDoc());

	updateMethodDeclaration();

        // only update IF we are NOT AutoGenerated
        if(getContentType() != AutoGenerated)
                return;

        updateContent();
}

#include "codemethodblock.moc"
