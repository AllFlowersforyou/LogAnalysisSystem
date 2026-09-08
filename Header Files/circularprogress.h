#ifndef CIRCULARPROGRESS_H
#define CIRCULARPROGRESS_H
#include <QWidget>
#include <QVector>
#include <QColor>
#include <QPropertyAnimation>

struct ChartSegment { QString label; double percent; QColor color; };

class CircularProgress : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double animProgress READ animProgress WRITE setAnimProgress)
public:
    explicit CircularProgress(QWidget *parent = nullptr);
    void setSegments(const QVector<ChartSegment> &segments);
    void setCenterText(const QString &title, const QString &value);
    double animProgress() const { return m_animProgress; }
    void setAnimProgress(double v) { m_animProgress = v; update(); }
protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override { return QSize(220, 220); }
private:
    QVector<ChartSegment> m_segments;
    QString m_centerTitle, m_centerValue;
    double m_animProgress = 0.0;
    QPropertyAnimation *m_anim;
};
#endif