/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "umlattributedialog.h"

// app includes
#include "attribute.h"
#include "classifier.h"
#include "documentationwidget.h"
#include "template.h"
#include "umldoc.h"
#include "uml.h"
#include "umldatatypewidget.h"
#include "umlstereotypewidget.h"
#include "visibilityenumwidget.h"
#include "umltypequalifierswidget.h"
#include "dialog_utils.h"
#include "object_factory.h"
#include "import_utils.h"

// kde includes
#include <klineedit.h>
#include <kcombobox.h>
#include <kcompletion.h>
#include <KLocalizedString>
#include <KMessageBox>

// qt includes
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QRadioButton>
#include <QVBoxLayout>

UMLAttributeDialog::UMLAttributeDialog(QWidget * pParent, UMLAttribute * pAttribute)
  : SinglePageDialogBase(pParent)
  , ui(new Ui::UMLAttributeDialog)
  , m_pAttribute(pAttribute)
{
    setCaption(i18n("Attribute Properties"));
    ui->setupUi(mainWidget());

    setupDialog();
}

UMLAttributeDialog::~UMLAttributeDialog()
{
}

/**
 *   Sets up the dialog
 */
void UMLAttributeDialog::setupDialog()
{
    ui->dataTypeWidget->setClassifierItem(dynamic_cast<UMLClassifierListItem*>(m_pAttribute));
    ui->typeQualifiersWidget->setUMLClassifierItem(dynamic_cast<UMLClassifierListItem*>(m_pAttribute));
    ui->stereotypeWidget->setUMLObject(m_pAttribute);
    ui->visibilityWidget->setUMLObject(m_pAttribute);
    ui->documentationWidget->setUMLObject(m_pAttribute);
}

void UMLAttributeDialog::slotNameChanged(const QString &_text)
{
    enableButtonOk(!_text.isEmpty());
}

/**
 * Checks if changes are valid and applies them if they are,
 * else returns false
 */
bool UMLAttributeDialog::apply()
{
    QString name = ui->tb_name->text();
    if (name.isEmpty()) {
        KMessageBox::error(this, i18n("You have entered an invalid attribute name."),
                           i18n("Attribute Name Invalid"), 0);
        ui->tb_name->setText(m_pAttribute->name());
        return false;
    }
    UMLClassifier * pConcept = dynamic_cast<UMLClassifier *>(m_pAttribute->parent());
    UMLObject *o = pConcept->findChildObject(name);
    if (o && o != m_pAttribute) {
        KMessageBox::error(this, i18n("The attribute name you have chosen is already being used in this operation."),
                           i18n("Attribute Name Not Unique"), 0);
        ui->tb_name->setText(m_pAttribute->name());
        return false;
    }
    m_pAttribute->setName(name);
    ui->visibilityWidget->apply();

    // Set the scope as the default in the option state
    Settings::optionState().classState.defaultAttributeScope = m_pAttribute->visibility();

    m_pAttribute->setInitialValue(ui->tb_initialValue->text());
    ui->stereotypeWidget->apply();
    m_pAttribute->setStatic(ui->cb_classifierScopeStatic->isChecked());

    ui->dataTypeWidget->apply();
    ui->typeQualifiersWidget->apply();
    ui->documentationWidget->apply();

    return true;
}
