/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003      Brian Thomas <thomas@mail630.gsfc.nasa.gov>   *
 *   copyright (C) 2004-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#ifndef JAVAANTCODEDOCUMENT_H
#define JAVAANTCODEDOCUMENT_H

#include "codedocument.h"

#include <QDomDocument>
#include <QDomElement>
#include <QString>

/**
 * class JavaANTCodeDocument
 * Represents
 */
class JavaANTCodeDocument : public CodeDocument
{
    Q_OBJECT
public:

    /**
     * Constructor
     */
    JavaANTCodeDocument ();

    /**
     * Empty Destructor
     */
    virtual ~JavaANTCodeDocument ();

    QString getPath ();

    void updateContent();

    /**
     * Save the XMI representation of this object.
     */
    virtual void saveToXMI1 (QDomDocument & doc, QDomElement & root);

    /**
     * Load params from the appropriate XMI element node.
     */
    virtual void loadFromXMI1 (QDomElement & root);

    /**
     * Create a new HierarchicalCodeBlock object belonging to this CodeDocument.
     * @return      HierarchicalCodeBlock
     */
    virtual HierarchicalCodeBlock * newHierarchicalCodeBlock ();

    //**
    // * Create a new CodeBlockWithComments object belonging to this CodeDocument.
    // * @return      CodeBlockWithComments
    // */
    //  virtual CodeBlockWithComments * newCodeBlockWithComments ();

protected:

    /**
     * Set attributes of the node that represents this class
     * in the XMI document.
     */
    virtual void setAttributesOnNode (QDomDocument & doc, QDomElement & blockElement);

    /**
     * Set the class attributes of this object from
     * the passed element node.
     */
    virtual void setAttributesFromNode (QDomElement & element);

    /**
     * Need to overwrite this for java since we need to pick up the
     * xml declaration blocks.
     */
    virtual void loadChildTextBlocksFromNode (QDomElement & root);

private:

};

#endif // JAVAANTCODEDOCUMENT_H
