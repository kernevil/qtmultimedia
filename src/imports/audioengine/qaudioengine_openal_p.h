/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QAUDIOENGINE_OPENAL_P_H
#define QAUDIOENGINE_OPENAL_P_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QTimer>

#include <AL/al.h>
#include <AL/alc.h>

#include "qsoundsource_p.h"
#include "qsoundbuffer_p.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QSoundBufferPrivateAL : public QSoundBuffer
{
    Q_OBJECT
public:
    QSoundBufferPrivateAL(QObject* parent);
    virtual void bindToSource(ALuint alSource) = 0;
    virtual void unbindFromSource(ALuint alSource) = 0;
};

class QSoundSourcePrivate : public QSoundSource
{
    Q_OBJECT
public:
    QSoundSourcePrivate(QObject *parent);
    ~QSoundSourcePrivate();

    void play();
    void pause();
    void stop();

    QSoundSource::State state() const;

    bool isLooping() const;
    void setLooping(bool looping);
    void setPosition(const QVector3D& position);
    void setDirection(const QVector3D& direction);
    void setVelocity(const QVector3D& velocity);

    QVector3D velocity() const;
    QVector3D position() const;
    QVector3D direction() const;

    void setGain(qreal gain);
    void setPitch(qreal pitch);
    void setCone(qreal innerAngle, qreal outerAngle, qreal outerGain);

    void bindBuffer(QSoundBuffer*);
    void unbindBuffer();

    void checkState();

    void release();

Q_SIGNALS:
    void activate(QObject*);

private:
    void sourcePlay();
    void sourcePause();

    ALuint  m_alSource;
    QSoundBufferPrivateAL *m_bindBuffer;
    bool                 m_isReady; //true if the sound source is already bound to some sound buffer
    QSoundSource::State  m_state;
    qreal   m_gain;
    qreal   m_pitch;
    qreal   m_coneInnerAngle;
    qreal   m_coneOuterAngle;
    qreal   m_coneOuterGain;
};

class QSampleCache;
class QAudioEnginePrivate : public QObject
{
    Q_OBJECT
public:
    QAudioEnginePrivate(QObject *parent);
    ~QAudioEnginePrivate();

    bool isLoading() const;

    QSoundSource* createSoundSource();
    void releaseSoundSource(QSoundSource *soundInstance);
    QSoundBuffer* getStaticSoundBuffer(const QUrl& url);
    void releaseSoundBuffer(QSoundBuffer *buffer);

    QVector3D listenerPosition() const;
    QVector3D listenerVelocity() const;
    qreal listenerGain() const;
    void setListenerPosition(const QVector3D& position);
    void setListenerVelocity(const QVector3D& velocity);
    void setListenerOrientation(const QVector3D& direction, const QVector3D& up);
    void setListenerGain(qreal gain);
    void setDopplerFactor(qreal dopplerFactor);
    void setSpeedOfSound(qreal speedOfSound);

    static bool checkNoError(const char *msg);

Q_SIGNALS:
    void isLoadingChanged();

private Q_SLOTS:
    void updateSoundSources();
    void soundSourceActivate(QObject *soundSource);

private:
    QList<QSoundSourcePrivate*> m_activeInstances;
    QList<QSoundSourcePrivate*> m_instancePool;
    QMap<QUrl, QSoundBufferPrivateAL*> m_staticBufferPool;

    QSampleCache *m_sampleLoader;
    QTimer m_updateTimer;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif