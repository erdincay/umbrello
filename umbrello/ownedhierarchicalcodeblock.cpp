
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
 *      Date   : Fri Aug 07 2003
 */

#include <iostream.h>
#include <kdebug.h>
#include "ownedhierarchicalcodeblock.h"
#include "association.h"
#include "umldoc.h"
#include "umlobject.h"
#include "umlrole.h"
#include "codedocument.h"
#include "codegenerator.h"

// Constructors/Destructors
//  

OwnedHierarchicalCodeBlock::OwnedHierarchicalCodeBlock ( UMLObject *parent, CodeDocument * doc, QString start, QString end, QString comment) 
   : HierarchicalCodeBlock ( doc, start, end, comment), OwnedCodeBlock(parent)
{
	initFields ( parent);
} 

OwnedHierarchicalCodeBlock::~OwnedHierarchicalCodeBlock ( ) { }

//  
// Methods
//  


// Accessor methods
//  

/**
 * Get the value of m_parentObject
 * @return the value of m_parentObject
 */
UMLObject * OwnedHierarchicalCodeBlock::getParentObject () {
	return m_parentObject;
}

// Other methods
//  

void OwnedHierarchicalCodeBlock::setAttributesOnNode (QDomDocument & doc, QDomElement & elem ) {

        // set super-class attributes
        HierarchicalCodeBlock::setAttributesOnNode(doc, elem);

        // set local class attributes
        elem.setAttribute("parent_id",QString::number(m_parentObject->getID()));

        // setting ID's takes special treatment
        // as UMLRoles arent properly stored in the XMI right now.
        // (change would break the XMI format..save for big version change )
        UMLRole * role = dynamic_cast<UMLRole*>(m_parentObject);
        if(role)
{
                elem.setAttribute("role_id",role->getID());
cerr<<" OWNED H BLOCK saveToXMI sets role_id:"<<role->getID()<<endl;
}       else

                elem.setAttribute("role_id","-1");


}

/** set the class attributes of this object from
 * the passed element node.
 */
void OwnedHierarchicalCodeBlock::setAttributesFromNode ( QDomElement & root)
{

	// disconnect(parent,SIGNAL(modified()),this,SLOT(syncToParent()));

        // set attributes from the XMI
        HierarchicalCodeBlock::setAttributesFromNode(root); // superclass load

       // set local attributes, parent object first
        int id = root.attribute("parent_id","-1").toInt();

        // always disconnect
        m_parentObject->disconnect(this);

        // now, what is the new object we want to set?
        UMLObject * obj = getParentDocument()->getParentGenerator()->getDocument()->findUMLObject(id);
        if(obj)
        {

                // FIX..one day.
                // Ugh. This is UGLY, but we have to do it this way because UMLRoles
                // dont go into the document list of UMLobjects, and have the same
                // ID as their parent UMLAssociations. So..the drill is then special
                // for Associations..in that case we need to find out which role will
                // serve as the parametger here. The REAL fix, of course, would be to
                // treat UMLRoles on a more even footing, but im not sure how that change
                // might ripple throughout the code and cause problems. Thus, since the
                // change appears to be needed for only this part, I'll do this crappy
                // change instead. -b.t.
                UMLAssociation * assoc = dynamic_cast<UMLAssociation*>(obj);
                if(assoc) {
                        // In this case we init with indicated role child obj.
                        UMLRole * role = 0;
                        int role_id = root.attribute("role_id","-1").toInt();
                        if(assoc->getUMLRoleA()->getID() == role_id)
                                role = assoc->getUMLRoleA();
                        else if(assoc->getUMLRoleB()->getID() == role_id)
{
                                role = assoc->getUMLRoleB();
cerr<<"SET ROLE B w/ hint of id:"<<id<<endl;
}
                        else // this will cause a crash
                                cerr<<"ERROR! corrupt save file? cant get proper UMLRole for codeparameter:"<<id<<" w/role_id:"<<role_id<<endl;

                        // init using UMLRole obj
                        initFields ( role);

                } else
                        initFields ( obj); // just the regular approach

        } else
                kdError()<<"CANT LOAD HIERACHICALCODEBLOCK: parentUMLObject w/id:"<<id<<" not found, corrupt save file?"<<endl;


}

CodeDocument * OwnedHierarchicalCodeBlock::getParentDocument() {
	return TextBlock::getParentDocument();
}

/**
 */
void OwnedHierarchicalCodeBlock::syncToParent ( ) {

        if(getContentType() != CodeBlock::AutoGenerated)
                return;

        updateContent();
}

void OwnedHierarchicalCodeBlock::initFields ( UMLObject * parent) { 
	m_parentObject = parent; 
	connect(parent,SIGNAL(modified()),this,SLOT(syncToParent()));
}


#include "ownedhierarchicalcodeblock.moc"
