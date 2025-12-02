#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "Simulador.h"
#include "Obstaculo.h"
#include <QPixmap>


class GameWidget : public QWidget {
    Q_OBJECT

signals:

    void gameEnded(int winnerPlayer);
    void projectileFinished();

public:
    explicit GameWidget(QWidget *parent = nullptr);


    void lanzarProyectil(double angGrados, double velocidad, int jugador);

    void setScale(double s) { scale = s; update(); }

    // Verifica victoria
    bool checkWinCondition();

    void reiniciarSimulacion();
    void setShotParameters(double angle, double speed, int player);


protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onTick();

private:
    Simulador sim;
    QTimer timer;
    double scale;   // pixeles por unidad
    double tiempo_dt;
    int frames_without_projectile;
    double cannon1X, cannon1Y;
    double cannon2X, cannon2Y;

    QPixmap backgroundTexture;
    QPixmap obstacleTexture;
    QPixmap cannonRTexture;
    QPixmap cannonLTexture;
    QPixmap projectileTexture;

    // parametros del tiro
    double currentAngle;
    double currentSpeed;
    int currentPlayer;

};

#endif // GAMEWIDGET_H
