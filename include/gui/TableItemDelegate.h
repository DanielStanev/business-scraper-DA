#pragma once

#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QLineEdit>

class TableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TableItemDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;
};
