
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
 *      Date   : Tue Aug 19 2003
 */

#include <kdebug.h>
#include <iostream.h>
#include "codegenobjectwithtextblocks.h"

#include "umldoc.h"
#include "codegenerator.h"
#include "codedocument.h"
#include "codeoperation.h"
#include "hierarchicalcodeblock.h"

// Constructors/Destructors
//  

CodeGenObjectWithTextBlocks::CodeGenObjectWithTextBlocks ( )
{
	initFields();
}

CodeGenObjectWithTextBlocks::~CodeGenObjectWithTextBlocks ( ) { 

      // delete all the text blocks we have
        for (TextBlock *tb = m_textblockVector.first(); tb; tb=m_textblockVector.next())
                delete tb;
	m_textblockVector.clear();

}

//  
// Methods
//  


// Accessor methods
//  

/**
 * Get the list of TextBlock objects held by m_textblockVector
 * @return QPtrList<TextBlock *> list of TextBlock objects held by
 * m_textblockVector
 */
QPtrList<TextBlock> * CodeGenObjectWithTextBlocks::getTextBlockList ( ) {
	return &m_textblockVector;
}

// Other methods
//  

/**
 * create a new CodeAccesorMethod object belonging to this object.
 * Strictly speaking, these can only belong in a classifier code document
 * OR a specialized code document, So the call here will return null.
 * @return      CodeAccessorMethod
 */
CodeAccessorMethod * CodeGenObjectWithTextBlocks::newCodeAccesorMethod( CodeClassField *cf, CodeAccessorMethod::AccessorType type) {
        kdWarning()<<"Warning: illegal attempt to create accessor method within object classfield:"<<cf<<" type:"<<type<<endl;
        return (CodeAccessorMethod*) NULL;
}

/**
 * create a new CodeOperation object belonging to this object.
 * Strictly speaking, these can only belong in a classifier code document,
 * OR a specialized code document, So the call here will return null.
 * @return      CodeOperation
 */
CodeOperation * CodeGenObjectWithTextBlocks::newCodeOperation( UMLOperation *op) {
        kdWarning()<<"Warning: illegal attempt to create codeoperation method from op:"<<op<<" within object:"<<this<<endl;
        return (CodeOperation*) NULL;
}


TextBlock * CodeGenObjectWithTextBlocks::findTextBlockByTag( QString tag )
{
        //if we already know to which file this class was written/should be written, just return it.
        if(m_textBlockTagMap->contains(tag))
                return ((*m_textBlockTagMap)[tag]);

        return (TextBlock*) NULL;
}

/**
 * @return	HierarchicalCodeBlock
 * @param	tag 
 * @param	comment 
 * @param	indentLevel 
 */
HierarchicalCodeBlock * CodeGenObjectWithTextBlocks::getHierarchicalCodeBlock (QString tag, QString comment, int indentLevel ) {

        // now actually declare the fields
        HierarchicalCodeBlock * codeBlock = (HierarchicalCodeBlock*) findTextBlockByTag(tag);
        if (!codeBlock) {
                codeBlock = newHierarchicalCodeBlock();
                codeBlock->setTag(tag);
                codeBlock->setComment(newCodeComment());
                if(!addTextBlock(codeBlock))
                        return (HierarchicalCodeBlock*) NULL;
        }
        codeBlock->setOverallIndentationLevel (indentLevel);
        codeBlock->getComment()->setText(comment);

        return codeBlock;
}


/**
 * @return	CodeBlockWithComments
 * @param	tag 
 * @param	comment 
 * @param	indentLevel 
 */
CodeBlockWithComments * CodeGenObjectWithTextBlocks::getCodeBlockWithComments (QString tag, QString comment, int indentLevel ) {

        // now actually declare the fields
        CodeBlockWithComments * codeBlock = (CodeBlockWithComments *) findTextBlockByTag(tag);
        if (!codeBlock) {
                codeBlock = newCodeBlockWithComments();
                codeBlock->setTag(tag);
                codeBlock->setComment(newCodeComment());
                if(!addTextBlock(codeBlock))
                        return (CodeBlockWithComments*) NULL;
        }
        codeBlock->setOverallIndentationLevel (indentLevel);
        codeBlock->getComment()->setText(comment);

        return codeBlock;

}


/**
 * @return	CodeComment
 * @param	tag 
 * @param	text 
 * @param	indentationLevel 
 */
CodeComment * CodeGenObjectWithTextBlocks::addOrUpdateTaggedCodeComment (QString tag, QString text, int indentationLevel) 
{

        TextBlock * tBlock = findTextBlockByTag(tag);
        CodeComment * codeComment = dynamic_cast<CodeComment*>(tBlock);

        if(!codeComment) {
                codeComment = newCodeComment();
                codeComment->setTag(tag);
                if(!addTextBlock(codeComment))
                {
                        delete codeComment;
                        return (CodeComment*) NULL; // hmm. total failure..,was there a preexisting comment with this tag?? lets return null
                }
        }

        if(!text.isEmpty())
                codeComment->setText(text);

        codeComment->setIndentationLevel(indentationLevel);

        return codeComment;
}


/**
 * @return	CodeBlockWithComments
 * @param	tag 
 * @param	text 
 * @param	comment 
 * @param	indentLevel 
 * @param	forceUserBlockUpdate 
 */
CodeBlockWithComments * CodeGenObjectWithTextBlocks::addOrUpdateTaggedCodeBlockWithComments (QString tag, QString text, QString ctext, int indentLevel, bool forceUserBlockUpdate ) 
{

        TextBlock * tBlock = findTextBlockByTag(tag);
        CodeBlockWithComments * codeBlock = dynamic_cast<CodeBlockWithComments*>(tBlock);

        if(!codeBlock) {
                codeBlock = newCodeBlockWithComments();
                codeBlock->setTag(tag);
                if(!addTextBlock(codeBlock))
                {
                        delete codeBlock;
                        return (CodeBlockWithComments*) NULL; // hmm. total failure..,was there a preexisting codeblock with this tag?? lets return null
                }
        }

        // ONLY update IF we are forcing the update of user blocks OR its an "AutoGenerated" Block
        if(forceUserBlockUpdate || codeBlock->getContentType() == CodeBlock::AutoGenerated)
        {

                if(!text.isEmpty())
                        codeBlock->setText(text);

                if(!ctext.isEmpty())
                        codeBlock->getComment()->setText(ctext);

                codeBlock->setOverallIndentationLevel(indentLevel);

        }

        return codeBlock;

}

void CodeGenObjectWithTextBlocks::resetTextBlocks() {
        m_textBlockTagMap->clear();
        m_textblockVector.clear();
}

void CodeGenObjectWithTextBlocks::setAttributesOnNode (QDomDocument & doc, QDomElement & root) {

	// set a section to hold document content
        QDomElement tblockElement = doc.createElement( "textblocks" );

        // only concrete calls to textblocks are saved
        QPtrList<TextBlock> * tbList = getTextBlockList();
        for (TextBlock * block = tbList->first(); block; block= tbList->next())
                block->saveToXMI(doc, tblockElement);

        root.appendChild( tblockElement);

}

/** set the class attributes of this object from
 * the passed element node.
 */
void CodeGenObjectWithTextBlocks::setAttributesFromNode ( QDomElement & root) 
{

        // clear existing codeblocks
        resetTextBlocks();

        // now load em back in
        loadChildTextBlocksFromNode(root);

}

// load text blocks
// in this vanilla version, we only load comments and codeblocks
// as they are the only instanciatable (vanilla) things
// this method should be overridden if this class is inherited
// by some other class that is concrete and takes children
// derived from codeblock/codecomment
void CodeGenObjectWithTextBlocks::loadChildTextBlocksFromNode ( QDomElement & root) 
{

        QDomNode tnode = root.firstChild();
        QDomElement telement = tnode.toElement();
        bool gotChildren = false;
        while( !telement.isNull() ) {
                QString nodeName = telement.tagName();

                if( nodeName == "textblocks" ) {

        		QDomNode node = telement.firstChild();
        		QDomElement element = node.toElement();

			while( !element.isNull() ) {
                		QString name = element.tagName();

cerr<<" *** (c) LOADING child textblock node : "<<name.latin1()<<endl;

/*
// Well, this is pretty ugly. What we really want is some kind of singleton that we
// can call on to supply us with the correct method to build this stuff with. That
// will take time, so in the interest of expediency, Im doing it this way for now. -b.t.
                if( m_textBlockCreateMap->contains(name))
		{
                	TextBlockCreator * creator = ((*m_textBlockCreateMap)[name]);
                	TextBlock * block = creator->create(this);
			block->loadFromXMI(element);
                        if(!addTextBlock(block))
                                kdWarning()<<"Unable to add "<<name<<"to obj:"<<this<<endl;
			else
                        	gotChildren= true;
		} else { 
			cerr<<" Got strange tag in text block stacK:"<<name.latin1()<<", ignoring"<<endl;
		}
*/
		                if( name == "codecomment" ) {
		                        CodeComment * block = newCodeComment();
		                        block->loadFromXMI(element);
		                        if(!addTextBlock(block))
		                        	kdWarning()<<"Unable to add codeComment to :"<<this<<endl;
		                        else
		                        	gotChildren= true;
		                } else 
		                if( name == "codeaccessormethod" || 
		                    name == "declarationcodeblock" 
                                  ) {
					QString acctag = element.attribute("tag",""); 
					// search for our method in the 
		                        TextBlock * tb = findCodeClassFieldTextBlockByTag(acctag);
		                        if(!tb || !addTextBlock(tb, true))
		                                kdWarning()<<"Unable to add codeaccessormethod to:"<<this<<endl;
		                        else
		                                gotChildren= true;
		               	} else
		                if( name == "codeblock" ) {
		                        CodeBlock * block = newCodeBlock();
		                        block->loadFromXMI(element);
		                        if(!addTextBlock(block))
		                                kdWarning()<<"Unable to add codeBlock to :"<<this<<endl;
		                        else
		                                gotChildren= true;
		                } else 
		                if( name == "codeblockwithcomments" ) {
		                        CodeBlockWithComments * block = newCodeBlockWithComments();
		                        block->loadFromXMI(element);
		                        if(!addTextBlock(block))
		                                kdWarning()<<"Unable to add codeBlockwithcomments to:"<<this<<endl;
		                        else
		                                gotChildren= true;
		                } else 
		                if( name == "header" ) {
					// do nothing.. this is treated elsewhere
		                } else 
		                if( name == "hierarchicalcodeblock" ) {
		                        HierarchicalCodeBlock * block = newHierarchicalCodeBlock();
		                        block->loadFromXMI(element);
		                        if(!addTextBlock(block))
		                                kdWarning()<<"Unable to add hierarchicalcodeBlock to:"<<this<<endl;
					else
		                                gotChildren= true;
		                } else 
		               	if( name == "codeoperation" ) {
					// find the code operation by id
		                        QString id = element.attribute("parent_id","-1");
					UMLObject * obj = getParentGenerator()->getDocument()->findUMLObject(id.toInt());
					UMLOperation * op = dynamic_cast<UMLOperation*>(obj);
cerr<<" *** parent operation id:"<<id.latin1()<<" obj is:"<<op<<endl;
					if(op) {
		                        	CodeOperation * block = newCodeOperation(op);
cerr<<" *** got parent operation..loading from XMI now"<<endl;
		                        	block->loadFromXMI(element);
		                        	if(addTextBlock(block))
		                                	gotChildren= true;
					} else 
		                              kdWarning()<<"Unable to add codeoperation to:"<<this<<endl;
		                } else
					cerr<<" ERROR: Got strange tag in text block stack:"<<name.latin1()<<endl;
			
                		node = element.nextSibling();
                		element = node.toElement();
			}
			break;
		}

                tnode = telement.nextSibling();
                telement = tnode.toElement();
        }

        if(!gotChildren)
                kdWarning()<<" loadFromXMI : Warning: unable to initialize any child blocks in:"<<this<<endl;

}

void CodeGenObjectWithTextBlocks::initFields ( ) {

	m_textBlockTagMap = new QMap<QString, TextBlock *>;
	m_textblockVector.setAutoDelete(false);

}

