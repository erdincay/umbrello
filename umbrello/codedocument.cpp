
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

#include <kdebug.h>
#include <qregexp.h>

#include "codedocument.h"
#include "codegenerator.h"

// Constructors/Destructors
//  

/**
 * Basic constructor for class.
 * @param       gen 
 */
CodeDocument::CodeDocument (CodeGenerator * gen , QDomElement & elem) 
    : QObject (gen, "ACodeDocument") 
{

kdWarning()<< " DOC LOADING FROM XMI" <<endl;
	initDoc( gen );
	loadFromXMI(elem);
}

CodeDocument::CodeDocument (CodeGenerator * gen ) 
    : QObject (gen, "ACodeDocument") 
{
	initDoc( gen );
}


CodeDocument::~CodeDocument ( ) { 

	// delete all the text blocks we have
//        for (TextBlock *tb = m_textblockVector.first(); tb; tb=m_textblockVector.next())
//		delete tb;

	kdWarning()<<"DESTROYED CODE DOCUMENT name:"<<getFileName().latin1()<<" id:"<<this<<endl;
}

//  
// Methods
//  


// Accessor methods
//  


// Public attribute accessor methods
//  


/**
 * Set the value of m_filename
 * @param new_var the new value of m_filename
 */
void CodeDocument::setFileName ( QString new_var ) {
	m_filename = new_var;
}

/**
 * Get the value of m_filename
 * @return the value of m_filename
 */
QString CodeDocument::getFileName ( ) const {
	return m_filename;
}

/**
 * Set the value of m_filename
 * @param new_var the new value of m_filename
 */
void CodeDocument::setFileExtension ( QString new_var ) {
        m_fileExtension = new_var;
	updateHeader(); // because we are using new heading file
}

/**
 * Get the value of m_filename
 * @return the value of m_filename
 */
QString CodeDocument::getFileExtension( ) const {
        return m_fileExtension;
}

/**
 * Set the value of the package. 
 * @param new_var the new value of m_packageName
 */
void CodeDocument::setPackage ( QString new_var ) { 
        m_packageName = new_var;
}

/**
 * Get the value of path for this code document.
 * @return the value of the path
 */
QString CodeDocument::getPath ( ) {

        QString path = getPackage();

    	// Replace all white spaces with blanks
        path.simplifyWhiteSpace();

        // Replace all blanks with underscore
        path.replace(QRegExp(" "), "_");

        // this allows multiple directory paths (ala Java, some other languages) 
        // in from the package specification
        path.replace(QRegExp("\\."),"/"); // Simple hack!.. but this is more or less language 
                                          // dependant and should probably be commented out. 
                                          // Still, as a general default it may be usefull -b.t. 
	return path;
}

/**
 * Get the value of package name.
 * @return the value of m_packageName
 */
QString CodeDocument::getPackage ( ) const {
        return m_packageName;
}

/**
 * Set the value of m_ID
 * @param new_var the new value of m_ID
 */
void CodeDocument::setID ( QString new_var ) {
        m_ID = new_var;
}

/**
 * Get the value of m_ID
 * @return the value of m_ID
 */
QString CodeDocument::getID ( ) const {
        return m_ID;
}

/**
 * Set the value of m_writeOutCode
 * Whether or not to write out this code document and any codeblocks, etc that it
 * owns.
 * @param new_var the new value of m_writeOutCode
 */
void CodeDocument::setWriteOutCode ( bool new_var ) {
	m_writeOutCode = new_var;
}

/**
 * Get the value of m_writeOutCode
 * Whether or not to write out this code document and any codeblocks, etc that it
 * owns.
 * @return the value of m_writeOutCode
 */
bool CodeDocument::getWriteOutCode ( ) {
	return m_writeOutCode;
}

/**
 * Get the value of m_parentgenerator
 * @return the value of m_parentgenerator
 */
CodeGenerator * CodeDocument::getParentGenerator ( ) {
	return m_parentgenerator;
}

/**
 * Set the Policy object 
 */
void CodeDocument::setPolicy ( CodeGenerationPolicy * add_object ) {
	m_codegeneratorpolicy = add_object;
}

/**
 * Get the Policy object 
 */
CodeGenerationPolicy * CodeDocument::getPolicy ( ) {
	if(m_codegeneratorpolicy)
		return m_codegeneratorpolicy;
	else
		return m_parentgenerator->getPolicy();
}

/**
 * Set the Header comment
 */
void CodeDocument::setHeader ( CodeComment * header ) {
	m_header = header;
}

/**
 * Get the Header comment
 */
CodeComment * CodeDocument::getHeader ( ) {
	return m_header;
}

QString CodeDocument::getUniqueTag ( QString prefix ) 
{

	if(prefix.isEmpty())
		prefix = "tblock";

        QString tag = prefix + "_0";
        int number = lastTagIndex;
        for ( ; findTextBlockByTag(tag, true); number++) {
                tag = prefix + "_" + QString::number(number);
        }
	lastTagIndex = number;
        return tag;
}

/**
 * Add a TextBlock object to the m_textblockVector List
 */
bool CodeDocument::addTextBlock ( TextBlock * add_object , bool replaceExistingBlock ) {

	QString tag = add_object->getTag();

	// assign a tag if one doesnt already exist
	if(tag.isEmpty())
	{
		tag = getUniqueTag();
		add_object->setTag(tag);
	}

        if(m_textBlockTagMap->contains(tag))
		return false; // return false, we already have some object with this tag in the list
	else
		m_textBlockTagMap->insert(tag, add_object);

	m_textblockVector.append(add_object);
	return true;
}

/**
 * Remove a TextBlock object from m_textblockVector List
 */
bool CodeDocument::removeTextBlock ( TextBlock * remove_object ) {

	// check if we can remove it from our local list
	if(!m_textblockVector.removeRef(remove_object)) 
 	{
		// may be hiding in child hierarchical codeblock
		for(TextBlock * tb = m_textblockVector.first(); tb ; tb = m_textblockVector.next())
		{
			HierarchicalCodeBlock * hb = dynamic_cast<HierarchicalCodeBlock*>(tb);
			if(hb && hb->removeTextBlock(remove_object))
				return true;
		}
	}

	// if we get here.. it was in this object so remove from our map
	QString tag = remove_object->getTag();
        if(!tag.isEmpty())
              m_textBlockTagMap->erase(tag);

	return true;
}

/**
 * Get the value of m_dialog
 * @return the value of m_dialog
 */
/*
CodeDocumentDialog * CodeDocument::getDialog ( ) {
	return m_dialog;
}
*/

// Other methods
//  

QString CodeDocument::cleanName (QString name) {
        CodeGenerator *g = getParentGenerator();
        return g->cleanName(name);
}

// update the text and status of the head comment 
void CodeDocument::updateHeader () {

        //try to find a heading file (license, coments, etc) then extract its text
        QString headingText = getParentGenerator()->getHeadingFile(getFileExtension()); 

        headingText.replace(QRegExp("%filename%"),getFileName()+getFileExtension());
        headingText.replace(QRegExp("%filepath%"),getPath());
        headingText.replace( QRegExp("%time%"), QTime::currentTime().toString());
        headingText.replace( QRegExp("%date%"), QDate::currentDate().toString());

        getHeader()->setText(headingText);

        // update the write out status of the header
        if(getPolicy()->getIncludeHeadings())
                getHeader()->setWriteOutText(true);
        else
                getHeader()->setWriteOutText(false);

}

/**
 * create the string representation of this object.
 * @return	QString
 */
QString CodeDocument::toString ( ) {

	// IF the whole document is turned "Off" then dont bother
	// checking individual code blocks, just send back empty string
	if(!getWriteOutCode())
		return QString("");

        QString content = getHeader()->toString();

	// update the time/date

        // comments, import, package codeblocks go next
        QPtrList<TextBlock> * items = getTextBlockList();
        for (TextBlock *c = items->first(); c; c = items->next())
        {
		if(c->getWriteOutText()) {
			QString str = c->toString();
			if(!str.isEmpty())
                		content.append(str);
		}
        }
        return content;
}

void CodeDocument::syncronize() {
kdWarning()<<" Syncronize code doc:"<<this<<endl;
	updateContent();
}

// need to overload method to beable to clear the childTextBlockMap
void CodeDocument::resetTextBlocks() {
	m_childTextBlockTagMap->clear();
	CodeGenObjectWithTextBlocks::resetTextBlocks();
}

/**
 * load params from the appropriate XMI element node.
 */
void CodeDocument::loadFromXMI ( QDomElement & root ) {
        setAttributesFromNode(root);
}

/** set attributes of the node that represents this class
 * in the XMI document.
 */
void CodeDocument::setAttributesOnNode ( QDomDocument & doc, QDomElement & docElement)
{

	// superclass call
	CodeGenObjectWithTextBlocks::setAttributesOnNode(doc,docElement);

	// now set local attributes/fields
        docElement.setAttribute("fileName",getFileName());
        docElement.setAttribute("fileExt",getFileExtension());
        docElement.setAttribute("package",getPackage());
        docElement.setAttribute("writeOutCode",getWriteOutCode()?"true":"false");
        docElement.setAttribute("id",getID());

        // set the a header
        // which we will store in its own separate child node block
        QDomElement commElement = doc.createElement( "header" );
        getHeader()->saveToXMI(doc, commElement); // comment
        docElement.appendChild( commElement);

	// doc codePolicy? 
	// FIX: store ONLY if different from the parent generator
	// policy.. something which is not possible right now. -b.t.

}

/** set the class attributes of this object from
 * the passed element node.
 */
void CodeDocument::setAttributesFromNode ( QDomElement & root) {

	// now set local attributes
        setFileName(root.attribute("fileName",""));
        setFileExtension(root.attribute("fileExt",""));
        setPackage(root.attribute("package",""));
        setWriteOutCode(root.attribute("writeOutCode","true") == "true" ? true : false);
        setID(root.attribute("id",""));

        // load comment now
        // by looking for our particular child element
        QDomNode node = root.firstChild();
        QDomElement element = node.toElement();
        while( !element.isNull() ) {
                QString tag = element.tagName();
                if( tag == "header" ) {
                        QDomNode cnode = element.firstChild();
                        QDomElement celem = cnode.toElement();
                        getHeader()->loadFromXMI(celem);
                        break;
                }
                node = element.nextSibling();
                element = node.toElement();
        }

	// a rare case where the super-class load is AFTER local attributes
	CodeGenObjectWithTextBlocks::setAttributesFromNode(root);
}

/**
 * Save the XMI representation of this object
 * @return      bool    status of save
 */
bool CodeDocument::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
	bool status = true;

 	QDomElement docElement = doc.createElement( "codedocument" );

	setAttributesOnNode(doc, docElement);

        root.appendChild( docElement );

	return status;
}

// vanilla code documents dont have much
// to do.. override this with a different
// version for your own documents
void CodeDocument::updateContent() {
        updateHeader(); // doing this insures time/date stamp is at the time of this call
}

/**
 * create a new CodeBlock object belonging to this CodeDocument.
 * @return      CodeBlock
 */
CodeBlock * CodeDocument::newCodeBlock ( ) {
        return new CodeBlock(this);
}

/**
 * create a new CodeBlockWithComments object belonging to this CodeDocument.
 * @return	CodeBlockWithComments
 */
CodeBlockWithComments * CodeDocument::newCodeBlockWithComments ( ) {
	return new CodeBlockWithComments(this);
}

/**
 * create a new CodeBlockWithComments object belonging to this CodeDocument.
 * @return      CodeBlockWithComments
 */
CodeComment * CodeDocument::newCodeComment ( ) {
        return new CodeComment(this);
}

HierarchicalCodeBlock * CodeDocument::newHierarchicalCodeBlock ( ) {
	return new HierarchicalCodeBlock(this);
}

/**
 * Wrapper around call to either getPolicy().getOverwritePolicy() OR
 * (if no individual codegeneration policy for this Code Document) then to 
 * getParentPackage().getDefaultCodeGenerationPolicy().getOverwritePolicy() 
 * @return	QString
 */
CodeGenerationPolicy::OverwritePolicy CodeDocument::getOverwritePolicy ( ) {
	return getPolicy()->getOverwritePolicy();
}


/**
 * Wrapper around call to either
 * getPolicy().getCodeVerboseSectionComments() OR (if no individual
 * codegeneration policy for this Code Document) then to 
 * getParentPackage().getDefaultCodeGenerationPolicy().getCodeVerboseSectionComments() 
 * @return	bool
 */
bool CodeDocument::getCodeVerboseSectionComments ( ) {
	return getPolicy()->getCodeVerboseSectionComments();
}


/**
 * Wrapper around call to either
 * getPolicy().getCodeVerboseDocumentComments() OR (if no individual
 * codegeneration policy for this Code Document) then to 
 * getParentPackage().getDefaultCodeGenerationPolicy().getCodeVerboseDocumentComments()
 * @return	bool
 */
bool CodeDocument::getVerboseDocumentComments ( ) {
	return getPolicy()->getCodeVerboseDocumentComments();
}

QString CodeDocument::getNewLineEndingChars ( ) {
        return getPolicy()->getNewLineEndingChars( );
}

void CodeDocument::removeChildTagFromMap ( QString tag ) 
{
	m_childTextBlockTagMap->erase(tag);
}

void CodeDocument::addChildTagToMap ( QString tag, TextBlock * tb) 
{
	m_childTextBlockTagMap->insert(tag, tb);
}

TextBlock * CodeDocument::findTextBlockByTag( QString tag , bool descendIntoChildren) 
{
        //if we already know to which file this class was written/should be written, just return it.
        if(m_textBlockTagMap->contains(tag))
                return ((*m_textBlockTagMap)[tag]);

	if (descendIntoChildren)
        	if(m_childTextBlockTagMap->contains(tag))
                	return ((*m_childTextBlockTagMap)[tag]);

	return (TextBlock*) NULL;
}

void CodeDocument::initDoc ( CodeGenerator * gen ) {

kdWarning()<<"INIT CODE DOCUMENT"<<endl;

	m_parentgenerator = gen;
	m_writeOutCode = true;
        m_packageName = QString(""); // no package name is the default 
        m_fileExtension = QString("");
        m_ID = QString(""); // leave with NO ID as a default 

	//m_textblockVector.setAutoDelete(false);

//	m_textBlockTagMap = new QMap<QString, TextBlock *>;
	m_childTextBlockTagMap = new QMap<QString, TextBlock *>;
	m_codegeneratorpolicy = gen->getPolicy(); // default to parent code gen policy

	setHeader(new CodeComment(this));

	lastTagIndex = 0;

//	m_dialog = new CodeDocumentDialog( );

kdWarning()<<"INIT CODE DOCUMENT - FINISHED "<<endl;
}

TextBlock * CodeDocument::findCodeClassFieldTextBlockByTag (QString tag) {
	kdWarning()<<"Called findCodeClassFieldMethodByTag("<<tag<<") for a regular CodeDocument"<<endl;
	return (TextBlock *) NULL;
}

#include "codedocument.moc"
