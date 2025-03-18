#ifndef MENU_H
#define MENU_H

#include <QMenu>

class MenuPrivate;

enum class TypeMenu {
    Other,
    PlayList
};

class Menu : public QMenu
{
    Q_OBJECT
    friend class MenuScrollArea;

public:
    explicit Menu(QWidget *parent = nullptr);
    explicit Menu(const QString &title, QWidget *parent = nullptr);
    ~Menu() override;

    QSize contentSizeHint() const;
    QSize sizeHint() const override;

    QAction *currentAction() const;
    QAction *activeAction() const;
    Menu *activeMenu() const;

    QAction *headerAction() const;
    void setHeader(QAction *action);

    void popup(const QPoint &pos);
    void correctSizePosFromDisplay();
    void click_button(QAction* action);
    QRect btn_rect_;
    TypeMenu type_menu_;

protected:
    void changeEvent(QEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;
#ifdef QT6
	void enterEvent(QEnterEvent*) override;
#else // QT5
    void enterEvent(QEvent *) override;
#endif
    void leaveEvent(QEvent *) override;
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void actionEvent(QActionEvent *) override;
    void timerEvent(QTimerEvent *) override;
    bool event(QEvent *) override;
    bool focusNextPrevChild(bool next) override;

private slots:
    void verticalSliderValueChanged(int sliderValue);
    void horizontalSliderValueChanged(int sliderValue);

private:
    int getLastVisibleAction() const;

    void init();
    void reloadScrollStyleSheet(bool mouseEntered);
    void adjustContent();
    void updateActionRects() const;

    void initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const;

    bool isContextMenu() const;

    void updateAction(QAction *act);

    QAction *actionAt(QPoint p) const;

    QWidget *topCausedWidget() const;
    QVector<QPointer<QWidget> > calcCausedStack() const;
    void hideUpToMenuBar();

    void activateAction(QAction *, QAction::ActionEvent, bool self=true);
    void activateCausedStack(const QVector<QPointer<QWidget> > &, QAction *, QAction::ActionEvent, bool);

    void setCurrentAction(QAction *action, int popup = -1, bool activateFirst = false);
    void popupAction(QAction *, int, bool);

    void hideMenu(Menu *menu);
    void internalDelayedPopup();

    void startDelayPopup(int delay, QAction *toStartAction);
    void stopDelayPopup();

    void pSetMouseTracking(QWidget *w);

private:
    MenuPrivate *d;
};

#endif // MENU_H
