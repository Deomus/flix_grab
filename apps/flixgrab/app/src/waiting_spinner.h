#pragma once

// Qt includes
#include <QtWidgets>
#include <QtCore>

enum ESpinnerType
{
    kSpinnerType_Analize,
    kSpinnerType_Pending,
};

class WaitingSpinner : public QWidget {
    Q_OBJECT
public:
    /*! Constructor for "standard" widget behaviour - use this
   * constructor if you wish to, e.g. embed your widget in another. */
    WaitingSpinner(QWidget *parent = 0,
                         bool centerOnParent = true,
                         bool disableParentWhenSpinning = true);

    /*! Constructor - use this constructor to automatically create a modal
   * ("blocking") spinner on top of the calling widget/window.  If a valid
   * parent widget is provided, "centreOnParent" will ensure that
   * QtWaitingSpinner automatically centres itself on it, if not,
   * "centreOnParent" is ignored. */
    WaitingSpinner(Qt::WindowModality modality,
                         QWidget *parent = 0,
                         bool centerOnParent = true,
                         bool disableParentWhenSpinning = true);

public slots:
    void    start();
    void    stop();

public:
    void    setColor(QColor color);
    QColor  color();

    void    setType(ESpinnerType type) { 
        type_ = type;
        setLineWidth(type_);
    };

    ESpinnerType Type() { return type_; };

    bool    isSpinning() const;

//SpinnerType_Pending
    void    setSpeeds(qreal slow, qreal fast);
    void    setSectorSize(qreal degrees) { solid_sector_ = degrees; }

protected:
    void	showEvent(QShowEvent* event) {
        start();
        QWidget::showEvent(event);
    }

    void	hideEvent(QHideEvent* event) {
        QWidget::hideEvent(event);
        stop();
    }

    void paintEvent(QPaintEvent *paintEvent);

    void initialize();
    void updateTimer();
    void updatePosition();
    //void updateSize();

private:
    void setLineWidth(ESpinnerType type) {
        switch (type) {
        case kSpinnerType_Analize:
            line_width_ = 1;
            break;
        case kSpinnerType_Pending:
            line_width_ = 4;
            break;
        }
    }

//SpinnerType_Analize
private:
    int             compress_ = 0;
    int             reduce_coef_ = 1;
    int             reduce_coef_backflow_ = 1;
    
    const int       max_value = 160; // more value - slow speed
    int             compress_backflow_ = max_value / 2;

//SpinnerType_Pending
private:
    qreal           slow_speed_ = 240;  //degree in sec
    qreal           fast_speed_ = 360;  //degree in sec
    qreal           solid_sector_ = 60; //degree;

    qreal           first_pos_ = 0;
    qreal           second_pos_ = 0;

    qreal           first_speed_ = 0;
    qreal           second_speed_ = 0;

//Common_
private:
    WaitingSpinner(const WaitingSpinner&);
    WaitingSpinner& operator=(const WaitingSpinner&);

    QTimer*         timer_;
    QElapsedTimer   time_;

    ESpinnerType    type_ = ESpinnerType::kSpinnerType_Analize;

    bool            center_on_parent_;
    bool            disable_parent_;
  
    bool            is_spinning_ = false;

    QColor          color_ = Qt::black;
    qreal           line_width_ = 1;
};
