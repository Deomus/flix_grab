#pragma once

#include <QtWidgets>
#include "ui_menu_action.h"

class SubmenuAction;

class SubmenuWidget : public QFrame {
    Q_OBJECT

public:
    
    SubmenuWidget(QWidget* parent = nullptr);
    SubmenuWidget(const QString& name, const QString& value, const bool& icon_need=TRUE, QWidget* parent = nullptr);

    void                    showCheckBox() {
        ui_.cbxSelected->show();
    }

    void                    hideCheckBox() {
        ui_.cbxSelected->hide();
    }

public:
    QString                 name() const { return ui_.lblName->text(); }
    QString                 value() const { return ui_.lblValue->text(); }
    QAction*                action() { return action_; }

    void                    setTextStylesheet(const QString& stylesheet) { ui_.lblName->setStyleSheet(stylesheet); ui_.lblValue->setStyleSheet(stylesheet);}

    void                    createSubmenu(const QList<QAction*>& subactions);
    QList<QAction*>         submenuActions() const;
    void                    clearSubmenu();

    void                    triggerAction(int index);
    void                    selectAction(int index);
    bool                    triggerActionByData(const QVariant& data);
    bool                    selectActionByData(const QVariant& data);

    void                    setActionEnabled(bool enabled = true) { action_->setEnabled(enabled); }

    bool                    isActionEnabled() const { return action_->isEnabled(); }
        
signals:
    void                    submenuTriggered(QAction*);
    void                    triggered();

public slots:
    void                    setName(QString name);
    void                    setValue(QString value);
    void                    setIconNeed(bool icon_need);

private slots:
    
    void                    popupSubmenu();
    void                    popupMenu();
           
    void                    updateAction();

    void                    hoverActionFromMenu(QMenu* menu);
   
public:
    static int              getNormalizeNameWidth(const QList<SubmenuWidget*>& widgets);
    static void             normalizeNameWidth(const QList<SubmenuWidget*>& widgets, int max_width);

protected:
    void                    showEvent(QShowEvent* e) override;
    void                    hideEvent(QHideEvent *e) override;
    bool                    event(QEvent * e) override;
    
private:
    static void             switchMenu(QMenu* menu_old, QMenu* menu_new);
    void                    setHovered(bool hovered = true);

private:
    Ui::MenuAction                  ui_;

private:
    QWidgetAction*                  action_;
    QMenu*                          submenu_ = nullptr;
    QAction*                        back_ = nullptr;
    QActionGroup*                   group_ = nullptr;
};
