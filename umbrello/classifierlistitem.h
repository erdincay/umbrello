/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CLASSIFIERLISTITEM_H
#define CLASSIFIERLISTITEM_H

#include "umlobject.h"

// forward declaration
class UMLClassifier;

/**
 * Classifiers (classes, interfaces) have lists of operations,
 * attributes, templates and others.  This is a base class for
 * the items in this list.  This abstraction should remove
 * duplication of dialogues and allow for stereotypes in lists.
 *
 * @short A base class for classifier list items (e.g. attributes)
 * @author Jonathan Riddell
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */

class UMLClassifierListItem : public UMLObject {
	Q_OBJECT
public:
	/**
	 * Constructor.  Empty.
	 *
	 * @param parent	The parent to this operation.
	 * @param name		The name of the operation.
	 * @param id		The id of the operation.
	 */
	UMLClassifierListItem(const UMLObject *parent, QString Name, int id);

	/**
	 * Constructor.  Empty.
	 *
	 * @param parent	The parent to this operation.
	 */
	UMLClassifierListItem(const UMLObject *parent);

	/**
	 * Destructor.  Empty.
	 */
	virtual ~UMLClassifierListItem();

	/**
	 * Returns the type of the UMLClassifierListItem.
	 *
	 * @return	The type of the UMLClassifierListItem.
	 */
	UMLClassifier * getType();

	/**
	 * Returns the type name of the UMLClassifierListItem.
	 *
	 * @return	The type name of the UMLClassifierListItem.
	 */
	QString getTypeName();

	/**
	 * Sets the type name of the UMLClassifierListItem.
	 * DEPRECATED - use setType() instead.
	 *
	 * @param type	The type name of the UMLClassifierListItem.
	 */
	void setTypeName(QString type);

	/**
	 * Sets the type of the UMLAttribute.
	 *
	 * @param type	Pointer to the UMLClassifier of the type.
	 */
	void setType(UMLClassifier *type);

	/**
	 * Returns a string representation of the operation.
	 *
	 * @param sig		What type of operation string to show.
	 * @return	The string representation of the operation.
	 */
	virtual QString toString(Signature_Type sig = st_NoSig) = 0;

	/**
	 * Display the properties configuration dialogue for the list item.
	 *
	 * @param parent	The parent widget.
	 * @return	True for success of this operation.
	 */
	virtual bool showPropertiesDialogue(QWidget* parent) = 0;

	/**
	 * Return a short name, usually just calls getName().
	 *
	 * @return	Short name of this UMLClassifierListItem.
	 */
	virtual QString getShortName();

	/**
	 * Copy the internal presentation of this object into the new
	 * object.
	 */
	virtual void copyInto(UMLClassifierListItem *rhs) const;

	/**
	 * The abstract method UMLObject::clone() must be implemented
	 * by the classes inheriting from UMLClassifierListItem.
	 */
	virtual UMLObject* clone() const = 0;

protected:
	UMLClassifier *m_pType;
	/**
	 * Text for the type - DEPRECATED.
	 * Only used if m_pType is NULL.
	 * FIXME: Change all usages of setTypeName() to setType().
	 */
	QString m_TypeName;
};

#endif
