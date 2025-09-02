#include "gui/TableItemDelegate.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSizePolicy>

TableItemDelegate::TableItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *TableItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(index)

    QLineEdit *editor = new QLineEdit(parent);

    // Calculate the expected height from the option
    int cellHeight = option.rect.height();

    // Set styling to make it completely seamless with the cell
    editor->setStyleSheet(
        QString(
        "QLineEdit {"
        "    border: none;"
        "    margin: 0px;"
        "    padding: 2px 8px;"
        "    background-color: #F8F8F8;"
        "    color: #333333;"
        "    selection-background-color: #CCE5F0;"
        "    selection-color: #333333;"
        "    font-size: inherit;"
        "    font-family: inherit;"
        "    max-height: %1px;"
        "    min-height: %1px;"
        "}").arg(cellHeight - 2)
    );

    // Remove any frame or border
    editor->setFrame(false);

    // Constrain the size policy to prevent expansion
    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Set explicit height constraints
    editor->setMaximumHeight(cellHeight - 2);
    editor->setMinimumHeight(cellHeight - 2);

    return editor;
}

void TableItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void TableItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}

void TableItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    Q_UNUSED(index)

    // Get the exact cell rectangle
    QRect rect = option.rect;

    // Adjust the rectangle to account for any internal padding/margins
    // Make it slightly smaller to fit perfectly within the cell
    rect.adjust(1, 1, -1, -1);

    // Set both geometry and fixed size to be absolutely certain
    editor->setGeometry(rect);
    editor->setFixedHeight(rect.height());
    editor->setFixedWidth(rect.width());

    // Ensure no extra spacing or margins
    editor->setContentsMargins(0, 0, 0, 0);

    // Make sure the editor appears on top and is properly positioned
    editor->raise();
    editor->show();
}
