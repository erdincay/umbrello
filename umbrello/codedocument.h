
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
 *      Date   : Wed Jun 18 2003
 */

#ifndef CODEDOCUMENT_H
#define CODEDOCUMENT_H

#include <qobject.h>
#include <qmap.h>
#include <qstring.h>
#include <qptrlist.h>

#include "codegenerationpolicy.h"
#include "codegenobjectwithtextblocks.h"
#include "hierarchicalcodeblock.h"

class CodeAccessorMethod;
class CodeBlockWithComments;
class CodeComment;
class CodeGenerator;
class CodeOperation;
class TextBlock;

//#include "codedocumentdialog.h"

/**
  * class CodeDocument
  * A document containing the code for one file. 
  */

// "friend" status is needed for HBlock so it may call addChildTagToMap which
// is protected.
class CodeDocument : public QObject, public CodeGenObjectWithTextBlocks 
{
	friend class HierarchicalCodeBlock;
	Q_OBJECT
public:

	// Constructors/Destructors
	//  


	/**
	 * Empty Constructor
	 */
        /**
         * Basic constructor for class.
         * @param       gen 
         */
        CodeDocument ( CodeGenerator * gen );
        CodeDocument ( CodeGenerator * gen , QDomElement & element );

	/**
	 * Empty Destructor
	 */
	virtual ~CodeDocument ( );

	// Public attributes
	//  

	// Public attribute accessor methods
	//  

        /**
         * Set the value of m_fileExtension
         * @param new_var the new value of m_fileExtension
         */
        void setFileExtension ( QString new_var );

        /**
         * Get the value of m_fileExtension
         * @return the value of m_fileExtension
         */
        QString getFileExtension ( ) const;

	/**
	 * Set the complete value (name plus any extension) of m_filename
	 * @param new_var the new value of m_filename
	 */
	void setFileName ( QString new_var );

	/**
	 * Get the value of m_filename. This name is the "complete" filename,
	 * meaning that it contains both the file name plus any extension (e.g. "file.cpp")
	 * @return the value of m_filename
	 */
	QString getFileName ( ) const;

        /**
         * Set the value of m_packageName
         * @param new_var the new value of m_packageName
         */
        void setPackage ( QString new_var );

        /**
         * Get the value of the package of this codedocument.
         * @return the value of m_pathName
         */
        QString getPackage ( ) const;

        /**
         * Get the value of the path to this codedocument.
         * @return the value of m_pathName
         */
        virtual QString getPath ( );

        /**
         * Set the value of m_ID
         * @param new_var the new value of m_ID
         */
        void setID ( QString new_id);

        /**
         * Get the value of m_ID
         * @return the value of m_ID
         */
        QString getID ( ) const;

	/**
	 * Set the value of m_writeOutCode
	 * Whether or not to write out this code document and any codeblocks, etc that it
	 * owns.
	 * @param new_var the new value of m_writeOutCode
	 */
	void setWriteOutCode ( bool new_var );

	/**
	 * Get the value of m_writeOutCode
	 * Whether or not to write out this code document and any codeblocks, etc that it
	 * owns.
	 * @return the value of m_writeOutCode
	 */
	bool getWriteOutCode ( );

	/**
	 * Set the CodeGeneratorPolicy object
	 */
	void setPolicy ( CodeGenerationPolicy * add_object );

	/**
	 * Get the CodeGeneratorPolicy object 
	 */
	CodeGenerationPolicy * getPolicy ( );

	/**
	 * Set a Header comment object 
	 */
	void setHeader ( CodeComment * comment );

	/**
	 * Get the Header comment object
	 */
	CodeComment * getHeader ( );

	/**
	 * Add a TextBlock object to the m_textblockVector List
         * @return boolean value where false means not added because an TextBlock
         *                 object with that tag already exists in this document. 
	 */
	bool addTextBlock ( TextBlock * add_object , bool replaceExisting = false);

	/**
	 * Remove a TextBlock object from m_textblockVector List
	 */
	bool removeTextBlock ( TextBlock * remove_object );

	/**
	 * Lookup a certain textblock by its tag value, returns NULL if it cant
         * find the TextBlock with such a tag. If descendIntoChildren is true, then
 	 * any child hierarchical textblocks will also be searched for a match.
   	 */
	TextBlock * findTextBlockByTag( QString tag , bool descendIntoChildren = false);

	/**
	 * create the string representation of this object.
	 * @return	QString
	 */
	virtual QString toString ( );

	/**
	 * Save the XMI representation of this object
	 * @return	bool 	status of save
	 */
	virtual bool saveToXMI ( QDomDocument & doc, QDomElement & root );

	/** 
 	 * load params from the appropriate XMI element node.
	 */
	virtual void loadFromXMI ( QDomElement & root );

       /**
         * create a new CodeBlock object belonging to this CodeDocument.
         * @return      CodeBlock
         */
        virtual CodeBlock * newCodeBlock ( );

	/**
	 * create a new HierarchicalCodeBlock object belonging to this CodeDocument.
	 * @return 	HierarchicalCodeBlock
	 */
	virtual HierarchicalCodeBlock * newHierarchicalCodeBlock ( );

	/**
	 * create a new CodeBlockWithComments object belonging to this CodeDocument.
	 * @return	CodeBlockWithComments
	 */
	virtual CodeBlockWithComments * newCodeBlockWithComments ( );

 	// create a code comment for this document
	virtual CodeComment * newCodeComment();

	/**
	 * Wrapper around call to either getCodeGenerationPolicy().getOverwritePolicy() OR
	 * (if no individual codegeneration policy for this Code Document) then to 
	 * getParentPackage().getDefaultCodeGenerationPolicy().getOverwritePolicy() 
	 * @return	QString
	 */
	CodeGenerationPolicy::OverwritePolicy getOverwritePolicy ( );

	/**
	 * Wrapper around call to either
	 * getCodeGenerationPolicy().getCodeVerboseSectionComments() OR (if no individual
	 * codegeneration policy for this Code Document) then to 
	 * getParentPackage().getDefaultCodeGenerationPolicy().getCodeVerboseSectionComments() 
	 * @return	bool
	 */
	bool getCodeVerboseSectionComments ( );

	/**
	 * Wrapper around call to either
	 * getCodeGenerationPolicy().getCodeVerboseDocumentComments() OR (if no individual
	 * codegeneration policy for this Code Document) then to 
	 * getParentPackage().getDefaultCodeGenerationPolicy().getCodeVerboseDocumentComments()
	 * @return	bool
	 */
	bool getVerboseDocumentComments ( );

	// Utility accessor. Wrapper around getParentGenerator->getNewLineEndingChars()
        QString getNewLineEndingChars ( );

	// return a unique, and currently unallocated, text block tag for this document 
	virtual QString getUniqueTag(QString prefix = QString(""));

       /**
         * Get the value of m_parentgenerator
         * @return the value of m_parentgenerator
         */
        CodeGenerator * getParentGenerator ( );

	/** a little utility method to make life easier for the code document programmer
	 */
	QString cleanName (QString name);

	// Cause this code document to syncronize to current generator policy
	virtual void syncronize();

protected:

       /** set attributes of the node that represents this class
         * in the XMI document.
         */
        virtual void setAttributesOnNode ( QDomDocument & doc, QDomElement & blockElement);

        /** set the class attributes of this object from
         * the passed element node.
         */
        virtual void setAttributesFromNode ( QDomElement & element);

	// these next 2 are needed by child hierarchical code blocks so
	// that when they call getUniqueTag, we really get a unique tag
	// Also, it allows 'findTextBlockByTag' To find any tagged text block
	// anywhere in the document, whether directly owned by the document OR
	// by some child hierarchical textblock
	void addChildTagToMap ( QString tag, TextBlock * tb);
	void removeChildTagFromMap ( QString tag );

	// update the header text of this codedocument
	void updateHeader ();

	// reset/clear our inventory of textblocks in this document
	void resetTextBlocks();

        // update the content of this code document
        // this is where you should lay out your code document structure of textblocks
        // in the inheriting class, should it have any text in it.
        virtual void updateContent();

	// this is needed by the parent codegenobjectwithtextblocks class
	// in order to search for text blocks by tag when loading from XMI
	CodeDocument * getCodeDocument() { return this; }

	// have to implement this for CodeObjectWithTextBlocks
	// doenst actually do anythying fo ra vannilla code document
	virtual TextBlock * findCodeClassFieldTextBlockByTag(QString tag);

private:

	int lastTagIndex;
	QString m_filename;
	QString m_fileExtension;
	QString m_ID;
	QString m_pathName;
	QString m_packageName;

        bool m_writeOutCode; // Whether or not to write out this code document 
		 	     // and any codeblocks, etc that it owns.

        CodeGenerator * m_parentgenerator;
        CodeGenerationPolicy * m_codegeneratorpolicy;
        CodeComment * m_header;


	void initDoc ( CodeGenerator * parent ) ;

        // QPtrList<TextBlock> m_textblockVector;
	// QMap<QString, TextBlock *> * m_textBlockTagMap;

	// for recording all of the textblocks held by child hierarchical codeblocks
	QMap<QString, TextBlock *> * m_childTextBlockTagMap;
 
};

#endif // CODEDOCUMENT_H
