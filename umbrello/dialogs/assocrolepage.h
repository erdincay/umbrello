/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ASSOCROLEPAGE_H
#define ASSOCROLEPAGE_H

//quicktime class includes
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qmultilineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

//my class includes
#include "../umlobject.h"
#include "../objectwidget.h"
#include "../umldoc.h"
#include "../associationwidget.h"

/**
 *	Displays properties of a UMLObject in a dialog box.  This is not usually directly
 *	called.  The class @ref AssocPropDlg	will set this up for you.
 *
 *	@short	Display properties on a UMLObject.
 *	@author Paul Hensgen	<phensgen@techie.com>
 *	@version 1.0
 */
class AssocRolePage : public QWidget {
	Q_OBJECT
public:

	/**
	 *	Sets up the AssocRolePage.
	 *
	 *	@param	d	The UMLDoc which controls controls object creation.
	 *	@param	parent	The parent to the AssocRolePage.
	 *	@param	o	The ObjectWidget to display the properties of.
	 */
	AssocRolePage(UMLDoc *d, QWidget *parent, AssociationWidget *a);

	/**
	 *	Standard deconstructor.
	 */
	~AssocRolePage();

	/**
	 *	Will move information from the dialog into the object.
	 *	Call when the ok or apply button is pressed.
	 */
	void updateObject();

private:
	QLineEdit *m_pRoleALE, *m_pRoleBLE, *m_pMultiALE, *m_pMultiBLE;
	QMultiLineEdit *m_pDocA, *m_pDocB;
	AssociationWidget *m_pAssociationWidget;
	UMLDoc * m_pUmldoc;
	ObjectWidget *m_pWidget;
	QRadioButton *m_PublicARB, *m_ProtectedARB, *m_PrivateARB;
	QRadioButton *m_PublicBRB, *m_ProtectedBRB, *m_PrivateBRB;
	QRadioButton *m_ChangeableARB, *m_AddOnlyARB, *m_FrozenARB;
	QRadioButton *m_ChangeableBRB, *m_AddOnlyBRB, *m_FrozenBRB;

	void constructWidget();

public slots:
	/**
	 * 	When the draw as actor check box is toggled, the draw
	 * 	as multi instance need to be enabled/disabled.  They
	 * 	both can't be available at the same time.
	 */
	// void slotActorToggled( bool state );
};

#endif
