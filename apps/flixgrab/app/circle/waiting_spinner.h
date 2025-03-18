#pragma once

// Qt includes
#include <QtWidgets>
#include <QtCore>

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
   
    void    setSpeeds(qreal slow, qreal fast);

    void    setLineWidth(qreal width) {
        line_width_ = width;
    }

    void    setSectorSize(qreal degrees) {
        solid_sector_ = degrees;
    }
     

    bool    isSpinning() const;


protected:
    void paintEvent(QPaintEvent *paintEvent);

    void initialize();
    //void updateSize();
    void updateTimer();
    void updatePosition();

private:
    QColor          color_ = Qt::black;
    qreal           line_width_ = 5;
    qreal           slow_speed_ = 120;  //degree in sec
    qreal           fast_speed_ = 240;  //degree in sec
    qreal           solid_sector_ = 60; //degree;

    qreal           first_pos_ = 0;
    qreal           second_pos_ = 0;

    qreal           first_speed_ = 0;
    qreal           second_speed_ = 0;
 

private:
    WaitingSpinner(const WaitingSpinner&);
    WaitingSpinner& operator=(const WaitingSpinner&);

    QTimer*         timer_;
    QElapsedTimer   time_;

    bool            center_on_parent_;
    bool            disable_parent_;
  
    bool            is_spinning_ = false;
};
