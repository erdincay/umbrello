
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
 *      Date   : Tue Jul 1 2003
 */

#include <kdebug.h>

#include "codeaccessormethod.h"
#include "classifiercodedocument.h"
#include "codeclassfield.h"

#include "attribute.h"
#include "umlobject.h"
#include "umlrole.h"

// Constructors/Destructors
//  

CodeAccessorMethod::CodeAccessorMethod ( CodeClassField * parentCF ) 
    : CodeMethodBlock ( (ClassifierCodeDocument*) parentCF->getParentDocument() ) 
{
	initFields(parentCF);
}

CodeAccessorMethod::~CodeAccessorMethod ( ) { }

//  
// Methods
//  


// Accessor methods
//  

/**
 * Get the value of m_parentclassfield
 * @return the value of m_parentclassfield
 */
CodeClassField * CodeAccessorMethod::getParentClassField ( ) {
	return m_parentclassfield;
}

bool CodeAccessorMethod::parentIsAttribute( ) {
	return getParentClassField()->parentIsAttribute();
}

/**
 * Utility method to get the value of the parent object of the parent classifield.
 * @return the value of the parent of the parent classfield
 */
UMLObject * CodeAccessorMethod::getParentObject ( ) {
        return getParentClassField()->getParentObject();
}

/** return the type of accessor method this is
 */
CodeAccessorMethod::AccessorType CodeAccessorMethod::getType( ) {
	return m_accessorType;
}

/** Set the type of accessor method this is
 */
void CodeAccessorMethod::setType ( CodeAccessorMethod::AccessorType atype) {
	m_accessorType = atype;
}

// Other methods
//  

/**
 * load params from the appropriate XMI element node.
 */
void CodeAccessorMethod::loadFromXMI ( QDomElement & root ) {
        setAttributesFromNode(root);
}

/**
 * Save the XMI representation of this object
 * @return      bool    status of save
 */
bool CodeAccessorMethod::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
        bool status = true;

        QDomElement docElement = doc.createElement( "codeaccessormethod" );

        setAttributesOnNode(doc, docElement);

        root.appendChild( docElement );

        return status;
}

/** set attributes of the node that represents this class
  * in the XMI document.
  */
void CodeAccessorMethod::setAttributesOnNode ( QDomDocument & doc, QDomElement & elem) 
{

        // set super-class attributes
        CodeMethodBlock::setAttributesOnNode(doc, elem);

        // set local class attributes
        elem.setAttribute("accessType",getType());
        elem.setAttribute("classfield_id",getParentClassField()->getID());

}

 /** set the class attributes of this object from
  * the passed element node.
  */
void CodeAccessorMethod::setAttributesFromNode ( QDomElement & root) {

        // set attributes from the XMI
        CodeMethodBlock::setAttributesFromNode(root); // superclass load

        QString id = root.attribute("classfield_id","-1");
        CodeClassField * newCF = 0;
	ClassifierCodeDocument * cdoc = dynamic_cast<ClassifierCodeDocument*>(getParentDocument());
	if(cdoc)
        	newCF = cdoc->findCodeClassFieldFromParentID (id.toInt());

        m_parentclassfield->disconnect(this); // always disconnect 
	if(newCF) 
		initFields(newCF);
	else
		kdError()<<"ERROR: code accessor method cant load parent codeclassfield, corrupt file?"<<endl;

        // now load/set other local attributes
        setType((AccessorType) root.attribute("accessType","0").toInt());
 
}

void CodeAccessorMethod::initFields(CodeClassField * parentClassField ) {

	m_parentclassfield = parentClassField;
	m_accessorType = GET;

        connect(m_parentclassfield,SIGNAL(modified()),this,SLOT(syncToParent()));
}

#include "codeaccessormethod.moc"
